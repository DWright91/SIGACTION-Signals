all: compute.c controller.c
	gcc -o compute -std=gnu99 compute.c
	gcc -o controller -std=gnu99 controller.c
clean:
	rm -f compute
	rm -f controller
