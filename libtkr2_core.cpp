//
// Created by konan on 02.03.19.
//

#include "libtkr2_core.h"

event& event::getInstance() {
    static event inst;
    return inst;
}

void event::_add(int htype, tkrBaseClass* cl, tHandler h) {
    evmap.emplace(htype, tEvRec(cl, h));
}

void event::_add(const std::string& ev, tkrBaseClass* cl, tHandler h) {
    auto i = idmap.find(ev);
    if (i == idmap.end()) {
        idmap[ev] = nextID++;
        i = idmap.find(ev);
    }
    _add(i->second, cl, h);
}

void event::_evoke(int ev, void* data) {
    std::pair<tEvMap::iterator, tEvMap::iterator> range = evmap.equal_range(ev);
    for(auto i = range.first; i != range.second; i++) (i->second.first->*i->second.second)(data);
}

void event::_evoke(const std::string& ev, void* data) {
    cev = &ev;
    auto i = idmap.find(ev);
    if (i != idmap.end()) {
        _evoke(i->second, data);
    }
    cev = nullptr;
}

bool event::_is(const std::string& ev) {
    if (!cev) return false;
    return (*cev == ev);
}

/**
 * @brief Зарегистрировать обработчик события
 *
 * @details Метод регистрирует обработчик события. При инициировании события
 * будут вызваны все обработчики, привязанные к идентификатору ev.
 *
 * @param[in] ev строка-идентификатор события
 * @param[in] cl указатель на экземпляр класса, методом которого является обработчик
 * @param[in] р указатель на метод класса, являющийся обработчиком события. Метод должен
 * принимать единственный параметр типа void*.
 */

 void event::reg(const std::string& ev, tkrBaseClass* cl, tHandler h) {
    getInstance()._add(ev, cl, h);
}

/**
 * @brief Инициировать событие
 *
 * @details Метод вызывает все обработчики, привязанные к идентификатору ev,
 * передавая им указатель на набор данных.
 *
 * @param[in] ev строка-идентификатор события
 * @param[in] data указатель на произвольный блок данных, который будет передан
 * обработчикам события
 */

void event::evoke(const std::string& ev, void* data) {
    getInstance()._evoke(ev, data);
}

/**
 * @brief Проверка вызванного события
 *
 * @details Метод возвращает true, если в настоящий момент происходит рассылка
 * события, переданного в качестве аргумента. Импользуется в случае, когда на
 * несколько событий назначен один обработчик, и ему нужно выяснить, какое именно
 * событие его вызвало.
 *
 * @param[in] ev строка-идентификатор события
 *
 * @return true, если переданный идентификатор совпадает с событием, которое
 * рассылается в настоящий момент.
 */

bool event::is(const std::string& ev) {
    return getInstance()._is(ev);
}

/**
 * @brief Зарегистрировать событие с обработчиком по умолчанию
 *
 * @details Метод регистрирует в синглтоне \ref event обработчик по умолчанию (handleEvent)
 * данного класса с привязкой к текстовому идентификатору события.
 *
 * @param[in] ev строка-идентификатор события
 */

void tkrBaseClass::regEvent(const std::string& ev) {
    event::reg(ev, this, &tkrBaseClass::handleEvent);
}

