/*
 * Float_transform.c
 * v1.3 Собрал рабочие переменные в структуру, чтоб не плодить их от файла к файлу
 * v1.2(Добавлена функция вывода преобразованных данных в буфер)
 * v1.1(Исправление ошибки, когда -1.00f < float value < 0.00f)
 *
 * Простенькая библиотека для разбития float на две части: целую и дробную.
 * Это нужно, чтоб не использовать -u_printf_float в линкере, дабы сэкономить память МК.
 * В библиотеке используется округление.
 * Пример Val = 25.436f. Мы хотим вывести число с 2 знаками после запятой. Получим 25.44
 *
 *  Created on: Mar 04, 2024
 *      Author: Oleg Volkov
 *
 *      P.S Хочу сказать огромное спасибо Дмитрию Мачневу за тестирование и подсказки по оптимизации!
 *      В будущем может еще чего переделаем.
 */

#include "Float_transform.h"
#include <stdio.h>
#include <string.h>

/*--------------------------------Переменные для работы-------------------------------------*/
FT_struct FT_lib;
/*--------------------------------Переменные для работы-------------------------------------*/

/*--------------------------------Скопировать в main.c--------------------------------------*/
//extern FT_struct FT_lib;
/*--------------------------------Скопировать в main.c--------------------------------------*/

/*============================Пример работы с библиотекой===================================*/
//Файл main.c
/*#include "Float_transform.h"
 extern FT_struct FT_lib;

 float Val = -3.14159265359f;

 //Преобразуем float на несколько частей
 Float_transform(Val, 4, &FT_lib.sign, &FT_lib.integer_part, &FT_lib.fractional_part);

 //Если вывод в буфер:
 Float_print_to_buf(tx_buffer, "Val = %d.%.04ld\r\n", (uint8_t*) &FT_lib.sign, (int*) &FT_lib.integer_part, (uint32_t*) &FT_lib.fractional_part);


 //Если просто вывод в printf:
 if (!FT_lib.sign) {
 printf("Val = %d.%.04ld\r\n", FT_lib.integer_part, FT_lib.fractional_part);
 } else {
 printf("Val = -%d.%.04ld\r\n", FT_lib.integer_part, FT_lib.fractional_part);
 }
 */
//Получим вывод: float = -3.1416
/*
 * Обратите внимание на вывод чисел. Сколько знаков после запятой, столько и отступ дробного числа при печати.
 * Т.к. если Val = 25.0126;
 * и мы сделаем
 * Float_transform(counter, 3, &FT_lib.sign, &FT_lib.integer_part, &FT_lib.fractional_part);
 * то
 * FT_lib.integer_part = 25;
 * FT_lib.fractional_part = 13;
 * а вывод должен быть 25.013, поэтому:
 * printf("float = %d.%.03ld\r\n", FT_lib.integer_part, FT_lib.fractional_part);
 *
 * Получим вывод: float = 25.013
 *
 */
/*============================Пример работы с библиотекой===================================*/

/**
 ******************************************************************************
 *  @breif Функция разбития числа float на две части: целую и дробную.
 *  @param  float value - число float, которое хотим разбить
 *  @param  uint8_t width - сколько знаков после запятой хотим выводить
 *  @param  uint8_t *sign - глобальная переменная.
 *  Знак числа(положительное или отрицательное)
 *  @param  int *integer_part - глобальная переменная для вещественной части
 *  @param  uint32_t *fractional_part - глобальная переменная для дробной части
 ******************************************************************************
 */

//void Float_transform(float value, uint8_t width, uint8_t *sign, int *integer_part, uint32_t *fractional_part) {
//
//    *sign = 0;
//    float rounding = 0.5f;
//    uint32_t rounding_2 = 1;
//    if (*((uint32_t*) &value) & 0x80000000) {
//        *((uint32_t*) &value) &= 0x7FFFFFFF;
//        *sign = 1;
//    }
//    for (uint8_t i = 0; i < width; i++) {
//        rounding_2 = rounding_2 * 10;
//    }
//    rounding = rounding / rounding_2;
//    *integer_part = (int) ((float) value + rounding);
//    *fractional_part = (((float) value + rounding) * rounding_2) - (*integer_part * rounding_2);
//
//}
void Float_transform(float value, uint8_t width) {
	FT_lib.sign = 0;
	float rounding = 0.5f;
	uint32_t rounding_2 = 1;
	if (*((uint32_t*) &value) & 0x80000000) {
		*((uint32_t*) &value) &= 0x7FFFFFFF;
		FT_lib.sign = 1;
	}

	for (uint8_t i = 0; i < width; i++) {
		rounding_2 = rounding_2 * 10;
	}
	rounding = rounding / rounding_2;
	FT_lib.integer_part = (int) ((float) value + rounding);
	FT_lib.fractional_part = (((float) value + rounding) * rounding_2) - (FT_lib.integer_part * rounding_2);
}

/**
 ******************************************************************************
 *  @breif Функция отправки преобразованного числа в буфер
 *  @param  char *buffer - буфер, в который будем писать текст с float
 *  (например tx_buffer, который мы используем для отправки на дисплей)
 *  @param  char *data - данные, которые будем писать в буфер,
 *  например "Температура: %3d.%.01ld°С"
 *  @attention P.S. Все последующие данные преобразовывались в функции Float_transform
 *  @param  uint8_t *sign - глобальная переменная.
 *  Знак числа(положительное или отрицательное)
 *  @param  int *integer_part - глобальная переменная для вещественной части
 *  @param  uint32_t *fractional_part - глобальная переменная для дробной части
 ******************************************************************************
 */

void Float_print_to_buf(char *buffer, char *data) {
	sprintf((char*) buffer, data, FT_lib.integer_part, FT_lib.fractional_part);
	if (FT_lib.sign) {
		for (int i = strlen(buffer) - 1; i >= 0; i--)
			buffer[i + 1] = buffer[i];
		buffer[0] = '-';
	}
}
