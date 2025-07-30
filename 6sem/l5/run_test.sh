#!/bin/bash

echo "=== Сравнительный анализ эффективности синхронизаций ===" > results.txt
echo "Дата: $(date)" >> results.txt
echo "===============================================" >> results.txt

# Массив конфигураций: (кол-во потоков) (итераций на поток)
CONFIGS=(
  "5 500"
  "10 500"
  "20 1000"
  "50 200"
)

for config in "${CONFIGS[@]}"; do
    read threads iters <<< "$config"
    echo -e "\n--- Конфигурация: Потоков = $threads, Итераций = $iters ---" | tee -a results.txt
    ./CompIPC $threads $iters | tee -a results.txt
done

cat <<EOF >> results.txt
EOF

echo "Все результаты записаны в results.txt"