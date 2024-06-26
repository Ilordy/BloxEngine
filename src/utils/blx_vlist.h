#pragma once
#include <stdlib.h>
#include <string.h>

///The factor to multiply our list size by when resizing.
#define BLX_GROWTH_FACTOR 2

///Copies a value to the list.
#define blxAddValueToList(list, value) \
    {\
        typeof(value) temp = value;\
        list = _blxAddToList(list, &temp);\
    }
#define blxAddArrayToList(list, array, arrayCount)\
{\
    for (size_t i = 0; i < arrayCount; i++)\
    {\
        blxAddValueToList(list, array[i]);\
    }\
}
#define blxInitList(type) (type*)_blxInitList(sizeof(type), 2)

#define blxInitListWithSize(type, size) (type*)_blxInitList(sizeof(type), size)

#define _BLX_HEADER_SIZE sizeof(unsigned long) * 3

#define _BLX_GET_HEADER(list, headerElement) *((unsigned long*)((char*)list - _BLX_HEADER_SIZE + headerElement * sizeof(unsigned long)))

#define _BLX_SET_HEADER(list, headerElement, value) *((unsigned long*)((char*)list - _BLX_HEADER_SIZE + headerElement * sizeof(unsigned long))) = (value)

#define _BLX_MAX_LENGTH_I 0
#define _BLX_COUNT_I 1
#define _BLX_ELEMENT_SIZE_I 2

static void* _blxInitList(unsigned long elementSize, unsigned long intitialSize)
{
    char* list = (char*)malloc(_BLX_HEADER_SIZE + intitialSize * elementSize);
    unsigned long* header = list;
    header[_BLX_MAX_LENGTH_I] = intitialSize;
    header[_BLX_COUNT_I] = 0;
    header[_BLX_ELEMENT_SIZE_I] = elementSize;
    list += _BLX_HEADER_SIZE;
    return list;
}

static inline unsigned long blxGetListCount(void* list)
{
    return _BLX_GET_HEADER(list, _BLX_COUNT_I);
}

static void* _blxAddToList(void* list, void* element)
{
    unsigned long maxLength = _BLX_GET_HEADER(list, _BLX_MAX_LENGTH_I);
    unsigned long count = _BLX_GET_HEADER(list, _BLX_COUNT_I);
    unsigned long elementSize = _BLX_GET_HEADER(list, _BLX_ELEMENT_SIZE_I);

    if (count >= maxLength)
    {
        void* newList = _blxInitList(elementSize, maxLength * BLX_GROWTH_FACTOR);
        memcpy(newList, list, count * elementSize);

        _BLX_SET_HEADER(newList, _BLX_COUNT_I, count);

        free((char*)list - _BLX_HEADER_SIZE);

        list = newList;
    }

    memcpy((char*)list + (count * elementSize), element, elementSize);

    _BLX_SET_HEADER(list, _BLX_COUNT_I, count + 1);

    return list;
}

static void blxRemoveFromListAt(void* list, unsigned int index)
{
    unsigned long count = _BLX_GET_HEADER(list, _BLX_COUNT_I);
    unsigned long elementSize = _BLX_GET_HEADER(list, _BLX_ELEMENT_SIZE_I);

    char* elemetAtIndex = (char*)list + index * elementSize;

    memcpy(elemetAtIndex, ((elemetAtIndex + (elementSize))), (count - index - 1) * (elementSize));

    _BLX_SET_HEADER(list, _BLX_COUNT_I, count - 1);
}