// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"



size_t bitLen = 0;   // длина битового поля - макс. к-во битов
elType* pMem = 0;      // память для представления битового поля
size_t memLen = 0;   // к-во эл-тов elType для представления бит.поля //длина массива pMem
size_t uint_bitLen = sizeof(elType) * 8; // число бит в elType

TBitField::TBitField(size_t len): bitLen(len)
{
    if (len < 0) throw("Incorrect input");
    memLen = (bitLen + uint_bitLen - 1) / uint_bitLen;
    if (len == 0)
        pMem = 0;
    else
        pMem = new elType[memLen];
    clrBitField();
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
        bitLen = bf.getLength();
        memLen = (bitLen + uint_bitLen - 1) / uint_bitLen;
        pMem = new elType[memLen];

        for (size_t i = 0; i < memLen; i++)
            pMem[i] = bf.pMem[i];
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return ((n + uint_bitLen)/ uint_bitLen) -1;
}

elType TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    elType mask = 1 << (n % uint_bitLen);
    return mask;
}

// доступ к битам битового поля
size_t TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return memLen * sizeof(elType);
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n < bitLen) {
        size_t index = getIndex(n);
        elType mask = getMask(n);
        pMem[index] |= mask;
    }
    else
        throw("ERROR: too large index");
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n < bitLen) {
        size_t index = getIndex(n);
        elType mask = getMask(n);
        pMem[index] &= (~mask);
    }
    else
        throw("ERROR: too large index");
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n < bitLen) {
        size_t index = getIndex(n);
        elType mask = getMask(n);
        return (mask & pMem[index]);
    }
    else
        throw("ERROR: too large index");
}

void TBitField::clrBitField()
{
    for (size_t i = 0; i < memLen; i++)
        pMem[i] = 0;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }
    else {
        delete[] pMem;
        bitLen = bf.bitLen;
        memLen = bf.memLen;
        pMem = new elType[memLen];
        
        for (size_t i = 0; i < memLen; i++)
            pMem[i] = bf.pMem[i];
        return *this;
    }
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this != &bf) {
        if (bitLen != bf.bitLen) {
            return false;
        }
        else {
            for (size_t i = 0; i < bitLen; i++)
                if (this->getBit(i) != bf.getBit(i))
                    return false;
            return true;
        }
    }
    else {
        return true;
    }
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (this != &bf) {
        if (bitLen != bf.bitLen) {
            return true;
        }
        else {
            for (size_t i = 0; i < bitLen; i++)
                if (this->getBit(i) != bf.getBit(i))
                    return true;
            return false;
        }
    }
    else {
        return false;
    }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    const TBitField* maxLenBF = 0;
    size_t maxLen = std::max(bitLen, bf.bitLen);  
    size_t minLen = std::min(bitLen, bf.bitLen);
    if (bitLen == maxLen)
        maxLenBF = this;
    else
        maxLenBF = &bf;

    TBitField result(maxLen);
    for (size_t i = 0; i < maxLen; i++)
    {
        if (i < minLen) {
            if (this->getBit(i) | bf.getBit(i))
                result.setBit(i);
            else
                result.clrBit(i);
        }
        else {
            if (maxLenBF->getBit(i))
                result.setBit(i);
            else
                result.clrBit(i);
        }
    }
    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    const TBitField* maxLenBF = 0;
    size_t maxLen = std::max(bitLen, bf.bitLen);
    size_t minLen = std::min(bitLen, bf.bitLen);
    if (bitLen == maxLen)
        maxLenBF = this;
    else
        maxLenBF = &bf;

    TBitField result(maxLen);
    for (size_t i = 0; i < maxLen; i++)
    {
        if (i < minLen) {
            if (this->getBit(i) & bf.getBit(i))
                result.setBit(i);
            else
                result.clrBit(i);
        }
        else {
            result.clrBit(i);
        }
    }
    return result;
}

TBitField TBitField::operator~() // отрицание
{
    for (size_t i = 0; i < memLen; i++)
        pMem[i] = ~pMem[i];
    return *this;
}

TBitField::~TBitField()
{
    delete[] pMem;
}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    for (size_t i = 0; i < bf.bitLen; i++)
    {
        bool bit;
        istr >> bit;
        if (bit)
            bf.setBit(i);
        else
            bf.clrBit(i);
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (size_t i = 0; i < bf.bitLen; i++)
    {
        ostr << bf.getBit(i) << " ";
    }
    return ostr;
}
