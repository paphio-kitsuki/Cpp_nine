#include "PmergeMe.hpp"
#include <list>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <string>

const std::string PmergeMe::NonPositiveException::ERROR_MESSAGE
        = "Error: input data contains non-positive integer.";

int get_good_index(int count) {
    int pre = 2;
    if (count == 0)
        return pre - 1;

    int base = 4;
    for (int i = 1; i < count; i++) {
        pre = base - pre;
        base *= 2;
    }
    return base - pre - 1;
}

static Element get_from_index(std::vector<Element>& vec, int index) {
    Element ret = vec[index];
    vec.erase(vec.begin() + index);
    return ret;
}

static Element get_from_index(std::list<Element>& list, int index) {
    std::list<Element>::iterator it = list.begin();
    while (index -- > 0)
        ++it;
    Element ret = *it;
    list.erase(it);
    return (ret);
}


PmergeMe::PmergeMe() : vec_data(), list_data() {
}

PmergeMe::PmergeMe(const PmergeMe& src) : vec_data(src.vec_data), list_data(src.list_data) {
}

PmergeMe& PmergeMe::operator = (const PmergeMe& src) {
    if (this == &src)
        return *this;
    this->vec_data = src.vec_data;
    this->list_data = src.list_data;
    return *this;
}

PmergeMe::~PmergeMe() {
}

void PmergeMe::inputDataToVec(const int src[], std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        if (src[i] <= 0)
            throw PmergeMe::NonPositiveException();
        this->vec_data.push_back(src[i]);
    }
}

void PmergeMe::inputDataToList(const int src[], std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        if (src[i] <= 0)
            throw PmergeMe::NonPositiveException();
        this->list_data.push_back(src[i]);
    }
}

void PmergeMe::execSortVec() {
    std::vector<Element> convert;
    for (std::vector<int>::iterator it = this->vec_data.begin(); it != this->vec_data.end(); ++it) {
        Element e = {*it, NULL, NULL};
        convert.push_back(e);
    }
    convert = this->fordJohnsonSort(convert);
    this->vec_data.clear();
    for (std::vector<Element>::iterator it = convert.begin(); it != convert.end(); ++it)
        this->vec_data.push_back((*it).value);
}

void PmergeMe::execSortList() {
    std::list<Element> convert;
    for (std::list<int>::iterator it = this->list_data.begin(); it != this->list_data.end(); ++it) {
        Element e = {*it, NULL, NULL};
        convert.push_back(e);
    }
    convert = this->fordJohnsonSort(convert);
    this->list_data.clear();
    for (std::list<Element>::iterator it = convert.begin(); it != convert.end(); ++it)
        this->list_data.push_back((*it).value);
}

const std::vector<int>& PmergeMe::getVec() const {
    return this->vec_data;
}

const std::list<int>& PmergeMe::getList() const {
    return this->list_data;
}

static std::vector<Element>::iterator elem_find(std::vector<Element>& vec, Element& e) {
    for (std::vector<Element>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if ((*it).value == e.value)
            return it;
    }
    return vec.end();
}

std::vector<Element> PmergeMe::fordJohnsonSort(std::vector<Element>& pre_data) {
    if (pre_data.empty() || pre_data.size() < 2)
        return pre_data;

    std::vector<Element> merge;
    for (std::vector<Element>::iterator it = pre_data.begin(); it != pre_data.end(); ++it) {
        Element e1 = {(*it).value, &(*it), NULL};
        ++it;
        if (it == pre_data.end())
            break;
        Element e2 = {(*it).value, &(*it), &e1};
        e1.pair = &e2;
        if (e1.value > e2.value)
            merge.push_back(e1);
        else
            merge.push_back(e2);
    }
    merge = PmergeMe::fordJohnsonSort(merge);

    std::vector<Element> ret;
    std::vector<Element> insert;
    for (std::vector<Element>::iterator it = merge.begin(); it != merge.end(); ++it) {
        if (it == merge.begin())
            ret.push_back(*((*it).pair->sub));
        else
            insert.push_back(*((*it).pair));
        ret.push_back(*((*it).sub));
    }
    if (pre_data.size() % 2 == 1) {
        Element tmp = {pre_data.back().value, &pre_data.back(), NULL};
        insert.push_back(tmp);
    }
    bool flag = true;
    for (int i = 0; !insert.empty(); i++) {
        Element tmp;
        if (flag) {
            std::size_t index = get_good_index(i);
            if (index >= insert.size()) {
                flag = false;
                continue;
            }
            tmp = get_from_index(insert, index);
            continue;
        } else {
            tmp = insert.back();
            insert.pop_back();
        }
        if (tmp.pair != NULL)
            binary_insert(ret, elem_find(ret, *tmp.pair), *(tmp.sub));
        else
            binary_insert(ret, ret.end(), *(tmp.sub));
    }
    return ret;
}

static std::list<Element>::iterator elem_find(std::list<Element>& vec, Element& e) {
    for (std::list<Element>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if ((*it).value == e.value)
            return it;
    }
    return vec.end();
}

std::list<Element> PmergeMe::fordJohnsonSort(std::list<Element>& pre_data) {
    if (pre_data.empty() || pre_data.size() < 2)
        return pre_data;

    std::list<Element> merge;
    for (std::list<Element>::iterator it = pre_data.begin(); it != pre_data.end(); ++it) {
        Element e1 = {(*it).value, &(*it), NULL};
        std::cout << e1.value << "??" << std::endl;
        ++it;
        if (it == pre_data.end())
            break;
        Element e2 = {(*it).value, &(*it), &e1};
        std::cout << e2.value << "??" << std::endl;
        e1.pair = &e2;
        if (e1.value > e2.value)
            merge.push_back(e1);
        else
            merge.push_back(e2);
    }
    std::cout << merge.front().pair->value << "?!?" << std::endl;
    merge = PmergeMe::fordJohnsonSort(merge);
    std::cout << merge.front().value << merge.front().pair->value << ", " << merge.size() << std::endl;

    std::list<Element> ret;
    std::list<Element> insert;
    for (std::list<Element>::iterator it = merge.begin(); it != merge.end(); ++it) {
        if (it == merge.begin())
            ret.push_back(*((*it).pair->sub));
        else
            insert.push_back(*((*it).pair));
        ret.push_back(*((*it).sub));
    }
    if (pre_data.size() % 2 == 1) {
        Element tmp = {pre_data.back().value, &pre_data.back(), NULL};
        insert.push_back(tmp);
        std::cout << insert.front().value << insert.size() << std::endl;
    }
    bool flag = true;
    for (int i = 0; !insert.empty(); i++) {
        Element tmp;
        if (flag) {
            std::size_t index = get_good_index(i);
            if (index >= insert.size()) {
                flag = false;
                continue;
            }
            tmp = get_from_index(insert, index);
            continue;
        } else {
            tmp = insert.back();
            insert.pop_back();
        }
        if (tmp.pair != NULL)
            binary_insert(ret, elem_find(ret, *tmp.pair), *(tmp.sub));
        else
            binary_insert(ret, ret.end(), *(tmp.sub));
    }
    return ret;
}

PmergeMe::NonPositiveException::NonPositiveException() : std::runtime_error(PmergeMe::NonPositiveException::ERROR_MESSAGE) {
}