CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -I.
LDFLAGS := -L.
LDLIBS := -lesin_13.3.0_x86-64

OBJS := src/ubicacio.o src/contenidor.o src/terminal.o

.PHONY: all clean tests test-ubicacio test-contenidor test-cataleg test-terminal test-public

all: driver_gestterm

driver_gestterm: $(OBJS) tests/driver_gestterm.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

src/ubicacio.o: src/ubicacio.cpp include/ubicacio.hpp include/reps/ubicacio.rep
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/contenidor.o: src/contenidor.cpp include/contenidor.hpp include/reps/contenidor.rep
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/terminal.o: src/terminal.cpp include/terminal.hpp include/reps/terminal.rep include/cataleg.hpp include/ubicacio.hpp include/contenidor.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: test-ubicacio test-contenidor test-cataleg test-terminal test-public

test-ubicacio: driver_gestterm
	cd tests && LD_LIBRARY_PATH=.. ../driver_gestterm < jp_public_ubicacio.in > /tmp/out_ubicacio && diff -u jp_public_ubicacio.res /tmp/out_ubicacio

test-contenidor: driver_gestterm
	cd tests && LD_LIBRARY_PATH=.. ../driver_gestterm < jp_public_contenidor.in > /tmp/out_contenidor && diff -u jp_public_contenidor.res /tmp/out_contenidor

test-cataleg: driver_gestterm
	cd tests && LD_LIBRARY_PATH=.. ../driver_gestterm < jp_public_cataleg.in > /tmp/out_cataleg && diff -u jp_public_cataleg.res /tmp/out_cataleg

test-terminal: driver_gestterm
	cd tests && LD_LIBRARY_PATH=.. ../driver_gestterm < jp_public_terminal.in > /tmp/out_terminal && diff -u jp_public_terminal.res /tmp/out_terminal

test-public: driver_gestterm
	cd tests && LD_LIBRARY_PATH=.. ../driver_gestterm < jp_public.in > /tmp/out_public && diff -u jp_public.res /tmp/out_public

clean:
	$(RM) $(OBJS) driver_gestterm
