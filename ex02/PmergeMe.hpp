#ifndef P_MERGE_ME_HPP
# define P_MERGE_ME_HPP

# include <vector>
# include <list>
# include <cstddef>

typedef struct s_element {
    int value;
    int parent;
} Element;

class PmergeMe {
    private:
        std::vector<int> vec_data;
        std::list<int> list_data;
    public:
        PmergeMe(int[]);

        template <typename T>
        void fordJohnsonSort(T&);
};

template <typename T>
void PmergeMe::fordJohnsonSort(T& t) {
    if (t.size() < 2)
        return;

    std::size_t elems_size = t.size() / 2;
    Element elems[] = new Element[elems_size];
    int index = 0;

    for (T::iterator it = t.begin(); it < t.end() - 1; it += 2) {
        Element e = {std::max(*it, *(it + 1)), std::min(*it, *(it + 1))};
        elems[index ++] = e;
    }
    for (int i = 0;)

    delete[] elems_size;
}

#endif