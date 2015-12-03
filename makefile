CXX = clang++ -std=c++11
CXXFLAGS =
CXXFLAGS += -O0 -g
CXXFLAGS += -Wall
CXXFLAGS += -Wextra

EXE = pimpl.exe

all:	${EXE}
	./${EXE}

valgrind:	${EXE}
	valgrind ./${EXE}

pimpl.o:	pimpl.cpp pimpl.hpp
	${CXX} ${CXXFLAGS} -c $< -o $@

catch.o:	catch.cpp catch.hpp
	${CXX} ${CXXFLAGS} -c -O3 $< -o $@

${EXE}:	pimpl.o catch.o
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.o *.exe
