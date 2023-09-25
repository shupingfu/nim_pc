/** @file nim_json_util.cpp
 * @brief JSON辅助方法
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @date 2015/09/08
 */
#include "nim_wrapper_util/nim_json_util.h"
#include <iostream>
#include "nim_wrapper_util/convert_utf.h"

namespace nim {

bool StrListToJsonString(const std::list<std::string>& list, std::string& out) {
    nim_cpp_wrapper_util::Json::Value json_ids;
    for (auto it = list.begin(); it != list.end(); it++) {
        json_ids.append(*it);
    }
    out = GetJsonStringWithNoStyled(json_ids);
    return true;
}

bool StrListToJsonString(const std::vector<std::string>& list, std::string& out) {
    nim_cpp_wrapper_util::Json::Value json_ids;
    for (auto it = list.begin(); it != list.end(); it++) {
        json_ids.append(*it);
    }
    out = GetJsonStringWithNoStyled(json_ids);
    return true;
}

bool JsonStrArrayToList(const nim_cpp_wrapper_util::Json::Value& array_str, std::list<std::string>& out) {
    if (array_str.isArray()) {
        out.clear();
        int size = array_str.size();
        for (int index = 0; index < size; index++) {
            out.push_back(array_str[index].asString());
        }
        return true;
    }
    return false;
}

bool JsonStrArrayToList(const nim_cpp_wrapper_util::Json::Value& array_str, std::vector<std::string>& out) {
    if (array_str.isArray()) {
        out.clear();
        int size = array_str.size();
        for (int index = 0; index < size; index++) {
            out.push_back(array_str[index].asString());
        }
        return true;
    }
    return false;
}

bool JsonArrayStringToList(const std::string& array_str, std::list<std::string>& out) {
    nim_cpp_wrapper_util::Json::Value value;
    if (ParseJsonValue(array_str, value) && value.isArray()) {
        return JsonStrArrayToList(value, out);
    }
    return false;
}

bool JsonArrayStringToList(const std::string& array_str, std::vector<std::string>& out) {
    nim_cpp_wrapper_util::Json::Value value;
    if (ParseJsonValue(array_str, value) && value.isArray()) {
        return JsonStrArrayToList(value, out);
    }
    return false;
}

bool ParseJsonValue(const std::string& content, nim_cpp_wrapper_util::Json::Value& values) {
    nim_cpp_wrapper_util::Json::Reader reader;
    try {
        if (!reader.parse(content, values)) {
            return false;
        }
    } catch (...) {
        std::cerr << "ParseJsonValue error: " << content << std::endl;
        return false;
    }
    return true;
}

bool ParseJsonValue(const char* content, nim_cpp_wrapper_util::Json::Value& values) {
    if (content == nullptr)
        return false;
    return ParseJsonValue(std::string(content), values);
}

std::string GetJsonStringWithNoStyled(const nim_cpp_wrapper_util::Json::Value& values) {
    if (values.empty())
        return "";
    nim_cpp_wrapper_util::Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    builder["emitUTF8"] = true;
    return nim_cpp_wrapper_util::Json::writeString(builder, values);
}

nim_cpp_wrapper_util::Json::Value GetJsonValueFromJsonString(const std::string& json_string) {
    nim_cpp_wrapper_util::Json::Value value;
    nim_cpp_wrapper_util::Json::CharReaderBuilder builder;
    std::unique_ptr<nim_cpp_wrapper_util::Json::CharReader> const reader(builder.newCharReader());
    if (!reader->parse(json_string.c_str(), json_string.c_str() + json_string.length(), &value, nullptr)) {
        assert(false);
    }
    return value;
}
}  // namespace nim
