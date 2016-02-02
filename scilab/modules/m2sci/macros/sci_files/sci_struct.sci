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

function [tree]=sci_struct(tree)
    // M2SCI function
    // Conversion function for Matlab struct()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    tree.lhs(1).type=Type(Struct,Unknown)

    // Dims...
    dims=tree.rhs(2).dims
    for k=4:2:rhs
        for l=1:lstsize(dims)
            if tree.rhs(k).dims(l)==Unknown then
                dims(l)=Unknown
            else
                if dims(l)<>Unknown then
                    if dims(l)<tree.rhs(k).dims(l) then
                        dims(l)=tree.rhs(k).dims(l)
                    end
                end
            end

        end
    end

    tree.lhs(1).dims=dims

    // Update contents
    dims=[0 0]
    allnotcells=%T
    for k=1:2:rhs
        allnotcells=allnotcells & tree.rhs(k+1).vtype<>Cell
    end
    if allnotcells then
        dims=[1 1]
        tree.lhs(1).dims=list(1,1)
    else
        for k=1:2:rhs
            if tree.rhs(k+1).vtype==Cell then
                if double(tree.rhs(k+1).dims(1))>dims(1) then
                    dims(1)=double(tree.rhs(k+1).dims(1))
                end
                if double(tree.rhs(k+1).dims(2))>dims(2) then
                    dims(2)=double(tree.rhs(k+1).dims(2))
                end
            end
        end
    end

    for k=1:2:rhs
        dim1=dims(1)
        dim2=dims(2)
        for kd1=1:dims(1)
            for kd2=1:dims(2)
                if tree.rhs(k+1).vtype==Cell then

                    if tree.rhs(k+1).dims(1)*tree.rhs(k+1).dims(2)<>1 then
                        tree.lhs(1).contents.index($+1)=list(list(Cste(kd1),Cste(kd2)),tree.rhs(k))
                        tree.lhs(1).contents.data($+1)=get_contents_infer(tree.rhs(k+1),list(list(Cste(kd1),Cste(kd2)),Cste("entries")))
                    else
                        tree.lhs(1).contents.index($+1)=list(list(Cste(kd1),Cste(kd2)),tree.rhs(k))
                        tree.lhs(1).contents.data($+1)=get_contents_infer(tree.rhs(k+1),list(list(Cste(1),Cste(1)),Cste("entries")))
                    end

                else
                    tree.lhs(1).contents.index($+1)=list(list(Cste(1),Cste(1)),tree.rhs(k))
                    tree.lhs(1).contents.data($+1)=tree.rhs(k+1).infer
                end
            end
        end
    end
endfunction
