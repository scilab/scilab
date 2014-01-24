/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


#ifndef __INTERNAL_HXX__
#define __INTERNAL_HXX__

#include <vector>
#include <map>
#include <iostream>
#include <string.h>
//#include "anytype.hxx"
#include "dynlib_types.h"

#define bsiz 4096

namespace types
{
class TYPES_IMPEXP InternalType
{
public :
    enum RealType
    {
        /* Internal Type */
        RealInternal,
        /* Generic Types */
        RealGeneric,
        RealInt8,
        RealUInt8,
        RealInt16,
        RealUInt16,
        RealInt32,
        RealUInt32,
        RealInt64,
        RealUInt64,
        RealString,
        RealDouble,
        RealBool,
        RealFloat,
        RealPoly,
        RealSinglePoly,
        /* Callable */
        RealFunction,
        RealMacro,
        RealMacroFile,
        /* Implicit List */
        RealImplicitList,
        /* Container */
        RealContainer,
        RealList,
        RealTList,
        RealMList,
        RealSingleStruct,
        RealStruct,
        RealCell,
        /* User */
        RealUserType,
        /*For list operation*/
        RealListOperation, //parent type
        RealListInsertOperation,
        RealListDeleteOperation,
        RealListUndefinedOperation,
        RealFile,
        RealColon,
        RealDollar,
        RealThreadId,
        RealSparse,
        RealSparseBool,
        RealSingleHandle,
        RealHandle,
        RealVoid
    };

protected :
    InternalType() : m_iRef(0), m_bAllowDelete(true), m_bPrintFromStart(true), m_iSavePrintState(0), m_iRows1PrintState(0), m_iRows2PrintState(0), m_iCols1PrintState(0), m_iCols2PrintState(0) {}

public :
    virtual                         ~InternalType() {};
    virtual void                    whoAmI(void)
    {
        std::cout << "types::Inernal";
    }

    virtual bool                    isAssignable(void)
    {
        return false;
    }
    virtual RealType                getType(void) = 0 ; //{ return RealInternal; }

    virtual bool                    hasToString()
    {
        return true;
    }
    virtual bool                    toString(std::wostringstream& ostr) = 0;

    virtual std::wstring            toStringInLine()
    {
        return getTypeStr();
    }
    virtual InternalType*           clone(void) = 0;


    virtual void IncreaseRef()
    {
        m_iRef++;
    }

    void DecreaseRef()
    {
        if (m_iRef > 0)
        {
            m_iRef--;
        }
    }

    bool	                        isDeletable()
    {
        return m_iRef == 0;
    }
    bool	                        isRef(int _iRef = 0)
    {
        return m_iRef > _iRef;
    }
    int                             getRef()
    {
        return m_iRef;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring            getTypeStr() = 0;
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring            getShortTypeStr() = 0;

    virtual bool                    operator==(const InternalType& it)
    {
        return (getType() == (const_cast<InternalType *>(&it))->getType());
    }
    virtual bool                    operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    /**
    ** GenericType
    ** \{
    */

    template <class T>
    T*                              getAs(void)
    {
        return static_cast<T*>(this);
    }

    virtual bool                    isGenericType(void);
    virtual bool                    isArrayOf(void)
    {
        return false;
    }
    virtual bool                    isString(void)
    {
        return false;
    }
    virtual bool                    isDouble(void)
    {
        return false;
    }
    virtual bool                    isSparse(void)
    {
        return false;
    }
    virtual bool                    isSparseBool(void)
    {
        return false;
    }
    virtual bool                    isFloat(void)
    {
        return false;
    }
    virtual bool                    isInt(void)
    {
        return false;
    }
    virtual bool                    isInt8(void)
    {
        return false;
    }
    virtual bool                    isUInt8(void)
    {
        return false;
    }
    virtual bool                    isInt16(void)
    {
        return false;
    }
    virtual bool                    isUInt16(void)
    {
        return false;
    }
    virtual bool                    isInt32(void)
    {
        return false;
    }
    virtual bool                    isUInt32(void)
    {
        return false;
    }
    virtual bool                    isInt64(void)
    {
        return false;
    }
    virtual bool                    isUInt64(void)
    {
        return false;
    }
    virtual bool                    isBool(void)
    {
        return false;
    }
    virtual bool                    isPoly(void)
    {
        return false;
    }
    virtual bool                    isSinglePoly(void)
    {
        return false;
    }
    virtual bool                    isCallable(void)
    {
        return false;
    }
    virtual bool                    isFunction(void)
    {
        return false;
    }
    virtual bool                    isMacro(void)
    {
        return false;
    }
    virtual bool                    isMacroFile(void)
    {
        return false;
    }
    virtual bool                    isContainer(void)
    {
        return false;
    }
    virtual bool                    isList(void)
    {
        return false;
    }
    virtual bool                    isStruct(void)
    {
        return false;
    }
    virtual bool                    isCell(void)
    {
        return false;
    }
    virtual bool                    isTList(void)
    {
        return false;
    }
    virtual bool                    isMList(void)
    {
        return false;
    }
    virtual bool                    isImplicitList(void)
    {
        return false;
    }
    virtual bool                    isColon(void)
    {
        return false;
    }
    virtual bool                    isDollar(void)
    {
        return false;
    }
    virtual bool                    isFile(void)
    {
        return false;
    }
    virtual bool                    isHandle(void)
    {
        return false;
    }
    virtual bool                    isSingleHandle(void)
    {
        return false;
    }
    virtual bool                    isThreadId(void)
    {
        return false;
    }
    virtual bool                    isListOperation(void)
    {
        return false;
    }
    virtual bool                    isListDelete(void)
    {
        return false;
    }
    virtual bool                    isListInsert(void)
    {
        return false;
    }
    virtual bool                    isListUndefined(void)
    {
        return false;
    }
    virtual bool                    isPointer(void)
    {
        return false;
    }

    void clearPrintState()
    {
        m_bPrintFromStart = true;
        m_iSavePrintState = 0;
        m_iRows1PrintState = 0;
        m_iCols1PrintState = 0;
        m_iRows2PrintState = 0;
        m_iCols2PrintState = 0;
    }


protected :
    int                             m_iRef;
    //use to know if we can delete this variables or if it's link to a scilab variable.
    bool                            m_bAllowDelete;

    /*variables to manage print taking care of lines*/
    bool                    m_bPrintFromStart;
    int                     m_iSavePrintState;
    int                     m_iRows1PrintState;
    int                     m_iCols1PrintState;
    int                     m_iRows2PrintState;
    int                     m_iCols2PrintState;

};

/*
** List of types
*/
typedef std::vector<InternalType *> typed_list;
typedef std::vector<std::pair<std::wstring, InternalType *> > optional_list;

}

#endif /* !__INTERNAL_HXX__ */
