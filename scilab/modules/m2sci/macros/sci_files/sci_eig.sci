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

function [tree]=sci_eig(tree)
    // M2SCI function
    // Conversion function for Matlab eig()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // eig(A)
    if rhs==1 then
        // Because %c_spec and %b_spec are not defined
        A = getrhs(tree)
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A)

        tree.name="spec"

        // d = eig(A)
        if lhs==1 then
            tree.lhs(1).dims=list(A.dims(1),1)
            tree.lhs(1).type=Type(Double,Unknown)
            // [V,D] = eig(A)
        else
            tree.lhs(1).dims=list(A.dims(1),A.dims(1))
            tree.lhs(1).type=Type(Double,Unknown)
            tree.lhs(2).dims=list(A.dims(1),A.dims(1))
            tree.lhs(2).type=Type(Double,Unknown)
        end
        // eig(A,B) or eig(A,'nobalance')
    elseif rhs==2 then
        [A,B] = getrhs(tree)

        // 'nobalance'
        if B.vtype==String then
            no_equiv(gettext("''nobalance'' option, IGNORED."))
            tree.rhs=Rhs_tlist(tree.rhs(1))
            if lhs==1 then
                tree.name="spec"
                tree.lhs(1).dims=list(A.dims(1),1)
                tree.lhs(1).type=Type(Double,Unknown)
            else
                tree.rhs(1)=Operation("+",list(tree.rhs(1),Variable("%i",Infer()),list()))
                rhs2=Operation("+",list(Cste(1),Variable("%eps",Infer()),list()))
                tree.rhs=Rhs_tlist(tree.rhs(1),rhs2)
                tree.name="bdiag"
                tree.lhs(1).dims=list(A.dims(1),A.dims(1))
                tree.lhs(1).type=Type(Double,Unknown)
                tree.lhs(2).dims=list(A.dims(1),A.dims(1))
                tree.lhs(2).type=Type(Double,Unknown)
            end
            return
        end

        if B.vtype<>Double then
            set_infos(gettext("eig() with 2 inputs: consider generalized eigen. Check."),2)
        end
        m=A.dims(1)
        n=A.dims(2)
        if lhs==1 then
            al=gettempvar(1)
            be=gettempvar(2)
            m2sci_insert(Equal(list(al,be),Funcall("spec",1,tree.rhs,list(al,be))))
            tree=Operation("./",list(al,be),tree.lhs)

            tree.out(1).dims=list(m,1)
            tree.out(1).type=Type(Double,Unknown)
        else
            al=gettempvar(1)
            be=gettempvar(2)

            m2sci_insert(Equal(list(al,be,tree.lhs(1)),Funcall("eig",1,tree.rhs,list(al,be,tree.lhs(1)))))

            tree=Operation("./",list(al,be),list(tree.lhs(1)))
            tree=Funcall("diag",1,list(tree),tree.out)

            tree.lhs(1).dims=list(A.dims(1),A.dims(1))
            tree.lhs(1).type=Type(Double,Unknown)
            tree.lhs(2).dims=list(A.dims(1),A.dims(1))
            tree.lhs(2).type=Type(Double,Unknown)
        end
        // eig(A,B,flag)
    else
        no_equiv(msprintf(gettext("%s, flag IGNORED."), strcat(expression2code(tree), "")))

        tree.rhs(3)=null()

        tree.lhs(1).dims=list(A.dims(1),A.dims(1))
        tree.lhs(1).type=Type(Double,Unknown)
        if lhs==2 then
            tree.lhs(2).dims=list(A.dims(1),A.dims(1))
            tree.lhs(2).type=Type(Double,Unknown)
        end
    end

endfunction
