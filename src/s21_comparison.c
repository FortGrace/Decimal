#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int a, b, res = FALSE;
  same_scale(&value_1, &value_2);
  if (check_zero(value_1) && check_zero(value_2)) {
    res = FALSE;
  } else {
    if (get_sign(&value_1) > get_sign(&value_2)) {
      res = TRUE;
    } else if (get_sign(&value_1) == 0 && get_sign(&value_2) == 0) {
      for (int i = 95; i >= 0; i--) {
        a = get_bit(&value_1, i);
        b = get_bit(&value_2, i);
        if (a == b) continue;
        if (a < b) {
          res = 1;
        } else {
          res = 0;
        }
        break;
      }
    } else if (get_sign(&value_1) == 1 && get_sign(&value_2) == 1) {
      for (int i = 95; i >= 0; i--) {
        a = get_bit(&value_1, i);
        b = get_bit(&value_2, i);
        if (a == b) continue;
        if (a > b) {
          res = 1;
        } else {
          res = 0;
        }
        break;
      }
    }
  }
  return res;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = FALSE;
  same_scale(&value_1, &value_2);
  if (check_zero(value_1) && check_zero(value_2)) {
    res = TRUE;
  } else {
    if (get_sign(&value_1) == get_sign(&value_2)) {
      if ((value_1.bits[2]) == (value_2.bits[2]) &&
          (value_1.bits[1]) == (value_2.bits[1]) &&
          (value_1.bits[0]) == (value_2.bits[0])) {
        res = TRUE;
      }
    }
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = FALSE;
  if (s21_is_equal(value_1, value_2) || s21_is_less(value_1, value_2))
    res = TRUE;
  return res;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_2, value_1);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = FALSE;
  if (s21_is_equal(value_1, value_2) || !s21_is_less(value_1, value_2))
    res = TRUE;
  return res;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}