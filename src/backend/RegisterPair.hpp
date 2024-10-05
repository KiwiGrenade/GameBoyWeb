#pragma once

#include "utils.hpp"

struct RegisterPair {

    RegisterPair() = default;

    RegisterPair(const u16 val) {
        setVal(val);
    }

    ~RegisterPair() = default;

    inline void setVal(const u16 val) {
        hi_ = val >> 8 & 0xFF;
        lo_ = val & 0xFF;
    }

    inline u16 getVal() const {
        return (hi_ << 8) | lo_;
    }


    inline RegisterPair& operator=(const RegisterPair rhs) {
        hi_ = rhs.hi_;
        lo_ = rhs.lo_;
        return *this;
    }

    inline RegisterPair& operator+=(const RegisterPair rhs) {
        setVal(getVal() + rhs.getVal());
        return *this;
    }

    inline RegisterPair& operator-=(const RegisterPair rhs) {
        setVal(getVal() - rhs.getVal());
        return *this;
    }

    inline RegisterPair& operator++() {
        setVal(getVal() + 1);
        return *this;
    }

    inline RegisterPair& operator--() {
        setVal(getVal() - 1);
        return *this;
    }

    inline RegisterPair operator++(int) {
        RegisterPair old = *this;
        operator++();
        return old;
    }

    inline RegisterPair operator--(int) {
        RegisterPair old = *this;
        operator--();
        return old;
    }

    friend RegisterPair operator+(RegisterPair lhs, const RegisterPair rhs) {
        lhs += rhs;
        return lhs;
    }

    friend RegisterPair operator+(RegisterPair lhs, const int rhs) {
        lhs += rhs;
        return lhs;
    }

    friend RegisterPair operator-(RegisterPair lhs, const RegisterPair rhs) {
        lhs -= rhs;
        return lhs;
    }

    inline operator u16() const {
        return (hi_ << 8) | lo_;
    }

    u8 hi_, lo_;
};


