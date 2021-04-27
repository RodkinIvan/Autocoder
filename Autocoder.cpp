#include "Autocoder.h"


std::vector<bool> getRaw(const std::string& s) {
    std::vector<bool> raw(s.size() * 8);
    for (size_t i = 0; i < s.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            raw[8 * i + j] = ((s[i] >> j) & 1);
        }
    }
    return raw;
}

std::vector<std::vector<bool>> getInput(const std::string& data, size_t chunkSize) {
    std::vector<bool> rawData = getRaw(data);
    for (size_t i = 0; i < (chunkSize - rawData.size() % chunkSize) % chunkSize; ++i) {
        rawData.push_back(false);
    }
    size_t chunkNum = rawData.size() / chunkSize;

    std::vector<std::vector<bool>> input(chunkNum, std::vector<bool>(chunkSize));

    for (size_t i = 0; i < chunkNum; ++i) {
        for (size_t j = 0; j < chunkSize; ++j) {
            input[i][j] = rawData[chunkSize * i + j];
        }
    }
    return input;
}

std::string makeString(std::vector<bool>& data){
    std::string res;
    for(size_t i = 0; i < (8-data.size()%8)%8; ++i){
        data.push_back(false);
    }
    for(size_t i = 0; i < data.size()/8; ++i){
        char c = 0;
        for(size_t j = 0; j < 8; ++j){
            c += short(data[8*i+j]) << j;
        }
        res.push_back(c);
    }
    return res;
}

Autocoder::Autocoder(int chinkSize, int to) : p({chinkSize, to, chinkSize}),
                                              chunkSize(chinkSize), outSize(to) { p.intLayer = 1; }

void Autocoder::learn(const std::string& data) {

    std::vector<std::vector<bool>> input = getInput(data, chunkSize);

    size_t chunkNum = input.size();

    std::vector<std::vector<double>> output(chunkNum, std::vector<double>(chunkSize));
    for (size_t i = 0; i < chunkNum; ++i) {
        for (size_t j = 0; j < chunkSize; ++j) {
            output[i][j] = input[i][j];
        }
    }

    p.learn(input, output);

}

std::vector<bool> Autocoder::encode(const std::string& data) {
    std::vector<std::vector<bool>> input = getInput(data, chunkSize);

    std::vector<double> coded(input.size() * outSize);
    std::vector<double> out(chunkSize);
    for (size_t i = 0; i < input.size(); ++i) {
        out = p.run(input[i], 0, static_cast<int>(p.intLayer));
        for (size_t j = 0; j < out.size(); ++j) {
            coded[outSize * i + j] = out[j];
        }
    }

    std::vector<bool> ans(coded.size());
    for (size_t i = 0; i < coded.size(); ++i) {
        ans[i] = bool(std::round(coded[i]));
    }
    return ans;
}

std::string Autocoder::decode(const std::vector<bool>& code) {

    size_t chunksNum = code.size() / outSize;
    std::vector<bool> decoded(chunksNum * chunkSize);
    std::vector<bool> toDecode(outSize);
    std::vector<double> out;
    for (size_t i = 0; i < chunksNum; ++i) {
        for (size_t j = 0; j < outSize; ++j) {
            toDecode[j] = code[outSize * i + j];
        }
        out = p.run(toDecode, static_cast<int>(p.intLayer), -1);
        for (size_t j = 0; j < chunkSize; ++j) {
            decoded[i * chunkSize + j] = bool(std::round(out[j]));
        }
    }
//    std::cout << "Decoded:          ";
//    for(auto i : decoded){
//        std::cout << i << ' ';
//    }
//    std::cout << '\n';
    return makeString(decoded);
}
