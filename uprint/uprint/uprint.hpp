#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <bitmath.h>

// ****** FUNCTIONS C ******

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Инициализирует USART.
 * @param baud Скорость в бодах.
 * @note Активирует порты RX и TX.
 */
void ubegin(const uint16_t baud);

/**
 * @defgroup base_print Базовые функции печати.
 * @brief Передают данные в USART.
 * @{
 */

/**
 * @brief Печатает 1 символ.
 * @param ch Символ.
 * @note Меняет \\n на пару \\r\\n для корректного перевода строки.
 * @warning Ждет освобождение буфера UDR0,
 *          может остановить выполнение программы на неопределенный срок.
 */
void uput_ch(const char ch);

/**
 * @brief Печатает строку.
 * @param str Строка.
 * @warning Использует функцию uput_ch.
 * @see uput_ch
 */
void uput_str(const char *str);
/** @} base_print */

/**
 * @defgroup base_print_num Функции печати чисел.
 * @brief Печатают числа в разных представлениях.
 * @{
 */

/**
 * @brief Печатает число в десятичном формате.
 * @param num Число 2 байта.
 * @param neg Флаг отрицательного числа (ставит перед числом минус).
 * @warning Использует функцию uput_ch.
 * @see uput_ch
 */
void uprint_dec(const uint16_t num, bool neg);

/**
 * @brief Печатает число в двоичном формате.
 * @param num Число 2 байта.
 * @warning Использует функцию uput_ch.
 * @see uput_ch
 */
void uprint_bin(const uint16_t num);

/**
 * @brief Печатает число в шестнадцатеричном формате.
 * @param num Число 2 байта.
 * @warning Использует функцию uput_ch.
 * @see uput_ch
 */
void uprint_hex(const uint16_t num);
/** @} base_print_num */

#ifdef __cplusplus
}

// ****** FUNCTIONS && STRUCTS C++ ******

/**
 * @defgroup specifiers Функции-спецификаторы.
 * @brief Помечают числа через структуры для представления их в разных форматах,
 *        Используется в шаблоне uprint (bin, hex, neg - отрицательное число).
 * @see uprint
 * @{
 */

/**
 * @defgroup marker_structs Структуры-метки.
 * @brief Помогают отличить форматы друг от друга.
 * @note ДЛЯ ВНУТРЕННЕГО ИСПОЛЬЗОВАНИЯ.
 * @{
 */
struct Bin { uint16_t val; };
struct Hex { uint16_t val; };
struct Neg { uint16_t val; };

/**
 * @brief Помечает число как двоичное.
 * @param arg Число-аргумент.
 * @return Bin (Структура-метка).
 */
inline Bin bin(uint16_t arg) { return {arg}; }

/**
 * @brief Помечает число как шестнадцатеричное.
 * @param arg Число-аргумент.
 * @return Hex (Структура-метка).
 */
inline Hex hex(uint16_t arg) { return {arg}; }

/**
 * @brief Помечает число как отрицательное,
 *        само число становится положительным с пометкой отрицательного.
 * @param arg Число-аргумент.
 * @return Neg (Структура-метка).
 */
inline Neg neg(int16_t arg)  { return {(uint16_t)(arg < 0 ? -arg : arg)}; }
/** @} marker_structs */
/** @} specifiers */

// ****** UPRINT TEMPLATE ******

/**
 * @namespace upsys
 * @brief Функции-обертки, работают через перегрузку.
 * @internal ДЛЯ ВНУТРЕННЕГО ИСПОЛЬЗОВАНИЯ.
 */
namespace upsys {
    inline void uprint_write(const char ch)      { uput_ch(ch); }
    inline void uprint_write(const char *str)    { uput_str(str); }

    inline void uprint_write(const int16_t num)  { uprint_dec(num, false); }
    inline void uprint_write(const uint16_t num) { uprint_dec(num, false); }

    inline void uprint_write(const Bin bin)      { uprint_bin(bin.val); }
    inline void uprint_write(const Hex hex)      { uprint_hex(hex.val); }
    inline void uprint_write(const Neg neg)      { uprint_dec(neg.val, true); }

    template<typename T>
    inline void uprint_write(const T others)     { static_assert(sizeof(T) == 0, "Invalid type."); }
}

/**
 * @defgroup result_print_templates Результирующие шаблоны печати.
 * @brief Дают удобный доступ в базовым функциям печати и функциям печати чисел.
 * @see \ref base_print
 * @see \ref base_print_num
 * @{
 */

/**
 * @brief Печатает все свой аргументы по порядку через выще описанные функции.
 * @tparam Args Тип переменных аргументов.
 * @param args Аргументы.
 * @warning Количество аргументов ограничено 20.
 * @warning Если передать в качестве аргумента отрицательное число без функции-спецификатора,
 *          то число напечатается некорректно.
 */
template<typename ... Args>
void uprint(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for uprint.");
    (upsys::uprint_write(args), ...);
}

/**
 * @brief Вызывает uprint и переносит строку.
 * @tparam Args Тип переменных аргументов.
 * @param args Аргументы.
 * @warning Не используйте для простого перевода строки, используйте endln.
 */
template<typename ... Args>
void uprintln(const Args& ... args) {
    uprint(args...);
    uput_ch('\n');
}

/** @brief Просто переносит строку. */
inline void endln() { uput_ch('\n'); }
/** @} result_print_templates */

#endif // __cplusplus

#endif //USARTPRINT_H_INCLUDED