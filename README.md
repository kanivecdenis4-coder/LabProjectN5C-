# Лабораторна робота: багатопотокові обчислення (варіант 14)

Програма на C++20 виконує обчислення `A1, A2, B, C1, C2, D1, D2` за оптимальною схемою
(мінімальний час 9 с, мінімум 3 одночасно працюючих потоки, 2 запуски потоків).

Умови варіанта: "повільні" — `A1, C2`; залежності — `B(A1,A2)`, `D1(B,C1)`, `D2(C2)`.

Асинхронність — лише через `<future>`, вивід синхронізовано через `std::osyncstream`.

## Збірка та запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/work            # Windows: build\Release\work.exe
```

Без CMake:

```bash
g++ -std=c++20 -O2 -pthread src/main.cpp -o work && ./work
```

Потрібен компілятор з підтримкою `<syncstream>`: GCC 11+, Clang 14+ (libc++/libstdc++ 11+), MSVC 2019 16.11+.

## Структура
- `src/main.cpp` — код (`slow`, `quick`, `work`, `main`)
- `docs/REPORT.md` — звіт зі схемою та обґрунтуванням
- `docs/schema.png` — схема синхронізації
- `docs/output.txt` — результат виконання
- `.github/workflows/build.yml` — CI збірка

