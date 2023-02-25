#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int err = 0;
  int sign = get_sign(&value);
  *result = null_decimal();
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
    if (sign == 1) {
      set_bit(result, 127);
    }
  } else if (sign == 0) {
    s21_truncate(value, result);
  } else if (sign == 1) {
    s21_truncate(value, result);
    if (get_scale(&value)) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_sub(*result, one, result);
    }
  }
  return err;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  *result = null_decimal();
  int scale = get_scale(&value);
  int sign = get_sign(&value);
  *result = equate(value);
  result->bits[3] = 0;
  while (scale > 0) {
    *result = division_by_ten_integer(*result);
    scale--;
  }
  if (sign) {
    set_sign(result);
  }
  return err;
}

s21_decimal equate(s21_decimal value) { return value; }

s21_decimal division_by_ten_integer(s21_decimal dividend) {
  s21_decimal copy_dividend = equate(dividend);
  s21_decimal power_of_ten = {0};
  s21_decimal zero = {0};
  s21_decimal result = {0};
  int sign = get_sign(&copy_dividend);
  int scale = get_scale(&copy_dividend);
  set_scale(0, &copy_dividend);
  null_bit(&copy_dividend, 127);
  while (s21_is_less(zero, copy_dividend)) {
    int degree = 0;
    int equal_flag = 0;
    while (degree <= 28) {
      power_of_ten = get_power_of_ten(degree);
      if (s21_is_less(copy_dividend, power_of_ten)) {
        break;
      }
      if (s21_is_equal(copy_dividend, power_of_ten)) {
        equal_flag = 1;
        break;
      }
      degree++;
    }
    if (equal_flag == 1 || s21_is_less(power_of_ten, copy_dividend)) {
      if (degree > 28) {
        degree = 28;
      }
      s21_sub(copy_dividend, power_of_ten, &copy_dividend);
    } else {
      degree--;
      s21_sub(copy_dividend, get_power_of_ten(degree), &copy_dividend);
    }
    if (degree > 0) {
      s21_add(result, get_power_of_ten(degree - 1), &result);
    }
  }
  if (scale) {
    scale--;
    set_scale(scale, &result);
  }
  if (sign) {
    set_sign(&result);
  }
  return result;
}
