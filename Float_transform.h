/*
 * Float_transform.h
 *
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

#ifndef INC_FLOAT_TRANSFORM_H_
#define INC_FLOAT_TRANSFORM_H_

#include <main.h>

typedef struct {
	uint8_t sign; //Какое число: положительное или отрицательное
	int integer_part; //целая часть числа
	uint32_t fractional_part; //дробная часть числа
}FT_struct;

//void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number);
void Float_transform(float value, uint8_t width);
void Float_print_to_buf(char *buffer, char *data);

#endif /* INC_FLOAT_TRANSFORM_H_ */
