#include "Disassembler.hpp"
#include "utils.hpp"

#include <cstdint>
#include <iomanip>
#include <memory>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qlogging.h>
#include <sstream>

#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>

Disassembler::Disassembler()
    : prefix_{false}{

}

void Disassembler::disassemble(const Memory& memory, QPlainTextEdit& plainTextEdit) {
    for(uint32_t i = 0; i < 300;) {
        Utils::Assembly ass = disassembleOneOpcode(memory.read(i));
        i += ass.bytes_;
        plainTextEdit.appendPlainText(ass.toString());
    }
}

std::string uint8_tToHexString(const uint8_t& byte) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    return ss.str();
}

QJsonObject Disassembler::getJsonObjectFromFile(std::string& filePath) {
    QString jsonFilePath = QString::fromStdString(filePath);

    QFile file(jsonFilePath);
    if(!file.open(QIODevice::ReadOnly))
        Utils::error("Could not open file: " + jsonFilePath.toStdString());

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);
    if(jsonError.error)
        Utils::error("Json parsing error: " + jsonError.errorString().toStdString());

    if(!document.isObject())
        Utils::error("Document does not have any object!");

    return document.object();
}

Utils::Assembly Disassembler::disassembleOneOpcode(const uint8_t& byte) {
    std::string jsonFilePath = "/home/desktop/GameBoyWeb/res/opcodes.json";

    QJsonObject obj = getJsonObjectFromFile(jsonFilePath);

    const QString opcodeType = prefix_ ? "prefixed" : "unprefixed";
    const QString opcode = QString::fromStdString(uint8_tToHexString(byte));
    QJsonObject jsonOpcode = obj.value(opcodeType).toObject().constFind(opcode)->toObject();

    return Utils::Assembly(jsonOpcode, prefix_);
}

