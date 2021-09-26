// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Множество - реализация через битовые поля

#include <iostream>
#include "tbitfield.h"
#include "tset.h"

size_t maxPower = 0;

TSet::TSet(size_t mp) : bitField(mp), maxPower(mp) {}

// конструктор копирования
TSet::TSet(const TSet &s) : bitField(s.bitField), maxPower(s.maxPower) {}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : bitField(bf), maxPower(bf.getLength()) {}

TSet::operator TBitField()
{
    return TBitField(bitField);
}

size_t TSet::getMaxPower() const // получить макс. к-во эл-тов
{
    return maxPower;
}

bool TSet::isMember(const uint elem) const // элемент множества?
{
    if (bitField.getBit(elem))
        return true;
    else
        return false;
}

void TSet::insElem(const uint elem) // включение элемента множества
{
    if (elem < maxPower && elem >= 0)
        bitField.setBit(elem);
    else
        throw("Incorrer input");
}

void TSet::delElem(const uint elem) // исключение элемента множества
{
    if (elem < maxPower && elem >= 0)
        bitField.clrBit(elem);
    else
        throw("Incorrer input");
}

// теоретико-множественные операции
TSet& TSet::operator=(const TSet &s) // присваивание
{
    maxPower = s.maxPower;
    bitField = s.bitField;
    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
    if (maxPower == s.maxPower && bitField == s.bitField)
        return true;
    else
        return false;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet result(std::max(maxPower, s.maxPower));
    result.bitField = bitField | s.bitField;
    return result;
}

TSet TSet::operator+(const uint elem) // объединение с элементом
{
    if (elem < 0 || elem>=maxPower) throw("Incorrect input"); 
    TSet result(std::max(maxPower, (size_t)elem+1));
    result.bitField = bitField | result.bitField;
    result.bitField.setBit(elem);
    return result;
    //
    // операция, в принципе, работает и для элементов с номером больше (maxPower-1),
    // но тест рассматривает это как ошибку
    //
}

TSet TSet::operator-(const uint elem) // разность с элементом
{
    if (elem < 0 || elem >= maxPower) throw("Incorrect input");
    TSet result(std::max(maxPower, (size_t)elem + 1));
    result.bitField = bitField | result.bitField;
    result.bitField.clrBit(elem);
    return result;
    //
    // операция, в принципе, работает и для элементов с номером больше (maxPower-1),
    // но тест рассматривает это как ошибку
    //
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet result(std::max(maxPower, s.maxPower));
    result.bitField = bitField & s.bitField;
    return result;
}

TSet TSet::operator~() // дополнение
{
    bitField = ~bitField;
    return *this;
}

// перегрузка ввода/вывода
std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    size_t n;
    istr >> n;
    if (n > maxPower) throw("Too large number of elements");
    s.bitField.clrBitField();
    for (size_t i = 0; i < n; i++) 
    {
        size_t elem;
        istr >> elem;
        s.bitField.setBit(elem);
    }
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << "Max power: " << maxPower << std::endl;
    for (size_t i = 0; i < maxPower; i++)
    {
        if (s.bitField.getBit(i))
            ostr << i << " ";
    }
    return ostr;
}
