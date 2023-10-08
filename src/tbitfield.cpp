// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw - 1;
    }
    BitLen = len;
    MemLen = (BitLen - 1) /( sizeof(TELEM) * 8)+1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n>BitLen) {
        throw -1;
    }
    const int index = n / 32;
    return index;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen) {
        throw -1;
    }
    const int bit_number = n % 32; // номер бита в ячейке pMem
    TELEM mask = 1;
    mask << bit_number;
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n<0 || n>BitLen) {
        throw -1;
    }
    const TELEM mask = GetMemMask(n);
    const int index = GetMemIndex(n);
    pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n<0 || n>BitLen) {
        throw -1;
    }
    TELEM mask = GetMemMask(n);
    const int index = GetMemIndex(n);
    mask = ~mask;
    pMem[index] = pMem[index] & mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n<0 || n>BitLen) {
        throw -1;
    }
    const TELEM mask = GetMemMask(n);
    const int index = GetMemIndex(n);
    TELEM perem = pMem[index] & mask;
    if (perem == 0) {
        return 0;
    }
    else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
        if (MemLen == bf.MemLen) {
            while (int i = 0 < MemLen) {
                if (pMem[i] == bf.pMem[i]) {
                    i++;
                }
                else {
                    return 0;
                }
            }
            return 1;
        }
        else return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (MemLen != bf.MemLen) {
        return 1;
    }
    else {
        while (int i = 0 < MemLen) {
            if (pMem[i] == bf.pMem[i]) {
                i++;
            }
            else {
                return 1;
            }
        }
        return 0;
    }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (MemLen != bf.MemLen) {
        throw -1;
    }
    for (int i = 0; i < MemLen; i++) {
       pMem[i] = pMem[i] | bf.pMem[i];
    }
    return *pMem;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (MemLen != bf.MemLen) {
        throw -1;
    }
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = pMem[i] & bf.pMem[i];
    }
    return *pMem;
}

TBitField TBitField::operator~(void) // отрицание
{
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = ~pMem[i];
    }
    return *pMem;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int* array = new int[bf.BitLen];
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> array[i];
    }
    for (int i = 0; i < bf.BitLen; i++) {
        if (array[i] == 1) {
            bf.SetBit(i);
        }
        if (array[i] == 0) {
            bf.ClrBit(i);
        }
        else throw -1;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
   }
    return ostr;
}
