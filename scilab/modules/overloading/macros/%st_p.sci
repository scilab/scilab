// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %st_p(s)

    //matlab-like struct display

    ll    = lines()
    t     = []
    multi = size(s,"*")

    // No fields
    if isempty(fieldnames(s)) then
        t = strcat(string(size(s)), "x") + " struct array with no fields.";
        mprintf("%s", t);
        return
    end

    // 0x0 struct with fields
    if multi == 0 then
        t = "0x0 struct array with fields:"
        for field = fieldnames(s)'
            t = [t; "   "+field]
        end
        mprintf("%s", t);
        return
    end
    // axb struct where a<>0 & b<>0

    if multi > 1 then
        t = strcat(string(size(s)), "x") + " struct array with fields:";
    end

    for field = fieldnames(s)'
        value = s(field)
        tp = typeof(value)
        if tp == "st" then
            str = "[" + strcat(string(size(value)), "x") + " struct" + "]";
        elseif type(value)> 10 then
            str = tp
        else
            sz = size(value)
            // If number of elements in value is greater than ll(1) (current page width)
            // then we do not call sci2exp
            // because the returned value will be ignored at line 64: size(str,"*")==1
            // Note that ll(1)/2 elements could be the max because of colon, semi-colon and brackets added between elements by sci2exp
            if sz(1) == 1 & type(value) <> 9 & prod(sz) < ll(1) then // This line can avoid some memory issues when field contains a big matrix
                str = sci2exp(value, ll(1))
            else
                str = "[" + strcat(string(size(value)), "x") + " " + tp + "]"
            end
        end
        txt = "   " + field
        if multi <= 1 then
            if size(str,"*") == 1 then
                txt = txt + ": " + str
            else
                tp = typeof(value)
                txt = txt + ": " + "[" + strcat(string(size(value)), "x") + " " + tp + "]"
            end
        end
        t = [t; txt]
    end

    mprintf("%s\n", t);

endfunction
