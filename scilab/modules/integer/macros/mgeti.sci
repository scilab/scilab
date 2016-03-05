// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x = mgeti(varargin)

    rhs=argn(2);
    if rhs == 0 | rhs > 3 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mgeti", 1, 3));
    end

    if rhs == 1 then
        typeToConvert = 4;
    else
        type = varargin(2);

        if typeof(type) <> "string" then
            error(msprintf(_("%s: Wrong type for argument #%d: string expected.\n"), "mgeti", 2));
        end

        if size(type, '*') <> 1 then
            error(msprintf(_("%s: Wrong size for argument #%d: A single string expected.\n"), "mgeti", 2));
        end

        typeToConvert = 0;
        len = length(type);
        partPos = 1;

        if len == 0 | len > 3 then
            error(msprintf(_("%s: Wrong size for argument #%d: A single string of size %d to %d expected.\n"), "mgeti", 2, 1, 3));
        end

        // unsigne
        if part(type, partPos) == 'u'
            typeToConvert = 10;
            partPos = 2;
        end

        // type to convert
        select part(type, partPos)
        case 'l'
            typeToConvert = typeToConvert + 8;
        case 'i'
            typeToConvert = typeToConvert + 4;
        case 's'
            typeToConvert = typeToConvert + 2;
        case 'c'
            typeToConvert = typeToConvert + 1;
        else
            error(msprintf(_("%s: Incorrect integer type: %s .\n"), "mgeti", part(type, partPos)));
        end
    end

    x = iconvert(mget(varargin(:)), typeToConvert);
endfunction
