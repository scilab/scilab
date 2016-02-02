// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=mtlb_format(_type,prec)
    // Emulation function for format() Matlab function

    rhs1=rhs
    if rhs==2 then
        fp=sci2exp("e")
        rhs=rhs-1
    else
        fp=sci2exp("v")
    end

    d=5;
    hex=%f;
    bank=%f;
    compact=%f;
    loose=%f,
    ratf=%f;
    symb=%f;
    unk=%f

    if rhs==1 then
        select _type
        case "''short''" then
            d=5
        case "''long''" then
            d=15
        case "''hex''" then
            hex=%t
        case "''+''" then
            symb=%t
        case "''bank''" then
            bank=%t
        case "''compact''" then
            compact=%t
        case "''loose''" then
            loose=%t
        case "''rat''" then
            ratf=%t
        else
            unk=%t
        end
    else
        d=5
    end


    if unk then
        error(msprintf(gettext("%s: Unknown type.\n"),"mtlb_format"))
    elseif ratf|loose|compact|bank|hex then
        warning(msprintf(gettext("%s: Unknown type ''%s'': IGNORED.\n"),"mtlb_format",_type));
    else
        if rhs1==1 then
            format(d+1)
        elseif rhs1==2 then
            format(fp,d+1)
        else
            format("v",6)
        end
    end
endfunction
