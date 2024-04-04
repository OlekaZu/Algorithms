CC := g++ --std=c++17
CFLAGS := -Wall -Werror -Wextra
ANT_DIR := AntAlgorithm
HEADERS := libs/my_matrix.h MatrixParser/matrix_parser.h $(ANT_DIR)/ant_interface.h \
	$(ANT_DIR)/solve_tsp.h $(ANT_DIR)/ant_colony.h 
SRC_MATRIX := libs/my_matrix.cc
OBJ_MATRIX = my_matrix.o
SRC_ANT :=  MatrixParser/matrix_parser.cc $(ANT_DIR)/main.cc $(ANT_DIR)/ant_interface.cc \
	$(ANT_DIR)/solve_tsp.cc $(ANT_DIR)/ant_colony.cc
SOURCES = $(SRC_ANT) $(SRC_MATRIX)
APP_NAME := StartProgram
INCLUDE_PATH = -I ./MatrixParser -I ./libs

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
	LDFLAGS=
	OPENFILE=open
else
	LDFLAGS=-pthread -lpthread -lrt -lm -ldl -lstdc++fs
	OPENFILE=xdg-open 
endif

.PHONY : all
.PHONY : clean

all: ant

matrix.a: $(SRC_MATRIX)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $(SRC_MATRIX)
	@ar rcs libmatrix.a $(OBJ_MATRIX)
	@ranlib libmatrix.a
	@rm -f $(OBJ_MATRIX)

ant: matrix.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE_PATH) $(SRC_ANT) -L . -lmatrix -o  $(ANT_DIR)/$(APP_NAME)
	@clear
	./$(ANT_DIR)/$(APP_NAME)

uninstall:
	rm -f *.a
	rm -f $(ANT_DIR)/$(APP_NAME)

clean: uninstall
	rm -f *.out
	rm -rf .vscode

clang:
	clang-format -i $(SOURCES) $(HEADERS)

#----Checking----
linter:
	echo "--------Linter Clang-Format--------"
	clang-format -n $(SOURCES) $(HEADERS)
leaks:
	echo "--------Leaks Checking--------"
	CK_FORK=no leaks -atExit -- ./$(ANT_DIR)/$(APP_NAME) 
cppcheck:
	echo "--------Static Code Checking--------"
	cppcheck --enable=all --suppress=missingInclude --suppress=unusedFunction ./
valgrind_ant:
	CK_FORK=no valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./$(ANT_DIR)/$(APP_NAME)