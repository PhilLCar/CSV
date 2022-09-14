
test:
	@g++ -g -Iinc src/* main.cpp -o bin/main

lib: obj
	ar rcs lib/csv.a obj/csv.o

obj:
	@g++ -Iinc src/csv.cpp -o obj/csv.o

clean:
	@rm -f bin/*
	@rm -f obj/*
	@rm -f lib/*