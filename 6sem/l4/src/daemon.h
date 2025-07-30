#ifndef DAEMON_H
#define DAEMON_H

#include <string>

void daemonize();
void signalHandler(int sig);
void writeLog(const std::string& message);
void loadSignalConfig();
void setupSignalHandlers();

#endif