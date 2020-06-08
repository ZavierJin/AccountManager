/*  Address.h
**  Brief introduction: definition of multithreaded Computer
**  Create by Zhang Zhecheng 2020/5/18
**  Modify:
**      2020/5/19 (by Zhang Zhecheng)
**          1. Move address class to namespace network
**          2. Add two new functions: 
**              Address getRequestAddress(const Address &)
**              Address getAnswerAddress(const Address &)
**      2020/5/24 (by Zhang Zhecheng)
**          1. Add getServerAddress(int)
**      2020/5/25
**          1. Include boost/filesystem.hpp
**      2020/5/27 (by Zhang Zhecheng)
**          1. Move function to NetworkImpl.h
*/

#pragma once
#include <string>
#include <boost/filesystem.hpp>

namespace network
{
    namespace fs = boost::filesystem;
    using Address = fs::path;
}