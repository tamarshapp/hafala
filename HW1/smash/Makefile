# Makefile for the smash program
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
CCLINK = $(CXX)
OBJS = smash.o commands.o signals.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) $(CXXFLAGS) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h signals.h
	$(CXX) $(CXXFLAGS) -c -fpic commands.cpp
smash.o: smash.cpp commands.h
	$(CXX) $(CXXFLAGS) -c -fpic smash.cpp
signals.o: signals.cpp signals.h commands.h
	$(CXX) $(CXXFLAGS) -c -fpic  signals.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o smash *~ "#"* core.*

