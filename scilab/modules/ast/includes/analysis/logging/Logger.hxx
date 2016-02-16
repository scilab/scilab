/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __LOGGER_HXX__
#define __LOGGER_HXX__

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

namespace analysis
{
namespace logging
{

class Logger
{
    std::shared_ptr<std::wostream> out;

public:

    //Logger(std::wostream & _out) : out(new std::wostream(&_out)) { }
    Logger(const std::string & file) : out(new std::wofstream(file, std::ofstream::out | std::ofstream::trunc)) { }

    template<typename T>
    inline void log(const T & x)
    {
        //*out << x << std::endl;
    }

    template<typename T>
    inline void log(const unsigned int line, const char * file, const T & x)
    {
        //*out << file << L"@" << line << L": " << x << std::endl;
    }

    template<typename T, typename... Args>
    inline void log(const T & x, Args... args)
    {
        //*out << x << L" ";
        log(args...);
    }
};

} // namespace logging

} // namespace analysis


#endif // __LOGGER_HXX__
