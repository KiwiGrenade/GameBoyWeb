#pragma once

#include <cstdint>
#include "utils.hpp"

struct RegisterPair
{
	public:
	explicit RegisterPair(u16 rp = 0);
	
    RegisterPair &operator+=(const u16);
    RegisterPair &operator-=(const u16);
	RegisterPair &operator++();
	RegisterPair operator++(int);
	RegisterPair &operator--();
	RegisterPair operator--(int);
	RegisterPair &operator=(const u16);
	operator u16() const;
	u8 hi, lo;
	
};

inline RegisterPair::RegisterPair(u16 rp)
	: hi {static_cast<u8>(rp >> 8 & 0xff)}, 
	  lo {static_cast<u8>(rp & 0xff)}
{}

inline RegisterPair &RegisterPair::operator+=(const u16 d)
{
	u16 x {*this};
	*this = x + d;
	return *this;
}

inline RegisterPair &RegisterPair::operator-=(const u16 d)
{
	u16 x {*this};
	*this = x - d;
	return *this;
}

inline RegisterPair &RegisterPair::operator++()
{
	u16 x {*this};
	*this = ++x;
	return *this;
}
	
inline RegisterPair RegisterPair::operator++(int)
{
	RegisterPair tmp {*this};
	++*this;
	return tmp;
}

inline RegisterPair &RegisterPair::operator--()
{
	u16 x {*this};
	*this = --x;
	return *this;
}

inline RegisterPair RegisterPair::operator--(int)
{
	RegisterPair tmp {*this};
	--*this;
	return tmp;
}

inline RegisterPair &RegisterPair::operator=(const u16 x)
{
	hi = x >> 8 & 0xff;
	lo = x & 0xff;
	return *this;
}
	
inline RegisterPair::operator u16() const
{
	return (hi << 8) | lo;
}
