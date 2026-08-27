/*
*  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#ifndef MODULES_SCICOS_INCLUDES_BASE64_HXX_
#define MODULES_SCICOS_INCLUDES_BASE64_HXX_

#include <array>
#include <charconv>
#include <cmath> // for trunc
#include <cstdio> // for sprintf
#include <limits>
#include <system_error> // for std::errc
#include <string>
#include <string_view>
#include <vector>
#if defined(__APPLE__) || defined(__FreeBSD__)
#include "fast_float/fast_float.h"
#endif
#ifdef max
#undef max
#endif // max

namespace org_scilab_modules_scicos
{

/*
 * Encode a string to a base64 encoded string.
 *
 * This implement the RFC 2045 Base64 variant. See https://tools.ietf.org/html/rfc2045
 */
namespace base64
{

/*
 * to_string() convert a value to a representative string, used to before encoding to base64.
 */
template<typename T> inline
std::string to_string(T v)
{
    // default implementation is provided by std::to_string
    return std::to_string(v);
}

template<> inline
std::string to_string(std::string v)
{
    return v;
}

template<> inline
std::string to_string(bool v)
{
    if (v)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

template<> inline
std::string to_string(double v)
{
    if (((int)v) == v)
    {
        return to_string((int)v);
    }

    // hexadecimal for a double interpreted as raw bytes 
    constexpr size_t needed_chars = sizeof(double) / sizeof(char);
    auto rawValue = (char(*)[needed_chars]) &v;
    
    std::string str = "0x";
    for (size_t i = 0; i < needed_chars; ++i)
    {
        str += "0123456789abcdef"[((*rawValue)[i] >> 4) & 0x0F];
        str += "0123456789abcdef"[(*rawValue)[i] & 0x0F];
    }

    return str;
}

template<typename U> inline
std::string to_string(const std::vector<U>& v)
{
    std::string str;
    if (v.size() == 0)
    {
        return str;
    }
    str += to_string(v[0]);
    for (size_t i = 1; i < v.size(); ++i)
    {
        if (std::is_arithmetic<U>::value)
        {
            // add a space between two elements
            str += " ";
        }
        else
        {
            // add the unicode character U+2028 (LINE SEPARATOR) between two elements
            str += "\xe2\x80\xa8";
        }
        str += to_string(v[i]);
    }

    return str;
}


/*
 * encode() a value to base64.
 */
template<typename T> inline
void encode(const T& v, std::string& into) {
    encode(to_string(v), into);
}

template<typename T> inline
std::string encode(const T& v) {
    std::string content;
    encode(v, content);
    return content;
}

// explicit instantiation for std::string
template<> inline
void encode(const std::string& strValue, std::string& into)
{
    const std::string Base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char Base64Pad = '=';

    // tips given by :
    //  * http://www.adp-gmbh.ch/cpp/common/base64.html
    //  * http://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
    into.clear();

    int val = 0;
    int val_byte = -6;
    for (unsigned char c : strValue)
    {
        val = (val << 8) + c;
        val_byte += 8;

        while (val_byte >= 0)
        {
            into.push_back(Base64Alphabet[(val >> val_byte) & 0x3F]);
            val_byte -= 6;
        }
    }
    // the trailing part is left in val_byte
    if (val_byte > -6)
    {
        into.push_back(Base64Alphabet[((val << 8) >> (val_byte + 8)) & 0x3F]);
    }
    // add padding if needed
    while (into.size() % 4)
    {
        into.push_back(Base64Pad);
    }
}


/*
 * from_string() convert a representative string to a value, used after decoding from base64.
 */
template<typename T> inline
std::errc from_string(const std::string_view& str, T& result)
{
    if (str.size() > 2 && str[0] == '0' && str[1] == 'x')
    {
        // hexadecimal numbers will be parsed as a raw bytes
        // tips given by :
        //  * https://github.com/stedonet/chex
        //  * https://lemire.me/blog/2019/04/17/parsing-short-hexadecimal-strings-efficiently/
        constexpr size_t needed_chars = sizeof(T) / sizeof(char);
        if (str.size() != needed_chars * 2 + 2)
        {
            return std::errc::invalid_argument; // error
        }
        auto rawValue = (char(*)[needed_chars]) &result;

        result = {};
        for (size_t i = 0, j = 2; i < needed_chars; ++i, j+= 2)
        {
            char hi = ((str[j] & 0xf) + (str[j] >> 6) * 9);
            char lo = ((str[j+1] & 0xf) + (str[j+1] >> 6) * 9);
            ((char*) rawValue)[i] = (hi << 4) | lo;
        }
        return {};
    }
    // implementation may be provided by https://github.com/fastfloat/fast_float
#ifdef FASTFLOAT_FLOAT_COMMON_H 
    auto [ptr, ec] = fast_float::from_chars(str.data(), str.data() + str.size(), result);
#else
    // use std::from_chars otherwise (eg. C++17)
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
#endif
    return ec;
}

template<> inline
std::errc from_string(const std::string_view& str, bool& result)
{
    if (str == "true")
    {
        result = true;
        return {};
    }
    else if (str == "false")
    {
        result = false;
        return {};
    }
    else
    {
        return std::errc::invalid_argument; // error
    }
}

template<> inline
std::errc from_string(const std::string_view& str, std::string& result)
{
    result = str;
    return {};
}


// separator is " " for numbers or the unicode character U+2028 (LINE SEPARATOR)
template<typename U> constexpr
std::string_view separator()
{
    using namespace std::literals;

    if (std::is_arithmetic<U>::value)
    {
        return " "sv;
    }
    else
    {
        return "\xe2\x80\xa8"sv;
    }
}

template<typename U> inline
std::errc from_string(const std::string_view& str, std::vector<U>& result)
{
    result.clear();
    size_t pos = 0;
    
    if (str.length() == 0)
    {
        return {};
    }

    while (pos <= str.length())
    {
        size_t next = str.find(separator<U>(), pos);

        if (next == std::string::npos)
        {
            next = str.length();
        }
        std::string_view token = str.substr(pos, next - pos);
        U value{};
        std::errc err = from_string(token, value);
        if (err != std::errc{})
        {
            return err; // error
        }
        result.push_back(value);
        pos = next + separator<U>().size();
    }
    return {};
}

template<typename T> inline
void decode(const std::string_view& content, T& into)
{
    std::string strValue;
    decode(content, strValue);
    from_string(strValue, into);
}

template<typename T> inline
T decode(const std::string_view& content)
{
    T v{};
    decode(content, v);
    return v;
}

// explicit instantiation for std::string
template<> inline
void decode(const std::string_view& content, std::string& str)
{
    // implementation from https://stackoverflow.com/a/37109258

    // inverse table
    static const int B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
        7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
        0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

    unsigned char* p = (unsigned char*) content.data();
    size_t len = content.size();

    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    str = std::string(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad)
    {
        int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
        str[str.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= B64index[p[L + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }
}

} /* namespace base64 */

} /* namespace org_scilab_modules_xcos */

#endif /* MODULES_SCICOS_INCLUDES_BASE64_HXX_ */
