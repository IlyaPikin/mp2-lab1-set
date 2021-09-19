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
uint* pMem = 0;      // память для представления битового поля
size_t memLen = 0;   // к-во эл-тов uint для представления бит.поля //длина массива pMem
size_t uint_bitLen = sizeof(uint) * 8; // число бит в uint

TBitField::TBitField(size_t len): bitLen(len)
{
    //uint_bitLen = sizeof(uint) * 8;
    memLen = getIndex(bitLen - 1);
    pMem = new uint[memLen];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    if (&bf != this) {
        //uint_bitLen = sizeof(uint) * 8;
        bitLen = bf.getLength();
        memLen = getIndex(bitLen - 1);
        pMem = new uint[memLen];

        for (size_t i = 0; i < bitLen; i++)
            if (bf.getBit(i))
                this->setBit(i);
            else
                this->clrBit(i);
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return n / uint_bitLen;
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    uint mask = 1 << (n % uint_bitLen);
    return mask;
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n < bitLen) {
        size_t index = getIndex(n);
        uint mask = getMask(n);
        pMem[index] |= mask;
    }
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n < bitLen) {
        size_t index = getIndex(n);
        uint mask = getMask(n);
        pMem[index] &= (~mask);
    }
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n < bitLen) {
        size_t index = getIndex(n);
        uint mask = getMask(n);
        return (mask & pMem[index]);
    }
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return TBitField(1);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return TBitField(1);
}

TBitField TBitField::operator~() // отрицание
{
    return TBitField(1);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
