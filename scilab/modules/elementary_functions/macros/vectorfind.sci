// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [ind, matching] = vectorfind(m, v, way, joker, indType)
    // way: "r" or 1, "c" or 2, 2 < i < ndims(m)
    // indType: "" (default), "headIJK", "headN"

    rhs = argn(2)

    // Check number of inputs, at least 2 needed
    if rhs < 2 | rhs > 5 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "vectorfind", 2, 5))
    end

    if ~isdef("joker", "l") | type(joker) == 0 then
        joker = []
    end

    // Types of m and v must be compatible:
    ok = (type(m) == type(v)) | ..
    (or(type(m) == [1 5 8]) & or(type(v) == [1 5 8])) | ..
    (type(m) == 4 & joker ~=[] & or(type(v) == [1 5 8]));

    if ~ok then
        msg = _("%s: Incompatible input arguments #%d and #%d: Same type expected.\n");
        error(msprintf(msg, "vectorfind", 1, 2))
    end

    // Checking the needle:
    if v == [] then
        ind = []
        return
    end
    if ~isvector(v) & ~isscalar(v) then
        msg = _("%s: Wrong size for input argument #%d: Vector expected.\n")
        error(msprintf(msg, "vectorfind", 2))
    else
        v = v(:);
    end

    Ndim = ndims(m);
    // Checking the direction:
    if ~isdef("way", "l") | type(way) == 0 then
        way = 1         // Default direction
    elseif size(way, "*") > 1 then
        msg = _("%s: Argument #%d: Scalar (1 element) expected.\n");
        error(msprintf(msg, "vectorfind", 3));
    elseif and(type(way) ~= [1 10]) then
        msg = _("%s: Argument #%d: Text or integer decimal expected.\n");
        error(msprintf(msg, "vectorfind", 3));
    elseif and(way ~= ["r" "c"]) & and(way ~= (1:Ndim)) then
        msg = _("%s: Argument #%d: Must be in the set {%s}.\n");
        tmp = msprintf("""r"",""c"",1,..,%d", Ndim);
        error(msprintf(msg, "vectorfind",3, tmp))
    else
        if way == "r" then
            way = 1
        elseif way == "c" then
            way = 2
        end
    end

    // Checking the joker
    if joker ~= [] & size(joker,"*") ~= 1 then
        msg = _("%s: Wrong size for input argument #%d: Scalar expected.\n")
        error(msprintf(msg, "vectorfind",4))
    end

    if joker ~= [] & ((or(type(joker) == [1 5 8]) & ~or(type(v) == [1 5 8])) | ..
        (or(type(joker) == [2 10]) & type(joker) ~= type(v)))
        msg = _("%s: Incompatible input arguments #%d and #%d: Same type expected.\n")
        error(msprintf(msg, "vectorfind", 2, 4))
    end
    if type(m) == 4 & joker ~= [] & joker == 0 then
        msg = _("%s: Argument #%d: non-zero number expected.\n")
        error(msprintf(msg, "vectorfind", 4))
    end
    // Checkinh indType
    if ~isdef("indType", "l") | type(indType) == 0 then
        indType = ""
    elseif type(indType) ~= 10 then
        msg = _("%s: Argument #%d: Text expected.\n");
        error(msprintf(msg, "vectorfind", 5));
    elseif size(indType, "*") > 1 then
        msg = _("%s: Argument #%d: Scalar (1 element) expected.\n");
        error(msprintf(msg, "vectorfind", 5));
    else
        indType = convstr(indType);
        if ~or(indType == ["" "headijk" "headn"]) then
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n");
            error(msprintf(msg, "vectorfind", 5,""""", ""headIJK"", ""headN"""));
        end
    end

    sm = size(m)
    sv = size(v,"*")
    matching = [];

    // Normalizing m and v shapes: setting the direction along COLUMNS
    if Ndim == 2 then
        if way == 1 then
            m = m.';
        end
    else    // Ndim>2
        i = 1:Ndim;
        if way == 1 then
            i([1 2]) = [2 1];
            m = permute(m,i);
            m = matrix(m, sm(2), -1);
        elseif way == 2 then
            m = matrix(m, sm(1), -1)
        elseif way==Ndim
            m = matrix(m, -1, sm(way)).';
        else
            m = matrix(m, prod(sm(1:way-1)), sm(way), -1);
            m = permute(m, [2 1 3]);
            m = matrix(m, sm(way), -1);
        end
    end
    // flag for partial v
    partial = sv < size(m, 1)

    //
    if sv > size(m,1)
        ind = []
        return

    elseif partial // v shorter than m size
        cs = size(m,1) - sv
        k = ndgrid(1:sv, 1:(cs+1)) + ones(sv,1)*(0:cs)
        M = matrix(m(k(:),:), sv, -1)
        ind = vectorfind(M, v, "c", joker)
        if joker~=[]
            matching = M(:,ind).';
        end
        clear M
        // We compute k = index of start of matching inside the full range
        if ind ~= [] then
            k = modulo(ind, cs + 1);
            k(k == 0) = cs + 1;
        else
            k = []
        end
        ind = ceil(ind / (cs + 1));  // linearized indices of full vectors

    else
        // Performing the detection:
        // ------------------------
        // Selecting only needle's components not being the joker
        if joker ~= [] then
            if type(v) == 1 & isnan(joker) then
                c = find(~isnan(v))
            else
                c = find(v ~= joker)
            end
        else
            c = 1:size(v, "*")
        end
        if type(m) == 4 & or(type(v) == [1 5 8]) then
            v = (v ~= 0);
        end

        ind = 1:size(m, 2);
        for k = c   // Loop over needle components not being jokers
            if isnan(v(k)) then
                i = find(isnan(m(k, ind)));
            else
                i = find(m(k, ind) == v(k));
            end
            ind = ind(i);
            if ind == [] then
                break
            end
        end
        if joker ~= [] then
            matching = m(:,ind).';
        end
    end

    // Post-processing output indices
    // ------------------------------
    if indType ~= "" | partial then
        if way == 1 then
            way = 2
        elseif way == 2 then
            way = 1
        end
        i = 1:Ndim;
        i(way) = [];
        I = ind2sub(sm(i), ind);
        I2 = [];
        if way>1
            I2 = I(:, 1:way-1);
        end
        if ~partial then
            I2 = [I2 ones(I(:,1))];
        else
            I2 = [I2 k(:)];
        end
        if way < Ndim then
            I2 = [I2 I(:,way:$)];
        end
        if indType == "headijk" then
            ind = I2;
            if partial then
                [tmp, k] = gsort(ind(:,$:-1:1), "lr", "i");
                ind = ind(k,:);
                // otherwise: it should already be sorted
            end
        elseif indType=="headn" | partial then
            ind = gsort(sub2ind(sm,I2)', "g", "i");
        end
    end
endfunction
