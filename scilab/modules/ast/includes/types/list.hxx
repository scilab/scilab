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
    int                             getSize();

    void                            whoAmI(void)
    {
        std::cout << "types::List";
    };

    inline ScilabType               getType(void)
    {
        return ScilabList;
    }
    inline ScilabId                 getId(void)
    {
        return IdList;
    }

    /**
    ** append(InternalType *_typedValue)
    ** Append the given value to the end of the List
    */
    List*                           append(InternalType *_typedValue);

    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    virtual List*                   clone();

    bool                            toString(std::wostringstream& ostr);

    bool                            isList()
    {
        return true;
    }

    List*                           insert(typed_list* _pArgs, InternalType* _pSource);
    InternalType*                   extract(typed_list* _pArgs);

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

    virtual bool isInvokable() const
    {
        return true;
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

    virtual InternalType*           get(const int _iIndex);
    virtual List*                   set(const int _iIndex, InternalType* _pIT);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring            getTypeStr()
    {
        return L"list";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring            getShortTypeStr()
    {
        return L"l";
    }

    virtual bool                    operator==(const InternalType& it);

protected :
    std::vector<InternalType *>*    m_plData;
};
}

#endif /* __LIST_HH__ */
