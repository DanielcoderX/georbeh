CC = gcc
flag = -lreadline -w -fno-stack-protector
build = build/
name = georbeh
ALL: build run
build: clean
	mkdir $(build)
	$(CC) src/main.c -o $(build)$(name) $(flag)
clean:
	rm -rdf $(build)
run:
	./$(build)$(name)
install: build
	sudo cp -r ./$(build)$(name) /usr/bin/georbeh