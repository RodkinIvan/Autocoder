#pragma once

#include <bits/stdc++.h>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstring>

struct Sigmoid {
    double operator()(double x) const {
        return 1 / (1 + exp(-x));
    }

    static double der(double o) {
        return o * (1 - o);
    }

    static double invert(double y) {
        return log(y) - log(1 - y);
    }
};

template<typename activation_function = Sigmoid>
class Perceptron {
private:
    std::mt19937 rnd;
    const size_t NUMBER_OF_STEPS = 10'000;
    const double spd = 1.0;
    activation_function f;
    int n;
    int m;
    int c;
    std::vector<std::vector<double>> neurons;
    std::vector<std::vector<std::vector<double>>> w;
    std::vector<std::vector<double>> b;

    double act_func(double x);

    double act_func_der(double o);

    void randomize();

    void backProp(const std::vector<double>& out, const std::vector<double>& output, int outCol, double alpha = 1);

public:
    size_t intLayer = 0;

    explicit Perceptron(const std::vector<int>& cols);

    const std::vector<double>& run(const std::vector<bool>& in, int from, int outCol);

    void learn(const std::vector<std::vector<bool>>& inputs, const std::vector<std::vector<double>>& outputs,
               int outCol = -1);

    const std::vector<std::vector<double>>& getCols() const;

    unsigned long long getWeightsNum() const;

    std::vector<double> getWeights() const;

    void setWeights(std::vector<double> weights);
};

template<typename activation_function>
double Perceptron<activation_function>::act_func(double x) {
    return f(x);
}

template<typename activation_function>
double Perceptron<activation_function>::act_func_der(double o) {
    return f.der(o);
}

template<typename activation_function>
void Perceptron<activation_function>::learn(const std::vector<std::vector<bool>>& inputs,
                                            const std::vector<std::vector<double>>& outputs, int outCol) {
    assert(inputs.size() == outputs.size());
    size_t sz = inputs.size();

    for (size_t r = 0; r < NUMBER_OF_STEPS; ++r) {
        for (size_t p = 0; p < sz; ++p) {
            backProp(run(inputs[p],0, outCol), outputs[p], outCol, 0.05);
        }
    }
    /////////////////////////////////////////////////////////////////////////////
    long double sum = 0;                                                      ///
    for (size_t i = 0; i < sz; ++i) {                                         ///
        std::vector<double> outp = run(inputs[i], 0, outCol);            ///
        for (size_t j = 0; j < neurons[(outCol + c) % c].size(); ++j) {       ///
            sum += (outp[j] - outputs[i][j]) * (outp[j] - outputs[i][j]);     ///
        }                                                                     ///
    }                                                                         ///
    std::cout << "Error = " << sum << '\n';                                   ///
    /////////////////////////////////////////////////////////////////////////////
}

template<typename activation_function>
const std::vector<double>& Perceptron<activation_function>::run(const std::vector<bool>& in, int from, int outCol) {
    for (size_t i = 0; i < neurons[from].size(); ++i) {
        neurons[from][i] = in[i];
    }
    if (outCol == -1) {
        outCol = c - 1;
    }
    for (int i = from + 1; i < outCol + 1; ++i) {
        for (size_t k = 0; k < neurons[i].size(); ++k) {
            double sum = 0;
            for (size_t j = 0; j < neurons[i - 1].size(); ++j) {
                sum += w[i - 1][j][k] * neurons[i - 1][j];
            }
            neurons[i][k] = act_func(sum + b[i - 1][k]);
        }
    }
    return neurons[outCol];
}

template<typename activation_function>
void Perceptron<activation_function>::backProp(const std::vector<double>& out, const std::vector<double>& output,
                                               int outCol, double alpha) {
    if (outCol == -1) {
        outCol = c - 1;
    }
    std::vector<double> del;
    std::vector<double> tmpDel(neurons[outCol].size());
    for (size_t i = 0; i < neurons[outCol].size(); ++i) {
        tmpDel[i] = out[i] - output[i];
    }
    for (int i = outCol - 1; i >= 0; --i) {
        del.resize(neurons[i].size());
        std::memset(del.data(), 0, neurons[i].size() * sizeof(double));

        for (size_t k = 0; k < neurons[i + 1].size(); ++k) {
            for (size_t j = 0; j < neurons[i].size(); ++j) {
                del[j] += tmpDel[k] * act_func_der(neurons[i + 1][k]) * w[i][j][k];
                w[i][j][k] -= spd * tmpDel[k] * act_func_der(neurons[i + 1][k]) * neurons[i][j];
            }
            b[i][k] -= spd * tmpDel[k] * act_func_der(neurons[i + 1][k]);
        }


        if (i == static_cast<int>(intLayer)) {
            for (size_t j = 0; j < neurons[intLayer].size(); ++j) {
                del[j] += alpha * (neurons[intLayer][j] - std::round(neurons[intLayer][j]));
            }
        }
        tmpDel = del;
    }
}

template<typename activation_function>
void Perceptron<activation_function>::randomize() {
    for (int i = 0; i < c - 1; ++i) {
        for (size_t k = 0; k < neurons[i + 1].size(); ++k) {
            for (size_t j = 0; j < neurons[i].size(); ++j) {
                w[i][j][k] = (1.0 * rnd()) / (std::mt19937::max());
            }
            b[i][k] = (1.0 * rnd()) / (std::mt19937::max());
        }
    }
}

template<typename activation_function>
const std::vector<std::vector<double>>& Perceptron<activation_function>::getCols() const {
    return neurons;
}

template<typename activation_function>
unsigned long long Perceptron<activation_function>::getWeightsNum() const {
    unsigned long long sum = 0;
    for (int i = 1; i < c - 1; ++i) {
        sum += neurons[i].size() * neurons[i + 1].size();
    }
    return sum + neurons[1].size();
}

template<typename activation_function>
std::vector<double> Perceptron<activation_function>::getWeights() const {
    std::vector<double> weights;
    weights.resize(getWeightsNum());
    int iter = 0;

    for (int k = 0; k < neurons[1].size(); ++k) {
        weights[iter] = w[0][0][k];
        ++iter;
    }
    for (int i = 1; i < c - 1; ++i) {
        for (int j = 0; j < neurons[i].size(); ++j) {
            for (int k = 0; k < neurons[i + 1].size(); ++k) {
                weights[iter] = w[i][j][k];
                ++iter;
            }
        }
    }
    return weights;
}

template<typename activation_function>
void Perceptron<activation_function>::setWeights(std::vector<double> weights) {
    for (int j = 0; j < neurons[0].size(); ++j) {
        for (int k = 0; k < neurons[1].size(); ++k) {
            w[0][j][k] = weights[k];
        }
    }
    int iter = neurons[1].size();
    for (int i = 1; i < c - 1; ++i) {
        for (int j = 0; j < neurons[i].size(); ++j) {
            for (int k = 0; k < neurons[i + 1].size(); ++k) {
                w[i][j][k] = weights[iter];
                ++iter;
            }
        }
    }
}

template<typename activation_function>
Perceptron<activation_function>::Perceptron(const std::vector<int>& cols) {
    c = cols.size();
    n = cols[0];
    m = cols[c - 1];
    neurons.resize(c);
    w.resize(c - 1);
    b.resize(c - 1);

    for (int i = 0; i < c; ++i) {
        neurons[i].resize(cols[i]);
        if (i != c - 1) {
            w[i].resize(cols[i]);
            b[i].resize(cols[i + 1]);
            for (int j = 0; j < cols[i]; ++j) {
                w[i][j].resize(cols[i + 1]);
            }
        }
    }
    randomize();
}
