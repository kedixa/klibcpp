# makefile

CXX = g++
RM = rm -f
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra
INCLUDE_DIR = ../include

LIB = ../lib/libklibcpp.a
SOURCES = ${wildcard *.cpp}
TARGETS = ${patsubst %.cpp, %.out, $(SOURCES)}
CHECKS  = ${patsubst %.cpp, %.check, $(SOURCES)}

all: $(LIB) $(TARGETS)

%.out: %.cpp
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) $< $(LIB) -o $@

check: $(CHECKS)

%.check: %.out
	./$<

clean:
	$(RM) $(TARGETS)


