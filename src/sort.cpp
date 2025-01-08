#include "sort.h"

void Sort::radixSort(std::vector<std::string>& arr) {
    int maxLength = 0;
    for (const auto &str : arr) {
        maxLength = std::max(maxLength, static_cast<int>(str.size()));
    }

    for (int pos = maxLength - 1; pos >= 0; --pos) {
        std::vector<std::vector<std::string>> buckets(256);

        for (const auto &str : arr) {
            char ch = getCharAt(str, pos);
            buckets[ch].push_back(str);
        }
        arr.clear();
        for (const auto &bucket : buckets) {
            for (const auto &str : bucket) {
                arr.push_back(str);
            }
        }
    }
}
char Sort::getCharAt(const std::string& str, int index) {
    if (index < str.size()) {
        return str[index];
    }
    return '\0';
}