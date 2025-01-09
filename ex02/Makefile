NAME        = PmergeMe

CC          = c++
FLAGS       = -Wall -Wextra -Werror -std=c++98
RM          = rm -rf

OBJDIR      = .objFiles

FILES       = main PmergeMe
SRC         = $(FILES:=.cpp)
OBJ         = $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADER      = PmergeMe.hpp

# Colors:
GREEN       =   \e[92;5;118m
YELLOW      =   \e[93;5;226m
GRAY        =   \e[33;2;37m
RESET       =   \e[0m
CURSIVE     =   \e[33;3m

# Debug
ifeq ($(DEBUG), 1)
   OPTS = -fsanitize=adress -g
endif

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(OBJ) $(OPTS) -o $(NAME)
	@printf "$(GREEN) - Executable ready.$(RESET)\n"

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(OPTS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR) $(OBJ)
	@printf "$(YELLOW) - Object files removed.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(YELLOW) - Executable removed.$(RESET)\n"

re: fclean all