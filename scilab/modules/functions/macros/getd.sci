// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
// Copyright (C) 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function getd(path,option)

    // get all .sci files in the specified directory

    [lhs,rhs]=argn(0);

    if rhs<=0 then
        path="./";
    end

    // convert path according to getos() == 'Windows' flag
    // and with env var substitutions
    path = pathconvert(path,%t,%t);

    // list the sci files
    global lst
    lst          = listfiles(path+"*.sci",%f);
    lst_filtered = [];

    // remove wrong files extension
    // bug 2289

    for i = 1:size(lst,"*")
        if( regexp(lst(i),"/.sci$/") <> [] ) then
            lst_filtered = [lst_filtered;lst(i)];
        end
    end

    lst = lst_filtered;
    clear lst_filtered;

    if lst==[] | lst== "" then
        error(msprintf(gettext("%s: I cannot find any files with extension %s in %s\n"),"getd",".sci",path));
        return ;
    end

    // Bug http://bugzilla.scilab.org/13583 : some scripts loaded through the
    //  loop may contain "clear" instructions deleting a) the list of files to
    //  be processed, and b) the reference list of internal getd() variables not
    //  to be returned.
    //  Informations to be protected are set in global variables: lst, old
    global old          // sets it as global, before first assignment
    old = who("local")
    old = old(isdef(old, "l"))
    //prot = funcprot(); funcprot(0)
    for k = 1:size(lst,"*");
        global lst // recover it whenever it would have been cleared
        if fileparts(lst(k),"extension")==".sci" then
            // disp(basename(lst(k))) // Beware: files are not sorted/loaded in alphabetical order
            if execstr("exec(lst(k));","errcatch")<>0 then
                warning(msprintf(gettext("%s: Incorrect function in file %s.\n"),"getd",lst(k)))
            end
        end
    end
    global old
    //funcprot(prot);
    new = who("local");
    new = new(isdef(new, "l"))
    new = setdiff(new, old)
    clearglobal old lst

    if new<>[] then
        execstr("["+strcat(new,",")+"]=resume("+strcat(new,",")+")")
    end

endfunction
