GCC = gcc -Wall -Wextra -Werror
clang =  clang-format -i *.c *.h;

tmp: clear
	$(clang) $(GCC) $@.c -o $@;

sprintf: clear
	$(clang) $(GCC) $@.c -o $@;

clear:
	rm -rf tmp sprintf


