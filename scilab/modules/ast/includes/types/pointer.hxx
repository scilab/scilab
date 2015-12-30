/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __POINTER_HXX__
#define __POINTER_HXX__

#include "user.hxx"

namespace types
{
class Pointer : public UserType
{
public :
    Pointer()
    {
        m_pvData = NULL;
    }

    Pointer(const void* _pvData)
    {
        m_pvData = _pvData;
    }

    Pointer(int _rows, int _cols, const void* _pvData, bool _complex = false)
    {
        m_iRows  = _rows;
        m_iCols  = _cols;
        m_pvData = _pvData;
        m_cplx   = _complex;
    }

    virtual ~Pointer() {}

    bool hasToString()
    {
        return false;
    }

    std::string getTypeStr()
    {
        return "pointer";
    }

    std::string getShortTypeStr()
    {
        return "ptr";
    }

    bool isPointer(void)
    {
        return true;
    }

    bool isComplex(void)
    {
        return m_cplx;
    }

    bool toString(std::ostringstream& ostr) override
    {
        return true;
    }

    Pointer* clone()
    {
        return new Pointer(m_iRows, m_iCols, m_pvData, m_cplx);
    }

    const void* get()
    {
        return m_pvData;
    }

    void set(const void* _pvData)
    {
        m_pvData = _pvData;
    }

    virtual bool isAssignable(void)
    {
        return true;
    }

protected :

    const void* m_pvData;
    bool m_cplx;
};
}

#endif /* !__USER_HXX__ */
