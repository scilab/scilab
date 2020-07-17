// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x, ki, ko, nb] = unique(x, varargin)
    // extract unique components of a vector, matrix, or hypermatrix
    // varargin : orient=1|2|"r"|"c", "uniqueNan", "keepOrder"
    //
    // History:
    // * 2019 - S. Gougeon :
    //   - add uniqueNan option: http://bugzilla.scilab.org/15522
    //   - add keepOrder option: http://bugzilla.scilab.org/15795
    //   - add nb output option: http://bugzilla.scilab.org/8418
    //
    // * 2020 - S. Gougeon :
    //   - Complex numbers are now completely sorted, by magnitude, + by phase
    //   - add ku output indices: http://bugzilla.scilab.org/16337
    //   - Sparse 2D matrices accepted: http://bugzilla.scilab.org/15842

    keepOrder = %f
    uniqueNan = %f
    orient = "*"
    newInf = [] // init Inf substitute in case of "uniqueNan" and or(x==%inf)
    ki = []
    ko = []
    nb = []
    sx = size(x)

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    in = varargin
    i = 2;  // index of the current input argument
    if size(in)>0 then
        a = in(1)
        if type(a)==0
            a = "*"
        end
        if typeof(a)=="string"
            a = convstr(a)
        end
        select a
        case 1
            orient = "r"
        case 2
            orient = "c"
        case "uniquenan"
            uniqueNan = %t
        case "keeporder"
            keepOrder = %t
        case "r"
            orient = "r"
        case "c"
            orient = "c"
        else
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "unique", i, "1,2,""r"",""c"",""keepOrder"",""uniqueNan"""))
        end
        in(1) = null()
        i = 3
    end
    if or(orient==["r" "c"]) & ndims(x)>2 then
        msg = _("%s: Argument #%d: ''%s'' not allowed for an hypermatrix.\n")
        error(msprintf(msg, "unique", 2, orient))
    end

    while size(in)>0 & i<5 then
        a = in(1)
        if typeof(a)=="string"
            a = convstr(a)
        end
        select a
        case "uniquenan"
            uniqueNan = %t
        case "keeporder"
            keepOrder = %t
        else
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "unique", i, """keepOrder"",""uniqueNan"""))
        end
        in(1) = null()
        i = i+1
    end
    uniqueNan = uniqueNan & or(type(x)==[1 5])

    sz = size(x);
    if size(x, orient)==1 then
        ki = 1
        ko = 1
        nb = 1
        return
    end
    if uniqueNan
        [x, newInf] = uniqueProcessNan(x, [], "removeNan")
    end
    getK = argn(1) > 1 | keepOrder


    // [] trivial case
    // ---------------
    if isempty(x) then
        return  // ki, ko, nb are already []. x is [] or sparse([])
    end

    // PROCESSING
    // ----------
    areComplex = or(type(x)==[1 5]) && ~isreal(x, 0)
    if orient=="*" then
        if getK then
            // ki (begin)
            if areComplex
                [x,ki] = gsort(x,"g",["i" "i"], list(abs, atan));
            else
                [x,ki] = gsort(x,"g","i");
            end
            keq = x(2:$) == x(1:$-1);
            if argn(1) > 2
                // ko (begin)
                ko(ki) = cumsum(~[%F ; keq(:)])
                if ko <> [] then
                    ko = matrix(ko, sx)
                end
                // nb
                if argn(1) > 3
                    nb = [0 find(~keq) size(x,"*")]
                    nb = nb(2:$) - nb(1:$-1)
                end
            end
            // ki (end)
            keq = find(keq);
            if keq <> [] then keq = keq + 1; end
            x(keq) = [];
            ki(keq) = [];
        else
            if areComplex
                x = gsort(x,"g",["d" "d"], list(abs, atan));
            else
                x = gsort(x,"g","d");
            end
            x = x($:-1:1);
            x( find(x(2:$) == x(1:$-1)) ) = [];
        end
        if sx(1) > 1 | length(sx) > 2
            x = x(:)
            ki = ki(:)
            nb = nb(:)
        end

    else // orient used
        if  orient==2 | orient=="c" then
            x = x.'
        end
        if getK then
            if areComplex
                [x,ki] = gsort(x,"lr",["i" "i"], list(abs, atan));
            else
                [x,ki] = gsort(x,"lr","i");
            end
            keq = and(x(2:$,:) == x(1:$-1,:),"c")
            if argn(1)>2
                ko(ki) = cumsum(~[%F ; keq(:)])
                // nb
                if argn(1) > 3
                    nb = [0 find(~keq) size(x,1)]
                    nb = nb(2:$) - nb(1:$-1)
                    nb = nb(:)
                end
            end
            keq = find(keq)
            if keq <> [] then keq = keq + 1;end
            x(keq,:) = [];
            ki(keq,:) = [];
        else
            if areComplex
                x = gsort(x,"lr",["i" "i"], list(abs, atan));
            else
                x = gsort(x,"lr","i");
            end
            x(find(and(x(2:$,:) == x(1:$-1,:),"c")),:) = [];
        end
        if  orient==2 | orient=="c" then
            x = x.'
            ki = matrix(ki, 1, -1)
            ko = ko'
            nb = nb'
        end
    end
    ko = full(ko)

    if uniqueNan
        x = uniqueProcessNan(x, newInf, "restoreNan")
    end

    if keepOrder
        [ki, kk] = gsort(ki,"g","i")
        select orient
        case "*"
            x = x(kk)
        case "r"
            x = x(kk,:)
        case "c"
            x = x(:,kk)
        end
        if argn(1)>2
            nb = nb(kk)
            [?, kk2] = gsort(kk,"g","i")
            ko = kk2(ko)
        end
    end
endfunction

// -------------------------------------------------------------------

// To consider Nan mutually equal, we replace all of them with a "regular" substitute.
// Since Nan are sorted as > Inf, we must use anyway Inf as the Nan substitute.
// If the original array have already some Inf, we must priorly replace them with
// a decimal greater than the finite maximum of the array values.
// After processing, we restore Inf => Nan (, and maxNum => Inf).

function [x, newInf] = uniqueProcessNan(x, newInf, way)

    if way=="removeNan" & or(isnan(x)) then
        // Replacing Nan
        // -------------
        if isreal(x)
            if or(x==%inf)
                b = x==%inf
                m = max([1 max(x(~b))])
                newInf = m*1.5
                x(b) = newInf
            end
            x(x<>x) = %inf
        else
            r = real(x)
            i = imag(x)
            if or([r i]==%inf)
                br = r==%inf
                m = max(r(~br),1)
                bi = i==%inf
                m = max(i(~bi),m,1)
                newInf = m*1.5
                r(br) = newInf
                i(bi) = newInf
            end
            r(r<>r) = %inf
            i(i<>i) = %inf
            x = r + imult(i);
        end

    // Restoring  NaN
    // --------------
    elseif way=="restoreNan"
        if isreal(x)
            x(x==%inf) = %nan
            if newInf~=[]
                x(x==newInf) = %inf
            end
        else
            r = real(x)
            r(r==%inf) = %nan
            i = imag(x)
            i(i==%inf) = %nan
            if newInf~=[]
                r(r==newInf) = %inf
                i(i==newInf) = %inf
            end
            x = r + imult(i)
        end
    end
endfunction
