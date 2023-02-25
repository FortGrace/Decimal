#include "s21_decimal.h"

/*
Устанавливает бит '1' в
структуру decimal по
индексу из аргумента
*/
void set_bit(s21_decimal *src, int position) {
  src->bits[position / 32] |= 1U << (position % 32);
}

/*
Показывает бит в
структуре decimal
по индексу из аргумента
*/

int get_bit(s21_decimal *value, int i) {
  return !!(value->bits[i / 32] & 1U << (i % 32));
}

/*
 Показывает бит в
 числе int
 по индексу из аргумента
 */
int get_bit_of_integer(int src, int index) { return (!!(src & 1U << index)); }

int get_bit_of_float(float src, int index) {
  unsigned int fbits = *((unsigned int *)&src);
  return (!!(fbits & 1U << index));
}

/*
Устанавливает в бит
знака структуры decimal
'1', т.е. отрицательное число
*/
void set_sign(s21_decimal *d) { set_bit(d, 127); }

/*
Находит экспоненту числа float.
Экспонента вычисляется следующим образом:
В 8 битах (с 24 (0x800000) по 31 (0x40000000))
находится число экспоненты, для получения
конечного результата из которого необходимо
вычесть 127 (константа для такого вычисления)
*/
int get_bin_exp(float f) {
  int result = 0;
  unsigned int fbits = *((unsigned int *)&f);
  int degree = 0;
  /*
  Есть простой способ для перевода двоичного
  кода в десятичный.Нужно возвести нулевой бит
  числа (мы же программисты - считаем с нуля)
  в нулевую степень, первый бит в первую степень,
  второй во вторую и т.д..
  Важно: если бит == '0', то возводим '0' в нужную
  степень. Если бит == '1',    то возводим '2' в
  нужную степень.
  Пример:
  Двоичный вид: 0101.
  Перевод в десятичный вид: 2^0+0^1+2^2+0^2 = 5
  Этот способ мы используем в цикле ниже.
  */
  for (unsigned int mask = 0x800000; mask <= 0x40000000; mask <<= 1) {
    if ((!!(fbits & mask)) == 1) {
      result += pow(2, degree);
    }
    degree++;
  }
  return result - 127;
}

/*
Устанавливаем значение скейл
в стуруктуру decimal
*/
int set_scale(int scale, s21_decimal *dst) {
  int result = 0;
  if (scale > 28) {
    printf("ERROR, scale > 28\n");
    result = 1;
  } else {
    int scale_index = 112;
    for (int i = 0; i < 5; i++, scale_index++) {
      if (get_bit_of_integer(scale, i) == 1) {
        set_bit(dst, scale_index);
      }
    }
  }
  return result;
}

/*
Определяет значение экспоненты
в структуре децимал
*/
int get_scale(s21_decimal *dec) {
  int scale_index = 112;
  int scale = 0;
  for (int i = 0; scale_index <= 119; i++, scale_index++) {
    if (get_bit(dec, scale_index)) {
      scale += pow(2, i);
    }
  }
  return scale;
}

int get_sign(s21_decimal *d) { return get_bit(d, 127); }

void null_bit(s21_decimal *src, int position) {
  src->bits[position / 32] &= ~(1U << (position % 32));
}

void set_bit_1(s21_decimal *n, int pos) {
  ADD_BIT(n->bits[pos / 32], pos % 32);
}

void set_bit_0(s21_decimal *n, int pos) {
  ZERO_BIT(n->bits[pos / 32], pos % 32);
}

s21_decimal get_power_of_ten(int pow) {
  s21_decimal result = {0};
  set_scale(0, &result);
  for (int i = 0; i < 96; ++i) {
    if (binary_powers_of_ten[pow][95 - i] == '1') {
      set_bit_1(&result, i);
    } else {
      set_bit_0(&result, i);
    }
  }
  return result;
}

int check_zero(s21_decimal value) {
  int res = FALSE;
  if ((value.bits[0] == 0) && (value.bits[1] == 0) && (value.bits[2] == 0))
    res = TRUE;
  return res;
}

s21_decimal null_decimal() {
  s21_decimal nul = {0};
  return nul;
}

// new
void clean_decimal(s21_decimal *value) {
  for (int i = 0; i < 4; i++) {
    value->bits[i] = 0;
  }
}

s21_decimal modul(s21_decimal *value) {
  null_bit(value, 127);
  return *value;
}

void copy_decimal(s21_decimal *dest, s21_decimal *src) {
  dest->bits[0] = src->bits[0];
  dest->bits[1] = src->bits[1];
  dest->bits[2] = src->bits[2];
  dest->bits[3] = src->bits[3];
}