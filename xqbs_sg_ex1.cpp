/*
* This software is copyright protected (C) 2009 XQBS
*
* Author:                Alexey N. Zhirov
* E-mail:                src@xqbs.ru
* Module:                xqbs_sg_ex1.cpp
*
*/

#include <windows.h>
#include <tchar.h>
#include <new>

#include "xqbs_smartguard.h"

// Используем пространство имен XQBS
using namespace XQBS;

///////////////////////////////////////////////////////////////////////////////
// Простой класс для проверки функциональности XQBS_SmartGuard-а
struct XQBS_SimpleClass
{
    // Конструктор
    XQBS_SimpleClass()
    {
        _tprintf(_T("Ctor XQBS_SimpleClass\n"));
    }

    // Деструктор
    ~XQBS_SimpleClass()
    {
        _tprintf(_T("Dtor XQBS_SimpleClass\n"));
    }
};

///////////////////////////////////////////////////////////////////////////////
// Ссылочный класс для проверки функциональности XQBS_SmartGuard-а
class XQBS_RefClass : public XQBS_RefBase
{
protected:

    // Виртуальный деструктор
    virtual ~XQBS_RefClass()
    {
        _tprintf(_T("Dtor XQBS_RefClass\n"));
    }

public:

    // Конструктор
    XQBS_RefClass()
    {
        _tprintf(_T("Ctor XQBS_RefClass\n"));
    }

    // Создать себя
    static XQBS_RefClass* CreateSelf(void)
    {
        return XQBS_new<XQBS_RefClass>();
    }

};

// Примеры использования мега полезного шаблонного класса XQBS_SmartGuard и его наиболее часто
// используемых определений XQBS_SmartFree, XQBS_SmartDelete, XQBS_SmartDeleteArray,
// XQBS_SmartRelease и XQBS_SmartHandle.
// Пройдите этот код по шагам в режиме отладки и получите удовольствие!
int main(void)
{
    // Пример динамического выделения памяти через malloc
    int* p1 = (int*)malloc( 10 );

    // Область видимости для теста SmartGuard-а. Пример автоматического освобождения памяти на
    // которую указывает p1 через вызов функции free и зануления указателя p1
    {
        XQBS_SmartGuard< int*,  void(__cdecl*)(void*) >  SmartGuard(p1, ::free, NULL);
    }

    // Область видимости для теста XQBS_SmartFree. Пример автоматического освобождения памяти
    {
        XQBS_SmartFree< int* >  SmartFree( (int*)malloc( 10 ) );
    }

    // Вывод текста
    _tprintf(_T("Test for XQBS_SmartGuard< XQBS_SimpleClass*,  XQBS_DeleteObject >:\n"));

    // Пример динамического создания объекта класса XQBS_SimpleClass
    XQBS_SimpleClass* p2 = XQBS_new<XQBS_SimpleClass>();

    // Область видимости для теста SmartGuard-а. Пример автоматического освобождения памяти на
    // которую указывает p2 через вызов оператора delete и зануления указателя p2
    {
        XQBS_SmartGuard< XQBS_SimpleClass*,  XQBS_DeleteObject >  SmartGuard(p2, NULL);
    }

    // Вывод текста
    _tprintf(_T("\nTest for XQBS_SmartDelete< XQBS_SimpleClass* >:\n"));

    // Область видимости для теста XQBS_SmartDelete-а. Пример автоматического удаления объекта
    {
        XQBS_SmartDelete< XQBS_SimpleClass* >  SmartDelete( XQBS_new<XQBS_SimpleClass>() );
    }

    // Вывод текста
    _tprintf(_T("\nTest for XQBS_SmartGuard< XQBS_SimpleClass*,  XQBS_DeleteObjectArray >:\n"));

    // Пример динамического создания массива объектов класса XQBS_SimpleClass
    p2 = XQBS_new<XQBS_SimpleClass>(3);

    // Область видимости для теста SmartGuard-а. Пример автоматического освобождения массива объектов XQBS_SimpleClass
    // на который указывает p2 через вызов оператора delete [] и зануления указателя p2
    {
        XQBS_SmartGuard< XQBS_SimpleClass*,  XQBS_DeleteObjectArray >  SmartGuard(p2, NULL);
    }

    // Вывод текста
    _tprintf(_T("\nTest for XQBS_SmartDeleteArray< XQBS_SimpleClass* >:\n"));

    // Область видимости для теста XQBS_SmartDeleteArray-а. Пример автоматического удаления массива объектов
    {
        XQBS_SmartDeleteArray< XQBS_SimpleClass* >  SmartDeleteArray( XQBS_new<XQBS_SimpleClass>(3) );
    }

    // Вывод текста
    _tprintf(_T("\nTest for XQBS_SmartGuard< XQBS_RefClass*, LONG (XQBS_RefBase::*)(void), XQBS_GuardWithoutParam, XQBS_RefBase >:\n"));

    // Пример динамического создания класса XQBS_RefClass
    XQBS_RefClass* p3 = XQBS_RefClass::CreateSelf();

    // Область видимости для теста SmartGuard-а.
    // Пример автоматического снятия своей ссылки через вызов функции XQBS_Refbase::Release()
    {
        XQBS_SmartGuard< XQBS_RefClass*,  LONG (XQBS_RefBase::*)(void), XQBS_GuardWithoutParam,
            XQBS_RefBase >  SmartGuard(p3, &XQBS_RefBase::Release, NULL, *p3);
    }

    // Вывод текста
    _tprintf(_T("\nTest for XQBS_SmartRelease< XQBS_RefClass* >:\n"));

    // Область видимости для теста SmartRelease-а. Пример автоматического удаления ссылки
    {
        XQBS_SmartRelease< XQBS_RefClass* >  SmartRelease( XQBS_RefClass::CreateSelf() );
    }

    // Для примера работы SmartGuard-а, создадим объект синхронизации (можно было бы создать и любой другой объект ядра)
    HANDLE h = ::CreateMutex(NULL, FALSE, NULL);

    // Область видимости для теста SmartGuard-а. Пример автоматического закрытия дескриптора.
    {
        XQBS_SmartGuard< HANDLE,  BOOL (__stdcall*)(HANDLE) >  SmartGuard(h, ::CloseHandle, NULL);
    }

    // Для примера работы SmartHandle-а, создадим объект синхронизации
    h = ::CreateMutex(NULL, FALSE, NULL);

    // Область видимости для теста XQBS_SmartHandle-а. Пример автоматического закрытия дескриптора.
    {
        // В этом случае, после закрытия, дескриптор h автоматически инициализируется значением NULL
        XQBS_SmartHandle SmartHandle1(h, NULL);
    }

    // Область видимости для теста XQBS_SmartHandle-а. Пример автоматического закрытия дескриптора.
    {
        // В этом случае, не используется дополнительная переменная для хранения значения дескриптора.
        XQBS_SmartHandle SmartHandle2( ::CreateMutex(NULL, FALSE, NULL) );
    }

    // Тест закончен, спасибо за внимание!
    return 0;
}
