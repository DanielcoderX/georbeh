CC = gcc
flag = -lreadline -w -fno-stack-protector
build = build/
src = src/
name = georbeh
ALL: build run
build: clean
	mkdir $(build)
	$(CC) $(src)*.c -o $(build)$(name) $(flag)
clean:
	rm -rdf $(build)
run:
	./$(build)$(name)
install: build
	sudo cp -r ./$(build)$(name) /usr/bin/$(name)
	sudo chmod +x /usr/bin/$(name)
