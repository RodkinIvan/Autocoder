#include <iostream>
#include <cstdlib>
#include "AutocoderSrc/Autocoder.h"

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
    std::cout << "Enter the learning layout (enter 'default' if want it to be 'Ohayo gozaimasen za warudo':)" << '\n';
    std::string learning;

    std::getline(std::cin, learning);
    learning = learning == "default" ? "Ohayo gozaimasen za warudo" : learning;
    std::cout << "Starting to learn '" << learning << "'..." << '\n';
    a.learn(learning);
    std::string text;
    std::cout << "Enter encoding phrase (exit if want to stop):" << '\n';
    while (true) {
        std::getline(std::cin, text);
        if (text == "exit") {
            break;
        }
        auto out = a.encode(text);
        std::vector<bool> expected = getR(text);
        std::cout << "Coded:            ";
        print(out);
        std::cout << "Expected decoded: ";
        print(expected);
        std::cout << '\'' + text + "\' -> '" + a.decode(out) << "'\n";
    }


}
