// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2022 - Dassault Systèmes S.E. - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [header, c, nblines] = detectHeader(str)
    r = size(str, "r");
    header = [];
    c = [];
    nblines = [];

    // headers are always at the top, only check first 20 lines
    checkr = min(r, 20);
    s = part(str(1:checkr), 1);

    split = strsubst(s, "/[""a-zA-Z0-9\.,;:|\s\-\+]+/", "", "r");
    j = split <> "";
    if or(j) then
        nblines = find(j);
        header = str(nblines);
        c = emptystr(nblines)
        for i = nblines
            c(i) = part(str(i), strindex(str(i), split(i)))
        end
        c = unique(c)
        nblines = nblines';
    end

    // for i = 1:r
    //     split = strsubst(s(i), "/[""a-zA-Z0-9\.,;:|\s]+/", "", "r")
    //     if split <> "" then
    //         nblines = [nblines; i]
    //         header = [header; str(i)]
    //         c = [c; part(str(i), strindex(str(i), split))];
    //     end
    // end
    // c = unique(c)

endfunction
