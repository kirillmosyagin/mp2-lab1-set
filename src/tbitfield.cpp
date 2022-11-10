// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
typedef unsigned int TELEM;

TBitField::TBitField(int len)
{
	if (len < 0)
		throw exception("you cant set bit with negative lenght");

	BitLen = len;
	MemLen = len / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen]{ 0 };
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8); // учитывая, что индексация от 0
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM mask = 1;
	return mask << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw exception("you cant set bit with negative index");
	if ((n >= BitLen) || (n < 0))
		throw exception("index too large or negative");
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= BitLen) || (n < 0))
		throw exception("index too large or negative");
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= BitLen) || (n < 0))
		throw exception("index too large or negative");
	int i = GetMemIndex(n);
	return pMem[i] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;

	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	delete[] pMem;
	pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	//if (MemLen != bf.MemLen) // возможно придется переделать и проверять хвост большего на равенство нулю
	  //	return 0;
	if (MemLen > bf.MemLen)
		for (int i = bf.MemLen; i < MemLen; i++)
			if (pMem[i] != 0)
				return 0;

	if (bf.MemLen > MemLen)
		for (int i = MemLen; i < bf.MemLen; i++)
			if (bf.pMem[i] != 0)
				return 0;

	for (size_t i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
			return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField tmp(bf);

	if (BitLen >= bf.BitLen)
	{
		tmp = *this;
		for (int i = 0; i < bf.MemLen; i++)
		{
			tmp.pMem[i] = tmp.pMem[i] | bf.pMem[i];
		}
		return tmp;
	}

	for (int i = 0; i < MemLen; i++)
	{
		tmp.pMem[i] = tmp.pMem[i] | pMem[i];
	}

	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField tmp(bf);

	if (bf.BitLen >= BitLen)
	{
		tmp = *this;
		for (int i = 0; i < MemLen; i++)
		{
			tmp.pMem[i] = tmp.pMem[i] & bf.pMem[i];
		}
		return tmp;
	}

	for (int i = 0; i < bf.MemLen; i++)
	{
		tmp.pMem[i] = tmp.pMem[i] & pMem[i];
	}

	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);
	for (int i = 0; i < MemLen - 1; i++)
	{
		tmp.pMem[i] = ~tmp.pMem[i];
	}
	int diff = BitLen - (MemLen - 1) * (sizeof(TELEM) * 8);
	for (int i = BitLen - diff; i < BitLen; i++)
	{
		if (tmp.GetBit(i))
			tmp.ClrBit(i);
		else
			tmp.SetBit(i);

	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char c;
	do
	{
		istr >> c;
	} while (c != ' ');
	while (true)
	{
		istr >> c;
		if (c == '0')
			bf.ClrBit(i++);
		else if (c == '1')
			bf.SetBit(i++);
		else break;
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.GetLength(); ++i)
	{
		ostr << (bf.GetBit(i) ? '1' : '0');
	}
	return ostr;
}
