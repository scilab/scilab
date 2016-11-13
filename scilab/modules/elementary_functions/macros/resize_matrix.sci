// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON : resize_matrix rewritten.
//                                       Hypermatrix, polynomials and
//                                       custom padding are now supported.
//                                       Inline examples added.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function mat = resize_matrix(mat, varargin)

    // Syntax:
    //     resize_matrix(M, nRows, nCols )
    //     resize_matrix(M, [nRows nCols] )
    //     resize_matrix(M, newSizes )
    //     resize_matrix(M, .. , resType )
    //     resize_matrix(M, .. , resType, padding )
    //     resize_matrix(M, .. ,   ""   , padding )

    [lhs, rhs] = argn(0)

    // EXAMPLES
    // --------
    if rhs == 0 then
        M = grand(4, 3, "uin", -9, 9)
        disp(_("RESIZING a matrix of DECIMALS:"))
        disp("M = ")
        disp(M)
        com = [ "resize_matrix(M, 3, 4)"
        "resize_matrix(M, [3 4 2])"
        "resize_matrix(M, [3 4 2], """", %i)"
        "resize_matrix(M, [3 4 2], ""string"", %i)"
        ]
        for c = com'
            disp(c)
            execstr("disp("+c+")")
        end

        halt(ascii(10)+_("Type <Enter> to see an example with polynomials:"))
        x = poly(0, "x")
        P = (1-x)^grand(4, 2, "uin", 0, 3)
        disp("P = ")
        disp(P)
        com = [ "resize_matrix(P, 3, 3)"
        "resize_matrix(P, [3 3 2])"
        "resize_matrix(P, [3 3 2],"""", %z)"
        ]
        for c = com'
            disp(c)
            execstr("disp("+c+")")
        end
        disp(_("The unknown variable of added values is forced to the P''s one"))
        disp(_("Polynomials can''t be converted"))

        halt(ascii(10)+_("Type <Enter> to see an example with character strings:"))
        T = string(grand(4, 3, 2, "unf", 0, 1)*100)
        disp("T = ")
        disp(T)
        com = [ "resize_matrix(T, 2, 5)"
        "resize_matrix(T, [2 5], ""constant"")"
        "resize_matrix(T, [2 5], """", ""abc"")"
        "resize_matrix(T, [2 5], ""int16"", ""-1e4"")"
        ]
        for c = com'
            disp(c)
            execstr("disp("+c+")")
        end
        warning(msprintf(_("Hypermatrices of character strings can''t be converted.\n")))

        mat = []
        return
    end

    // INITIALIZATIONS
    // ---------------
    // if needed, default padding will be defined later:
    padding = []
    // Default type of the result:
    resType = typeof(mat)

    // ARGUMENTS ANALYSIS & CHECKING
    // -----------------------------
    if rhs==1 | rhs > 5 then
        msg = _("%s: Wrong number of input argument(s): %d to %d expected.\n")
        error(msprintf(msg, "resize_matrix", 2, 5))
    end

    if ~(or(type(mat)~=[15 16 17]) ..
        & or(type(mat(:))==[1 2 4 5 6 8 10]))
        msg = _("%s: Wrong type of input argument #%d: ""%s"" not supported.\n")
        error(msprintf(msg, "resize_matrix", 1, typeof(mat(:))))
    end

    arg = varargin(1)
    nextvarg = 2
    if ~isscalar(arg) then
        newsizes = arg
    else
        if rhs>2 then
            varg2 = varargin(2)
            if type(varg2)~=1 | ~isscalar(varg2) then
                msg = _("%s: Wrong input argument #%d: An integer value expected.\n")
                error(msprintf(msg, "resize_matrix", 2))
            end
            nextvarg = 3
        else
            varg2 = 1
        end
        newsizes = [ arg varg2 ]
    end

    if or(type(mat) == [15 16]) & size(size(newsizes), "*") <> 1  then
        msg = _("%s: Wrong sizes requested, cannot convert list to matrix.\n")
        error(msprintf(msg, "resize_matrix"))
    end


    if  size(varargin)>=nextvarg then
        // Type of output
        resType = varargin(nextvarg)
        if typeof(resType)~="string"
            resType = typeof(mat(:))
        elseif resType=="" then
            resType = typeof(mat(:))
            nextvarg = nextvarg + 1
        else
            nextvarg = nextvarg + 1
        end
        // Padding pattern
        if size(varargin)>=nextvarg then
            padding = varargin(nextvarg)
            err = %f
            tpad = type(padding)
            if tpad~=type(mat(:)) then  // TRYING to CONVERT the given PADDING
                if or(type(mat(:))==[1 5 8]) then    // numbers
                    if or(tpad==[4 6])      // boolean
                        padding = bool2s(tpad)
                    elseif tpad==10         // string
                        padding = strtod(padding)
                        err = isnan(padding)
                    elseif or(tpad==[2 9]) | tpad>10
                        err = %t
                    end
                elseif type(mat(:))==2  // polynomials
                    if or(tpad==[1 5 8]) then   // decimals or encoded integers
                        padding = double(padding)
                    elseif or(tpad==[4 6])      // booleans
                        padding = bool2s(padding)
                    else
                        err = %t
                    end
                elseif or(type(mat(:))==[4 6])  // booleans
                    if or(tpad==[1 5 8]) then       // numbers
                        padding = (padding~=0)
                    elseif tpad==10
                        padding = (padding~="")     // string
                    else
                        err = %t
                    end
                else
                    err = %t
                end
                if err then
                    msg = _("%s: Wrong type for input argument #%d: cannot convert the padding value to the input matrix type.\n")
                    error(msprintf(msg, "resize_matrix", nextvarg+1))
                end
            else
                padding = varargin(nextvarg)
                if type(padding)==2 then
                    // the padding's unknown is forced to the mat's one
                    padding = varn(padding, varn(mat(1)))
                end
            end
        end
    end

    // FORMATTING SIZES VECTORS
    // ------------------------
    // if the vector of new sizes is shorter than ndims(mat), it is padded with ones
    oldsizes = size(mat)
    newsizes = int(real(newsizes(:)'))
    newsizes = [ newsizes  ones(1, length(oldsizes)-length(newsizes)) ]
    oldsizes = [ oldsizes  ones(1, length(newsizes)-length(oldsizes)) ]
    imax = length(newsizes)

    // TRIMMING
    // --------
    k = (newsizes>0 & newsizes<oldsizes)
    if or(k) then
        strcommand = ""
        for i=1:imax
            if k(i) then
                strcommand = strcommand + msprintf("1:%d",newsizes(i))
            else
                strcommand = strcommand + ":"
            end
            if i<imax then
                strcommand = strcommand + ","
            end
        end
        execstr("mat = mat("+strcommand+")")
    end

    // PADDING
    // -------
    k = (newsizes>0 & newsizes>oldsizes)
    if or(k) then
        if padding==[] then
            // Padding with default: needs just to set the upper bounds
            // setting defaults
            if or(type(mat(:))==[1 2 5 8]) // decimals, polynomials, decimal sparse, encoded integers
                padding = "0"
            elseif or(type(mat(:))==[4 6]) // booleans, boolean sparse
                padding = "%f"
            elseif type(mat(:))==10        // text
                padding = """"""
            end
            // padding:
            strcommand = ""
            for i=1:imax
                if k(i) then
                    strcommand = strcommand + msprintf("%d",newsizes(i))
                else
                    strcommand = strcommand + "1"
                end
                if i<imax then
                    strcommand = strcommand + ","
                end
            end
            execstr("mat("+strcommand+")="+padding)
        else    // Explicit padding
            Pad = padding
            padding = "Pad"
            for i=1:imax
                if k(i) then
                    s = zeros(1,imax)
                    s(i) = i
                    s = strcat(string(s),",")
                    s = strsubst(s,"0",":")
                    padRange = string(oldsizes(i)+1)+":"+string(newsizes(i))
                    s = strsubst(s, string(i), padRange)
                    execstr("mat("+s+") = "+padding)
                end
            end
        end
    end

    // CASTING : converting the type of the result
    // -------
    type_0 = typeof(mat(:))
    if resType~=type_0
        if type_0=="polynomial"
            msg = _("%s: conversion of polynomials is not supported\n")
            error(msprintf(msg, "resize_matrix"))
        elseif resType=="string"
            mat = string(mat)
        elseif or(resType==["int8" "int16" "int32" "uint8" "uint16" "uint32"])
            if type_0=="string"
                mat = strtod(mat)       // strings => decimals
            end
            // encoded integers, reals, complexes, booleans  => #int#
            if type(mat(:))==1 & ~isreal(mat)
                mat = abs(mat)  // taking the module of complexes
            end
            execstr("mat = "+resType+"(mat)")
        elseif resType=="constant"
            if type_0=="string" then
                mat = strtod(mat)
            else // encoded integers, booleans => decimals
                mat = double(mat)
            end
        elseif resType=="boolean"
            if type_0=="string" then
                mat = (mat~="")
            else
                mat = (mat~=0)
            end
        else
            msg = _("%s: conversion into ""%s"" is not supported\n")
            error(msprintf(msg, "resize_matrix", resType))
        end
    end
endfunction
