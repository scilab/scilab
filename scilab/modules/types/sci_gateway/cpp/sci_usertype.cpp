/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types.hxx"
#include "funcmanager.hxx"
#include "user.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C" {
    #include "Scierror.h"
    #include "localization.h"
}

using namespace types;

class MyDataType : public User<MyDataType>
{
public:
    MyDataType(std::string _shortName, std::string _longName) :
        m_shortName(_shortName),
        m_longName(_longName)
    {
    }

public :
    MyDataType * clone()
    {
        return new MyDataType(m_shortName, m_longName);
    }

    /* This will be used when calling typeof */
    std::string getTypeStr()            { return m_longName; }

    /* This will be used to generate Overloading functions name. */
    std::string getShortTypeStr()       { return m_shortName; }

    /* This is Scilab standard display or in disp. */
    std::string toString(int _iPrecision, int _iLineLen)
    {
        return "I'm a user dataType: "+m_longName+" ("+m_shortName+").";
    }

private :
    std::string m_shortName;
    std::string m_longName;
};

Function::ReturnValue sci_usertype(typed_list &in, int _piRetCount, typed_list &out)
{
    /* First check if all fields are Strings */
    typed_list::iterator itInput;
    if(in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "usertype", 2);
        return Function::Error;
    }

    int i = 1;
    for (itInput = in.begin() ; itInput != in.end() ; itInput++, i++)
    {
        if (!(*itInput)->isString() || (*itInput)->getAsString()->size_get() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "usertype", i);
            return Function::Error;
        }
    }

    MyDataType *pRetVal = new MyDataType(in[0]->getAsString()->string_get(0), in[1]->getAsString()->string_get(0));

    out.push_back(pRetVal);

    return Function::OK;
}
