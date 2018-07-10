.PHONY: all
all:
	gcc -o mult mult.c


.PHONY: clean
clean:
	rm -f mult
