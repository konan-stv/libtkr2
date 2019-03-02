//
// Created by konan on 26.02.19.
//

#ifndef LIBTKR2_TYPES_H
#define LIBTKR2_TYPES_H

#include "svnversion.h"
#include <bitset>

/**
 * @class ifBitset
 * @brief Интерфейс для хранения любых значений с побитовым доступом
 *
 * @details Класс предназначен для обеспечения побитового доступа к значениям любого типа
 * с сохранением обычного интерфейса - операторов присваивания, взятия адреса и приведения
 * типов. Прямой доступ к битам осуществляется через битсет (b[0] - b[N], установка и сброс
 * бита по индексу также возможен соответствующими методами:
 * @code
    ifBitmap<uint16_t> b(0xf0);         // создаём и инициализируем экземпляр
    std::cout << b.b[2] << std::endl;   // доступ к битам
    std::cout << b.b[4] << std::endl;
    b.clear(4);                         // сброс бита
    std::cout << b.b[4] << std::endl;
    uint16_t a = b + 1;                 // присваивание значения
    std::cout << a << std::endl;
    uint16_t& c(b);                     // создаём и иницализируем ссылку на значение
    c = 0xff;
    std::cout << b.b[2] << std::endl;
 * @endcode
 *
 * @warning Переопределён оператор взятия адреса! Выражение &bitset вернёт указатель на (T*),
 * а не на класс (ifBitset*).
 */

template <typename T>
union ifBitmap {
    T base;     /// переменная исходного типа
    std::bitset<sizeof(T)> b;       /// битсет в той же области памяти
    explicit ifBitmap(T v) : base(v) {}
    void set(unsigned int b) {base |= (1<<b);}       /// установить бит в 1 (устаревшая)
    void clear(unsigned int b) {base &= ~(1<<b);}    /// сбросить бит в 0 (устаревшая)
    T& operator = (const T& v) {base=v; return *this;}      /// присваивание значение типа T
    operator T() const {return base;}       /// неявное приведение к типу T
    operator T&() {return base;}        /// неявное приведение к ссылке на T
    T* operator &() {return &base;}     /// взятие адреса возвращает адрес на base
};

#endif //LIBTKR2_TYPES_H
