#pragma once
#include "Perceptron.h"



class Autocoder {
private:

    Perceptron<Sigmoid> p;
    size_t chunkSize = 0;
    size_t outSize = 0;

public:
    Autocoder(int chinkSize, int to);

    void learn(const std::string& data);

    std::vector<bool> encode(const std::string& data);

    std::string decode(const std::vector<bool>& code);
};

std::vector<bool> getRaw(const std::string& s);

std::vector<std::vector<bool>> getInput(const std::string& data, size_t chunkSize);

std::string makeString(std::vector<bool>& data);
