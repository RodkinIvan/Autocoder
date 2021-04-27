#include <iostream>
#include "Autocoder.h"

void print(const std::vector<bool>& v) {
    for (auto i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

void print(const std::vector<double>& v) {
    for (auto i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

std::vector<bool> getR(const std::string& s) {
    std::vector<bool> raw(s.size() * 8);
    for (size_t i = 0; i < s.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            raw[8 * i + j] = ((s[i] >> j) & 1);
        }
    }
    return raw;
}

int main() {
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    Autocoder a(8, 5);

    a.learn("Ohayo gozaimasen");
    std::string text = "Ohayo";
    auto out = a.encode(text);
    std::vector<bool> expected = getR(text);
    std::cout << "Coded:            ";
    print(out);
    std::cout << "Expected decoded: ";
    print(expected);
    std::cout << '\'' + text + "\' -> '" + a.decode(out) << "'\n";
}
