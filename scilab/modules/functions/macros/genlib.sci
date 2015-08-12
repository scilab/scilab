// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2012 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [success, funcs, success_files, failed_files] = genlib(nam, path, force, verbose, names)

    // get all .sci files in the specified directory

    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "genlib", 1, 5));
    end

    if type(nam) <> 10 then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "genlib", 1));
    end

    if size(nam, "*") <> 1 then
        error(999, msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"), "genlib", 1));
    end

    if exists("force", "local") == 0 then
        force = %f;
    else
        if type(force) <> 4 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"), "genlib", 3));
        end

        if size(force, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong size for input argument #%d: A boolean expected.\n"), "genlib", 3));
        end
    end

    if exists("verbose", "local") == 0 then
        verbose = %f;
    else
        if type(verbose) <> 4 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"), "genlib", 4));
        end

        if size(verbose, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong size for input argument #%d: A boolean expected.\n"), "genlib", 4));
        end
    end

    W          = who("get");
    np         = predef();
    predefined = or(W($-np+1:$) == nam);

    success       = %t;
    funcs         = [];
    success_files = [];
    failed_files  = [];

    if verbose then
        mprintf(gettext("-- Creation of [%s] (Macros) --\n"), nam);
    end

    updatelib = %f; //true if updating an already defined library

    //check if a library with the same name exists

    oldlib    = [];
    old_path  = [];
    old_names = [];

    if exists(nam)==1 then
        execstr("oldlib="+nam);
        if typeof(oldlib)=="library" then
            //yes, get its path and function list
            old_names = string(oldlib);
            clear oldlib;
            old_path     = old_names(1);
            old_names(1) = [];
        end
    end

    if (exists("path", "local") == 0) then
        if (old_path <> []) then
            path = old_path;
            updatelib = %t;
        else
            path = pwd();
        end
    else
        if type(path) <> 10 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "genlib", 2));
        end

        if size(path, "*") <> 1 then
            error(999, msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"), "genlib", 2));
        end
    end

    // convert path according to getos() == 'Windows' flag
    // without env variable substitution

    path1 = pathconvert(path, %t, %f);
    // with env subsitution
    path = pathconvert(path, %t, %t);

    if exists("names", "local")==0 then
        // list the sci files
        files          = gsort(listfiles(path+"*.sci",%f), "r", "i");
        files_filtered = [];

        for i=1:size(files,"*")
            if( regexp(files(i),"/\.sci$/") <> [] ) then
                files_filtered = [files_filtered;files(i)];
            end
        end

        files = files_filtered;
        clear files_filtered;

        if files==[] | files== "" then
            warning(msprintf(gettext("%s: No files with extension %s found in %s\n"),"genlib",".sci", path));
            return ;
        end

        names = basename(files,%f);

    else
        if type(names) <> 10 then
            error(999, msprintf(_("%s: Wrong type for input argument #%d:  A matrix of string expected.\n"), "genlib", 5));
        end

        files = path + names;
        names = strsubst(names, "/\.sci$/", "", "r");
    end

    names_changed = %t;

    if updatelib then
        if and(gsort(names)==gsort(old_names)) then names_changed=%f,end
    end

    modified = %f;

    if force == %t then
        modified = %t;
        for i=1:size(files,"*")  // loop on .sci files
            scif = files(i);
            if verbose then
                mprintf(gettext("%s: %s file compilation forced\n"),"genlib",names(i)+".sci");
            end
            // getf sci file and save functions it defines as a .bin file
            getsave(scif);
        end
    else
        for i=1:size(files,"*")  // loop on .sci files
            scif      = files(i);
            binf      = strsubst(scif, "/\.sci$/",".bin","r");
            binf_info = fileinfo(binf);
            recompile = %f;

            if binf_info == [] then
                recompile = %t;
            else
                scif_info = fileinfo(scif);
                if newest(scif,binf) == 1 then
                    recompile = %t ;
                end
            end

            if recompile == %t then

                if verbose then
                    mprintf(gettext("%s: Processing file: %s\n"), "genlib", names(i) + ".sci");
                end

                // getf sci file and save functions it defines as a .bin file
                result = getsave(scif);
                modified = %t;
                if result <> [] then
                    success_files($+1) = scif
                    funcs = [funcs result]
                else
                    failed_files($+1) = scif
                    success = %f
                end
            end
        end
    end

    if modified then

        if verbose then
            mprintf(gettext("%s: Regenerate names and lib\n"),"genlib");
        end

        if names_changed
            mputl(names,path+"names"); // write 'names' file in directory
        end

        // create library
        execstr(nam+"=lib(''"+getshortpathname(path1)+"'')")
        //save it

        warnMode = warning("query");
        warning("off");
        if execstr("save(''"+path1+"lib''"+","+nam+")","errcatch")<>0 then
            success = %f;
            warning(warnMode);
            error(msprintf(gettext("%s: %s file cannot be created\n"),"genlib",path+"lib"));
        end
        warning(warnMode);
    else
        execstr(nam+"=lib(''"+path1+"'')")
    end

    if names_changed then
        if ~predefined then
            execstr(nam+"=resume("+nam+")")
        else
            mprintf(gettext("Library file %s has been updated,\nbut cannot be loaded into Scilab because %s is a protected variable.\n"),path1+"lib",nam);
        end
    end

endfunction

function result = getsave(scifile)

    // utility function
    // performs a exec on file scifile

    result = %f;
    prot   = funcprot();
    nold   = size(who("get"),"*");

    funcprot(0);

    ierr=exec(scifile,"errcatch",-1);
    if ierr <> 0 then
        execstr("error(ierr)","errcatch");
        error(msprintf(gettext("%s: Error in file %s : %s.\n"), "genlib", scifile, lasterror()));
        clear ierr;
        result = %f;

    else

        clear ierr;

        // lookfor names of the functions defined in file 'scifile'
        new = who("get")
        new = new(1:(size(new,"*")-nold-1))

        // create output file name (just replace the ".sci" extension by ".bin"
        binfile = strsubst(scifile,"/\.sci$/",".bin","r")

        // save all functions in the output file
        [u,ierr]=mopen(binfile,"wb")
        if ierr<>0 then
            clear ierr;
            nf = length(binfile);
            if nf>40 then
                binfile="..."+part(binfile,nf-40:nf);
            end
            error(msprintf(gettext("%s: Impossible to open file %s for writing\n"),"genlib",binfile));
        end

        clear ierr

        if new<>[] then
            result = new($:-1:1)';
            warnMode = warning("query");
            warning("off");
            execstr("save(u,"+strcat(new($:-1:1),",")+")");
            warning(warnMode);
        else
            msprintf(gettext("%s: File %s does not contain any function.\n"),"genlib",binfile)
            result = %f;
        end

        mclose(u);
    end

    funcprot(prot);

endfunction
