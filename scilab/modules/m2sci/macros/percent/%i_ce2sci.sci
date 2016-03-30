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

function [tree]=%i_ce2sci(tree)
    // M2SCI function
    // Conversion function for Matlab insertion in cells
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    from=tree.operands($)
    to=tree.operands(1)
    // Verify that to is not a struct (cell of struct)
    inds=tree.operands;inds(1)=null();inds($)=null()
    if type(inds)<>15 then
        inds=list(inds)
    end
    for kinds=1:lstsize(inds)
        if typeof(inds(kinds))<>"list" & inds(kinds).vtype==String & typeof(inds(kinds))=="cste" & inds(kinds).value<>":" then
            tree=%i_st2sci(tree)
            return
        end
    end

    if to.vtype<>Struct then
        if and(to.vtype<>[Cell,Unknown]) then
            if to.vtype==Double & and(to.dims==list(0,0)) then
                m2sci_insert(Equal(list(to),Funcall("cell",1,list(),list(to))))
                // To be sure that variable will now be of type Cell
                [bval,index]=isdefinedvar(to)
                varslist(index).infer.type.vtype=Cell
            else
                error(msprintf(gettext("destination variable is not a cell: %s is of type %s."),to.name,string(to.vtype)))
            end
        elseif to.vtype==Unknown then
            m2sci_insert(Equal(list(to),Funcall("cell",1,list(),list(to))))
            // To be sure that variable will now be of type Cell
            [bval,index]=isdefinedvar(to)
            varslist(index).infer.type.vtype=Cell
        end
    end
    // Just one index value
    if rhs==1 then
        ind=tree.operands(2)
        if type(ind)<>15 then // One value index A(xx)={...}
            tree.operands(2)=list(Cste(1),ind)
            tree.out(1).vtype=Cell
            if typeof(ind)=="cste" then
                if ind.vtype<>String then // Not :
                    tree.out(1).dims=list(1,ind.value)

                    // Data added so that extraction of a cell element can also be infered
                    tree.out(1).contents.index($+1)=tree.operands(2)
                    tree.out(1).contents.data($+1)=Infer(list(1,1),Type(Cell,Unknown),from.contents)

                    if lstsize(from.contents.data)==1 then
                        tree.out(1).contents.index($+1)=list(tree.operands(2),Cste("entries"))
                        tree.out(1).contents.data($+1)=from.contents.data(1)
                    else
                        error(gettext("Not yet implemented."))
                    end
                else
                    tree.out(1).infer=from.infer
                end
            else
                tree.out(1).dims=list(1,Unknown)
            end
        else // --- Insertion with more than one index value (index is a list) ---
            // Cell array of struct A{p,q,...}.name... or recursive index A{p,q,...}(1,2)...
            for kind=1:lstsize(tree.operands(2))
                if typeof(tree.operands(2)(kind))=="cste" then
                    if tree.operands(2)(kind).vtype<>String then
                        tree.operands(2)(kind)=list(Cste(1),tree.operands(2)(kind))
                    end
                end
            end
            IND=tree.operands(2)(1)
            // Update cell dims for inference
            if typeof(IND)=="list" then
                if lstsize(IND)>lstsize(tree.out(1).dims) then
                    for kd=lstsize(tree.out(1).dims):lstsize(IND)
                        tree.out(1).dims(kd)=Unknown
                    end
                end
                for kd=1:lstsize(tree.out(1).dims)
                    if typeof(IND(kd))=="cste" & tree.out(1).dims(kd)<>Unknown & tree.out(1).dims(kd)<IND(kd).value then
                        tree.out(1).dims(kd)=IND(kd).value
                    end
                end
            else
                tree.out(1).dims=list(1,1)
            end
            tree.out(1).type=Type(Cell,Unknown)

            ind=tree.operands(2)
            if typeof(ind($))=="list" | ind($).vtype~=String then // X.p(m,n)=y
                tmp=gettempvar()
                oplist=list()

                tmpind=ind
                tmpind($)=null()
                if or(get_contents_infer(tree.operands(1),tmpind)<>Infer()) then
                    tmp.infer=get_contents_infer(tree.operands(1),tmpind)
                end
                oplist(1)=tmp

                for kind=1:size(ind($))
                    oplist($+1)=ind($)(kind)
                end

                oplist($+1)=tree.operands($)

                newop=Operation("ins",oplist,list(tmp))
                newop=%i2sci(newop)
                tree.out(1).infer.contents.index($+1)=tmpind
                tree.out(1).infer.contents.data($+1)=newop.out(1).infer
            elseif ind($).vtype==String then
                tree.out(1).type=Type(Struct,Unknown)
            end

            // Update cell contents
            infertree=tree.operands(2)
            tree.out(1).contents.index($+1)=infertree
            tree.out(1).contents.data($+1)=from.infer
        end
        // Two indexes: to(ind1,ind2,...)=from or more
    else
        tree.out(1).dims=list()
        for k=1:lstsize(tree.operands)-2
            tree.out(1).dims(k)=Unknown
        end

        // dim can be infered when index is a constant and when index value is greater than older dim and this dim is not unknown
        for kdim=1:size(tree.operands)-2
            if typeof(tree.operands(kdim+1))=="cste" then
                if to.dims(kdim)<>Unknown then
                    if to.dims(kdim)<=tree.operands(kdim+1).value then
                        tree.out(1).dims(kdim)=tree.operands(kdim+1).value;
                    else
                        tree.out(1).dims(kdim)=to.dims(kdim)
                    end
                end
            end
        end
        tree.out(1).type=from.type

        // Update contents...
        infertree=tree.operands
        infertree(1)=null()
        infertree($)=null()

        // Data added so that extraction of a cell element can also be infered
        tree.out(1).contents.index($+1)=infertree
        tree.out(1).contents.data($+1)=Infer(list(1,1),Type(Cell,Unknown),Contents())

        infertree=list(infertree,Cste("entries"))
        if lstsize(from.contents.index)==1 then
            tree.out(1).contents.index($+1)=infertree
            tree.out(1).contents.data($+1)=from.contents.data(1);
        elseif lstsize(from.contents.index)==0 then
            tree.out(1).contents=Contents()
        else
            error(gettext("Not yet implemented."))
        end
    end
endfunction

