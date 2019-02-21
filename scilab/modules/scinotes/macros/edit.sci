// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function edit(macroname, linenumber)
    // macroname : character string giving a macroname
    // linenumber : line number (as decimal number or literal one)

    [lhs,rhs] = argn(0);
    if (rhs > 2) then
        msg = _("%s: Wrong number of input argument(s): At least %d expected.\n")
        error(msprintf(msg, "edit", 1));
    end

    if (rhs >= 1 & type(macroname) ~= 10) then
        msg = _("%s: Wrong type for input argument #%d: String expected.\n")
        error(msprintf(msg, "edit", 1));
    end

    if rhs == 2 then
        if and(type(linenumber)~=[1 10]) | (type(linenumber)==10 & isnan(strtod(linenumber(1)))) then
            msg = _("%s: Wrong type for input argument #%d: Number expected.\n")
            error(msprintf(msg, "edit", 2));
        end
        linenumber = strtod(linenumber(1))
    end

    found = %f;
    tmp = %f;
    // tmpdir will have trailing / or \
    tmpdir= pathconvert(TMPDIR);

    if rhs >= 1 then // macroname or filename is given
        if regexp(macroname, "/^([a-zA-Z%_#!$?][0-9a-zA-Z_#!$?]*)$/") == [] then
            // File
            fname = macroname;
            found = %t;
        else
            if isdef(macroname)
                execstr("object = "+macroname)
                if type(object)==13
                    // macroname may be an alias. Example: sinus = sind
                    // We need to get the original macro name to edit it:
                    tree = macr2tree(object);
                    tmp = tree.name
                    if tmp ~= macroname then
                        msg = _("edit: ""%s"" is an alias of ""%s"" => editing %s()\n      Please reset %s=%s if %s() is recompiled.\n")
                        warning(msprintf(msg, macroname, tmp, tmp, macroname, tmp, tmp))
                        macroname = tmp
                    end

                    libr = whereis(macroname);
                    if type(libr($))==10 & libr($)=="script"
                        // the user-defined macro may overwrite a macro in library
                        path = whereis_in_libs(macroname)
                        if path ~= []
                            // priority = from library:
                            fname = pathconvert(path) + macroname + ".sci"
                        else
                            txt = tree2code(tree, %t);
                            fname = tmpdir + macroname + ".sci";
                            mputl(txt, fname);
                        end
                        found = %t
                    else // libr is either a module name or a library name
                        libr = libr($)
                        err = execstr("tmp="+libr, "errcatch")
                        if ~err & type(tmp)==14
                            [macrolist, path] = libraryinfo(libr);
                            fname = pathconvert(path) + macroname + ".sci"
                            found = %t
                        else // either a module name or any variable name:
                            // If macroname is a user-defined function,
                            // whereis() would have reported it as "script".
                            // => it's a macro in a library, but with a
                            // macroname equal to a built-in name
                            // => we must retrieve the library without whereis()
                            path = whereis_in_libs(macroname)
                            if path ~= []
                                fname = pathconvert(path) + macroname + ".sci"
                                found = %t
                            end
                        end
                    end
                end

                if ~found & funptr(macroname)<>0 then
                    error(msprintf(gettext("%s: %s() is an uneditable hard coded function.\n"), "edit", macroname));
                end
            end
        end
    end

    if ~found
        if ~isdef("macroname","l")
            macroname = "untitled"
        end
        fname = tmpdir + macroname + ".sci";
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

// ----------------------------------------------------------------------------

function path = whereis_in_libs(macroname)
    path = []
    for l = librarieslist()'
        [m, p] = libraryinfo(l)
        if or(m==macroname)
            path = p
            break
        end
    end
endfunction
