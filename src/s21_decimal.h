#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

//#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for converters
#define S21_INF 1.0 / 0.0
#define S21_MINUS_INF log(0)
#define S21_NAN 0.0 / 0.0

// for arithmetic
#define OK_A 0
#define S21_INF_A 1
#define S21_NEG_INF_A 2
#define S21_NAN_A 3

// def from open source
#define ADD_BIT(X, POS) ((X) |= (1U << (POS)))
#define ZERO_BIT(X, POS) (X &= ~(1UL << POS))

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef enum { SUCCESS, CONVERTING_ERROR } e_cast_code_t;

enum {
  LOW,
  MID,
  HIGH,
  SCALE,
};

typedef struct {
  unsigned int bits[6];
} s21_big_decimal;

// for comparison
enum { FALSE, TRUE };

int s21_from_int_to_decimal(int src, s21_decimal *dst);
void set_bit(s21_decimal *d, int index);
int get_bit(s21_decimal *d, int index);
void set_sign(s21_decimal *d);
int get_bin_exp(float f);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int get_bit_of_integer(int src, int index);
int set_scale(int scale, s21_decimal *dst);
int get_bit_of_float(float src, int index);
int get_scale(s21_decimal *dec);
int get_sign(s21_decimal *d);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
void null_bit(s21_decimal *src, int position);
void set_bit_1(s21_decimal *n, int pos);
void set_bit_0(s21_decimal *n, int pos);
s21_decimal get_power_of_ten(int pow);
s21_decimal binary_multiplication(s21_decimal value_1, s21_decimal value_2,
                                  int *err);
s21_decimal equate(s21_decimal value);
s21_decimal division_by_ten_integer(s21_decimal dividend);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
s21_decimal null_decimal();

// Comparison functions
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// Arithmetic functions
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void copy_decimal(s21_decimal *dest, s21_decimal *src);
int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void shift_left(s21_decimal *dec);
void upper_scale(s21_decimal *dec);
void lower_scale(s21_decimal *dec);
s21_decimal div_no_scale(s21_decimal dividend, s21_decimal divisor);
void clean_decimal(s21_decimal *value);
s21_decimal dop_cod(s21_decimal value);  // переводим отриц. число в доп код
void set_scale_A(s21_decimal *value, int scale_value);
s21_decimal modul(s21_decimal *value);
void mul_10(s21_decimal *value, int scale_dif);
void same_scale(s21_decimal *dec1, s21_decimal *dec2);
int check_zero(s21_decimal value);

// big_decimal
void clean_big_decimal(s21_big_decimal *big);
s21_big_decimal div_big_on_ten(s21_big_decimal big1);
int get_bit_big(s21_big_decimal big, int index);
void set_bit_big(s21_big_decimal *big, int index);
void decimal_to_big_decimal(s21_big_decimal *dst, s21_decimal *src);
void big_to_decimal(s21_decimal *dst, s21_big_decimal *src);
s21_big_decimal sum_big_decimal(s21_big_decimal big1, s21_big_decimal big2);
void shift_left_big(s21_big_decimal *big);
void shift_right_big(s21_big_decimal *big);
int is_greater_or_equal_big(s21_big_decimal big1, s21_big_decimal big2);
void big_dop_code(s21_big_decimal *big);
void copy_big(s21_big_decimal *big_dst, s21_big_decimal *big_src);

static const char *const binary_powers_of_ten[29] = {
    [0] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000001",
    [1] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000001010",
    [2] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000001100100",
    [3] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000001111101000",
    [4] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000010011100010000",
    [5] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000011000011010100000",
    [6] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000011110100001001000000",
    [7] =
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00100110001001011010000000",
    [8] =
        "0000000000000000000000000000000000000000000000000000000000000000000001"
        "01111101011110000100000000",
    [9] =
        "0000000000000000000000000000000000000000000000000000000000000000001110"
        "11100110101100101000000000",
    [10] =
        "0000000000000000000000000000000000000000000000000000000000000010010101"
        "00000010111110010000000000",
    [11] =
        "0000000000000000000000000000000000000000000000000000000000010111010010"
        "00011101101110100000000000",
    [12] =
        "0000000000000000000000000000000000000000000000000000000011101000110101"
        "00101001010001000000000000",
    [13] =
        "0000000000000000000000000000000000000000000000000000100100011000010011"
        "10011100101010000000000000",
    [14] =
        "0000000000000000000000000000000000000000000000000101101011110011000100"
        "00011110100100000000000000",
    [15] =
        "0000000000000000000000000000000000000000000000111000110101111110101001"
        "00110001101000000000000000",
    [16] =
        "0000000000000000000000000000000000000000001000111000011011110010011011"
        "11110000010000000000000000",
    [17] =
        "0000000000000000000000000000000000000001011000110100010101111000010111"
        "01100010100000000000000000",
    [18] =
        "0000000000000000000000000000000000001101111000001011011010110011101001"
        "11011001000000000000000000",
    [19] =
        "0000000000000000000000000000000010001010110001110010001100000100100010"
        "01111010000000000000000000",
    [20] =
        "0000000000000000000000000000010101101011110001110101111000101101011000"
        "11000100000000000000000000",
    [21] =
        "0000000000000000000000000011011000110101110010011010110111000101110111"
        "10101000000000000000000000",
    [22] =
        "0000000000000000000000100001111000011001111000001100100110111010101100"
        "10010000000000000000000000",
    [23] =
        "0000000000000000000101010010110100000010110001111110000101001010111101"
        "10100000000000000000000000",
    [24] =
        "0000000000000000110100111100001000011011110011101100110011101101101000"
        "01000000000000000000000000",
    [25] =
        "0000000000001000010001011001010100010110000101000000000101001000010010"
        "10000000000000000000000000",
    [26] =
        "0000000001010010101101111101001011011100110010000000110011010010111001"
        "00000000000000000000000000",
    [27] =
        "0000001100111011001011100011110010011111110100001000000000111100111010"
        "00000000000000000000000000",
    [28] =
        "0010000001001111110011100101111000111110001001010000001001100001000100"
        "00000000000000000000000000",
};

#endif  // SRC_S21_DECIMAL_H_