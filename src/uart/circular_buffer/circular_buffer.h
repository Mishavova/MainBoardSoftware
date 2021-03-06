/***************************************************************************************************

 В этом файле находится шаблонный класс кольцевого буфера. Эта реализация НЕ ПОТОКОБЕЗОПАСНАЯ,
 ее нельзя использовать в прерываниях и вне прерываний.

***************************************************************************************************/

#pragma once

#include <stdint.h>
#include <algorithm>
#include "project_config.h"

/**************************************************************************************************
Описание:  Класс кольцевого буфера

Аргументы: T - тип элемента буфера
           TBufSize - размер буфера

Замечания: Все функции помечены как volatile, чтобы их можно было вызывать для volatile экземпляра.
           Этот класс не предназначен для использования в прерываниях и вне прерываний,
           для этого используйте BlockingCircularBuffer!
**************************************************************************************************/
template< typename T, uint32_t TBufSize>
class CircularBuffer
{

public:

    CircularBuffer() : headPos(0),
                       tailPos(0),
                       count(0)
    {
    }

    /**************************************************************************************************
    Описание:  Функция сброса
    Аргументы: -
    Возврат:   -
    Замечания: Просто обнуляет все счетчики
    **************************************************************************************************/
    void reset(void) volatile
    {
        headPos = 0;
        tailPos = 0;
        count = 0;
    }

    /**************************************************************************************************
    Описание:  Запись элемента в начало буфера
    Аргументы: T elem - новый элемент
    Возврат:   -
    Замечания: -
    **************************************************************************************************/
    void writeHead(T elem) volatile
    {
        buffer[headPos] = elem;
        this->incHead();
        ++count;

        UMBA_ASSERT(count <= TBufSize);
        
        // хвост обогнал голову - этого не должно происходить
        UMBA_ASSERT( (tailPos + count) % TBufSize == headPos );
    }

    /**************************************************************************************************
    Описание:  Чтение элемента с конца буфера
    Аргументы: -
    Возврат:   Прочитанный элемент
    Замечания: -
    **************************************************************************************************/
    T readTail( void ) volatile
    {
        T elem = buffer[tailPos];
        incTail();
        --count;

        UMBA_ASSERT(count <= TBufSize);
        
        UMBA_ASSERT( (tailPos + count) % TBufSize == headPos );

        return elem;
    }

    /**************************************************************************************************
    Описание:  Пуст ли буфер?
    Аргументы: -
    Возврат:   true/false
    Замечания: -
    **************************************************************************************************/
    bool isEmpty(void) const volatile
    {
        if(count == 0 && headPos != tailPos)
        {
            UMBA_ASSERT(true);
        }

        return count == 0;
    }

    /**************************************************************************************************
    Описание:  Полон ли буфер?
    Аргументы: -
    Возврат:   true/false
    Замечания: -
    **************************************************************************************************/
    bool isFull(void) const volatile
    {
        return count == TBufSize;
    }

    /**************************************************************************************************
    Описание:  Сколько элементов использовано
    Аргументы: -
    Возврат:   Количество использованных элементов
    Замечания: -
    **************************************************************************************************/
    uint32_t getUsedElemNum(void) const volatile
    {
        return count;
    }

    /**************************************************************************************************
    Описание:  Сколько элементов пусто
    Аргументы: -
    Возврат:   Количество пустых элементов
    Замечания: -
    **************************************************************************************************/
    uint32_t getFreeElemNum(void) const volatile
    {
        return TBufSize - count;
    }

    /**************************************************************************************************
    Описание:  Записать массив в кольцевой буфер
    Аргументы: newData - указатель на массив, который нужно записать
               size - сколько элементов нужно записать
    Возврат:   -
    Замечания: -
    **************************************************************************************************/
    void putData(const T * newData, uint32_t size) volatile
    {
        // не хватает места для записи
        UMBA_ASSERT( getFreeElemNum() >= size );

        // количество свободных ячеек в буфере до его "конца"
        uint32_t freeTillEnd = TBufSize - headPos;
                
        count += size;

        // если новые данные влезают в буфер без закольцовки
        if( size < freeTillEnd )
        {
            // то просто добавим их как в обычный массив
            std::copy( newData, newData+size, buffer + headPos );
            headPos += size;
        }
        // если не влезают
        else
        {
            // то добавим часть до "конца" буфера
            std::copy(newData, newData+freeTillEnd, buffer + headPos );
            size -= freeTillEnd;

            // а остальное - с "начала"
            std::copy( newData+freeTillEnd, newData+freeTillEnd+size, buffer );
            headPos = size;
        }

        return;
    }

private:

    // увеличить указатель для записи
    void incHead(void) volatile
    {
        headPos++;
        headPos %= TBufSize;
    }

    // увеличить указатель для чтения
    void incTail(void) volatile
    {
        tailPos++;
        tailPos %= TBufSize;
    }

    T buffer[TBufSize];
    uint32_t headPos;
    uint32_t tailPos;
    uint32_t count;

};
