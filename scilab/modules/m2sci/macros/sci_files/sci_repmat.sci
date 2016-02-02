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

function [tree]=sci_repmat(tree)
    // M2SCI function
    // Conversion function for Matlab repmat()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_repmat()

    // repmat(A,m)
    if rhs==2 then
        [A,m] = getrhs(tree)
        vtype=A.vtype
        if A.vtype==Unknown then
            tree.name="mtlb_repmat"
            if typeof(m)=="cste" then
                for kd=1:lstsize(A.dims)
                    tree.lhs(1).dims(kd)=A.dims(kd)*m.value
                    if tree.lhs(1).dims(kd)<0 then
                        tree.lhs(1).dims(kd)=Unknown
                    end
                end
            end
            tree.lhs(1).vtype=A.vtype
            return
        elseif A.vtype==Boolean then
            A=convert2double(A)
        elseif A.vtype==String then
            A=convert2double(A)
        end

        // repmat(A,m) equivalent to repmat(A,m,m)
        if is_a_scalar(m) then
            tree.rhs=list(m,m);
            tree.name="ones";
            tree=Operation(".*.",list(tree,A),tree.lhs)

            if typeof(m)=="cste" then
                tree.out(1).dims=A.dims
                for kd=1:2
                    tree.out(1).dims(kd)=A.dims(kd)*m.value
                    if tree.out(1).dims(kd)<0 then
                        tree.out(1).dims(kd)=Unknown
                    end
                end
            end
            tree.out(1).type=Type(A.vtype,A.property)
            // repmat(A,[m,n,...])
        elseif not_a_scalar(m)
            tmp=m
            if typeof(m)<>"variable" then
                tmp=gettempvar()
                m2sci_insert(Equal(list(tmp),m))
            end
            newrhs=list()
            for k=1:size(m.dims)
                newrhs(k)=Operation("ext",list(tmp,Cste(k)),list())
            end
            tree.rhs=newrhs;
            tree.name="ones";
            tree=Operation(".*.",list(tree,A),tree.lhs)

            tree.out(1).type=Type(Double,A.property)
            // repmat(A,m) or repmat(A,[m,n,...])
        else
            tree.name="mtlb_repmat"
            tree.lhs(1).type=Type(A.vtype,A.property)
            if or(vtype==[String,Boolean]) then
                tree.lhs(1).type=Type(vtype,A.property)
            end
            return
        end
        // repmat(A,m,n)
    else
        [A,m,n] = getrhs(tree)
        vtype=A.vtype
        if A.vtype==Unknown then
            tree.name="mtlb_repmat"
            if is_a_scalar(A) & typeof(m)=="cste" then
                tree.lhs(1).dims(1)=m.value
            end
            if is_a_scalar(A) & typeof(n)=="cste" then
                tree.lhs(1).dims(2)=n.value
            end
            tree.lhs(1).vtype=A.vtype
            return
        elseif A.vtype==Boolean then
            A=convert2double(A)
        elseif A.vtype==String then
            A=convert2double(A)
        end
        tree.rhs=Rhs_tlist(m,n)
        tree.name="ones";
        tree=Operation(".*.",list(tree,A),tree.lhs)

        if typeof(m)=="cste" & typeof(n)=="cste" then
            tree.out(1).dims=A.dims
            tree.out(1).dims(1)=A.dims(1)*m.value
            if tree.out(1).dims(1)<0 then
                tree.out(1).dims(1)=Unknown
            end
            tree.out(1).dims(2)=A.dims(2)*n.value
            if tree.out(1).dims(2)<0 then
                tree.out(1).dims(2)=Unknown
            end
        end
    end

    if vtype==String then
        tree=Funcall("asciimat",1,list(tree),tree.out);
        tree.lhs(1).type=Type(String,Real)
    elseif vtype==Boolean then
        tree.out(1).type=Type(Boolean,Real)
    end

endfunction
