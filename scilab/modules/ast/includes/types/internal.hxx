/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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


#ifndef __INTERNAL_HXX__
#define __INTERNAL_HXX__

#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <string>

extern "C"
{
#include "dynlib_ast.h"
#include "configvariable_interface.h"
}

#include "localization.hxx"
#ifndef NDEBUG
#include "inspector.hxx"
#endif

#define bsiz 4096

//#define _SCILAB_DEBUGREF_
#ifdef _SCILAB_DEBUGREF_
//#define _SCILAB_DEBUGREF_WITHOUT_START_END
#define DecreaseRef() _decreaseref(__FILE__, __LINE__)
#define IncreaseRef() _increaseref(__FILE__, __LINE__)
#define killMe() _killme(__FILE__, __LINE__)
#endif

#include "visitor.hxx"

class Location;

namespace types
{

/*
** List of types
*/
class InternalType;
typedef std::vector<InternalType *> typed_list;
typedef std::unordered_map<std::wstring, InternalType *> optional_list;

class EXTERN_AST InternalType
{
public :
    enum ScilabType
    {
        ScilabNull = 0, //no type, no data, nothing !
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

    virtual void                    whoAmI(void);
    virtual bool                    isAssignable(void);
    virtual ScilabType              getType(void) = 0 ; //{ return ScilabInternal; }
    virtual ScilabId                getId(void) = 0 ; //{ return ScilabInternal; }
    virtual bool                    hasToString();
    virtual bool                    toString(std::wostringstream& ostr) = 0;
    virtual std::wstring            toStringInLine();
    virtual InternalType*           clone(void) = 0;
    virtual ast::Exp*               getExp(const Location& /*loc*/);

    template <typename T, typename F, typename ... A>
    T* checkRef(T* _pIT, F f, A ... a)
    {
        if (getRef() > 1)
        {
            // A types:: content in more than one Scilab variable
            // must be cloned before being modified.
            T* pClone = _pIT->clone()->template getAs<T>();
            T* pIT = (pClone->*f)(a...);
            if (pIT == NULL)
            {
                pClone->killMe();
            }

            return pIT;
        }

        return _pIT;
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

    virtual bool isTrue();
    virtual bool neg(InternalType *& /*out*/);
    virtual bool transpose(InternalType *& /*out*/);
    virtual bool adjoint(InternalType *& out);
    virtual bool isFieldExtractionOverloadable() const;
    virtual bool invoke(typed_list & /*in*/, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & /*out*/, const ast::Exp & /*e*/);
    virtual bool isInvokable() const;
    virtual bool hasInvokeOption() const;
    virtual int getInvokeNbIn();
    virtual int getInvokeNbOut();
    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring            getTypeStr() = 0;
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring            getShortTypeStr() = 0;
    virtual bool                    operator==(const InternalType& it);
    virtual bool                    operator!=(const InternalType& it);

    /**
    ** GenericType
    ** \{
    */

    template <class T>
    inline T*                              getAs(void)
    {
        return static_cast<T*>(this);
    }

    virtual bool isGenericType(void);
    virtual bool isArrayOf(void);
    virtual bool isString(void);
    virtual bool isDouble(void);
    virtual bool isSparse(void);
    virtual bool isSparseBool(void);
    virtual bool isFloat(void);
    virtual bool isInt(void);
    virtual bool isInt8(void);
    virtual bool isUInt8(void);
    virtual bool isInt16(void);
    virtual bool isUInt16(void);
    virtual bool isInt32(void);
    virtual bool isUInt32(void);
    virtual bool isInt64(void);
    virtual bool isUInt64(void);
    virtual bool isBool(void);
    virtual bool isPoly(void);
    virtual bool isSinglePoly(void);
    virtual bool isCallable(void);
    virtual bool isFunction(void);
    virtual bool isMacro(void);
    virtual bool isMacroFile(void);
    virtual bool isContainer(void);
    virtual bool isList(void);
    virtual bool isStruct(void);
    virtual bool isSingleStruct(void);
    virtual bool isCell(void);
    virtual bool isTList(void);
    virtual bool isMList(void);
    virtual bool isImplicitList(void);
    virtual bool isColon(void);
    virtual bool isDollar(void);
    virtual bool isFile(void);
    virtual bool isHandle(void);
    virtual bool isSingleHandle(void);
    virtual bool isThreadId(void);
    virtual bool isListOperation(void);
    virtual bool isListDelete(void);
    virtual bool isListInsert(void);
    virtual bool isListUndefined(void);
    virtual bool isPointer(void);
    virtual bool isLibrary(void);
    virtual bool isUserType(void);

    void clearPrintState();

protected :
    int          m_iRef;
    //use to know if we can delete this variables or if it's link to a scilab variable.
    bool         m_bAllowDelete;

    /*variables to manage print taking care of lines*/
    bool m_bPrintFromStart;
    int  m_iSavePrintState;
    int  m_iRows1PrintState;
    int  m_iCols1PrintState;
    int  m_iRows2PrintState;
    int  m_iCols2PrintState;

    bool bKillMe;

};

}

#ifdef _SCILAB_DEBUGREF_
#undef _SCILAB_DEBUGREF_
#endif

#endif /* !__INTERNAL_HXX__ */
