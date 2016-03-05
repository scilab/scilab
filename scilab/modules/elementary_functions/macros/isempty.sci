// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998 - INRIA
// Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function answ = isempty(m)

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "isempty", 1));
    end

    m_type = type(m);

    if( (m_type >= 11) & (m_type <= 13) | (m_type >= 128) ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"), "isempty", 1));
    end

    select m_type
    case 1
        answ = m == [];
    case 10
        // matrix of character string
        answ = ( max(length(m)) == 0 );

    case 15
        // list
        answ = %t;
        for i=1:size(m),
            clear __element__;
            __element__ = m(i);
            if isdef("__element__") then
                r = isempty(m(i));
            else
                r = %F;
            end
            answ = answ & r;
        end;

    case 16
        // typed list
        if typeof(m) == "rational" then
            answ = size(m, "*") == 0;
        else
            answ = %t;
            for i=2:size(m),
                clear __element__;
                __element__ = m(i);
                if isdef("__element__") then
                    r = isempty(m(i));
                else
                    r = %F;
                end
                answ = answ & r;
            end;
        end

    case 17
        // mlist
        answ = %f;

    else
        answ = size(m,"*")==0;
    end

endfunction
