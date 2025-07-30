#!/bin/bash

set -e

echo "=============================="
echo " 1. Сборка проекта через make"
echo "=============================="
make clean
make

echo
echo "=============================="
echo " 2. Запуск демона в фоне"
echo "=============================="
./build/lab4 &
sleep 1

PID=$(cat daemon.pid)
echo " Демон запущен с PID: $PID"

echo
echo "=============================="
echo " 3. Отправка сигналов"
echo "=============================="
kill -SIGUSR1 $PID
sleep 0.5
kill -SIGHUP $PID
sleep 0.5

echo
echo "=============================="
echo " 4. Просмотр лога"
echo "=============================="
LOGFILE="log/signal_daemon.log"
if [ -f "$LOGFILE" ]; then
    cat "$LOGFILE"
else
    echo " Лог-файл не найден!"
    exit 1
fi

echo
echo "=============================="
echo " 5. Завершение демона через --quit"
echo "=============================="
./build/lab4 --quit
sleep 1

if kill -0 $PID 2>/dev/null; then
    echo " Демон всё ещё работает — ошибка завершения"
    exit 1
else
    echo " Демон завершён успешно"
fi

echo
echo " Тест завершён успешно!"