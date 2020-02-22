/*
* This software is copyright protected (C) 2009 XQBS
*
* Author:                Alexey N. Zhirov
* E-mail:                src@xqbs.ru
* Module:                xqbs_sp_ex1.cpp
*
*/

// Работа с вводом/выводом
#include <stdio.h>
// Работа со строками
#include <string>

///////////////////////////////////////////////////////////////////////////////
// Это самый простой в мире SmartPointer, который умеет контролировать только
// один объект, не использующий подсчет ссылок.
template<class T>
class XQBS_SmartPtr
{
    // Указатель на контролируемый объект
    volatile T* m_ptr;

public:

    // Конструктор
    XQBS_SmartPtr()
    {
        // Инициализируем указатель
        m_ptr = NULL;
    }

    // Деструктор
    virtual ~XQBS_SmartPtr()
    {
        // Если указатель валидный (корректный)
        if (m_ptr)
        {
            // Удалить объект
            delete m_ptr;
            // После удаления объекта, обязательно зануляем указатель
            m_ptr = NULL;
        }
    }

    // Перегрузим оператор стрелочка для прозрачного доступа к реальному объекту
    T* operator-> ()
    {
        // Если объекта еще нет, создаем его
        if (!m_ptr)
        {
            // Создаем реальный объект
            m_ptr = new T;
            // Если объект не создался
            if (!m_ptr)
            {
                // Генерируем исключение
                throw std::bad_alloc();
            }
        }

        // Возвращаем указатель на реальный объект
        return m_ptr;
    }

    // Перегрузим оператор разыменования для прозрачного доступа к реальному объекту
    T& operator* ()
    {
        // Для создания объекта используем уже реализованный метод
        return *operator->();
    }
};

// Точка входа
int main(void)
{
    // Описываем SmartPointer для динамического контроля на строкой std::string
    XQBS_SmartPtr<std::string> SmartPtr;

    try
    {
        // Инициализируем строку
        SmartPtr->assign("Hello world!");

        // Распечатываем строку в консоль
        printf("%s", (*SmartPtr).c_str());
    }
    catch (std::bad_alloc& e) // Перехватываем исключение std::bad_alloс
    {
        // Распечатываем строку исключения
        printf("%s", e.what());
    }

    // Все ОК
    return 0;
}
