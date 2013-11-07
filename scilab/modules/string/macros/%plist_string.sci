// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// %plist_string --
//   Returns the string containing the parameters list.
//
function str = %plist_string ( this )

    // In case there is a function field
    prot = funcprot()
    funcprot(0)

    fieldmat = getfield(1, this);
    nf = size(fieldmat, "*") - 1;
    str = emptystr(nf+3, 1); // Allocating the result
    str(1) = sprintf("Parameters list:"); // Headers
    str(2) = sprintf("================");
    str(3) = sprintf("Number of fields = %d", nf);

    k = 3; // Start printing after headers
    for i = 1 : nf
        key = fieldmat(i+1);
        value = getfield(i+1, this);
        if ( or ( typeof(value) == ["constant" "boolean" "string" ] ) ) then
            k = k + 1;
            if ( size(value, "*") == 1 ) then
                str(k) = sprintf("%s = %s (""%s"")", key , string(value) , typeof(value) );
            else
                siz = size(value);
                str(k) = sprintf("%s = ""%s"" %dx%d ", key , typeof(value) , siz(1), siz(2) );
            end
        else
            k = k + 1;
            str(k) = sprintf("%s = ""%s""", key , typeof(value) );
        end
    end

    funcprot(prot)

endfunction
