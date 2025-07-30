#include "daemon.h"
#include <iostream>
#include <fstream>
#include <csignal>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc == 2 && (std::string(argv[1]) == "-q" || std::string(argv[1]) == "--quit")) {
        std::ifstream pidFile("/home/vboxuser/Desktop/lab4/daemon.pid");
        if (!pidFile) {
            std::cerr << "Не удалось найти PID-файл. Демон не запущен?" << std::endl;
            return 1;
        }

        int pid;
        pidFile >> pid;
        if (kill(pid, SIGTERM) == 0) {
            std::cout << "Отправлен сигнал SIGTERM процессу " << pid << std::endl;
        } else {
            perror("Ошибка при отправке SIGTERM");
        }

        return 0;
    }

    daemonize();
    loadSignalConfig();
    setupSignalHandlers();

    while (true) pause();

    return 0;
}