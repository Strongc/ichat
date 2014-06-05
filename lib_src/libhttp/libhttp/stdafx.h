// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

#include <iostream>
#include <string>

#include <boost/filesystem/path.hpp>  
#include <boost/filesystem/operations.hpp> 
#include <boost/format.hpp> 
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
// TODO: 在此处引用程序需要的其他头文件
