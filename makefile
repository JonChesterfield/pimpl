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

example.o:	example.cpp example.hpp pimpl.hpp pimpl_impl.hpp
	${CXX} ${CXXFLAGS} -c $< -o $@

test_example.o:	test_example.cpp example.hpp pimpl.hpp pimpl_impl.hpp catch.hpp
	${CXX} ${CXXFLAGS} -c $< -o $@

catch.o:	catch.cpp catch.hpp
	${CXX} ${CXXFLAGS} -c -O3 $< -o $@

${EXE}:	example.o test_example.o catch.o
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.o *.exe
