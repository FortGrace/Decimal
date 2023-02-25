#include "s21_decimal.h"

/*
Перевод числа int
в s21_decimal
*/
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  *dst = null_decimal();
  int result_code = 0;
  if (src == S21_INF || src == S21_MINUS_INF || src == S21_NAN) {
    result_code = 1;
  } else {
    if (src < 0) {
      src *= -1;
      set_sign(dst);
    }
    dst->bits[LOW] = src;
  }
  return result_code;
}

/*
Общая логика:

Пример: src = 450.250104.
1. Домнажаем src на 10, пока оно не станет максимум 8-разрядным, но при каждом
умножении проверям значение дробной части. Если оно больше 0, то продолжаем
домножение. Каждую итерацию считаем: src = 45025010. Потребовалось 5 умножений,
т.е. scale = 5.
2. Далее побитово заполняем младшее слово (также среднее и старшее, если число
большое).: 2.1. После домнажения пользуемся функцией get_bin_exp и вычисляем
новую экспоненту для домноженного src 2.2. С помощью функции set_bit
устанавливаем бит на позицию, полученую в результате функции get_bin_exp 2.3.
Далее последовательно делаем побитовое сложение (&) с побитовым представлением
src
3. Скейл, который мы нашли в первом шаге, вставляем в SCALE децимала
4. Готово!

P.S. В программе пример может сработать иначе, т.к. тип данных float может с
самого начала потерять точность. Этот пример необходим лишь для объяснения
логики функции.
*/
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  *dst = null_decimal();
  int result = 0;
  if (src == S21_INF || src == S21_MINUS_INF || src == S21_NAN) {
    result = 1;
  }
  if (get_bit_of_float(src, 31)) {
    src *= -1;
    set_sign(dst);
  }
  int scale = 0;
  int bin_exp = get_bin_exp(src);
  if (bin_exp > 95) {
    result = 1;
  } else {
    double left = 0;
    double right = 0;
    right = modf(src, &left);
    if (right > 0.0) {
      for (int i = 0; right > 0.0 && src < 1E+8; scale++, i++) {
        src *= 10;
        right = modf(src, &left);
        /*
         округление (если дано число 0.03, то без такого
          округления число будет умножаться как 0.2999999..)
         */
        if (right >= 1 - (1E-6)) {
          scale++;
          break;
        }
      }
    }
    src = round(src);
    bin_exp = get_bin_exp(src);
    set_bit(dst, bin_exp);
    bin_exp--;
    int bit_index_src = 22;
    for (; bit_index_src >= 0; bin_exp--, bit_index_src--) {
      if (get_bit_of_float(src, bit_index_src) == 1) {
        set_bit(dst, bin_exp);
      }
    }
    set_scale(scale, dst);
  }
  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  *dst = 0.0;
  double accuracy_dst = *dst;
  int error = 0;
  for (int i = 0; i < 96; i++) {
    if (get_bit(&src, i)) {
      accuracy_dst += pow(2, i);
    }
  }
  int scale = get_scale(&src);
  for (; scale > 0; scale--) {
    accuracy_dst /= 10;
  }
  if (get_sign(&src)) {
    accuracy_dst *= -1;
  }
  *dst = accuracy_dst;
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  *dst = 0;
  s21_decimal min = {0};
  s21_decimal max = {0};
  s21_from_int_to_decimal(INT_MIN, &min);
  s21_from_int_to_decimal(INT_MAX, &max);
  int sign = get_sign(&src);
  int err = 0;
  if (get_scale(&src)) {
    s21_truncate(src, &src);
  }
  if ((s21_is_less(max, src) == 1) || (s21_is_less(src, min) == 1)) {
    err = 1;
    printf("Bad incoming data\n");
  } else {
    *dst = src.bits[0];
    if (sign) {
      *dst *= -1;
    }
  }
  return err;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int err = 0;
  int scale = get_scale(&value);
  int sign = get_sign(&value);
  value.bits[3] = 0;
  if (scale) {
    while (scale > 1) {
      value = division_by_ten_integer(value);
      scale--;
    }
    s21_decimal five = {{5, 0, 0, 0}};
    s21_add(value, five, result);
    *result = division_by_ten_integer(*result);
    s21_truncate(*result, result);
  } else {
    *result = equate(value);
  }
  if (sign) {
    set_sign(result);
  }
  return err;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  *result = equate(value);
  (get_sign(&value) == 1) ? (null_bit(result, 127)) : (set_sign(result));
  return err;
}
