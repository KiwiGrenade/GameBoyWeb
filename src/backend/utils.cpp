#include "utils.hpp"
#include <algorithm>
#include <cctype>

namespace Utils {

std::string uint8_tToHexString(const uint8_t& byte) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
    std::string res = ss.str();
    // capitalise last 2 letters
    std::transform(res.end() - 2, res.end(), res.end() - 2, ::toupper);
    return res;
}

QJsonObject getJsonObjectFromFile(std::string& filePath) {
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

u8 getBytesFromJsonObject(const QJsonObject& obj) {
    return obj.value("bytes").toInt();
}

std::pair<u8, u8> getCyclesFromJsonObject(const QJsonObject& obj) {
    QJsonArray cyclesJson = obj.value("cycles").toArray();

    return std::pair<u8, u8> {
        cyclesJson[0].toInt(),
        cyclesJson[1].toInt()
    };
}

flagArray getFlagsFromJsonObject(const QJsonObject& obj) {
    QJsonArray flagsJson = obj.value("flags").toArray();
    flagArray flags;

    for (u8 i = 0; i < flags.size(); ++i) {
        char c = flagsJson.at(i).toString().toStdString()[0];
        if(c == '0')
            flags[i] = Utils::Flag::reset;
        else if(c == '1')
            flags[i] = Utils::Flag::set;
        else if(c == '-')
            flags[i] = Utils::Flag::nothing;
        else
            flags[i] = Utils::Flag::setOrReset;
    }

    return flags; 
}

} // Utils
