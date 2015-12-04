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

pimpl_operators.i: gen_pimpl_operators.py
	python $< > $@

pimpl.o:	pimpl.cpp pimpl.hpp pimpl_operators.i
	${CXX} ${CXXFLAGS} -c $< -o $@

test_pimpl.o:	test_pimpl.cpp pimpl.hpp catch.hpp
	${CXX} ${CXXFLAGS} -c $< -o $@

catch.o:	catch.cpp catch.hpp
	${CXX} ${CXXFLAGS} -c -O3 $< -o $@

${EXE}:	pimpl.o test_pimpl.o catch.o
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.i
	rm -f *.o *.exe
