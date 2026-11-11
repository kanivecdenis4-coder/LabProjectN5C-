// Варіант №14
// Обчислення: A1, A2, B, C1, C2, D1, D2
// "Повільні": A1, C2 (7 с), решта "швидкі" (1 с)
// Залежності: B(A1,A2), D1(B,C1), D2(C2)
//
// Схема виконання (оптимальна: 9 с, 3 потоки одночасно, запускається 2 додаткові потоки):
//
//   головний потік : A2 -> C1 ----------------------------------- (чекає)
//   потік #1       : A1 (7 с) -> B -> D1       (B чекає A2, D1 чекає C1)
//   потік #2       : C2 (7 с) -> D2
//
// Критичний шлях: A1 -> B -> D1 = 7+1+1 = 9 с (D1 транзитивно залежить від A1, A2).

#include <chrono>
#include <exception>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <syncstream>
#include <thread>

using namespace std::chrono_literals;


void slow(const std::string& name) {
    std::this_thread::sleep_for(7s);
    std::osyncstream(std::cout) << name << '\n';
}


void quick(const std::string& name) {
    std::this_thread::sleep_for(1s);
    std::osyncstream(std::cout) << name << '\n';
}


void work() {
    const auto start = std::chrono::steady_clock::now();

   
    std::promise<void> a2Done;
    std::promise<void> c1Done;
    std::future<void> a2Ready = a2Done.get_future();
    std::future<void> c1Ready = c1Done.get_future();

    
    auto chainA = std::async(std::launch::async, [&] {
        slow("A1");
        a2Ready.get();   
        quick("B");
        c1Ready.get();   
        quick("D1");
        });

    
    auto chainC = std::async(std::launch::async, [&] {
        slow("C2");
        quick("D2");
        });

    
    try {
        quick("A2");
        a2Done.set_value();
    }
    catch (...) {
        a2Done.set_exception(std::current_exception());
    }
    try {
        quick("C1");
        c1Done.set_value();
    }
    catch (...) {
        c1Done.set_exception(std::current_exception());
    }

    chainA.get();
    chainC.get();

    const std::chrono::duration<double> elapsed =
        std::chrono::steady_clock::now() - start;

    std::osyncstream out(std::cout);
    out << std::fixed << std::setprecision(2)
        << "Time: " << elapsed.count() << " s\n"
        << "Work is done!\n";
}

int main() {
    work();
    return 0;
}