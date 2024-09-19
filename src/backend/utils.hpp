#pragma once

#include <iostream>
#include <cfloat>
#include <cstdint>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <string>

typedef uint8_t u8;
typedef uint8_t Register;
typedef uint16_t u16;

#define U16_MAX UINT16_MAX 

namespace Utils {

const std::string YELLOW = "\e[36m";
const std::string RED = "\e[31m";
const std::string END = "\e[0m";

inline void warning(const std::string& str) { std::cerr << YELLOW << "[WRN]: " << str << END << std::endl; }
inline void error(const std::string& str) { std::cerr << RED << "[ERR]: " << str << END << std::endl; exit(2); }

enum Operator {
    none,
    increment,
    decrement
};

enum Flag {
    nothing,
    reset,
    set,
    setOrReset
};

struct Operand {
    Operand() = default;
    Operand(const QJsonObject& obj) {
        name_ = obj.value("name").toString().toStdString();
        bytes_ = obj.value("bytes").toInt();
        immediate_ = obj.value("immediate").toBool();
        getOperatorFromJsonObject(obj);
    }

    std::string name_;
    Operator    operator_;
    uint8_t     bytes_;
    bool        immediate_;

private:
    void getOperatorFromJsonObject(const QJsonObject& obj) {
        if(obj.contains("decrement"))
            operator_ = Operator::decrement;
        else if(obj.contains("increment"))
            operator_ = Operator::increment;
        else
            operator_ = Operator::none;
    }
};

struct Assembly {
    Assembly() = default;
    Assembly(const QJsonObject& obj, bool& prefix) {
        mnemonic_ = obj.value("mnemonic").toString().toStdString();
        bytes_ = obj.value("bytes").toInt();

        QJsonArray jsonCycles = obj.value("cycles").toArray();
        cycles_.first = jsonCycles.first().toInt();
        cycles_.second = jsonCycles.size() > 1 ? jsonCycles.last().toInt() : 0;

        QJsonArray jsonOperands = obj.value("operands").toArray();
        for(auto op : jsonOperands) {
            operands_.push_back(Operand(op.toObject()));
        }
        immediate_ = obj.value("immediate").toBool();

        QJsonObject jsonFlags = obj.value("flags").toObject();
        Z_ = getFlagFromString(jsonFlags.value("Z").toString().toStdString());
        N_ = getFlagFromString(jsonFlags.value("N").toString().toStdString());
        H_ = getFlagFromString(jsonFlags.value("H").toString().toStdString());
        C_ = getFlagFromString(jsonFlags.value("C").toString().toStdString());
        prefix = mnemonic_ == "PREFIX"; 
    }
    ~Assembly() = default;
    const QString toString() {
        QString str = "";
        str += mnemonic_;
        for(auto op : operands_)
            str += " " + op.name_;
        return str;
    };

    std::string mnemonic_;

    std::pair<uint8_t, uint8_t> cycles_;
    uint8_t                     bytes_;

    std::vector<Operand>    operands_;
    bool                    immediate_;

    Flag Z_;
    Flag N_;
    Flag H_;
    Flag C_;

private:
    Flag getFlagFromString(const std::string& str) {
        if(str == "-")
            return Flag::nothing;
        else if(str == "1")
            return Flag::set;
        else if(str == "0")
            return Flag::reset;
        else
            return Flag::setOrReset;
    }
};

} // Utils

