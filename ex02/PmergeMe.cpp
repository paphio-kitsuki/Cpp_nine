#include "PmergeMe.hpp"
#include <list>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <string>

const std::string PmergeMe::NonPositiveException::ERROR_MESSAGE
        = "Error: input data contains non-positive integer.";

static std::pair<Element, Element*> get_from_index(std::list<std::pair<Element, Element*> >& list, int index) {
    std::list<std::pair<Element, Element*> >::iterator it = list.begin();
    while (index -- > 0)
        ++it;
    std::pair<Element, Element*> ret = *it;
    list.erase(it);
    return (ret);
}

static std::vector<Element>::iterator elem_find(std::vector<Element>& vec, Element& e) {
    for (std::vector<Element>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if ((*it).value == e.value)
            return it;
    }
    return vec.end();
}

static std::list<Element>::iterator elem_find(std::list<Element>& vec, Element& e) {
    for (std::list<Element>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if ((*it).value == e.value)
            return it;
    }
    return vec.end();
}

static Element* make_elem(Element& e, Element* pair) {
    Element *ret = new Element;
    ret->value = e.value;
    ret->sub = &e;
    ret->pair = pair;
    return ret;
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
    std::vector<Element*> delete_elems;

    for (std::vector<int>::iterator it = this->vec_data.begin(); it != this->vec_data.end(); ++it) {
        Element e = {*it, NULL, NULL};
        Element *e2 = make_elem(e, NULL);
        convert.push_back(*e2);
        delete_elems.push_back(e2);
    }
    convert = PmergeMe::ford_johnson_sort(convert);
    delete_all_elems(delete_elems);
    this->vec_data.clear();
    for (std::vector<Element>::iterator it = convert.begin(); it != convert.end(); ++it)
        this->vec_data.push_back((*it).value);
}

void PmergeMe::execSortList() {
    std::list<Element> convert;
    std::list<Element*> delete_elems;

    for (std::list<int>::iterator it = this->list_data.begin(); it != this->list_data.end(); ++it) {
        Element e = {*it, NULL, NULL};
        Element *e2 = make_elem(e, NULL);
        convert.push_back(*e2);
        delete_elems.push_back(e2);
    }
    convert = PmergeMe::ford_johnson_sort(convert);
    delete_all_elems(delete_elems);
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

int PmergeMe::get_good_index_list(int pre_count, int count) {
    if (pre_count < 0)
        return 1;

    int border = 0;
    int base = 1;
    while (count -- > 0) {
        base *= 2;
        border = base - border;
    }

    if (pre_count == 0)
        return 2 * base - border - 1;
    return pre_count - 1;
}

int PmergeMe::get_good_index_vec(int pre_count) {
    if (pre_count < 0)
        return 1;

    int border = 2;
    int base = 2;
    int ret = -1;
    pre_count--;
    while (ret < pre_count) {
        base *= 2;
        ret += border;
        border = base - border;
    }

    if (ret == pre_count)
        return ret + 2 * base;
    return pre_count;
}

std::vector<Element> PmergeMe::ford_johnson_sort(std::vector<Element>& pre_data) {
    if (pre_data.empty() || pre_data.size() < 2)
        return pre_data;

    std::vector<Element*> delete_elems;
    std::vector<Element> merge;
    for (std::vector<Element>::iterator it = pre_data.begin(); it != pre_data.end(); ++it) {
        Element *e1 = make_elem(*it, NULL);
        delete_elems.push_back(e1);
        ++it;
        if (it == pre_data.end())
            break;
        Element *e2 = make_elem(*it, NULL);
        delete_elems.push_back(e2);
        e1->pair = e2;
        e2->pair = e1;
        if (e1->value > e2->value)
            merge.push_back(*e1);
        else
            merge.push_back(*e2);
    }

/* debug
    for (std::vector<Element>::iterator it = merge.begin(); it != merge.end(); ++it)
        std::cout << (*it).value << ", " << (*it).pair->value << std::endl;
    std::cout << std::endl;
*/

    merge = PmergeMe::ford_johnson_sort(merge);

/* debug
    for (std::vector<Element>::iterator it = merge.begin(); it != merge.end(); ++it)
        std::cout << (*it).value << ", " << (*it).pair->value << std::endl;
    std::cout << std::endl;
*/

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
        Element *tmp = make_elem(pre_data.back(), NULL);
        delete_elems.push_back(tmp);
        insert.push_back(*tmp);
    }

    bool is_index_over = false;
    std::size_t index = -1;
    while (true) {
        index = PmergeMe::get_good_index_vec(index);
        if (index >= insert.size()) {
            if (is_index_over)
                break;
            index = insert.size();
            is_index_over = true;
            continue;
        }
        Element tmp = insert[index];
        if (tmp.pair != NULL)
            binary_insert(ret, ++ elem_find(ret, *(tmp.pair)), *(tmp.sub));
        else
            binary_insert(ret, ret.end(), *(tmp.sub));
    }
    delete_all_elems(delete_elems);

/* debug
    for (std::vector<Element>::iterator it = ret.begin(); it != ret.end(); ++it)
        std::cout << (*it).value << std::endl;
    std::cout << std::endl;
*/

    return ret;
}

std::list<Element> PmergeMe::ford_johnson_sort(std::list<Element>& pre_data) {
    if (pre_data.empty() || pre_data.size() < 2)
        return pre_data;

    std::list<Element*> delete_elems;
    std::list<Element> merge;
    for (std::list<Element>::iterator it = pre_data.begin(); it != pre_data.end(); ++it) {
        Element& pre1 = *it;
        ++it;
        if (it == pre_data.end())
            break;
        Element& pre2 = *it;
        Element* e;
        if (pre1.value > pre2.value)
            e = make_elem(pre1, &pre2);
        else
            e = make_elem(pre2, &pre1);
        merge.push_back(*e);
        delete_elems.push_back(e);
    }

    // for (std::list<Element>::iterator it = merge.begin(); it != merge.end(); ++it)
    //     std::cout << (*it).value << ", " << (*it).pair->value << std::endl;
    // std::cout << std::endl;

    merge = PmergeMe::ford_johnson_sort(merge);

    // for (std::list<Element>::iterator it = merge.begin(); it != merge.end(); ++it)
    //     std::cout << (*it).value << ", " << (*it).pair->value << std::endl;
    // std::cout << std::endl;

    std::list<Element> ret;
    std::list<std::pair<Element, Element*> > insert;
    for (std::list<Element>::iterator it = merge.begin(); it != merge.end(); ++it) {
        if (it == merge.begin())
            ret.push_back(*((*it).pair));
        else
            insert.push_back(std::make_pair(*((*it).pair), &(*it)));
        ret.push_back(*((*it).sub));
    }
    if (pre_data.size() % 2 == 1)
        insert.push_back(std::make_pair(pre_data.back(), static_cast<Element*>(NULL)));

    bool is_index_over = false;
    int count = 0;
    for (std::size_t index = -1; !insert.empty();) {
        std::pair<Element, Element*> tmp;
        if (!is_index_over) {
            std::size_t tmp_index = PmergeMe::get_good_index_list(index, count);
            if (index <= tmp_index)
                count++;
            index = tmp_index;
            if (index >= insert.size()) {
                is_index_over = true;
                continue;
            }
            tmp = get_from_index(insert, index);
        } else {
            tmp = insert.back();
            insert.pop_back();
        }
        if (tmp.second != NULL)
            binary_insert(ret, elem_find(ret, *(tmp.second)), tmp.first);
        else
            binary_insert(ret, ret.end(), tmp.first);
    }
    delete_all_elems(delete_elems);

    // for (std::list<Element>::iterator it = ret.begin(); it != ret.end(); ++it)
    //     std::cout << (*it).value << std::endl;
    // std::cout << std::endl;

    return ret;
}

PmergeMe::NonPositiveException::NonPositiveException() : std::runtime_error(PmergeMe::NonPositiveException::ERROR_MESSAGE) {
}