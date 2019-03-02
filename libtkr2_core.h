//
// Created by konan on 02.03.19.
//

#ifndef LIBTKR2_BASE_H
#define LIBTKR2_BASE_H
#include <map>

class tkrBaseClass;

/**
 * @brief Тип обработчика события, используется в системе рассылки событий
 * синглтоном \ref event.
 */

using tHandler = void (tkrBaseClass::*)(void* data);

using tEvRec = std::pair<tkrBaseClass*, tHandler>;
using tEvMap = std::multimap<int, tEvRec>;
using tEvIDMap = std::map<std::string, int>;

/**
 * @class event
 * @brief Синглтон для рассылки событий
 *
 * @details Класс предназначен для рассылки событий всем остальным классам проекта.
 * Идентификация событий осуществляется по строковым константам. Классы регистрируют
 * свои обработчики событий в конструкторе. Обработчик принимает единственный параметр
 * типа void*, предполагается, что обработчик знает, что за структуру ему передали и
 * что с ней дальше делать.
 */

class event {
private:
    tEvMap evmap;
    tEvIDMap idmap;
    int nextID;
    const std::string* cev;
    event() {nextID = 0; cev = nullptr;}
    static event& getInstance();
    void _add(int htype, tkrBaseClass* cl, tHandler h);
    void _evoke(int ev, void* data);
    void _add(const std::string& ev, tkrBaseClass* cl, tHandler h);
    void _evoke(const std::string& ev, void* data = nullptr);
    bool _is(const std::string& ev);
public:
    event(const event&) = delete;
    event& operator=(event&) = delete;
    static void reg(const std::string& ev, tkrBaseClass* cl, tHandler h);
    static void evoke(const std::string& ev, void* data = nullptr);
    static bool is(const std::string& ev);
};

// TODO: очередь событий, оббег события отдельным тредом
// TODO: разделение на дискретные (кнопка, все состояния сохраняются в очереди) и аналоговые
//       (при наличии в очереди такого же события заменяется значение)

/**
 * @class tkrBaseClass
 * @brief Корневой класс-родитель для библиотеки libtkr
 *
 * @details Абстрактный класс. Предоставляет методы регистрации обработчиков событий,
 * записи в лог и механизм заворачивания в тред.
 */

class tkrBaseClass {
private:
    virtual void handleEvent(void* data) = 0;
    friend event;
protected:
    void regEvent(const std::string& ev);
    template <typename T> void regEvent(const std::string& ev, T h);
public:
};

/**
 * @brief Зарегистрировать событие с собственным обработчиком
 *
 * @details Метод регистрирует в синглтоне \ref event собственный обработчик
 * (метод класса-наследника, принимающий единственный аргумент void*)
 * с привязкой к текстовому идентификатору события.
 *
 * @param[in] ev строка-идентификатор события
 * @param[in] h метод этого же класса, который будет обрабатывать события. Метод
 * должен принимать единственный параметр типа void*.
 */

template <typename T>
void tkrBaseClass::regEvent(const std::string& ev, T h) {
    event::reg(ev, this, reinterpret_cast<tHandler>(h));
}

#endif //LIBTKR2_BASE_H
