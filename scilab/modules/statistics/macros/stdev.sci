// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enteprises - Paul Bignier: added mean squared deviation
//                                                        (third input argument)
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function sd = stdev(x, o, m)
    //
    //This function computes  the  standard deviation  of  the values  of  a
    //vector or matrix x.
    //
    //For a vector or a  matrix x, s=stdev(x)  returns in  the scalar s  the
    //standard deviation of all the entries of x.
    //
    //s=stdev(x,'r')  (or,  equivalently,   s=stdev(x,1))   is the   rowwise
    //standard deviation. It returns  in each entry of the  row vector s the
    //standard deviation of each column of x.
    //
    //s=stdev(x,'c')  (or,  equivalently, s=stdev(x,2))  is   the columnwise
    //standard  deviation. It returns in each  entry of the  column vector y
    //the standard deviation of each row of x.
    //
    //The input argument m represents the a priori mean. If it is present, then the sum is
    //divided by size(x,"*"). Otherwise ("sample standard deviation"), it is divided by size(x,"*")-1.
    //

    [lhs, rhs] = argn(0);

    if rhs < 1 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "stdev", 1, 3))
    end

    if rhs < 2 then
        o = "*"
        on = 0
    else
        select o
        case "*" then
            on = 0
        case "r" then
            on = 1
        case "c" then
            on = 2
        else
            if type(o) <> 1 || size(o, "*") <> 1 || floor(o) <> o | o < 1 | o > ndims(x) then
                msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
                sset =  """*"" ""r"" ""c"" 1 2"
                if ndims(x)>2
                    sset = sset + strcat(msprintf(" %d\n",(3:ndims(x))'))
                end
                error(msprintf(msg, "stdev", 2, sset))
            else
                on = o
            end
        end
    end

    if (length(size(x)) > 2) then
        if rhs == 3 then
            sd = %hm_stdev(x, on, m);
        else
            sd = %hm_stdev(x, on);
        end
        return
    end

    if type(x) ~= 1 | ~isreal(x) then
        tmp = _("%s: Wrong type for input argument #%d: A real matrix expected.\n")
        error(msprintf(tmp, "stdev", 1))
    end

    siz = size(x);
    if rhs == 3 then
        if typeof(m) ~= "constant" | ~isreal(m) then
            tmp = _("%s: Wrong type for input argument #%d: A real matrix expected.\n")
            error(msprintf(tmp, "stdev", 3))
        elseif on == 0 then
            if ~isscalar(m) then
                tmp = _("%s: Wrong size for input argument #%d.\n")
                error(msprintf(tmp, "stdev", 3))
            end
        elseif on == 1 then
            if or(size(m) ~= [1 siz(2:$)]) then
                if isscalar(m) then
                    m = resize_matrix(m, [1 siz(2:$)], m);
                else
                    tmp = _("%s: Wrong size for input argument #%d.\n")
                    error(msprintf(tmp, "stdev", 3))
                end
            end
        elseif on == 2 then
            if or(size(m) ~= [siz(1) 1 siz(3:$)]) then
                if isscalar(m) then
                    m = resize_matrix(m, [siz(1) 1 siz(3:$)], m);
                else
                    tmp = _("%s: Wrong size for input argument #%d.\n")
                    error(msprintf(tmp, "stdev", 3))
                end
            end
        end
    end

    if x == [] then
        sd = %nan;
        return
    end

    if rhs == 3 & isnan(m) then
        // This will compute the "biased variance": the denominator will be size(x,orien)
        // but the a priori mean is not considered as provided.
        rhs = 2
    end

    // Remove the mean
    if on == 0 then
        if rhs == 3
            y = x - m;
        else
            y = x - mean(x);
        end
    elseif on == 2 then
        if rhs == 3
            y = x - m*ones(x(1,:));
        else
            y = x - mean(x,on)*ones(x(1,:));
        end
    elseif on == 1 then
        if rhs == 3
            y = x - ones(x(:,1))*m;
        else
            y = x - ones(x(:,1))*mean(x,1);
        end
    else // on > ndims(x) then
        sd = 0*x;
        return
    end

    mn = size(x, o);

    if mn == 1 then
        sd = 0*y;
    else
        if rhs <= 2 & exists("m", "local") == 0 then // If m is provided but rhs=2, that means we want the biased deviation
            sd = sqrt(sum(y.^2,o)/(mn-1));
        else
            sd = sqrt(sum(y.^2,o)/mn);
        end
    end

endfunction
