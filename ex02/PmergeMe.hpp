#ifndef P_MERGE_ME_HPP
# define P_MERGE_ME_HPP

# include <vector>
# include <list>
# include <iostream>
# include <cstddef>

typedef struct s_element {
    int value;
    struct s_element* sub;
    struct s_element* pair;
} Element;

class PmergeMe {
    private:
        std::vector<int> vec_data;
        std::list<int> list_data;

        static std::vector<Element> fordJohnsonSort(std::vector<Element>&);
        static std::list<Element> fordJohnsonSort(std::list<Element>&);
    public:
        PmergeMe(void);
        PmergeMe(const PmergeMe&);
        PmergeMe& operator = (const PmergeMe&);
        ~PmergeMe();

        void inputDataToVec(const int[], std::size_t);
        void inputDataToList(const int[], std::size_t);
        void execSortVec(void);
        void execSortList(void);
        const std::vector<int>& getVec(void) const;
        const std::list<int>& getList(void) const;

        class NonPositiveException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                NonPositiveException(void);
        };
};

int get_good_index(int);

template <typename T>
static void binary_insert(T& data, typename T::iterator end, Element e) {
    typename T::iterator start = data.begin();
    typename T::iterator mid = start;

    while (start != end) {
        mid = start;
        std::advance(mid, (std::distance(start, end) + 1) / 2);
        if ((*mid).value == e.value || mid == data.begin())
            break;
        else if ((*mid).value < e.value)
            start = ++mid;
        else
            end = --mid;
    }
    data.insert(mid, e);
}

#endif