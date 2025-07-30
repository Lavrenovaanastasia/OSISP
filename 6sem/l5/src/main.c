#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <cstring>
#include <vector>

using namespace std;

int iterations = 1000;
bool verbose = false;

// Получение времени в секундах
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

void critical_function(int thread_id) {
    if (verbose)
        cout << "Thread " << thread_id << " accessed critical section." << endl;
}

//==================== MUTEX ====================
struct MutexCtx {
    pthread_mutex_t* mutex;
    int thread_id;
    int iterations;
};

void* mutex_thread(void* arg) {
    MutexCtx* ctx = (MutexCtx*)arg;
    for (int i = 0; i < ctx->iterations; ++i) {
        pthread_mutex_lock(ctx->mutex);
        critical_function(ctx->thread_id);
        pthread_mutex_unlock(ctx->mutex);
    }
    return NULL;
}

pthread_mutex_t create_mutex(int type) {
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, type);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    return mutex;
}

void run_mutex_test(int type, const string& name, int num_threads, int local_iterations) {
    pthread_mutex_t mutex = create_mutex(type);
    pthread_t threads[num_threads];
    double t_start = get_time();

    for (int i = 0; i < num_threads; ++i) {
        MutexCtx* ctx = new MutexCtx{&mutex, i, local_iterations};
        pthread_create(&threads[i], NULL, mutex_thread, ctx);
    }
    for (int i = 0; i < num_threads; ++i) pthread_join(threads[i], NULL);
    double t_end = get_time();
    cout << name << " time: " << t_end - t_start << " sec" << endl;

    pthread_mutex_destroy(&mutex);
}

//==================== SPINLOCK ====================
struct SpinCtx {
    pthread_spinlock_t* spin;
    int thread_id;
    int iterations;
};

void* spin_thread(void* arg) {
    SpinCtx* ctx = (SpinCtx*)arg;
    for (int i = 0; i < ctx->iterations; ++i) {
        pthread_spin_lock(ctx->spin);
        critical_function(ctx->thread_id);
        pthread_spin_unlock(ctx->spin);
    }
    return NULL;
}

void run_spinlock_test(int num_threads, int local_iterations) {
    pthread_spinlock_t spin;
    pthread_spin_init(&spin, 0);
    pthread_t threads[num_threads];
    double t_start = get_time();

    for (int i = 0; i < num_threads; ++i) {
        SpinCtx* ctx = new SpinCtx{&spin, i, local_iterations};
        pthread_create(&threads[i], NULL, spin_thread, ctx);
    }
    for (int i = 0; i < num_threads; ++i) pthread_join(threads[i], NULL);
    double t_end = get_time();
    cout << "Spinlock time: " << t_end - t_start << " sec" << endl;
    pthread_spin_destroy(&spin);
}

//==================== POSIX SEM ====================
struct SemPosixCtx {
    sem_t* sem;
    int thread_id;
    int iterations;
};

void* sem_posix_thread(void* arg) {
    SemPosixCtx* ctx = (SemPosixCtx*)arg;
    for (int i = 0; i < ctx->iterations; ++i) {
        sem_wait(ctx->sem);
        critical_function(ctx->thread_id);
        sem_post(ctx->sem);
    }
    return NULL;
}

void run_posix_test(int num_threads, int local_iterations) {
    sem_t sem;
    sem_init(&sem, 0, 1);
    pthread_t threads[num_threads];
    double t_start = get_time();

    for (int i = 0; i < num_threads; ++i) {
        SemPosixCtx* ctx = new SemPosixCtx{&sem, i, local_iterations};
        pthread_create(&threads[i], NULL, sem_posix_thread, ctx);
    }
    for (int i = 0; i < num_threads; ++i) pthread_join(threads[i], NULL);
    double t_end = get_time();
    cout << "POSIX semaphore time: " << t_end - t_start << " sec" << endl;
    sem_destroy(&sem);
}

//==================== SYSV SEM ====================
struct SemSysVCtx {
    int sem_id;
    int thread_id;
    int iterations;
};

void* sem_sysv_thread(void* arg) {
    SemSysVCtx* ctx = (SemSysVCtx*)arg;
    struct sembuf op;
    for (int i = 0; i < ctx->iterations; ++i) {
        op = {0, -1, 0};
        semop(ctx->sem_id, &op, 1);
        critical_function(ctx->thread_id);
        op = {0, 1, 0};
        semop(ctx->sem_id, &op, 1);
    }
    return NULL;
}

void run_sysv_test(int num_threads, int local_iterations) {
    key_t key = ftok(".", 's');
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, 1);
    pthread_t threads[num_threads];
    double t_start = get_time();

    for (int i = 0; i < num_threads; ++i) {
        SemSysVCtx* ctx = new SemSysVCtx{sem_id, i, local_iterations};
        pthread_create(&threads[i], NULL, sem_sysv_thread, ctx);
    }
    for (int i = 0; i < num_threads; ++i) pthread_join(threads[i], NULL);
    double t_end = get_time();
    cout << "System V semaphore time: " << t_end - t_start << " sec" << endl;
    semctl(sem_id, 0, IPC_RMID);
}

//==================== BARRIER ====================
struct BarrierCtx {
    pthread_barrier_t* barrier;
    int thread_id;
    int iterations;
};

void* barrier_thread(void* arg) {
    BarrierCtx* ctx = (BarrierCtx*)arg;
    for (int i = 0; i < ctx->iterations; ++i) {
        pthread_barrier_wait(ctx->barrier);
        critical_function(ctx->thread_id);
    }
    return NULL;
}

void run_barrier_test(int num_threads, int local_iterations) {
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, num_threads);
    pthread_t threads[num_threads];
    double t_start = get_time();

    for (int i = 0; i < num_threads; ++i) {
        BarrierCtx* ctx = new BarrierCtx{&barrier, i, local_iterations};
        pthread_create(&threads[i], NULL, barrier_thread, ctx);
    }
    for (int i = 0; i < num_threads; ++i) pthread_join(threads[i], NULL);
    double t_end = get_time();
    cout << "Barrier time: " << t_end - t_start << " sec" << endl;
    pthread_barrier_destroy(&barrier);
}

int main(int argc, char* argv[]) {
    int num_threads = 20;
    if (argc > 1) num_threads = atoi(argv[1]);
    if (argc > 2) iterations = atoi(argv[2]);
    if (argc > 3 && strcmp(argv[3], "--verbose") == 0) verbose = true;

    cout << "Running tests with " << num_threads << " threads and " << iterations << " iterations...\n";
    run_mutex_test(PTHREAD_MUTEX_NORMAL, "Mutex NORMAL", num_threads, iterations);
    run_mutex_test(PTHREAD_MUTEX_ERRORCHECK, "Mutex ERRORCHECK", num_threads, iterations);
    run_mutex_test(PTHREAD_MUTEX_RECURSIVE, "Mutex RECURSIVE", num_threads, iterations);
    run_posix_test(num_threads, iterations);
    run_sysv_test(num_threads, iterations);
    run_spinlock_test(num_threads, iterations);
    run_barrier_test(num_threads, iterations);

    return 0;
}