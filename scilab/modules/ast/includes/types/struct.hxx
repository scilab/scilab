/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __STRUCT_HXX__
#define __STRUCT_HXX__

#include "list.hxx"
#include "arrayof.hxx"
#include "singlestruct.hxx"

extern "C"
{
#include "localization.h"
}

namespace types
{
class EXTERN_AST Struct : public ArrayOf<SingleStruct*>
{
public :
    ~Struct();
    Struct();
    Struct(int _iRows, int _iCols);
    Struct(int _iDims, const int* _piDims);

private :
    Struct(Struct* _oCellCopyMe);

public :

    void                        whoAmI(void)
    {
        std::cout << "types::Struct";
    };

    inline ScilabType           getType(void)
    {
        return ScilabStruct;
    }
    inline ScilabId             getId(void)
    {
        return IdStruct;
    }

    bool                        isStruct()
    {
        return true;
    }
    bool                        isEmpty();

    bool transpose(InternalType *& out);


    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    Struct*                     clone();

    Struct*                     set(int _iRows, int _iCols, SingleStruct* _pIT);
    Struct*                     set(int _iRows, int _iCols, const SingleStruct* _pIT);
    Struct*                     set(int _iIndex, SingleStruct* _pIT);
    Struct*                     set(int _iIndex, const SingleStruct* _pIT);
    Struct*                     set(SingleStruct** _pIT);

    bool                        operator==(const InternalType& it);
    bool                        operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string        getTypeStr()
    {
        return "struct";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string        getShortTypeStr()
    {
        return "st";
    }
    virtual bool                isContainer(void)
    {
        return true;
    }

    bool isTrue()
    {
        return false;
    }

    virtual bool neg(InternalType *& /*out*/)
    {
        return false;
    }

    bool                        subMatrixToString(std::ostringstream& ostr, int* _piDims, int _iDims) override;
    String*                     getFieldNames();
    bool                        exists(const std::string& _sKey);
    Struct*                     addField(const std::string& _sKey);
    Struct*                     addFieldFront(const std::string& _sKey);
    Struct*                     removeField(const std::string& _sKey);
    bool                        toString(std::ostringstream& ostr) override;
    List*                       extractFieldWithoutClone(const std::string& _wstField);
    std::vector<InternalType*>  extractFields(const std::vector<std::string>& _wstFields);
    std::vector<InternalType*>  extractFields(typed_list* _pArgs);
    inline InternalType *       extractField(const std::string& wstField);

    Struct*                     resize(int* _piDims, int _iDims);
    Struct*                     resize(int _iNewRows, int _iNewCols);

    /*specials functions to disable clone operation during copydata*/
    InternalType*               insertWithoutClone(typed_list* _pArgs, InternalType* _pSource);
    InternalType*               extractWithoutClone(typed_list* _pArgs);
    void                        setCloneInCopyValue(bool _val);

    using ArrayOf<SingleStruct *>::extract;
    bool extract(const std::string& name, InternalType *& out);

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e) override;

    virtual int getInvokeNbOut()
    {
        return -1;
    }

private :
    virtual SingleStruct*       getNullValue();
    virtual Struct*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual Struct*             createEmpty();
    virtual SingleStruct*       copyValue(SingleStruct* _pData);
    virtual void                deleteAll();
    virtual void                deleteImg();
    virtual SingleStruct**      allocData(int _iSize);
    virtual void                deleteData(SingleStruct* data);

    bool                        m_bDisableCloneInCopyValue;

};
}

#ifdef _MSC_VER
template class types::ArrayOf<types::SingleStruct*>; //Struct
#endif
#endif /* !__STRUCT_HXX__ */
