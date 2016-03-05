// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) 2010 - Samuel Gougeon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function whos(%_opt,%_sel)

    // get the variables local & global
    [%_nams, %_vol] = who("local");
    [%_nams_global, %_vol_global] = who("global");

    // Gathering locals + globals
    %_nams_all = [%_nams ; %_nams_global];
    %_vol_all  = [%_vol ; %_vol_global];
    %_LG_all   = [emptystr(size(%_vol,1),1)+"L" ; ..
    emptystr(size(%_vol_global,1),1)+"G"];

    // Sorting the whole set w/o.r.t. the case
    [trash, k] = gsort(convstr(%_nams_all,"l"),"g","i");
    %_nams_all = %_nams_all(k);
    %_vol_all  = %_vol_all(k);
    %_LG_all   = %_LG_all(k);

    //=============================================================================
    // getSizeAsString, writeWhosLine, doDisplayWhosLine defined only in whos
    // to remove some tmp variables defined on stack
    //=============================================================================
    function sz = getSizeAsString(_varname, _type)
        sz = " ";
        if _type == 0 then
            sz = "?";
        elseif _type <= 10 then
            execstr("sz = size(" + _varname + ");")
        elseif _type == 17 | _type == 16 then
            execstr("%_tt = getfield(1," + _varname + ")");
            %_tt = %_tt(1);
            if execstr("sz = %" + %_tt + "size(" + _varname + ")", "errcatch") <> 0 then
                sz = "?";
            end
        end
    endfunction

    //============================================================================

    function writeWhosLine(_name, _namedtype, _sz, _bytes)
        mprintf("%s\n", part(_name, 1:25) + part(_namedtype,1:15) + ..
        part(strcat(string(_sz), _(" by ")), 1:15) + ..
        part(string(_bytes), 1:13));
    endfunction

    //============================================================================

    function bOK = doDisplayWhosLine(_rhs, _opt, _name, _namedtype, _sel)
        bOK = %t;
        if _rhs == 2 then
            if _opt == "-type" ..
                & part(_namedtype, 1:length(_sel)) <> _sel then
                bOK = %f;
            elseif _opt == "-name" ..
                & part(convstr(_name,"l"), 1:length(_sel)) <> convstr(_sel,"l") then
                bOK = %f;
            end
        end
    endfunction

    //============================================================================

    // display defined variable in a long form

    [%_lhs, %_rhs] = argn();

    if %_rhs == 0 then
        %_opt = [];
        %_sel = [];
    end

    if %_rhs == 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "whos", 0, 2));
    end

    if %_rhs == 2 then
        %_nams($-1:$) = [];
        if %_opt <> "-type" & %_opt <> "-name" then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "whos", 1, "-name", "-type"));
        end
    end

    //write the display header
    mprintf("%s\n", part(_("Name"),1:25) + ..
    part(_("Type"),1:15) + ..
    part(_("Size"),1:15) + ..
    part(_("Bytes"),1:13));

    mprintf("%s\n"," ");

    // loop on all variables
    for %_k = 1:size(%_nams_all, 1)
        %_sz   = " ";
        %_typn = "";
        %_typ  = 0;
        b_ok   = %f;

        if ~isdef(%_nams_all(%_k))
            execstr("global " + %_nams_all(%_k));
        end
        execstr("%_typn = typeof(" + %_nams_all(%_k) + ")");
        if execstr("%_typ = type(" + %_nams_all(%_k) + ")","errcatch") == 228 then
            %_typn = "*deleted";
        end

        b_ok = doDisplayWhosLine(%_rhs, %_opt, %_nams_all(%_k), %_typn, %_sel);

        if b_ok then
            %_sz = getSizeAsString(%_nams_all(%_k), %_typ);
            if %_LG_all(%_k) == "G"
                %_typn = %_typn + "*";
            end
            writeWhosLine(%_nams_all(%_k), %_typn, %_sz, %_vol_all(%_k)*8);
        end
        if  %_LG_all(%_k) == "G" then
            execstr("clear " + %_nams_all(%_k));
        end
    end

endfunction
