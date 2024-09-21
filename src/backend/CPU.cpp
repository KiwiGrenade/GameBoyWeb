#include "CPU.hpp"
#include "utils.hpp"
#include <QtLogging>
#include <array>
#include <qjsonobject.h>
#include <qlogging.h>
#include <string>

CPU::CPU(Memory& memory)
    : memory_(memory)
    , A_(AF_.hi_)
    , flags_(AF_.lo_)
    , B_(BC_.hi_)
    , C_(BC_.lo_)
    , D_(DE_.hi_)
    , E_(DE_.lo_)
    , H_(HL_.hi_)
    , L_(HL_.lo_) {
    std::string jsonFilePath = "/home/desktop/GameBoyWeb/res/opcodes.json";
    QJsonObject obj = Utils::getJsonObjectFromFile(jsonFilePath);
    QJsonObject unprefixed = obj.value("unprefixed").toObject();
    QJsonObject prefixed = obj.value("cbprefixed").toObject();

    for(u8 i = 0; i < 0xFF; ++i) {
        std::string opcode = Utils::uint8_tToHexString(i);


        QJsonObject unprefOpcodeJsonObj = 
            unprefixed.value(QString::fromStdString(opcode)).toObject();

        QJsonObject prefOpcodeJsonObj = 
            prefixed.value(QString::fromStdString(opcode)).toObject();


        unprefInstrMap_.emplace(
            std::make_pair(i, InstrInfo(unprefOpcodeJsonObj))
        );

        prefInstrMap_.emplace(
            std::make_pair(i, InstrInfo(prefOpcodeJsonObj))
        );
    }
}

u8 CPU::InstrInfo::getBytesFromJsonObject(const QJsonObject& obj) {
    return obj.value("bytes").toInt();
}

std::pair<u8, u8> CPU::InstrInfo::getCyclesFromJson(const QJsonObject& obj) {
    QJsonArray cyclesJson = obj.value("cycles").toArray();

    return std::pair<u8, u8> {
        cyclesJson[0].toInt(),
        cyclesJson[1].toInt()
    };
}

std::array<Utils::Flag, 4> CPU::InstrInfo::getFlagsFromJsonObject(const QJsonObject& obj) {
    QJsonArray flagsJson = obj.value("flags").toArray();

    return std::array<Utils::Flag, 4> {
        Utils::Flag(flagsJson.at(0).toString().toStdString()[0]),
        Utils::Flag(flagsJson.at(1).toString().toStdString()[0]),
        Utils::Flag(flagsJson.at(2).toString().toStdString()[0]),
        Utils::Flag(flagsJson.at(3).toString().toStdString()[0]),
    };
}

void CPU::step() {
    u8 opcode = memory_.read(pc_);

    ccf();
    bool conditionSatisfied = executeOpcode(opcode);

    handleFlags();

    /*cycles_ += conditionSatisfied ? bytes.first : bytes.second;*/
}


