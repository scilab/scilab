// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = %cc2sci(tree)
    // Vertical concatenation [ ; ]
    // Make a 'column' with many rows

    if tree.operands(1).vtype==Cell then
        tree = %cc_cell2sci(tree)
        return
    end

    // Computing the type and property of the result
    // ---------------------------------------------
    rowsize = tree.operands(1).dims(1)
    colsize = tree.operands(1).dims(2)

    vtype = tree.operands(1).vtype
    prop = tree.operands(1).property
    sparseRes = vtype==Sparse | prop==Sparse

    rownb = size(tree.operands)

    //disp(getTypeName(vtype), getPropName(prop))
    for k = 2:rownb
        ck = tree.operands(k);
        if ck.vtype==String then
            vtype = String
            prop = ck.property  // Char | String (dominant à préciser)
        end
        if ck.vtype==Unknown & vtype<>String
            vtype = Unknown
        elseif ck.vtype==Int & or(vtype==[1 4 5])
            [vtype, prop] = (Int, ck.property)
        elseif ck.vtype==Sparse & or(vtype==[1 4 5])
            [sparseRes, vtype] = (%t, Sparse)
            if or(vtype==[1 5])
                if ck.property==Complex
                    prop = Complex
                elseif ck.property==Unknown & prop==Real
                    prop = Unknown
                elseif prop <> Complex
                    prop = Real
                end
            else
                prop = ck.property
            end
        elseif or(ck.vtype==[Double Sparse]) & or(vtype==[Double Sparse])
            if ck.property==Complex
                prop = Complex
            end
        elseif ck.vtype==Double & vtype==Boolean
            if sparseRes
                [vtype, prop] = (Sparse, ck.property)
            else
                [vtype, prop] = (Double, ck.property)
            end
        elseif ck.property==Sparse
            if vtype==Boolean               // Boolean < Unknown < SparseBool
                [sparseRes, prop] = (%t, Sparse)
            elseif vtype==Double
                [sparseRes, vtype] = (%t, Sparse)
            end
        end
        rowsize = [rowsize ck.dims(1)]
        colsize = [colsize ck.dims(2)]
    end


    // Computing the size of the result
    // --------------------------------
    undef = find(colsize==-1)
    void = find(colsize==0)
    colsize([undef void]) = []
    if colsize==[] then
        if undef<>[] then
            sc = -1
        else
            sc = 0
        end
    else
        [w,k] = min(length(colsize))
        sc = colsize(k)
    end
    if find(rowsize==-1)==[] then
        w = rowsize(1)
        for k = 2:rownb
            w = w+rowsize(k)
        end
        sr = w
    else
        sr = -1
    end
    tree.out(1).dims = list(sr,sc)
    tree.out(1).type = Type(vtype,prop)
    tree.out(1).contents = Contents()
endfunction
