// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have
// received along with this program.

function [sorted, indin] = %s_gsort(array, method, sortdir, criteria)
    // Sorting complex numbers.
    // This overload sorts only dense matrices.
    // For hypermatrices, %hm_gsort() is called upstream
    // For sparse matrices, %sp_gsort() is called upstream.
    //
    // method  : "g" "r" "c" "lr" "lc"
    // sortdir : [], or 1 or 2 elements in ["i" "d"]. Default = "d"
    // criteria: list() of functions handles: real, imag, abs, atan

    // DEMO
    // ----
    if argn(2)==0
        i = grand(10,3,"uin",-1,1);
        r = grand(10,3,"uin",-1,1);
        c = r+i*%i
        [cs, k] = %s_gsort(c, "g", ["d" "i"], list(abs, atan));
        a = atand(imag(cs),real(cs));
        disp(cs, abs(cs) + a*%i)
        sorted = []
        indin = []
        return
    end

    // CHECKING INPUT PARAMETERS
    // -------------------------
    // array:
    // This overload is called only when array is defined and are complex numbers

    // DEFAULT VALUES
    // method: checked in the gateway (and initialized when default)
    if ~isdef("method", "l") | method==[] | (type(method)==10 & method(1)=="")
        method = "g"
    end

    // sortdir: checked in %gsort_multilevel. Only setting the default:
    if ~isdef("sortdir", "l") | sortdir==[] | (type(sortdir)==10 & sortdir(1)=="")
        sortdir = "d"           // for back-compatibility
    end
    //criteria:  checked in %gsort_multilevel. Only setting the default:
    if ~isdef("criteria", "l") | (type(criteria)==1 & criteria==[])
        if size(sortdir,"*")==2
            criteria = list(abs, atan)
        else
            criteria = list(abs)
        end
    end

    // PROCESSING
    // ----------
    // Indices are computed anyway.
   [sorted, indin] = %gsort_multilevel(array, method, sortdir, criteria)

   if ~isreal(array) & isreal(sorted) then
       sorted = complex(sorted, 0)
   end
endfunction
