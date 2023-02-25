CC = gcc
Flags = -Wall -Wextra -Werror
FILE = s21_decimal
PLATFORM := $(shell uname)

# Обязательные цели (all, clean, test, s21_decimal.a, gcov_report)

all : clean s21_decimal.a test gcov_report

clean :
	rm -rf s21_test
	rm -rf *.out
	rm -rf *.html
	rm -rf *.css
	rm -rf report
	rm -rf ./lcov_report
	rm -rf *.o *.a *.gcno *.gcda *.gcov *.info test_with_gcov gcov_report

ifeq ($(PLATFORM), Darwin)
test :
	gcc -c -std=c11 -Wall -Werror -Wextra test_decimal.c s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c
	gcc -o s21_test *.o -L /usr/local/lib -lcheck 
	./s21_test
else
test :
	gcc -c -std=c11 -Wall -Werror -Wextra test_decimal.c s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c
	gcc -o s21_test *.o -L /usr/local/lib -lcheck -lsubunit -lpthread -lrt -lm 
	./s21_test
endif

s21_decimal.a : s21_another.o s21_arithmetic.o s21_comparison.o s21_converters.o s21_helpers.o
	ar -rc s21_decimal.a s21_another.o s21_arithmetic.o s21_comparison.o s21_converters.o s21_helpers.o

# Промежуточная цель для создания библиотеки
s21_decimal.o : s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c
	$(CC) -c s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c
	
#------------------------------------------
ifeq ($(PLATFORM), Darwin)
gcov_report :
	gcc -c -std=c11 -Wall -Werror -Wextra s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c --coverage
	ar rcs gcov_s21_decimal.a *.o

	gcc -c -std=c11 -Wall -Werror -Wextra test_decimal.c
	gcc -o test_with_gcov test_decimal.o gcov_s21_decimal.a -L /usr/local/lib -lcheck -lgcov
	./test_with_gcov
	
	lcov -t 'gcov_report' -o gcov_report.info -c -d .
	genhtml -o gcov_report gcov_report.info
	open gcov_report/index.html
else
gcov_report :
	gcc -c -std=c11 -Wall -Werror -Wextra s21_another.c s21_arithmetic.c s21_comparison.c s21_converters.c s21_helpers.c --coverage
	ar rcs gcov_s21_decimal.a *.o

	gcc -c -std=c11 -Wall -Werror -Wextra test_decimal.c
	gcc -o test_with_gcov test_decimal.o gcov_s21_decimal.a -L /usr/local/lib -lcheck -lgcov -lsubunit -lpthread -lrt -lm 
	./test_with_gcov
	
	lcov -t 'gcov_report' -o gcov_report.info -c -d .
	genhtml -o gcov_report gcov_report.info
	open gcov_report/index.html
endif

# Проверки на стиль, код, утечки памяти
# (cpplint, cppcheck, leaks, valgrind)

style : 
	clang-format -n s21_arithmetic.c
	clang-format -n s21_another.c
	clang-format -n s21_comparison.c
	clang-format -n s21_converters.c
	clang-format -n s21_helpers.c
	clang-format -n s21_decimal.h
	clang-format -n test_decimal.c

