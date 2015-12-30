/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    std::shared_ptr<std::ostream> out;

public:

    //Logger(std::wostream & _out) : out(new std::wostream(&_out)) { }
    Logger(const std::string & file) : out(new std::ofstream(file, std::ofstream::out | std::ofstream::trunc)) { }

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
