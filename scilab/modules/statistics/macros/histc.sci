// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - A. Khorshidi
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [cf, ind] = histc(n, data, normalization)

    // Example:
    //     n = 10;
    //     data = rand(1, 1000, "normal");
    //     [class_freq, ind] = histc(n, data)

    rhs = argn(2);

    if rhs < 2
        error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"), "histc", 2));
    end

    if rhs == 3 & exists("normalization", "local") then
        if type(normalization) ~= 4 | size(normalization, "*") ~= 1 then
            error(msprintf(_("%s: Wrong type of input argument #%d: A single boolean expected.\n"), "histc", 3));
        end
    else
        // By default, normalize the result
        normalization = %t
    end

    if type(n) ~= 1 & type(n) ~= 8
        error(msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"histc",1));
    end
    if type(n) == 8 then
        n = iconvert(n, 0); // To pass to dsearch
    end

    if type(data) ~= 1 & type(data) ~= 8
        error(msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"histc",2))
    end
    if type(data) == 8 then
        data = iconvert(data, 0); // To pass to dsearch
    end

    if length(n) == 1 then  // The number of classes is provided
        if n < 1
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"),"histc",1,"[1, oo)"));
        elseif n ~= floor(n)
            error(msprintf(_("%s: Wrong type for input argument #%d: Integer expected.\n"),"histc",1))
        end
        mind = min(data);
        maxd = max(data);
        if (mind == maxd) then
            mind = mind - floor(n/2);
            maxd = maxd + ceil(n/2);
        end
        cb = linspace(mind, maxd, n+1); // Class boundary
    else // The classes are provided
        cb = matrix(n, 1, -1);   // Force row form
        if min(diff(cb)) <= 0 then
            error(msprintf(_("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"),"histc",1))
        end
    end

    [ind, cf, info] = dsearch(data, cb); // cf: class frequency, ind: class number of each count

    // Normalization
    if normalization == %t then
        nd = size(data, "*"); // Number of data values
        //cw = cb(2:$)-cb(1:$-1); // Bin width
        //cf = cf./(nd*cw); // Normalization in bin heights
        cf = cf./nd; // Heights normalization
    end

endfunction
