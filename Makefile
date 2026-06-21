# Sudoku Game - Makefile
# 支持 Windows (MinGW/Git Bash) 和 Linux

CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET   = sudoku
SRCS     = main.cpp sudoku.cpp
OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp sudoku.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
