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

function tree = %rc2sci(tree)

    // Make a 'row' with many columns

    if tree.operands(1).vtype==Cell then
        tree = %rc_cell2sci(tree)
        return
    end

    colnb = size(tree.operands)

    // Computing the type and property of the result
    // ---------------------------------------------
    rowsize = tree.operands(1).dims(1)
    colsize = tree.operands(1).dims(2)

    vtype = tree.operands(1).vtype
    prop = tree.operands(1).property
    sparseRes = vtype==Sparse | prop==Sparse

    //disp(getTypeName(vtype), getPropName(prop))
    for k = 2:colnb
        rk = tree.operands(k);
        if rk.vtype==String then
            vtype = String
            prop = rk.property  // Char | String (dominant à préciser)
        end
        if rk.vtype==Unknown & vtype<>String
            vtype = Unknown
        elseif rk.vtype==Int & or(vtype==[1 4 5])
            [vtype, prop] = (Int, rk.property)
        elseif rk.vtype==Sparse & or(vtype==[1 4 5])
            [sparseRes, vtype] = (%t, Sparse)
            if or(vtype==[1 5])
                if rk.property==Complex
                    prop = Complex
                elseif rk.property==Unknown & prop==Real
                    prop = Unknown
                elseif prop <> Complex
                    prop = Real
                end
            else
                prop = rk.property
            end
        elseif rk.vtype==Double & vtype==Double
            if rk.property==Complex
                prop = Complex
            end
        elseif rk.vtype==Double & vtype==Boolean
            if sparseRes
                [vtype, prop] = (Sparse, rk.property)
            else
                [vtype, prop] = (Double, rk.property)
            end
        elseif rk.property==Sparse
            if vtype==Boolean               // Boolean < Unknown < SparseBool
                [sparseRes, prop] = (%t, Sparse)
            elseif vtype==Double
                [sparseRes, vtype] = (%t, Sparse)
            end
        end
        rowsize = [rowsize rk.dims(1)]
        colsize = [colsize rk.dims(2)]
    end

    // Computing the size of the result
    // --------------------------------
    undef = find(rowsize==-1)
    void = find(rowsize==0)
    rowsize([undef void]) = []
    if rowsize==[] then
        if undef<>[] then
            sr = -1
        else
            sr = 0
        end
    else
        [w,k] = min(length(rowsize))
        sr = rowsize(k)
    end

    if vtype<>String then
        if find(colsize==-1)==[] then
            w = colsize(1)
            for k = 2:colnb
                w = w+colsize(k)
            end
            sc = w
        else
            sc = -1
        end

        tree.out(1).dims = list(sr,sc)
        tree.out(1).type = Type(vtype,prop)
    else
        if find(colsize==-1)==[] then
            w = colsize(1)
            for k = 2:colnb
                w = w+colsize(k)
            end
            sc = w
        else
            sc = -1
        end

        tree.out(1).dims = list(sr,sc)
        tree.out(1).type = Type(String, prop)
        tree = Operation("+",tree.operands,tree.out)
    end
    tree.out(1).contents = Contents()
endfunction
