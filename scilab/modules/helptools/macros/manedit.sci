// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function manedit(manitem)

    lhs=argn(1);
    rhs=argn(2);

    if rhs<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"manedit",1));
    end

    // manitem : character string giving a manitem

    path=get_item_xml_path(manitem);

    if path==[] then
        ierr=execstr("t=type("+manitem+")","errcatch")
        if ierr==0&t==13 then
            path = pathconvert(TMPDIR+"/"+manitem+".xml",%F,%F);
            help_skeleton(manitem,TMPDIR);
        else
            path=[]
        end
    end

    if path <> [] then
        if (isdef("editor") | (funptr("editor")<>0)) then
            editor(path);
        end
    end

endfunction



function path = get_item_xml_path(manitem)

    path = [];

    global %helps
    global %helps_modules;
    if %helps_modules == [] then
        moduleslist = getmodules();
        for i = 1:size(moduleslist,"*")
            add_module_help_chapter(moduleslist(i));
        end
    end
    %HELPS=[%helps_modules;%helps];

    for k=1:size(%HELPS(:,1),"*")
        if isfile(pathconvert(%HELPS(k,1)+"/"+manitem+".xml",%f,%f)) then
            path = pathconvert(%HELPS(k,1)+"/"+manitem+".xml",%f,%f);
            return;
        end
    end

endfunction
