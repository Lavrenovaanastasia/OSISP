#include <iostream>
#include <fstream>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <map>
#include <cstring>

std::string configFile = "/home/fulcrum/BSUIR/OSISP/LR4/mydaemon.conf";
std::string logFile = "/home/fulcrum/BSUIR/OSISP/LR4/mydaemon.log";
std::map<int, std::string> signalMap;
pid_t daemonPid = 0;

void readConfig() {
    std::ifstream config(configFile);
    signalMap.clear();
    std::string line;
    while (std::getline(config, line)) {
        int sig = std::stoi(line);
        signalMap[sig] = strsignal(sig);
    }
    config.close();
}

void logSignal(int sig) {
    std::ofstream log(logFile, std::ios_base::app);
    if (log.is_open()) {
        log << "Received signal: " << signalMap[sig] << " (" << sig << ")\n";
    }
}

void signalHandler(int sig) {
    if (sig == SIGHUP || sig == 1) {
        readConfig();
        logSignal(sig); 
    } else if (sig == SIGTERM) {
        logSignal(sig);
        exit(0);
    } else {
        logSignal(sig);
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(1);
    }
    if (pid > 0) {
        exit(0);
    }

    umask(0);

    pid_t sid = setsid();
    if (sid < 0) {
        std::cerr << "setsid failed!" << std::endl;
        exit(1);
    }

    if ((chdir("/")) < 0) {
        std::cerr << "chdir failed!" << std::endl;
        exit(1);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char* argv[]) {
    if (argc == 2 && (strcmp(argv[1], "-q") == 0 || strcmp(argv[1], "--quit") == 0)) {
        std::ifstream pidFile("/var/run/mydaemon.pid");
        if (pidFile.is_open()) {
            pidFile >> daemonPid;
            pidFile.close();
            if (daemonPid > 0) {
                kill(daemonPid, SIGTERM);
                std::cout << "Daemon stopped." << std::endl;
                return 0;
            }
        }
        std::cerr << "Daemon is not running." << std::endl;
        return 1;
    }

    daemonize();

    std::ofstream pidFile("/var/run/mydaemon.pid");
    if (pidFile.is_open()) {
        pidFile << getpid();
        pidFile.close();
    } else {
        std::cerr << "Could not create PID file." << std::endl;
        exit(1);
    }

    readConfig();

    for (const auto& sig : signalMap) {
        signal(sig.first, signalHandler);
    }

    while (true) {
        pause();
    }

    return 0;
}