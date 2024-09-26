/*#include "Disassembler.hpp"*/
/*#include "utils.hpp"*/
/**/
/*#include <cstdint>*/
/*#include <iomanip>*/
/*#include <qjsonobject.h>*/
/*#include <qjsonvalue.h>*/
/*#include <qlogging.h>*/
/*#include <sstream>*/
/**/
/*#include <QObject>*/
/*#include <QString>*/
/*#include <QFile>*/
/*#include <QJsonParseError>*/
/*#include <QJsonObject>*/
/**/
/*Disassembler::Disassembler()*/
/*    : prefix_{false}{*/
/**/
/*}*/
/**/
/*void Disassembler::disassemble(const Memory& memory, QPlainTextEdit& plainTextEdit) {*/
/*    for(uint32_t i = 0; i < 300;) {*/
/*        Assembly ass = disassembleOneOpcode(memory.read(i));*/
/*        i += ass.bytes_;*/
/*        plainTextEdit.appendPlainText(ass.toString());*/
/*    }*/
/*}*/
/**/
/**/
/*Assembly Disassembler::disassembleOneOpcode(const uint8_t& byte) {*/
/*    std::string jsonFilePath = "/home/desktop/GameBoyWeb/res/opcodes.json";*/
/**/
/*    QJsonObject obj = Utils::getJsonObjectFromFile(jsonFilePath);*/
/**/
/*    const QString opcodeType = prefix_ ? "prefixed" : "unprefixed";*/
/*    const QString opcode = QString::fromStdString(Utils::uint8_tToHexString(byte));*/
/*    QJsonObject jsonOpcode = obj.value(opcodeType).toObject().constFind(opcode)->toObject();*/
/**/
/*    return Assembly(jsonOpcode, prefix_);*/
/*}*/

