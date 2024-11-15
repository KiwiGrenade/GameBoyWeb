#include "utils.hpp"
#include <algorithm>
#include <cctype>

namespace Utils {

// std::string uint8_tToHexString(const uint8_t& byte) {
//     std::stringstream ss;
//     ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
//     std::string res = ss.str();
//     // capitalise last 2 letters
//     std::transform(res.end() - 2, res.end(), res.end() - 2, ::toupper);
//     return res;
// }


QJsonObject getInstrMapJsonObjectFromFile(const bool prefixed) {
    QString jsonFilePathQStr = QString::fromStdString(jsonFilePath);

    QFile file(jsonFilePathQStr);
    if(!file.open(QIODevice::ReadOnly))
        Utils::error("Could not open file: " + jsonFilePathQStr.toStdString());

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);
    if(jsonError.error)
        Utils::error("Json parsing error: " + jsonError.errorString().toStdString());

    if(!document.isObject())
        Utils::error("Document does not have any object!");

    QString str = prefixed ? "cbprefixed" : "unprefixed";

    // document -> document Json Object -> opcode map Json Object
    return document.object().value(str).toObject();
}

u8 getBytesFromInstrJsonObject(const QJsonObject& obj) {
    return obj.value("bytes").toInt();
}

std::pair<u8, u8> getCyclesFromInstrJsonObject(const QJsonObject& obj) {
    QJsonArray cyclesJson = obj.value("cycles").toArray();

    return std::pair<u8, u8> {
        cyclesJson[0].toInt(),
        cyclesJson[1].toInt()
    };
}

flagArray getFlagsFromInstrJsonObject(const QJsonObject& obj) {
    QJsonObject flagsJson = obj.value("flags").toObject();
    flagArray flags;

    for (u8 i = 0; i < flags.size(); ++i) {
        // acces array in reverse order (apparently values are stored in reverse)
        char c = (flagsJson.end()-i-1)->toString().toStdString()[0];
        switch (c) {
            case '0':
                flags[i] = Utils::Flag::reset; break;
            case '1':
                flags[i] = Utils::Flag::set; break;
            case '-':
                flags[i] = Utils::Flag::nothing; break;
            default:
                flags[i] = Utils::Flag::setOrReset; break;
        }
    }

    return flags; 
}

} // Utils
