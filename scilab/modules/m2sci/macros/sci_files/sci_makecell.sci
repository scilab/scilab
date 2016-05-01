// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_makecell(tree)
    // M2SCI function
    // Conversion function for Matlab {}
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==0 then
        tree.rhs(1)=Operation("rc",list(Cste(0),Cste(0)),list())
        tree.lhs(1).dims=list(0,0)
        tree.lhs(1).type=Type(Cell,Unknown)
        tree.lhs(1).contents=Contents()
        return
    end

    dimsrhs=Operation("rc",list(Cste(double(tree.rhs(1).dims(1))),Cste(double(tree.rhs(1).dims(2)))),list())

    inrhs=tree.rhs(1)

    tmprhslist=list()
    tmprhslist(1)=dimsrhs
    if double(tree.rhs(1).dims(1))<>1 then

        for kr=1:double(tree.rhs(1).dims(1))-1

            for krhs=lstsize(tmprhslist):-1:2
                tmprhslist(krhs+1)=tmprhslist(krhs)
            end

            tmprhslist(2)=inrhs.operands(2)
            inrhs=inrhs.operands(1)
        end

        for krhs=lstsize(tmprhslist):-1:2
            tmprhslist(krhs+1)=tmprhslist(krhs)
        end
        tmprhslist(2)=inrhs

    else
        tmprhslist(2)=inrhs
    end

    rhslist=list()
    rhslist(1)=dimsrhs
    if double(tree.rhs(1).dims(2))<>1 then

        tmprhslist(1)=null()

        for ksz=lstsize(tmprhslist):-1:1
            inrhs=tmprhslist(ksz)
            for kr=1:double(tree.rhs(1).dims(2))-1

                for krhs=lstsize(rhslist):-1:2
                    rhslist(krhs+1)=rhslist(krhs)
                end

                rhslist(2)=inrhs.operands(2)
                inrhs=inrhs.operands(1)
            end

            for krhs=lstsize(rhslist):-1:2
                rhslist(krhs+1)=rhslist(krhs)
            end
            rhslist(2)=inrhs

        end
    else
        rhslist=tmprhslist
    end

    // Del cell in first rhslist(2)
    rhslist(2)=rhslist(2).operands(2)
    tree.lhs(1).dims=tree.rhs(1).dims
    tree.lhs(1).type=tree.rhs(1).type
    tree.lhs(1).contents=tree.rhs(1).contents

    tree.rhs=rhslist

endfunction
