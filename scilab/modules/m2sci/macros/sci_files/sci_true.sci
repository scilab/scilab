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

function [tree]=sci_true(tree)
    // File generated from sci_PROTO3.g: PLEASE DO NOT EDIT !
    // M2SCI function
    // Conversion function for Matlab true()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_true()

    // Used for false and true
    name_sav=tree.name

    if rhs==-1 then
        if tree.name=="ones"|tree.name=="eye" then
            tree=Cste(":")
        elseif tree.name=="zeros" then
            tree=Cste(0)
        elseif tree.name=="rand" then
            // Nothing changed
        elseif tree.name=="randn" then
            tree.rhs=Rhs_tlist(1,1,"normal")
            tree.lhs(1).dims=list(1,1)
            tree.lhs(1).type=Type(Double,Real)
        elseif tree.name=="false" then
            tree=Cste(%F)
        elseif tree.name=="true" then
            tree=Cste(%T)
        end
        return
    end

    if rhs==0 then
        if tree.name=="ones"|tree.name=="eye" then
            tree=Cste(1)
        elseif tree.name=="zeros" then
            tree=Cste(0)
        elseif tree.name=="rand" then
            // Nothing changed
        elseif tree.name=="randn" then
            tree.name="rand";
            tree.rhs=Rhs_tlist(1,1,"normal")
            tree.lhs(1).dims=list(1,1)
            tree.lhs(1).type=Type(Double,Real)
        elseif tree.name=="false" then
            tree=Cste(%F)
        elseif tree.name=="true" then
            tree=Cste(%T)
        end
        return
    end

    // Special cases for rand and randn
    if tree.name=="randn" |tree.name=="rand" then
        if tree.rhs(1).vtype==String then // State
            if rhs==1 then // Get the state
                if tree.name=="rand" then
                    onescall=Funcall("ones",1,Rhs_tlist(35,1),list())
                    randcall=Funcall("rand",1,Rhs_tlist("seed"),list())
                    tree=Operation("*",list(onescall,randcall),tree.lhs)
                    tree.out(1).dims=list(35,1)
                    tree.out(1).type=Type(Double,Real)
                else
                    onescall=Funcall("ones",1,Rhs_tlist(2,1),list())
                    randcall=Funcall("rand",1,Rhs_tlist("seed"),list())
                    tree=Operation("*",list(onescall,randcall),tree.lhs)
                    tree.out(1).dims=list(2,1)
                    tree.out(1).type=Type(Double,Real)
                end
            else // Set the state
                tree=Funcall("rand",1,Rhs_tlist("seed",tree.rhs(2)),tree.lhs)
                tree.lhs(1).dims=list(0,0)
                tree.lhs(1).type=Type(Unknown,Unknown)
            end
            return
        end
    end

    opt=list(),if tree.name=="randn" then opt=Cste("normal"),end

    // true(n)
    if rhs==1 then
        n = getrhs(tree)
        n=convert2double(n)
        if is_a_scalar(n) then
            if typeof(n)=="cste" then
                dim=n.value
            else
                dim=Unknown
            end

            tree=Funcall("ones",1,Rhs_tlist(n,n,opt),tree.lhs)
            tree.lhs(1).dims=list(dim,dim)
            tree.lhs(1).type=Type(Double,Real)
            // true([n1,n2,...])
        elseif not_a_scalar(n)
            tmp=n
            if typeof(n)<>"variable" then
                tmp=gettempvar()
                m2sci_insert(Equal(list(tmp),n))
            end
            newrhs=list()
            for k=1:size(n.dims)
                newrhs(k)=Operation("ext",list(tmp,Cste(k)),list())
            end
            if opt<>list() then
                newrhs($+1)=opt
            end
            tree=Funcall("ones",1,newrhs,tree.lhs)

            // All dimensions are unknown because we can not compute them here...
            tree.lhs(1).dims=list()
            if opt<>list() then
                for k=1:size(tree.rhs)-1
                    tree.lhs(1).dims(k)=Unknown
                end
            else
                for k=1:size(tree.rhs)
                    tree.lhs(1).dims(k)=Unknown
                end
            end
            tree.lhs(1).type=Type(Double,Real)
            // true(n) or true([n1,n2,...]) ?
        else
            tree.name="mtlb_true"

            tree.lhs(1).dims=list(Unknown,Unknown)
            tree.lhs(1).type=Type(Double,Real)
        end

        // true(n1,n2,..)
    else
        for k=1:size(tree.rhs)
            tree.rhs(k)=convert2double(tree.rhs(k))
        end
        if opt<>list() then
            tree.rhs($+1)=opt
        end
        tree.name="ones"
        // If one input is a Cste, we can infer corresponding dimension...
        tree.lhs(1).dims=list()
        for k=1:size(tree.rhs)
            if typeof(tree.rhs(k))=="cste" then
                if tree.rhs(k).value<>"normal" then
                    tree.lhs(1).dims($+1)=tree.rhs(k).value
                end
            else
                tree.lhs(1).dims($+1)=Unknown
            end
        end
        tree.lhs(1).type=Type(Double,Real)
    end

    if or(name_sav==["false","true"]) then
        if name_sav=="false" then
            tree.name="zeros"
        else
            tree.name="ones"
        end
        tree.lhs(1).type=Type(Boolean,Real)
        tree=Operation("==",list(tree,Cste(1)),tree.lhs)
    end
endfunction






