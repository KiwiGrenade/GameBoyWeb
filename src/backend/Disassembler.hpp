#pragma once


#include <cstdint>

#include "utils.hpp"
#include "Memory.hpp"

#include <QPlainTextEdit>

enum Operator {
    none,
    increment,
    decrement
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

    Utils::Flag Z_;
    Utils::Flag N_;
    Utils::Flag H_;
    Utils::Flag C_;

private:
    Utils::Flag getFlagFromString(const std::string& str) {
        if(str == "-")
            return Utils::Flag::nothing;
        else if(str == "1")
            return Utils::Flag::set;
        else if(str == "0")
            return Utils::Flag::reset;
        else
            return Utils::Flag::setOrReset;
    }
};


class Disassembler {
public:
    Disassembler();
    ~Disassembler() = default;

    void disassemble(const Memory& memory, QPlainTextEdit& plainTextEdit);

private:
    
    Assembly disassembleOneOpcode(const uint8_t& byte);
    bool prefix_;
};


