GCC = gcc -Wall -Wextra -Werror
clang =  clang-format -i *.c *.h;

tmp: clear
	$(clang) $(GCC) $@.c -o $@; echo "\n"; ./tmp

sprintf: clear
	$(clang) $(GCC) $@.c -o $@;

clear:
	rm -rf tmp sprintf

valgrind: clear sprintf
	valgrind --tool=memcheck -s --leak-check=yes ./sprintf > /dev/null;


