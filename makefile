# MIT License
#
# Copyright(c) 2011-2019 João Vicente Souto and Bruno Izaias Bonotto
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Defines
CXX      = gcc
CPPFLAGS = `pkg-config --cflags gtk+-3.0` -std=c++11 -Wall
LDFLAGS  = 
LDLIBS   = -lstdc++ -lm `pkg-config --libs gtk+-3.0`

PHONY: main clean

# CPP Source Files
CPP_SRC += $(wildcard *.cpp)

# Object Files
OBJ = $(CPP_SRC:.cpp=.o)

# Builds All Object Files
main: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS) $(LDFLAGS)

# Cleans All Object Files
clean:
	rm -rf $(OBJ)
	rm main

# Builds a CPP Source file
%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@
