/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#ifndef MODULES_SCICOS_SRC_CPP_BASE64_HXX_
#define MODULES_SCICOS_SRC_CPP_BASE64_HXX_

#include <array>
#include <vector>
#include <sstream>
#include <string>
#include <limits>
#include <cmath> // for std::trunc

namespace org_scilab_modules_scicos
{

/*
 * Encode a string to a base64 encoded string.
 *
 * This implement the RFC 2045 Base64 variant. See https://tools.ietf.org/html/rfc2045
 */
struct base64
{
    template<typename T> static std::string encode(const T& v);
    template<typename T> static T decode(const std::string& content);
};
template<> inline std::string base64::encode(const std::string& v);
template<> inline std::string base64::decode(const std::string& content);


template<> std::string base64::encode<std::string>(const std::string& strValue)
{
    const std::string Base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char Base64Pad = '=';

    // tips given by :
    //  * http://www.adp-gmbh.ch/cpp/common/base64.html
    //  * http://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
    std::string content;

    int val = 0;
    int val_byte = -6;
    for (unsigned char c : strValue)
    {
        val = (val << 8) + c;
        val_byte += 8;

        while (val_byte >= 0)
        {
            content.push_back(Base64Alphabet[(val >> val_byte) & 0x3F]);
            val_byte -= 6;
        }
    }
    // the trailing part is left in val_byte
    if (val_byte > -6)
    {
        content.push_back(Base64Alphabet[((val << 8) >> (val_byte + 8)) & 0x3F]);
    }
    // add padding if needed
    while (content.size() % 4)
    {
        content.push_back(Base64Pad);
    }

    return content;
}

template<> std::string base64::decode<std::string>(const std::string& content)
{
    const std::string Base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // inverse table (might be const or constexpr)
    std::array<char, 255> T;
    T.fill(255);
    for (size_t i = 0; i < Base64Alphabet.length(); i++)
    {
        T[Base64Alphabet[i]] = i;
    }

    // decoding loop
    std::string strValue;

    int val = 0;
    int val_byte = -8;
    for (char c : content)
    {
        if (T[c] == std::numeric_limits<unsigned char>::max()) // not in the table, stop decoding
        {
            break;
        }

        val = (val << 6) + T[c];
        val_byte += 6;
        if (val_byte >= 0)
        {
            strValue.push_back((val >> val_byte) & 0xFF);
            val_byte -= 8;
        }
    }

    return strValue;
}


template<typename T>
std::string base64::encode(const T& v)
{
    // convert to string components and compress using int conversion
    std::string strValue;
    for (auto it = v.begin(); it != v.end(); it++)
    {
        typename T::value_type d = *it;

        if (std::trunc(d) == d)
        {
            strValue.append(std::to_string((int) d));
        }
        else
        {
            strValue.append(std::to_string(d));
        }

        if (it + 1 != v.end())
        {
            strValue.push_back(' ');
        }
    }

    return encode<std::string>(strValue);
}

template<typename T>
T base64::decode(const std::string& content)
{
    std::string strValue = decode<std::string>(content);

    T vector;
    std::istringstream ss(strValue);
    while (ss)
    {
        typename T::value_type v;
        ss >> v;
        vector.push_back(v);
    }

    return vector;
}

} /* namespace org_scilab_modules_xcos */

#endif /* MODULES_SCICOS_SRC_CPP_BASE64_HXX_ */
