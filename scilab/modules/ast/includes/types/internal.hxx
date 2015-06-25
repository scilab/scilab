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

extern "C"
{
#include "dynlib_ast.h"
#include "configvariable_interface.h"
}

#include "exp.hxx" // for invoke
#include "visitor.hxx" // for invoke
#include "callexp.hxx"
#include "localization.hxx"
#include "scilabexception.hxx"


#define bsiz 4096

//#define _SCILAB_DEBUGREF_
#ifdef _SCILAB_DEBUGREF_
//#define _SCILAB_DEBUGREF_WITHOUT_START_END
#define DecreaseRef() _decreaseref(__FILE__, __LINE__)
#define IncreaseRef() _increaseref(__FILE__, __LINE__)
#define killMe() _killme(__FILE__, __LINE__)
#endif

namespace types
{
/*
** List of types
*/
class InternalType;
typedef std::vector<InternalType *> typed_list;
typedef std::vector<std::pair<std::wstring, InternalType *> > optional_list;

class EXTERN_AST InternalType
{
public :
    enum ScilabType
    {
        ScilabNull, //no type, no data, nothing !
        /* Internal Type */
        ScilabInternal,
        /* Generic Types */
        ScilabGeneric,
        ScilabInt8,
        ScilabUInt8,
        ScilabInt16,
        ScilabUInt16,
        ScilabInt32,
        ScilabUInt32,
        ScilabInt64,
        ScilabUInt64,
        ScilabString,
        ScilabDouble,
        ScilabBool,
        ScilabFloat,
        ScilabPolynom,
        ScilabSinglePolynom,
        /* Callable */
        ScilabFunction,
        ScilabMacro,
        ScilabMacroFile,
        /* Implicit List */
        ScilabImplicitList,
        /* Container */
        ScilabContainer,
        ScilabList,
        ScilabTList,
        ScilabMList,
        ScilabSingleStruct,
        ScilabStruct,
        ScilabCell,
        /* User */
        ScilabUserType,
        /*For list operation*/
        ScilabListOperation, //parent type
        ScilabListInsertOperation,
        ScilabListDeleteOperation,
        ScilabListUndefinedOperation,
        ScilabFile,
        ScilabColon,
        ScilabThreadId,
        ScilabSparse,
        ScilabSparseBool,
        ScilabHandle,
        ScilabVoid,
        ScilabLibrary
    };

    enum ScilabId
    {
        IdNull, //no type, no data, nothing !
        /* Internal Type */
        IdInternal,
        /* Generic Types */
        IdGeneric,
        IdEmpty,
        IdIdentity,
        IdIdentityComplex,
        IdInt8,
        IdScalarInt8,
        IdUInt8,
        IdScalarUInt8,
        IdInt16,
        IdScalarInt16,
        IdUInt16,
        IdScalarUInt16,
        IdInt32,
        IdScalarInt32,
        IdUInt32,
        IdScalarUInt32,
        IdInt64,
        IdScalarInt64,
        IdUInt64,
        IdScalarUInt64,
        IdString,
        IdScalarString,
        IdDouble,
        IdDoubleComplex,
        IdScalarDouble,
        IdScalarDoubleComplex,
        IdBool,
        IdScalarBool,
        IdPolynom,
        IdScalarPolynomComplex,
        IdScalarPolynom,
        IdPolynomComplex,
        IdSinglePolynom,
        /* Callable */
        IdFunction,
        IdMacro,
        IdMacroFile,
        /* Implicit List */
        IdImplicitList,
        /* Container */
        IdContainer,
        IdList,
        IdTList,
        IdMList,
        IdSingleStruct,
        IdStruct,
        IdCell,
        /* User */
        IdUserType,
        /*For list operation*/
        IdListOperation, //parent type
        IdListInsertOperation,
        IdListDeleteOperation,
        IdListUndefinedOperation,
        IdFile,
        IdColon,
        IdThreadId,
        IdSparse,
        IdSparseComplex,
        IdSparseBool,
        IdHandle,
        IdScalarHandle,
        IdVoid,
        IdLibrary,
        IdLast //msut always be the last value
    };

protected :
    InternalType() : m_iRef(0), m_bAllowDelete(true), m_bPrintFromStart(true), m_iSavePrintState(0), m_iRows1PrintState(0), m_iCols1PrintState(0), m_iRows2PrintState(0), m_iCols2PrintState(0), bKillMe(false)
    {
#ifdef _SCILAB_DEBUGREF_
#if defined(_SCILAB_DEBUGREF_WITHOUT_START_END)
        if (getStartProcessing() == 0 && getEndProcessing() == 0)
#endif
        {
            std::cout << "new_IT " << m_iRef << " " << (void*)this << std::endl;
        }
#endif
    }

public :
    virtual                         ~InternalType()
    {
#ifdef _SCILAB_DEBUGREF_
#if defined(_SCILAB_DEBUGREF_WITHOUT_START_END)
        if (getStartProcessing() == 0 && getEndProcessing() == 0 && bKillMe == false)
#endif
        {
            std::cout << "delete_IT " << m_iRef << " " << (void*)this << std::endl;
        }
#endif
    }
    virtual void                    whoAmI(void)
    {
        std::cout << "types::Internal";
    }

    virtual bool                    isAssignable(void)
    {
        return false;
    }
    virtual ScilabType              getType(void) = 0 ; //{ return ScilabInternal; }
    virtual ScilabId                getId(void) = 0 ; //{ return ScilabInternal; }

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
    virtual ast::Exp*               getExp(const Location& /*loc*/)
    {
        return nullptr;
    }

#ifdef _SCILAB_DEBUGREF_
    inline void _killme(const char * f, int l)
    {
#if defined(_SCILAB_DEBUGREF_WITHOUT_START_END)
        if (getStartProcessing() == 0 && getEndProcessing() == 0)
#endif
        {
            std::cout << "killme " << m_iRef << " " << (void*)this << " in " << f << " at line " << l << std::endl;
        }

        if (isDeletable())
        {
            bKillMe = true;
            delete this;
        }
    }

    inline void _increaseref(const char * f, int l)
    {
        m_iRef++;
#if defined(_SCILAB_DEBUGREF_WITHOUT_START_END)
        if (getStartProcessing() == 0 && getEndProcessing() == 0)
#endif
        {
            std::cout << "incref " << m_iRef << " " << (void*)this << " in " << f << " at line " << l << std::endl;
        }
    }

    inline void _decreaseref(const char * f, int l)
    {
        if (m_iRef > 0)
        {
            m_iRef--;
        }

#if defined(_SCILAB_DEBUGREF_WITHOUT_START_END)
        if (getStartProcessing() == 0 && getEndProcessing() == 0)
#endif
        {
            std::cout << "decref " << m_iRef << " " << (void*)this << " in " << f << " at line " << l << std::endl;
        }
    }
#else

    inline void killMe()
    {
        if (isDeletable())
        {
            delete this;
        }
    }

    inline void IncreaseRef()
    {
        m_iRef++;
    }

    inline void DecreaseRef()
    {
        if (m_iRef > 0)
        {
            m_iRef--;
        }
    }
#endif

    inline bool isDeletable()
    {
        return m_iRef == 0;
    }

    inline bool isRef(int _iRef = 0)
    {
        return m_iRef > _iRef;
    }

    inline int getRef()
    {
        return m_iRef;
    }

    virtual bool isTrue()
    {
        return false;
    }

    virtual bool neg(InternalType *& /*out*/)
    {
        return false;
    }

    virtual bool transpose(InternalType *& /*out*/)
    {
        return false;
    }

    virtual bool adjoint(InternalType *& out)
    {
        return transpose(out);
    }

    virtual bool isFieldExtractionOverloadable() const
    {
        return false;
    }

    virtual bool invoke(typed_list & /*in*/, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & /*out*/, ast::ConstVisitor & /*execFunc*/, const ast::Exp & /*e*/)
    {
        return false;
    }

    virtual bool isInvokable() const
    {
        return false;
    }

    virtual bool hasInvokeOption() const
    {
        return false;
    }

    virtual int getInvokeNbIn()
    {
        return -1;
    }

    virtual int getInvokeNbOut()
    {
        return -1;
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
    inline T*                              getAs(void)
    {
        return static_cast<T*>(this);
    }

    virtual bool                    isGenericType(void)
    {
        return false;
    }
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
    virtual bool                    isSingleStruct(void)
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
    virtual bool                    isLibrary(void)
    {
        return false;
    }
    virtual bool                    isUserType(void)
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

    bool bKillMe;

};

}

#ifdef _SCILAB_DEBUGREF_
#undef _SCILAB_DEBUGREF_
#endif

#endif /* !__INTERNAL_HXX__ */
