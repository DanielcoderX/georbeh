CC = gcc
flag = -lreadline
build = build/
NAME = georbeh
ALL: build run
build: clean
	mkdir $(build)
	$(CC) src/main.c -o build/$(NAME) $(flag)
clean:
	rm -rdf $(build)
run:
	./$(build)/$(NAME)