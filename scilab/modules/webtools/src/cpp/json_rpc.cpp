/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <string>
#include <sstream>
#include <regex>

// data string format: "Content-Length: xxx\r\n\r\nJSON"
size_t json_rpc_decode(const std::string& data, std::string& json_string)
{
    // Look for Content-Length part
    std::smatch content_length_match;
    const std::regex content_length_reg("Content-Length: [0-9]+");
    if(! std::regex_search(data, content_length_match, content_length_reg))
    {
        // Data not fully sent, wait for the next onMessage
        return 0;
    }

    // Get the Content-Length value
    std::smatch size_match;
    const std::regex size_reg("\\d+");
    const std::string content_length(content_length_match[0]);
    std::regex_search(content_length, size_match, size_reg);
    size_t JSON_size = (size_t)atoi(size_match[0].str().c_str());

    // Look for next 2 empty lines
    std::smatch empty_lines_match;
    const std::regex empty_lines_reg("(\r\n|\n){2}");
    if(! std::regex_search(data, empty_lines_match, empty_lines_reg))
    {
        // Data not fully sent, wait for the next onMessage
        return 0;
    }

    // Go to the start of the JSON string data
    json_string = empty_lines_match.suffix().str().substr(0, JSON_size);
    if(json_string.length() < JSON_size)
    {
        // Data not fully sent, wait for the next onMessage
        return 0;
    }

    return empty_lines_match.prefix().length() + empty_lines_match[0].length() + JSON_size;
}

size_t json_rpc_encode(const std::string& json_string, std::string& data)
{
    std::stringstream ss;
    ss << "Content-Length: " << json_string.length() << "\r\n\r\n" << json_string;
    data = ss.str();
    return data.length();
}
