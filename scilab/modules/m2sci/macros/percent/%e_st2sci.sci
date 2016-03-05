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

function [tree]=%e_st2sci(tree)
    // M2SCI function
    // Conversion function for Matlab extraction from structs
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_e()

    // Global variable for M2SCI
    global("varslist")

    var=tree.operands(1)

    // Do not consider variable as a rhs...
    rhs=rhs-1

    // One index value
    if rhs==1 then
        ind=tree.operands(2)

        // --- Recursive extraction ---
        if type(ind)==15 then
            // If last index value is not a fieldname, it is ignored
            // it will be considered after all other indexes
            // Avoid some extraction problems from character strings
            lastisnotfield=typeof(ind($))=="list"
            if ~lastisnotfield then
                lastisnotfield=ind($).vtype<>String
            end

            // Inference
            infertree=tree.operands(2)
            if lastisnotfield then // Last index is not a fieldname, ignored here
                infertree($)=null()
            end
            // Change index value if just one double
            for k=1:lstsize(infertree)
                if typeof(infertree(k))=="cste" | (typeof(infertree(k))<>"list" & is_a_scalar(infertree(k))) then
                    if infertree(k).vtype<>String then
                        infertree(k)=list(Cste(1),infertree(k))
                    end
                end
            end

            [bval,index]=isdefinedvar(var)
            if ~bval then
                error(msprintf(gettext("M2SCI bug: extraction from unknown variable %s in varslist."),var.name))
            else
                tmp=get_contents_infer(var,infertree);
                tree.out(1).dims=tmp.dims
                tree.out(1).type=tmp.type
                tree.out(1).contents=tmp.contents
            end

            // Convert last extraction operation is not already done
            if lastisnotfield then
                [inftlist,pos]=get_contents_infer(var,infertree)
                tmp=gettempvar()
                tmp.infer=inftlist
                varslist($+1)=M2scivar(tmp.name,tmp.name,tmp.infer)

                oplist=list()
                oplist(1)=tmp
                infertree=tree.operands($)($)
                if typeof(infertree)<>"list" & infertree.vtype<>String then
                    infertree=list(infertree);
                end
                for k=1:size(infertree)
                    oplist($+1)=infertree(k)
                end
                newop=Operation("ext",oplist,tree.out)
                rhs=size(newop.operands)
                newop=%e2sci(newop)
                if typeof(newop)=="operation" then
                    tree.out(1).dims=newop.out(1).dims
                    tree.out(1).type=newop.out(1).type
                    tree.out(1).contents=newop.out(1).contents
                else
                    tree.out(1).dims=newop.lhs(1).dims
                    tree.out(1).type=newop.lhs(1).type
                    tree.out(1).contents=newop.lhs(1).contents
                end
            end
        else
            // extraction from struct
            // this case should happen only if var(ind.value) contains only one value
            [bval,index]=isdefinedvar(var)
            if ~bval then
                error(msprintf(gettext("M2SCI bug: extraction from unknown variable %s in varslist."),var.name))
            else
                if varslist(index).vtype<>Struct then
                    // variable not defined as a struct, modify inference
                    varslist(index).infer.vtype=Struct
                end
            end
            outdims=list(Unknown,Unknown)
            if ind.dims(1)==1 then // a scalar or a vector
                outdims=list(1,ind.dims(2))
            end
            if typeof(ind)=="cste" | (typeof(ind)<>"list" & is_a_scalar(ind)) then
                if ind.vtype<>String then
                    if var.dims(1)==1 then // row vector
                        ind=list(Cste(1),ind)
                    elseif var.dims(2)==1 then // column vector
                        ind=list(ind,Cste(1))
                    end
                end
            end
            if typeof(ind)<>"list" then
                ind=list(ind);
            end
            tree.out(1).infer=get_contents_infer(var,ind)
            if tree.out(1).dims==list(Unknown,Unknown) then
                tree.out(1).dims=outdims
            end
        end
        // More than one index value
    else
        dims=list()
        for k=2:rhs+1
            dimsum=0
            for l=1:size(tree.operands(k).dims)
                dimsum=dimsum+tree.operands(k).dims(l)
            end
            if dimsum==size(tree.operands(k).dims) // All dims are 1
                dims(k-1)=1
                if typeof(tree.operands(k))=="cste" then
                    if tree.operands(k).value==":" then
                        dims(k-1)=var.dims(k-1)
                    end
                end
            else
                dims(k-1)=Unknown
            end
        end

        tree.out(1).dims=dims
        tree.out(1).type=var.type

        IND=tree.operands
        IND(1)=null() // Variable is not an index

        tree.out(1).contents.index($+1)=IND
        tree.out(1).contents.data($+1)=get_contents_infer(tree.operands(1),IND)
    end
endfunction

