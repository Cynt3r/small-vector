TESTS=test_run
CC=g++
CFLAGS=-std=c++17 -Wall -pedantic

all: compile

compile: $(TESTS)

test: $(TESTS)
	@./$(TESTS)

$(TESTS): main.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(TESTS) 2>/dev/null