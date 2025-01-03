/** @file nim_string_util.cpp
 * @brief string操作辅助方法
 * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
 * @date 2016/09/08
 */

#include "nim_wrapper_util/nim_string_util.h"
#include "nim_wrapper_util/convert_utf.h"

std::string PCharToString(const char* str) {
    if (str) {
        return str;
    } else {
        return "";
    }
}
