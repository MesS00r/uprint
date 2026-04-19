/**
 * @file USARTInput.hpp
 * @brief Заголовочный файл для USART-ввода (буферизованный ввод с поддержкой C++ обёрток).
 */

#ifndef USARTINPUT_HPP_INCLUDED
#define USARTINPUT_HPP_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h> // IWYU pragma: keep

/** @brief Размер буфера в символах. */
#define BUFFER_SIZE 32

// ****** FUNCTIONS && METHODS C ******

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup c_func C функции и структура sys_buffer.
 * @brief Используются для реализации C++ обертки.
 * @note ДЛЯ ВНУТРЕННЕГО ИСПОЛЬЗОВАНИЯ.
 * @{
 */

/** @brief Структура-буфер для сохранения полученного ввода. */
typedef struct sys_buffer sys_buffer;

/**
 * @brief Инициализирует буфер.
 * @return sys_buffer* Указатель на структуру-буфер.
 */
sys_buffer* buffer_init(void);

/**
 * @defgroup std_buffer_methods Методы Структуры-буфера.
 * @brief Работают с буфером.
 * @{
 */

/**
 * @brief Сбрасывает буфер.
 * @param buf Указатель на структуру-буфер.
 * @warning Данные теряются и указатели first и last перемещаются в начало буфера.
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool buffer_reset(sys_buffer *buf);

/**
 * @brief Записывает 1 символ в начало буфера.
 * @note Указатель first увеличивается на 1.
 * @param buf Указатель на структуру-буфер.
 * @param ch Символ, который будет записан.
 * @warning Использует buffer_reset.
 * @see buffer_reset
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool buffer_sendch(sys_buffer *buf, char ch);

/**
 * @brief Читает 1 символ из буфера и записывает его в переменную.
 * @note Указатель last увеличивается на 1.
 * @param buf Указатель на структуру-буфер.
 * @param ch Указатель на переменную, куда будет записан символ.
 * @warning Использует buffer_reset.
 * @see buffer_reset
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool buffer_readch(sys_buffer *buf, char *ch);
// std_buffer_methods ends

/**
 * @brief Инициализирует аппаратный таймер 1 (нужен для работы uset_ch).
 * @see uset_ch
 */
void input_timer_init(void);

/**
 * @defgroup uscan_buffer_methods Методы Структуры-буфера, 
 *           необходимые для 1 части интерфейса ввода (uscan).
 * @brief Читают пользовательский ввод и работают с буфером.
 * @see uscan
 * @{
 */

/**
 * @brief Читает 1 символ и записывает его в буфер.
 * @param buf Указатель на структуру-буфер.
 * @param copy Копия прочитанного символа (временно).
 * @warning Использует buffer_sendch.
 * @see buffer_sendch
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool uset_ch(sys_buffer *buf, char *copy);

/**
 * @brief Читает строку из пользовательского ввода и записывает её в буфер.
 * @param buf Указатель на структуру-буфер.
 * @warning Использует buffer_sendch.
 * @see buffer_sendch
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool uset_line(sys_buffer *buf);
// uscan_buffer_methods ends

/**
 * @defgroup uread_buffer_methods Методы Структуры-буфера,
 *           необходимые для 2 части интерфейса ввода (uread).
 * @brief Работают с буфером (читают).
 * @see uread
 * @{
 */

/**
 * @brief Читает 1 слово (строку) из буфера до \\0 включительно и
 *        записывает его в массив.
 * @param buf Указатель на структуру-буфер.
 * @param word_buf Массив, куда будет записано слово.
 * @param len Длина читаемого слова в символах.
 * @warning Если длина читаемого слова будет меньше слова, которое находится в буфере,
 *          то слово обрежется, а остальная часть будет удалена из буфера.
 * @warning Использует buffer_readch.
 * @see buffer_readch
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool uread_word(sys_buffer *buf, char *word_buf, uint8_t len);

/**
 * @brief Читает 1 число до 2-х байтов из буфера.
 * @param buf Указатель на структуру-буфер.
 * @param num Указатель на переменную, куда будет записано число (2 байта).
 * @warning Если в буфере будет лежать не число в 
 *          строковом представлении, то вернет false.
 * @warning Использует buffer_readch.
 * @see buffer_readch
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
bool uread_num(sys_buffer *buf, uint16_t *num);
// uread_buffer_methods ends

/** @} uread_buffer_methods */
/** @} uscan_buffer_methods */
/** @} std_buffer_methods */
/** @} c_func */

#ifdef __cplusplus
}

// ****** FUNCTIONS && CLASS C++ ******

/**
 * @brief Управляет вводом и буфером.
 * @note Использует методы и функции из c_func.
 * @see \ref c_func
 */
class Input {
private:
    /** 
     * @brief Указатель на структуру-буфер.
     * @see sys_buffer
     */
    static inline sys_buffer *buf;
public:
    /**
     * @brief Инициализирует интерфейс ввода.
     * @note Использует инициализатор buffer_init и input_timer_init.  
     * @see buffer_init
     * @see input_timer_init
     */
    static void uinput_init() {
        buf = buffer_init();
        input_timer_init();
    }
    
    /**
     * @defgroup input_buffer Интерфейс ввода (буфер).
     * @brief Управляют буфером.
     * @note Используют методы из std_buffer_methods.
     * @see \ref std_buffer_methods
     * @{
     */

    /**
     * @brief Сбрасывает буфер.
     * @warning Использует buffer_reset.
     * @see buffer_reset
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool buf_reset()                        { return buffer_reset(buf); }
    
    /**
     * @brief Записывает 1 символ в начало буфера.
     * @warning Использует buffer_sendch.
     * @param ch Символ, который будет записан.
     * @see buffer_sendch
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool buf_sendch(char ch)                { return buffer_sendch(buf, ch); }
    
    /**
     * @brief Читает 1 символ из буфера и записывает его в переменную.
     * @warning Использует buffer_readch.
     * @param ch Ссылка на переменную, куда будет записан символ.
     * @see buffer_readch
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool buf_readch(char &ch)               { return buffer_readch(buf, &ch); }
    // input_buffer ends

    /**
     * @defgroup input_set Интерфейс ввода (запись).
     * @brief Записывают пользовательский ввод в буфер.
     * @note Используют методы из uscan_buffer_methods.
     * @see \ref uscan_buffer_methods
     * @{
     */

    /**
     * @brief Читает 1 символ и записывает его в буфер.
     * @warning Использует uset_ch.
     * @see uset_ch
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool set_ch()                           { return uset_ch(buf, NULL); }
    
    /**
     * @brief Читает строку из пользовательского ввода и записывает её в буфер.
     * @warning Использует uset_line.
     * @see uset_line
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool set_line()                         { return uset_line(buf); }
    // input_set ends

    /**
     * @defgroup input_read Интерфейс ввода (чтение).
     * @brief Читают данные из буфера и отдают их пользователю.
     * @note Используют методы из uread_buffer_methods.
     * @see \ref uread_buffer_methods
     * @{
     */

    /**
     * @brief Читает 1 слово (строку) из буфера до \\0 включительно и
     *        записывает его в массив.
     * @warning Использует uread_word.
     * @param word Массив, куда будет записано слово.
     * @param len Длина читаемого слова в символах.
     * @see uread_word
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool read_word(char *word, uint8_t len) { return uread_word(buf, word, len); }

    /**
     * @brief Читает 1 число до 2-х байтов из буфера.
     * @warning Использует uread_num.
     * @param num Ссылка на переменную, куда будет записано число (2 байта).
     * @see uread_num
     * @return true Успешное выполнение.
     * @return false Ошибка выполнения.
     */
    static bool read_num(uint16_t &num)            { return uread_num(buf, &num); }
    // input_read ends

    /** @} input_read */
    /** @} input_set */
    /** @} input_buffer */

    Input() = delete; /** @brief Создавать объекты класса запрещено. */
};

// ****** USART SCAN ******

/** 
 * @brief Константные значения, которые определяют что пользователь хочет считать.
 * @note Используются как аргументы в шаблоне uscan.
 * @see uscan
 */
typedef enum { 
    CH, /**< Считать 1 символ. */
    STR /**< Считать 1 слово (строку). */
} specs;

/** @internal ДЛЯ ВНУТРЕННЕГО ИСПОЛЬЗОВАНИЯ. */
namespace uscan_sys {
    inline bool uscan_check(specs spec) {
        switch (spec) {
        case CH: Input::set_ch();    return true;
        case STR: Input::set_line(); return true;
        }
        return false;
    }

    template<typename T>
    inline void uscan_check(const T others) { static_assert(sizeof(T) == 0, "Invalid specifier."); }
}

/**
 * @brief Принимает в качестве аргументов константы и считывает пользовательский ввод.
 * @note Использует константы типа specs.
 * @note Использует методы класса Input из группы input_set.
 * @tparam Args Тип переменных аргументов.
 * @param args Аргументы.
 * @warning Количество аргументов ограничено 20.
 * @warning Если передать в качестве аргумента не константу типа specs,
 *          то будет ошибка (Invalid specifier).
 * @see specs
 * @see \ref input_set
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
template<typename ... Args>
bool uscan(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for uscan.");
    return (uscan_sys::uscan_check(args) && ...);
}

// ****** USART READ ******

/**
 * @defgroup specifiers Функции-спецификаторы.
 * @brief Принимают ссылку на объект, в который будут записаны данные,
 *        Используется в шаблоне uread (s_word - слово (строка), s_num - число).
 * @see uread
 * @{
 */

/**
 * @defgroup marker_structs Структуры-метки.
 * @brief Помогают отличить данные друг от друга.
 * @note ДЛЯ ВНУТРЕННЕГО ИСПОЛЬЗОВАНИЯ.
 * @{
 */
struct Word { char *word; uint8_t len; };
struct Num  { uint16_t &num; };
// marker_structs ends

/**
 * @brief Принимает массив произвольного размера.
 * @tparam L Размер массива (вычисляется автоматически).
 * @return Word (Структура-метка).
 */
template<uint8_t L>
inline Word s_word(char (&word)[L]) { return {word, sizeof(word)}; }

/**
 * @brief Принимает число до 2-х байт.
 * @param num Ссылка на переменную, куда будет записано число.
 * @return Num (Структура-метка).
 */
inline Num s_num(uint16_t &num)     { return {num}; }
/** @} marker_structs */
/** @} specifiers */

/** @internal ДЛЯ ВНУТРЕННЕГО ИСПОЛЬЗОВАНИЯ. */
namespace uread_sys {
    inline bool uread_accept(Word word)     { return Input::read_word(word.word, word.len); }
    inline bool uread_accept(Num num)       { return Input::read_num(num.num); }

    template<typename T>
    inline void uread_accept(const T others) { static_assert(sizeof(T) == 0, "Invalid specifier."); }
};

/**
 * @brief Принимает в качестве аргументов функции-спецификаторы и считывает данные из буфера.
 * @note Использует методы класса Input из группы input_read.
 * @tparam Args Тип переменных аргументов.
 * @param args Аргументы.
 * @warning Количество аргументов ограничено 20.
 * @warning Если передать в качестве аргумента не функцию-спецификатор,
 *          то будет ошибка (Invalid specifier).
 * @see specs
 * @see \ref input_read
 * @return true Успешное выполнение.
 * @return false Ошибка выполнения.
 */
template<typename ... Args>
bool uread(const Args& ... args) {
    static_assert(sizeof...(args) <= 20, "Too many arguments for uread.");
    return (uread_sys::uread_accept(args) && ...);
}

#endif // __cplusplus

#endif //USARTINPUT_HPP_INCLUDED