﻿#pragma once
#include "resource.h"

//c++ header
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>
#include <memory>

//base header
#include "base/callback/callback.h"
#include "base/util/string_util.h"
#include "base/util/unicode.h"
#include "base/util/string_number_conversions.h"
#include "base/memory/deleter.h"
#include "base/memory/singleton.h"
#include "base/win32/platform_string_util.h"
#include "base/win32/path_util.h"
#include "base/win32/win_util.h"
#include "base/thread/thread_manager.h"
#include "base/macros.h"
#include "base/base_types.h"
#include "base/file/file_util.h"
#include "base/time/time.h"
#include "base/framework/task.h"

//shared project
#include "shared/utf8_file_util.h"
#include "shared/tool.h"

//third_party
#include "duilib/UIlib.h"

//json
#include "jsoncpp/include/json/json.h"

//xml
#include "tinyxml/tinyxml.h"

//window header
#include <atlbase.h>
#include <WTypes.h>
#include <shellapi.h>

//log
#include "shared/log.h"

//util
#include "shared/util.h"

//sdk
#include "nim_all_cpp_wrapper.h"

//msg
#include "module/msglog/msglog.h"

//notify
#include "module/service/notify_center.h"
#include "module/service/user_service.h"
#include "module/service/team_service.h"

//helper
#include "shared/closure.h"

//没有分离出来的部分
//demo依赖window_ex类
#include "util/window_ex.h"
#include "util/windows_manager.h"