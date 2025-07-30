#include "daemon.h"
#include <iostream>
#include <fstream>
#include <csignal>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <set>
#include <map>

std::ofstream logFile;
std::set<int> handledSignals;
std::map<std::string, int> signalMap = {
    {"SIGHUP", SIGHUP},
    {"SIGTERM", SIGTERM},
    {"SIGUSR1", SIGUSR1},
    {"SIGUSR2", SIGUSR2}
};

std::string getTime() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

void writeLog(const std::string& message) {
    logFile << "[" << getTime() << "] " << message << std::endl;
    logFile.flush();
}

void loadSignalConfig() {
    handledSignals.clear();
    std::ifstream conf("/home/vboxuser/Desktop/lab4/config/signals.conf");
    if (!conf) {
        writeLog("Не удалось открыть config/signals.conf. Обрабатываем все сигналы по умолчанию.");
        handledSignals = {SIGHUP, SIGTERM, SIGUSR1, SIGUSR2};
        return;
    }

    std::string line;
    while (std::getline(conf, line)) {
        if (signalMap.count(line)) {
            handledSignals.insert(signalMap[line]);
        }
    }

    writeLog("Файл конфигурации сигналов загружен.");
}

void setupSignalHandlers() {
    for (int sig : handledSignals) {
        signal(sig, signalHandler);
    }
}

void signalHandler(int sig) {
    if (!handledSignals.count(sig)) return;

    switch (sig) {
        case SIGHUP:
            writeLog("Получен сигнал SIGHUP: перечитываем конфигурацию");
            loadSignalConfig();
            setupSignalHandlers();
            break;
        case SIGTERM:
            writeLog("Получен сигнал SIGTERM: завершение демона");
            logFile.close();
            remove("daemon.pid");
            exit(EXIT_SUCCESS);
            break;
        case SIGUSR1:
            writeLog("Получен сигнал SIGUSR1");
            break;
        case SIGUSR2:
            writeLog("Получен сигнал SIGUSR2");
            break;
        default:
            writeLog("Получен неизвестный сигнал №" + std::to_string(sig));
            break;
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    logFile.open("/home/vboxuser/Desktop/lab4/log/signal_daemon.log", std::ios::out | std::ios::trunc);
    if (!logFile) exit(EXIT_FAILURE);


    std::ofstream pidFile("/home/vboxuser/Desktop/lab4/daemon.pid");
    pidFile << getpid();
    pidFile.close();

    writeLog("Демон запущен и готов к приёму сигналов");
}