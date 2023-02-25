#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK_A;
  same_scale(&value_1, &value_2);
  s21_decimal dec_null;
  clean_decimal(&dec_null);
  int scale_result = 0;
  int sign = 0;
  if (get_sign(&value_1) && get_sign(&value_2)) {
    sign = 0;
    set_bit(&value_1, 127);
    set_bit(&value_2, 127);
  } else if (get_sign(&value_1) || get_sign(&value_2)) {
    sign = 1;
    set_bit(&value_1, 127);
    set_bit(&value_2, 127);
  }
  if (s21_is_equal(value_2, dec_null)) {
    res = S21_NAN_A;
    clean_decimal(result);
  } else if (s21_is_equal(value_1, dec_null)) {
    clean_decimal(result);
  } else if (s21_is_equal(value_1, value_2)) {
    clean_decimal(result);
    result->bits[0] = 1;
    set_scale_A(result, scale_result);
    if (sign) set_bit(result, 127);
  } else {
    clean_decimal(result);
    s21_decimal mask;
    clean_decimal(&mask);
    mask.bits[0] = 1;
    s21_decimal ten;
    clean_decimal(&ten);
    ten.bits[0] = 10;
    s21_decimal temp;
    copy_decimal(&temp, &value_2);
    s21_decimal temp_value_2;
    clean_decimal(&temp_value_2);

    while (s21_is_not_equal(value_1, dec_null) && scale_result < 28) {
      if (s21_is_less(modul(&value_1), modul(&value_2))) {
        scale_result++;
        s21_mul(value_1, ten, &value_1);
      }
      copy_decimal(&temp_value_2, &value_2);
      shift_left(&temp_value_2);
      if (s21_is_greater_or_equal(value_1, temp_value_2) &&
          s21_is_greater(temp_value_2, dec_null)) {
        shift_left(&value_2);
        shift_left(&mask);
      } else {
        s21_sum(*result, mask, result);
        s21_sub(value_1, value_2, &value_1);
        copy_decimal(&value_2, &temp);
        clean_decimal(&mask);
        mask.bits[0] = 1;
      }
    }
    set_scale_A(result, scale_result);
    if (sign) set_bit(result, 127);
  }
  return res;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK_A;
  clean_decimal(result);
  if (check_zero(value_2)) {
    res = S21_NAN_A;
  } else {
    int sign_1 = 0;
    sign_1 = get_sign(&value_1);
    set_bit(&value_1, 127);
    set_bit(&value_2, 127);
    s21_decimal n;
    clean_decimal(&n);
    n = div_no_scale(value_1, value_2);
    s21_mul(n, value_2, &value_2);
    s21_sub(value_1, value_2, result);
    if (sign_1) {
      set_bit(result, 127);
    } else {
      null_bit(result, 127);
    }
  }
  return res;
}

void set_scale_A(s21_decimal *value, int scale_value) {
  for (int i = 120; i >= 112; i--) {
    null_bit(value, i);
  }
  value->bits[3] |= (scale_value << 16);
}

// Умнож. на 10 для целых чисел. Исп. в функции same_scale !раб!
void mul_10(s21_decimal *value, int scale_dif) {
  s21_decimal tmp_1, tmp_2;
  s21_decimal res;
  s21_decimal *result = &res;
  clean_decimal(result);
  tmp_1 = *value;
  tmp_2 = *value;
  for (int i = 0; i < scale_dif; i++) {
    for (int i = 0; i < 9; i++) {
      s21_sum(tmp_1, tmp_2, result);
      tmp_1 = *result;
    }
    tmp_2 = *result;
  }
  *value = *result;
}

void same_scale(s21_decimal *dec1, s21_decimal *dec2) {
  int scale_dec1 = 0, scale_dec2 = 0;
  scale_dec1 = get_scale(dec1);
  scale_dec2 = get_scale(dec2);
  s21_decimal five;
  clean_decimal(&five);
  five.bits[0] = 5;
  if (scale_dec1 < scale_dec2) {
    while (scale_dec1 < scale_dec2) {
      if (get_bit(dec1, 92) || get_bit(dec1, 93) || get_bit(dec1, 94) ||
          get_bit(dec1, 95) || scale_dec1 > 28) {
        if (scale_dec2 - scale_dec1 == 1) {
          s21_sum(*dec2, five, dec2);
          set_scale_A(dec2, scale_dec2);
        }
        lower_scale(dec2);
        scale_dec2 = get_scale(dec2);
      } else {
        upper_scale(dec1);
        scale_dec1 = get_scale(dec1);
      }
    }
  } else if (scale_dec2 < scale_dec1) {
    while (scale_dec2 < scale_dec1) {
      if (get_bit(dec2, 92) || get_bit(dec2, 93) || get_bit(dec2, 94) ||
          get_bit(dec2, 95) || scale_dec2 > 28) {
        if (scale_dec1 - scale_dec2 == 1) {
          s21_sum(*dec1, five, dec1);
          set_scale_A(dec1, scale_dec1);
        }
        lower_scale(dec1);
        scale_dec1 = get_scale(dec1);
      } else {
        upper_scale(dec2);
        scale_dec2 = get_scale(dec2);
      }
    }
  }
}

void shift_left(s21_decimal *dec) {
  int bit_31 = 0;
  int bit_63 = 0;
  bit_31 = get_bit(dec, 31);
  bit_63 = get_bit(dec, 63);
  dec->bits[0] <<= 1;
  dec->bits[1] <<= 1;
  dec->bits[1] += bit_31;
  dec->bits[2] <<= 1;
  dec->bits[2] += bit_63;
}

void upper_scale(s21_decimal *dec) {
  int scale = 0;
  unsigned int sign = 0;
  sign = get_sign(dec);
  scale = get_scale(dec) + 1;
  mul_10(dec, 1);
  set_scale_A(dec, scale);
  if (sign) set_bit(dec, 127);
}

void lower_scale(s21_decimal *dec) {
  int scale = 0;
  unsigned int sign = 0;
  sign = get_sign(dec);
  scale = get_scale(dec) - 1;
  s21_decimal ten;
  clean_decimal(&ten);
  ten.bits[0] = 10;
  *dec = div_no_scale(*dec, ten);
  set_scale_A(dec, scale);
  if (sign) set_bit(dec, 127);
}

s21_decimal div_no_scale(s21_decimal dividend, s21_decimal divisor) {
  s21_decimal quotient;
  s21_decimal dec_null;
  clean_decimal(&dec_null);
  dividend.bits[3] = 0;
  if (s21_is_equal(dividend, divisor)) {
    clean_decimal(&quotient);
    quotient.bits[0] = 1;
  } else if (s21_is_equal(dividend, dec_null)) {
    clean_decimal(&quotient);
  } else {
    clean_decimal(&quotient);
    s21_decimal mask;
    clean_decimal(&mask);
    mask.bits[0] = 1;
    s21_decimal one;
    clean_decimal(&one);
    one.bits[0] = 1;
    s21_decimal temp;
    temp = divisor;
    s21_decimal t_divisor;
    clean_decimal(&t_divisor);
    s21_decimal ten;
    clean_decimal(&ten);
    ten.bits[0] = 10;
    int flag = 0;
    while (s21_is_greater_or_equal(modul(&dividend), modul(&divisor))) {
      t_divisor = divisor;
      shift_left(&t_divisor);
      if (get_bit(&t_divisor, 95)) flag = 1;
      if (s21_is_greater_or_equal(dividend, t_divisor) &&
          s21_is_greater(t_divisor, dec_null) && flag == 0) {
        shift_left(&divisor);
        shift_left(&mask);
      } else {
        s21_sum(quotient, mask, &quotient);
        s21_sub(dividend, divisor, &dividend);
        divisor = temp;
        clean_decimal(&mask);
        mask.bits[0] = 1;
        flag = 0;
      }
    }
  }
  return quotient;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clean_decimal(result);
  int tmp = 0;
  int flag = 0;
  int res = OK_A;
  int sign_value_1 = get_sign(&value_1);
  int sign_value_2 = get_sign(&value_2);
  same_scale(&value_1, &value_2);
  if (sign_value_1 ^ sign_value_2) {
    if (sign_value_1 == 1) {
      if (s21_is_less(modul(&value_2), modul(&value_1))) flag = 1;
      value_1 = dop_cod(value_1);
      tmp = s21_sum(value_1, value_2, result);
      if (flag) {
        *result = dop_cod(*result);
        set_bit(result, 127);
      }
    }
    if (sign_value_2 == 1) {
      if (s21_is_less(modul(&value_1), modul(&value_2))) flag = 1;
      value_2 = dop_cod(value_2);
      tmp = s21_sum(value_1, value_2, result);
      if (flag) {
        *result = dop_cod(*result);
        set_bit(result, 127);
      }
    }
  } else {
    if (sign_value_1 == 1 && sign_value_2 == 1) {
      tmp = s21_sum(value_1, value_2, result);
      set_bit(result, 127);
      if (tmp == 1) {
        clean_decimal(result);
        res = S21_NEG_INF_A;
      }
    }
    if (sign_value_1 == 0 && sign_value_2 == 0)
      tmp = s21_sum(value_1, value_2, result);
    if (tmp == 1) {
      clean_decimal(result);
      res = S21_INF_A;
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clean_decimal(result);
  int tmp = 0;
  int flag = 0;
  int res = OK_A;
  int sign_value_1 = get_sign(&value_1);
  int sign_value_2 = get_sign(&value_2);
  same_scale(&value_1, &value_2);
  if (sign_value_1 == 0 && sign_value_2 == 1) {
    tmp = s21_sum(value_1, value_2, result);
  }

  if (sign_value_1 == 1 && sign_value_2 == 0) {
    tmp = s21_sum(value_1, value_2, result);
    set_bit(result, 127);
  }

  if (sign_value_1 == 0 && sign_value_2 == 0) {
    if (s21_is_less(modul(&value_1), modul(&value_2))) {
      flag = 1;
    }
    value_2 = dop_cod(value_2);
    s21_sum(value_1, value_2, result);
    if (flag) {
      *result = dop_cod(*result);
      set_bit(result, 127);
    }
  }

  if (sign_value_1 == 1 && sign_value_2 == 1) {
    if (s21_is_greater(modul(&value_1), modul(&value_2))) {
      flag = 1;
    }
    value_1 = dop_cod(value_1);
    s21_sum(value_1, value_2, result);
    if (flag) {
      *result = dop_cod(*result);
      set_bit(result, 127);
    }
  }
  if (tmp == 1) {
    clean_decimal(result);
    res = S21_INF_A;
  }
  return res;
}

int s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sum = 0;
  int tmp = 0;
  clean_decimal(result);
  for (int i = 0; i <= 95; i++) {
    int bit_1 = get_bit(&value_1, i);
    int bit_2 = get_bit(&value_2, i);
    sum = bit_1 + bit_2 + tmp;
    tmp = sum / 2;
    if ((sum % 2) == 1) set_bit(result, i);
  }
  set_scale_A(result, get_scale(&value_1));
  return tmp;
}

s21_decimal dop_cod(s21_decimal value) {
  s21_decimal res, result, one;
  clean_decimal(&res);
  clean_decimal(&result);
  clean_decimal(&one);
  set_bit(&one, 0);

  for (int i = 0; i <= 95; i++) {
    if (get_bit(&value, i) == 0) set_bit(&result, i);
  }
  s21_sum(result, one, &res);
  set_scale_A(&res, get_scale(&value));

  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK_A;
  clean_decimal(result);
  int sign_value_1 = get_sign(&value_1);
  int sign_value_2 = get_sign(&value_2);
  if ((check_zero(value_1) == 1 && value_1.bits[3] == 0) ||
      (check_zero(value_2) == 1 && value_2.bits[3] == 0)) {
    if (sign_value_1 ^ sign_value_2) set_bit(result, 127);
  } else {
    int scale_result = get_scale(&value_1) + get_scale(&value_2);
    s21_big_decimal v1, v2, res_big;
    clean_big_decimal(&v1);
    clean_big_decimal(&v2);
    clean_big_decimal(&res_big);
    decimal_to_big_decimal(&v1, &value_1);
    decimal_to_big_decimal(&v2, &value_2);

    while (v2.bits[0] != 0 || v2.bits[1] != 0 || v2.bits[2] != 0 ||
           v2.bits[3] != 0 || v2.bits[4] != 0 || v2.bits[5] != 0) {
      if (v2.bits[0] & 1) {
        res_big = sum_big_decimal(v1, res_big);
      }
      shift_right_big(&v2);
      shift_left_big(&v1);
    }
    s21_big_decimal five;
    clean_big_decimal(&five);
    five.bits[0] = 5;
    while ((res_big.bits[3] != 0 || res_big.bits[4] != 0 ||
            res_big.bits[5] != 0) ||
           scale_result > 28) {
      res_big = sum_big_decimal(res_big, five);
      res_big = div_big_on_ten(res_big);
      scale_result--;
    }

    if ((res_big.bits[3] == 0 && res_big.bits[4] == 0 &&
         res_big.bits[5] == 0) &&
        (scale_result >= 0 && scale_result <= 28)) {
      big_to_decimal(result, &res_big);
      set_scale_A(result, scale_result);
      if (sign_value_1 == 1 && sign_value_2 == 0) {
        set_bit(result, 127);
      } else if (sign_value_1 == 0 && sign_value_2 == 1) {
        set_bit(result, 127);
      }
    } else {
      if (sign_value_1 == 1 && sign_value_2 == 0) {
        res = S21_NEG_INF_A;
      } else if (sign_value_1 == 0 && sign_value_2 == 1) {
        res = S21_NEG_INF_A;
      } else {
        res = S21_INF_A;
      }
      clean_decimal(result);
    }
  }
  return res;
}

s21_big_decimal div_big_on_ten(s21_big_decimal big1) {
  s21_big_decimal ten, result, mask, temp;
  clean_big_decimal(&ten);
  ten.bits[0] = 10;
  clean_big_decimal(&result);
  clean_big_decimal(&mask);
  mask.bits[0] = 1;
  clean_big_decimal(&temp);
  int flag = 0;
  while (is_greater_or_equal_big(big1, ten)) {
    copy_big(&temp, &ten);
    shift_left_big(&temp);
    if (get_bit_big(temp, 191)) flag = 1;
    if (is_greater_or_equal_big(big1, temp) && flag == 0) {
      shift_left_big(&ten);
      shift_left_big(&mask);
    } else {
      result = sum_big_decimal(result, mask);
      big_dop_code(&ten);
      big1 = sum_big_decimal(big1, ten);
      clean_big_decimal(&ten);
      ten.bits[0] = 10;
      clean_big_decimal(&mask);
      mask.bits[0] = 1;
      flag = 0;
    }
  }
  return result;
}

int is_greater_or_equal_big(s21_big_decimal big1, s21_big_decimal big2) {
  int res = 0;
  if (big1.bits[0] == big2.bits[0] && big1.bits[1] == big2.bits[1] &&
      big1.bits[2] == big2.bits[2] && big1.bits[3] == big2.bits[3] &&
      big1.bits[4] == big2.bits[4] && big1.bits[5] == big2.bits[5]) {
    res = 1;
  } else {
    int a = 0, b = 0;
    for (int i = 192; i >= 0; --i) {
      a = get_bit_big(big1, i);
      b = get_bit_big(big2, i);
      if (a == b) continue;
      res = a > b ? 1 : 0;
      break;
    }
  }
  return res;
}

void big_dop_code(s21_big_decimal *big) {
  for (int i = 0; i < 6; ++i) {
    big->bits[i] = ~big->bits[i];
  }
  s21_big_decimal one;
  clean_big_decimal(&one);
  one.bits[0] = 1;
  *big = sum_big_decimal(*big, one);
}

void shift_right_big(s21_big_decimal *big) {
  int bit_160 = 0;
  int bit_128 = 0;
  int bit_96 = 0;
  int bit_64 = 0;
  int bit_32 = 0;
  bit_160 = get_bit_big(*big, 160);
  bit_128 = get_bit_big(*big, 128);
  bit_96 = get_bit_big(*big, 96);
  bit_64 = get_bit_big(*big, 64);
  bit_32 = get_bit_big(*big, 32);
  big->bits[5] >>= 1;
  big->bits[4] >>= 1;
  if (bit_160) set_bit_big(big, 159);
  big->bits[3] >>= 1;
  if (bit_128) set_bit_big(big, 127);
  big->bits[2] >>= 1;
  if (bit_96) set_bit_big(big, 95);
  big->bits[1] >>= 1;
  if (bit_64) set_bit_big(big, 63);
  big->bits[0] >>= 1;
  if (bit_32) set_bit_big(big, 31);
}

void shift_left_big(s21_big_decimal *big) {
  int bit_31 = 0;
  int bit_63 = 0;
  int bit_95 = 0;
  int bit_127 = 0;
  int bit_159 = 0;
  bit_31 = get_bit_big(*big, 31);
  bit_63 = get_bit_big(*big, 63);
  bit_95 = get_bit_big(*big, 95);
  bit_127 = get_bit_big(*big, 127);
  bit_159 = get_bit_big(*big, 159);
  big->bits[0] <<= 1;
  big->bits[1] <<= 1;
  big->bits[1] += bit_31;
  big->bits[2] <<= 1;
  big->bits[2] += bit_63;
  big->bits[3] <<= 1;
  big->bits[3] += bit_95;
  big->bits[4] <<= 1;
  big->bits[4] += bit_127;
  big->bits[5] <<= 1;
  big->bits[5] += bit_159;
}

s21_big_decimal sum_big_decimal(s21_big_decimal big1, s21_big_decimal big2) {
  int a, b, res, temp;
  s21_big_decimal result;
  clean_big_decimal(&result);
  temp = 0;
  for (int i = 0; i < 192; i++) {
    a = get_bit_big(big1, i);
    b = get_bit_big(big2, i);
    res = a + b + temp;
    if (res == 0) {
      temp = 0;
    } else if (res == 1) {
      temp = 0;
      set_bit_big(&result, i);
    } else if (res == 2) {
      temp = 1;
    } else if (res == 3) {
      temp = 1;
      set_bit_big(&result, i);
    }
  }
  return result;
}

void clean_big_decimal(s21_big_decimal *big) {
  for (int i = 0; i < 6; i++) {
    big->bits[i] = 0;
  }
}
void set_bit_big(s21_big_decimal *big, int index) {
  int mask = 1 << index;
  big->bits[index / 32] |= mask;
}

int get_bit_big(s21_big_decimal big, int index) {
  return !!(big.bits[index / 32] & 1U << (index % 32));
}

void decimal_to_big_decimal(s21_big_decimal *dst, s21_decimal *src) {
  for (int i = 0; i < 3; ++i) {
    dst->bits[i] = src->bits[i];
  }
}

void big_to_decimal(s21_decimal *dst, s21_big_decimal *src) {
  for (int i = 0; i < 3; ++i) {
    dst->bits[i] = src->bits[i];
  }
}

void copy_big(s21_big_decimal *big_dst, s21_big_decimal *big_src) {
  for (int i = 0; i < 6; ++i) {
    big_dst->bits[i] = big_src->bits[i];
  }
}
