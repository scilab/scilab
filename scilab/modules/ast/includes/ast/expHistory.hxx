/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterpprises - Cedric Delamarre
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

#ifndef __EXPHISTORY_HXX__
#define __EXPHISTORY_HXX__

#include <list>
#include <vector>
#include "types.hxx"
#include "ast.hxx"
#include "simplevar.hxx"

class ExpHistory
{
public :
    ExpHistory();
    ExpHistory(ExpHistory*, ast::SimpleVar*);
    ExpHistory(ExpHistory*, types::typed_list*);
    ExpHistory(ExpHistory*, ast::SimpleVar*, types::typed_list*, int, bool, types::InternalType*);

    ~ExpHistory();

    // expression "a"
    void                    setExp(ast::SimpleVar*);
    ast::SimpleVar*         getExp();
    std::wstring            getExpAsString();

    // arguments x of a(x)
    void                    setArgs(types::typed_list*);
    types::typed_list*      getArgs();
    int                     getSizeFromArgs();
    int                     getArgsDims();
    int*                    getArgsDimsArray();
    void                    computeArgs();
    bool                    needResize();
    void                    setArgsOwner(bool);
    void                    setExpOwner(bool);

    // current InternalType a
    bool                    setCurrent(types::InternalType*);
    types::InternalType*    getCurrent();
    void                    setDeleteCurrent(bool bDelete); // false by default

    // parent of me
    ExpHistory*             getParent();

    // use to know if we have to reinsert current in there parent and where
    void                    setReinsertion(bool bForce = false);
    void                    resetReinsertion();
    bool                    reinsertMe();
    int                     getWhereReinsert();
    void                    setWhereReinsert(int);

    // use to know where we are in expression
    void                    setLevel(int);
    int                     getLevel();

    // management of expression a(x) or a{x}
    void                    setCellExp();
    bool                    isCellExp();

private :

    void                    initHistory();

    types::typed_list*      m_pArgs;
    int*                    m_piArgsDimsArray;
    ast::SimpleVar*         m_pExp;
    ExpHistory*             m_pParent;
    types::InternalType*    m_pITCurrent;
    bool                    m_bReinsertMe;
    bool                    m_bCellExp;
    int                     m_iArgsDims;
    int                     m_iWhere;
    int                     m_iLevel;
    bool                    m_pArgsOwner;
    bool                    m_pExpOwner;
    bool                    m_bDeleteCurrent;

};

#endif /* __EXPHISTORY_HXX__ */
