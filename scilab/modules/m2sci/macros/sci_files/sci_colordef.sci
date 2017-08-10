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

function [tree]=sci_colordef(tree)
    // M2SCI function
    // Conversion function for Matlab colordef()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if tree.lhs(1).name=="ans" then
        f=gettempvar()
    else
        f=tree.lhs(1)
    end

    // colordef color_option
    if rhs==1 then
        if typeof(tree.rhs(1))=="cste" then
            if or(tree.rhs(1).value==["black","none"]) then
                m2sci_insert(Equal(list(f),Funcall("gcf",1,list(),list())))
                LHS=Operation("ins",list(f,Cste("background")),list())
                m2sci_insert(Equal(list(LHS),Cste(-1)))
                tree=list()
            elseif tree.rhs(1).value=="white" then
                m2sci_insert(Equal(list(f),Funcall("gcf",1,list(),list())))
                LHS=Operation("ins",list(f,Cste("background")),list())
                m2sci_insert(Equal(list(LHS),Cste(-2)))
                tree=list()
            else
                no_equiv(expression2code(tree))
            end
        else
            tree.name="mtlb_colordef"
        end
    else
        // colordef('new',color_opt)
        if tree.rhs(1).vtype==String then
            winsid_funcall=Funcall("winsid",1,list(),list())
            max_funcall=Funcall("max",1,list(winsid_funcall),list())
            addition=Operation("+",list(max_funcall,Cste(1)),list())
            scf_funcall=Funcall("scf",1,list(addition),list())
            m2sci_insert(Equal(list(f),scf_funcall))
            if or(tree.rhs(2).value==["black","none"]) then
                LHS=Operation("ins",list(f,Cste("background")),list())
                m2sci_insert(Equal(list(LHS),Cste(-1)))
                tree=list()
            elseif tree.rhs(2).value=="white" then
                LHS=Operation("ins",list(f,Cste("background")),list())
                m2sci_insert(Equal(list(LHS),Cste(-2)))
                tree=list()
            else
                no_equiv(expression2code(tree))
            end

            // colordef(fig,color_opt)
        elseif tree.rhs(1).vtype==Handle then
            f=tree.rhs(1)
            if or(tree.rhs(2).value==["black","none"]) then
                LHS=Operation("ins",list(f,Cste("background")),list())
                m2sci_insert(Equal(list(LHS),Cste(-1)))
                if tree.lhs(1).name<>"ans" then
                    m2sci_insert(Equal(list(tree.lhs(1)),f))
                end
                tree=list()
            elseif tree.rhs(2).value=="white" then
                LHS=Operation("ins",list(f,Cste("background")),list())
                m2sci_insert(Equal(list(LHS),Cste(-2)))
                if tree.lhs(1).name<>"ans" then
                    m2sci_insert(Equal(list(tree.lhs(1)),f))
                end
                tree=list()
            else
                no_equiv(expression2code(tree))
            end
        else
            tree.name="mtlb_colordef"
        end
    end

endfunction
