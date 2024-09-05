#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <exception>
#include <ctime>

int main(int argc, char **argv) {
    if (argc < 1) {
        std::cerr << "Error: invalid argc." << std::endl;
        return 1;
    }
    int input;
    std::vector<int> buffer;
    for (int i = 1; i < argc; i++) {
        std::stringstream ss;
        ss << argv[i];
        ss >> input;
        if (ss.fail() || !ss.eof()) {
            std::cerr << "Error: input value is wrong." << std::endl;
            return 1;
        }
        buffer.push_back(input);
    }
    int data[buffer.size()];
    std::copy(buffer.begin(), buffer.end(), data);
    PmergeMe pmergeMe;
    clock_t vec_clock_start;
    clock_t vec_clock_end;
    clock_t list_clock_start;
    clock_t list_clock_end;
    try {
        vec_clock_start = std::clock();
        pmergeMe.inputDataToVec(data, buffer.size());
        pmergeMe.execSortVec();
        vec_clock_end = std::clock();
        list_clock_start = std::clock();
        pmergeMe.inputDataToList(data, buffer.size());
        pmergeMe.execSortList();
        list_clock_end = std::clock();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "Before:\t";
    for (std::size_t i = 0; i < buffer.size(); i++)
        std::cout << " " << data[i];
    std::cout << std::endl;
    std::cout << "After:\t";
    const std::list<int>& result = pmergeMe.getList();
    for (std::list<int>::const_iterator it = result.begin(); it != result.end(); ++it)
        std::cout << " " << *it;
    std::cout << std::endl;

    // shell コマンドで、 seq 1 10000 | shuf  とすると、1〜10000のすべての整数を用いたランダム数列が生成される
    //
    // std::cout << "debug: is sorted: ";
    // bool flag = true;
    // std::size_t i = 1;
    // for (std::list<int>::const_iterator it = result.begin(); it != result.end(); ++it) {
    //     if (i != static_cast<std::size_t>(*it)) {
    //         flag = false;
    //         break;
    //     }
    //     i++;
    // }
    // std::cout << (flag ? "OK!" : "NG...") << std::endl;

    std::cout << "Time to process a range of " << buffer.size() << " elements with std::vector : ";
    std::cout << (double)(vec_clock_end - vec_clock_start) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    std::cout << "Time to process a range of " << buffer.size() << " elements with std::list : ";
    std::cout << (double)(list_clock_end - list_clock_start) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
    return 0;
}