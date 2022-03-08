build:
	gcc -Wall -Wextra -std=c99 main.c image.c matrix.c rotate.c commands.c -o image_editor -lm

run:
	valgrind ./image_editor

clean:
	rm image_editor

.PHONY: clean
