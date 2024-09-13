#include "Disassembler.hpp"

#include <cstdint>
#include <iomanip>
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

/*std::vector<Assembly> Disassembler::disassemble() {*/
/**/
/*}*/

std::string uint8_tToHexString(const uint8_t& byte) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    return ss.str();
}

QJsonObject getJsonObjectFromFile(std::string& filePath) {
    QString jsonFilePath = QString::fromStdString(filePath);

    QFile file(jsonFilePath);
    if(!file.open(QIODevice::ReadOnly))
        error("Could not open file: " + jsonFilePath.toStdString());

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);
    if(jsonError.error)
        error("Json parsing error: " + jsonError.errorString().toStdString());

    if(!document.isObject())
        error("Document does not have any object!");

    return document.object();
}

Assembly Disassembler::disassembleOneOpcode(uint8_t* byte) {

    std::string jsonFilePath = "/home/desktop/GameBoyWeb/res/opcodes.json";

    QJsonObject obj = getJsonObjectFromFile(jsonFilePath);

    Assembly result;
    if(*byte == 0xCB) {
        prefix_ = true;
        return disassembleOneOpcode(byte + 1);
    }

    const QString opcode = QString::fromStdString(uint8_tToHexString(0));

    qDebug() << opcode;

    qDebug() << obj.value("unprefixed").toObject().value(opcode).toObject().value("mnemonic").toString();

    return result;
}

