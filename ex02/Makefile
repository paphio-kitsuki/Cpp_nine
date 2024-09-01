NAME	= PmergeMe
SRCS	= main.cpp PmergeMe.cpp
OBJS	= $(SRCS:.cpp=.o)
CXX		= c++
RM		= rm -f
CXXFLAGS	= -Wall -Werror -Wextra -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
