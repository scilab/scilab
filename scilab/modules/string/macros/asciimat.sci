// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - 2021 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = asciimat(x)
    // This function converts a matrix of strings into a matrix of ascii codes
    // using ascii() Scilab function,
    // and converts an array of ascii codes into a array of string
    // Returned value have same size as input value instead of second dims !
    // Fonction created because ascii() Scilab function returns a row vector

    y = []
    if x==[] then
        return
    end

    if typeof(x) == "string" // convert string to ascii code
        dims = size(x)
        x = strcat(matrix(permute(x,[2 1 3:length(dims)]), dims(2), -1),"","r")'
        tmp = ascii(1:127);
        for c = ["\" "/" "]"]
            tmp = strsubst(tmp, c, "\"+c);
        end
        kUTF = grep(x, "/[^" + tmp + "]/", "regexp")
        if kUTF == [] then
            L = length(x)
            m = max(L)
            if or(length(x)<>m) then
                // Padding with spaces (like Octave, unlike Matlab)
                x = part(x(:),1:m)
            end
            y = matrix(ascii(x)', [length(x(1)), dims(1), dims(3:$)])
            y = permute(y, [2 1 3:length(dims)])
        else
            ext = []
            for k = kUTF
                a = ascii(x(k));
                ext($+1,1:length(a)) = a;
            end
            ext(ext==0) = 32;    // Padding with spaces

            noUTF = x(:)
            noUTF(kUTF) = ""
            m = max(size(ext,2), max(length(noUTF)))
            noUTF = part(noUTF, 1:m)
            y = matrix(ascii(noUTF),m,-1)'
            if size(ext,2)<m
                ext(1,m) = 32 // extends a to m columns
            end
            for i = 1:length(kUTF)
                y(kUTF(i),:) = ext(i,:) ;
            end
            y = permute(matrix(y',[m dims(1) dims(3:$)]), [2 1 3:length(dims)])
        end

    else  // convert ascii codes to string
        dims = size(x)
        x = permute(x, [2 1 3:ndims(x)]);
        x = matrix(x,size(x,1),-1);
        // Tags the EOL:
        for c = ascii("$€£")
            x($+1,:) = c;
        end
        //
        x = matrix(x,1,-1)
        // Any zero will block ascii(): http://bugzilla.scilab.org/15101
        x(x==0) = []  // 0 <=> ""
        x = [x, 65]   // works around http://bugzilla.scilab.org/16686
        y = strsplit(ascii(x), "$€£");
        y($) = []     // removes the ending ascii(65)
        y = matrix(y, [dims(1) 1 dims(3:$)]); // dims#2 not squeezed (Matlab, Octave)
        y = stripblanks(y,%f,1) // Matlab & Octave trim trailing ascii(32)(not \t)
    end
endfunction
