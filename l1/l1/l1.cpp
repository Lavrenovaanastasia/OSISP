#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <atomic>
#include <ctime>
#include <string>
#include <iomanip>
#include <psapi.h>

using namespace std;

const int M = 1000;
const int N = 100;
vector<vector<int>> matrix1(M, vector<int>(M));
vector<vector<int>> matrix2(M, vector<int>(M));
vector<vector<int>> result(M, vector<int>(M));
// Mutex для синхронизации доступа к результатам
mutex resultMutex;
// Атомарные переменные для подсчета завершенных задач
atomic<int> tasksCompleted;
double getCPULoad() {
    FILETIME idleTime, kernelTime, userTime;
    // WinAPI
    GetSystemTimes(&idleTime, &kernelTime, &userTime);//заполняет переменные  данными о времени простоя,
    //времени работы ядра и времени работы пользователей

     // Преобразование FILETIME в 64-битный целочисленный тип
    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    // Вычисление общей активности процессора
    ULARGE_INTEGER totalTime = kernel;
    totalTime.QuadPart += user.QuadPart;

    // Вычисление загрузки процессора в процентах
    double cpuLoad = (double)(totalTime.QuadPart - idle.QuadPart) / totalTime.QuadPart * 100;
    return cpuLoad;
}
void ymnozenieMBlock(int startRow, int endRow, int startCol, int endCol) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = startCol; j < endCol; ++j) {
            for (int k = 0; k < M; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    // Переключение на другой поток
    // Sleep(1); // Используем Sleep() из WinAPI для задержки
    this_thread::sleep_for(chrono::milliseconds(1));//из WinAPI для задержки выполнения потока на указанное время
    ++tasksCompleted;//счетчик завершенных задач
}

void ymnozeniePotokami() {
    int blockSize = M / N;
    int numberOfBlocks = (M + blockSize - 1) / blockSize; // Количество блоков
    tasksCompleted = 0;
    vector<thread> threads;
    auto start = chrono::high_resolution_clock::now();
    double cpuLoadBefore = getCPULoad();
    for (int i = 0; i < numberOfBlocks; ++i) {
        int startRow = i * blockSize;
        int endRow = (i == numberOfBlocks - 1) ? M : (i + 1) * blockSize; // Обработка последнего блока
        int startCol = i * blockSize;
        int endCol = (i == numberOfBlocks - 1) ? M : (i + 1) * blockSize; // Обработка последнего блока
        threads.push_back(thread(ymnozenieMBlock, startRow, endRow, startCol, endCol));
    }
    for (auto& thread : threads) {
        thread.join();//ожидание завершения потоков
    }
    auto end = chrono::high_resolution_clock::now();
    double cpuLoadAfter = getCPULoad();
    double duration = chrono::duration<double>(end - start).count();
    cout << "Умножение потоками: " << duration << " сек, загрузка процессора: " << cpuLoadAfter - cpuLoadBefore << "%" << endl;
}

void ymnozenieNitiami() {
    int threadId = 0;//для передачи управления
    int blockSize = M / N;
    int numberOfBlocks = (M + blockSize - 1) / blockSize; // Количество блоков
    tasksCompleted = 0;
    auto start = chrono::high_resolution_clock::now();
    double cpuLoadBefore = getCPULoad();
    for (int i = 0; i < numberOfBlocks; ++i) {
        int startRow = i * blockSize;
        int endRow = (i == numberOfBlocks - 1) ? M : (i + 1) * blockSize;
        int startCol = i * blockSize;
        int endCol = (i == numberOfBlocks - 1) ? M : (i + 1) * blockSize;
        ymnozenieMBlock(startRow, endRow, startCol, endCol);
        threadId = (threadId + 1) % N;//передача управления следующей нити
        if (threadId == 0) {
            this_thread::yield();//сообщает планировщику, что поток готов уступить процессорное время
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double cpuLoadAfter = getCPULoad();
    double duration = chrono::duration<double>(end - start).count();
    cout << "Умножение нитями: " << duration << " сек, загрузка процессора: " << cpuLoadAfter - cpuLoadBefore << "%" << endl;
}

void linenoeYmnozenie() {
    auto start = chrono::high_resolution_clock::now();
    double cpuLoadBefore = getCPULoad();
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            for (int k = 0; k < M; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double cpuLoadAfter = getCPULoad();
    double duration = chrono::duration<double>(end - start).count();
    cout << "Линейное умножение: " << duration << " сек, загрузка процессора: " << cpuLoadAfter - cpuLoadBefore << "%" << endl;
}
/*void printMatrix(const vector<vector<int>>& matrix, const string& name) {
    cout << name << ":" << endl;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << setw(3) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}*/
int main() {
    setlocale(LC_ALL, "Russian");
    // Инициализация матриц
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            matrix1[i][j] = rand() % 10;
            matrix2[i][j] = rand() % 10;
        }
    }

    cout << "Размер матриц: " << M << "x" << M << endl;
    cout << "Количество потоков/нитей: " << N << endl;

    // Вычисление произведения матриц разными способами
    ymnozeniePotokami();
    ymnozenieNitiami();
    linenoeYmnozenie();

    return 0;
}

/*//Вывод матриц
    cout << "Матрица 1:" << endl;
    printMatrix(matrix1, "matrix1");

    cout << "Матрица 2:" << endl;
    printMatrix(matrix2, "matrix2");

    cout << "Результат умножения линейным способом:" << endl;
    printMatrix(result, "result");

    // Сброс результата
    result = vector<vector<int>>(M, vector<int>(M));
    ymnozeniePotokami();
    cout << "Результат умножения потоками:" << endl;
    printMatrix(result, "result");

    // Сброс результата
    result = vector<vector<int>>(M, vector<int>(M));
    ymnozenieNitiami();
    cout << "Результат умножения нитями:" << endl;
    printMatrix(result, "result");*/
