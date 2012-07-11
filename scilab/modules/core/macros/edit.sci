// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function edit(macroname,linenumber)
    // macroname : character string giving a macroname
    // linenumber : line number

    [lhs,rhs] = argn(0);
    if (rhs > 2) then
        error(sprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "edit", 1));
    end

    if (rhs >= 1 & type(macroname) ~= 10) then
        error(sprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"edit",1));
    end

    if (rhs == 2 & type(linenumber) ~= 1) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Double expected.\n"),"edit",2));
    end

    found = %f;
    tmp = %f;
    // tmpdir will have trailing / or \
    tmpdir= pathconvert(TMPDIR);

    if rhs >= 1 then // macroname or filename is given
        if regexp(macroname, "/^([a-zA-Z%_#!$?][0-9a-zA-Z_#!$?]*)$/") == [] then
            fname = macroname;
            found = %t;
        else
            tmpfile = tmpdir + macroname + ".sci";
            if funptr(macroname)<>0 then
                error(msprintf(gettext("%s: %s is a uneditable hard coded function.\n"), "edit", macroname));
            end
            libr = whereis(macroname);
            if libr <> [] then // macroname is the name of a defined function
                if size(libr, "*") > 1 then
                    // we take last definition
                    libr = libr(1);
                end
                [macrolist, path] = libraryinfo(libr);
                clear macrolist;
                // convert path according to getos() == "Windows" value and expand SCI
                fname = pathconvert(path) + macroname + ".sci";
                found = isfile(fname);
            elseif isdef(macroname)
                if typeof(evstr(macroname)) == "function" then
                    txt = fun2string(evstr(macroname));
                    txt = strsubst(txt, "=ans(", " = " + macroname + "(");
                    fname = tmpfile;
                    mputl(txt, fname);
                    found = %t;
                end
            end
        end
    else //no macroname specified
        macroname = "untitled";
        tmpfile = tmpdir + macroname + ".sci";
        found = %f;
    end

    if ~found then // macroname is the name of an undefined function
        fname = tmpfile;
        txt = ["function [] = " + macroname + "()"; "endfunction"];
        mputl(txt, fname);
    end

    // call the editor with the filename
    if (rhs == 2) then
        editor(fname, linenumber, macroname);
    else
        editor(fname);
    end

endfunction
