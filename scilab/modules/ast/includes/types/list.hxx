/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#ifndef __LIST_HH__
#define __LIST_HH__

#include <list>
#include "container.hxx"

namespace types
{
class EXTERN_AST List : public Container
{
public :
    List();
    virtual ~List();

protected :
    std::vector<InternalType *>*    getData();
    List(List *_oListCopyMe);
public :
    int                             getSize() const override;
    bool                            isVector() override;

    void                            whoAmI(void) override
    {
        std::cout << "types::List";
    };

    inline ScilabType               getType(void) override
    {
        return ScilabList;
    }
    inline ScilabId                 getId(void) override
    {
        return IdList;
    }

    bool                            getMemory(long long* _piSize, long long* _piSizePlusType) override;

    /**
    ** append(InternalType *_typedValue)
    ** Append the given value to the end of the List
    */
    List*                           append(InternalType *_typedValue);

    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    virtual List*                   clone() override;

    bool                            toString(std::wostringstream& ostr) override;

    bool                            isList() override
    {
        return true;
    }

    List*                           insert(typed_list* _pArgs, InternalType* _pSource) override;
    InternalType*                   extract(typed_list* _pArgs) override;

    virtual bool invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & /*e*/) override
    {
        if (in.size() == 0)
        {
            out.push_back(this);
        }
        else
        {
            InternalType * _out = extract(&in);
            if (_out == NULL)
            {
                // invalid index
                return false;
            }

            List* pList = _out->getAs<types::List>();
            for (int i = 0; i < pList->getSize(); i++)
            {
                out.push_back(pList->get(i));
            }
            pList->killMe();
        }

        return true;
    }

    virtual bool isInvokable() const override
    {
        return true;
    }

    virtual bool hasInvokeOption() const override
    {
        return false;
    }

    virtual int getInvokeNbIn() override
    {
        return -1;
    }

    virtual int getInvokeNbOut() override
    {
        return -1;
    }

    virtual InternalType*           get(const int _iIndex);
    virtual List*                   set(const int _iIndex, InternalType* _pIT);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring            getTypeStr() const override
    {
        return L"list";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring            getShortTypeStr() const override
    {
        return L"l";
    }

    virtual bool                    operator==(const InternalType& it) override;

protected :
    std::vector<InternalType *>*    m_plData;
};
}

#endif /* __LIST_HH__ */
