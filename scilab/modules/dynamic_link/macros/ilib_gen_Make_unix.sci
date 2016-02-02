// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU (rewrite to use autotools)
// Copyright (C) 2009-2010 - DIGITEO - Sylvestre LEDRU
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
// Generate a Makefile which can be used by ilib_*
//=============================================================================

function ilib_gen_Make_unix(names,   ..
    files,   ..
    libs,    ..
    libname, ..
    ldflags, ..
    cflags,  ..
    fflags,  ..
    cc,      ..
    tables   ..
    )

    if getos() == "Windows" then
        return
    end

    if libname == "" then libname = names(1);end

    if ( strncpy(libname,3) == "lib") then
        l       = strsplit(libname,3);
        libname = l(2);
        clear l;
    end

    if isdef("tables") then

        /// Check tables ... the second element should be the file name
        if typeof(tables)<>"list" then
            tables=list(tables)
        end
        L=length(tables);

        // for each element in tables
        for it=1:L
            table = tables(it)
            [mt,nt]=size(table);
            for i=1:mt ;
                // mex files to be added to the build process
                if table(i,3)=="cmex" | table(i,3)=="fmex" | table(i,3)=="Fmex" then
                    if isempty(find(basename(files)==table(i,2))) then // If not already in the array
                        files=[files, table(i,2)] // add it to the file list
                    end
                end
            end
        end

    end // isdef('tables')


    originPath  = pwd();
    linkBuildDir    = TMPDIR+"/"+libname;
    mkdir(linkBuildDir);
    commandpath = SCI+"/modules/dynamic_link/src/scripts";
    [fd,ierr] = mopen(commandpath+"/write.test","w+");

    if (ierr<>0) then
        writePerm=%F
    else
        writePerm=%T
        mclose(fd)
        mdelete(commandpath+"/write.test");
    end

    if (writePerm == %T & ( fileinfo(commandpath+"/Makefile.orig") == [] | fileinfo(commandpath+"/libtool") == [] )) then
        // We have write permission on the scilab tree, then generate the stuff into the directory in order to avoid the configure each time.
        generateConfigure(commandpath);
    end


    // Copy files => linkBuildDir
    chdir(linkBuildDir);

    if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("   %s: Copy compilation files (Makefile*, libtool...) to TMPDIR\n"),"ilib_gen_Make");
    end

    // List of the files mandatory to generate a lib with the detection of the env
    mandatoryFiles = ["compilerDetection.sh", ..
    "configure.ac", ..
    "configure", ..
    "Makefile.am", ..
    "Makefile.in", ..
    "config.sub", ..
    "libtool", ..
    "config.guess", ..
    "config.status", ..
    "depcomp", ..
    "install-sh", ..
    "ltmain.sh", ..
    "missing", ..
    "aclocal.m4"];

    // Copy files to the working tmpdir
    for x = mandatoryFiles(:)' ;
        fullPath=commandpath+"/"+x;
        if (isfile(fullPath)) then
            [status,msg]=copyfile(fullPath,linkBuildDir);
            if (status <> 1)
                error(msprintf(gettext("%s: An error occurred: %s\n"), "ilib_gen_Make",msg));
            end
        end
    end

    filelist = "";

    for x = files(:)' ;
        // Pre added file in the list ... don't really know why

        if (x <> "csci") then
            // Old way: to compile a fun.c file, the user had to provide fun.o
            filename = strsubst(x,".o","");

            chdir(originPath); // Switch back to the source dir in order to have only the filename
            filesMatching = ls(filename+".*");

            // Two cases here:
            // * The user provided the real filename. Then, take if straight
            // * The user provided a file ending by .o (example: myfile.o)
            // We stripped the ending .o and looked for all files
            if filesMatching == [] | fileinfo(x) <> [] then

                [pathFrom, file_name, file_extension]=fileparts(x); // Retrieve the path of the file
                if length(pathFrom) == 0 then // Empty => it should be PWD
                    pathFrom=pwd();
                end

                if pathFrom <> linkBuildDir then
                    if ( ilib_verbose() <> 0 ) then
                        mprintf(gettext("   %s: Copy %s to TMPDIR\n"),"ilib_gen_Make",x);
                    end
                    copyfile(x, linkBuildDir);
                else
                    if ( ilib_verbose() <> 0 ) then
                        mprintf(gettext("   %s: Did not copy %s: Source and target directories are the same (%s).\n"),"ilib_gen_Make",x,pathFrom);
                    end
                end

                if ~isfile(pathFrom + "/" + file_name + file_extension) then
                    error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected. Provided: %s\n"), "ilib_gen_Make_unix", 2, pathFrom + "/" + file_name + file_extension));
                end

                filelist = filelist + " " + file_name + file_extension;

            else

                // Or copy the file matching to what we were looking for
                // (this stuff could lead to bug if you have fun.c fun.f
                // or fun.cxx but it was already the case before ...

                // Not that we don't want to copy working files
                ignoredFileExtension=[".lo",".la",".lai"]
                for f=filesMatching(:)'

                    if ~isfile(f) then
                        error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make_unix", 2));
                    end

                    if strindex(f,ignoredFileExtension) == [] then
                        if ( ilib_verbose() <> 0 ) then
                            mprintf(gettext("   %s: Copy %s to TMPDIR\n"),"ilib_gen_Make",f);
                        end

                        copyfile(f, linkBuildDir);
                        filelist = filelist + " " + f;
                    else
                        if ( ilib_verbose() <> 0 ) then
                            mprintf(gettext("   %s: File %s ignored.\n"),"ilib_gen_Make",f);
                        end
                    end
                end
            end
            chdir(linkBuildDir);
        end
    end

    if ldflags <> "" | cflags <> "" | fflags <> "" | cc <> "" | fileinfo(commandpath+"/Makefile.orig") == [] | fileinfo(commandpath+"/libtool") == [] then
        // Makefile.orig doesn't exists or may be invalid regarding the flags
        // run the ./configure with the flags

        if ( ilib_verbose() == 2 ) then
            mprintf(gettext("   %s: Need to run the compiler detection (configure).\n"),"ilib_gen_Make");
        end

        mdelete(linkBuildDir+"/Makefile.orig");
        generateConfigure(linkBuildDir, ldflags, cflags, fflags, cc)
    else
        // Reuse existing Makefile.orig because compilation flags are all empty
        [status,msg]=copyfile(commandpath+"/Makefile.orig",linkBuildDir);

        if ( ilib_verbose() == 2 ) then
            mprintf(gettext("   %s: Use the previous detection of compiler.\n"),"ilib_gen_Make");
        end

        if (status <> 1)
            error(msprintf(gettext("%s: An error occurred: %s\n"), "ilib_gen_Make",msg));
        end

        // We just copied the configure script, so its modification time is "now".
        // But Makefile timestamp is also "now", since we just created it. Since
        // Makefile depends on configure, "make" re-run the configure script, and
        // hence rebuild Makefile from Makefile.in, overwriting the result of
        // scicompile.sh. We want to avoid this, so we have to force Makefile's
        // timestamp to one second later.
        // (just try "touch configure Makefile; make" on any autoconf project)
        sleep(1000);
        unix_g("touch Makefile");
    end

    // Alter the Makefile in order to compile the right files
    if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("   %s: Modification of the Makefile in TMPDIR.\n"),"ilib_gen_Make");
    end

    cmd=commandpath + "/scicompile.sh " + libname + " " + filelist

    [msg,ierr, stderr] = unix_g(cmd);

    if ( ilib_verbose() == 2 ) then
        mprintf(gettext("   %s: Substitute the reference by the actual file.\n"),"ilib_gen_Make");
        mprintf(gettext("   Command: %s\n"),cmd);
        if (length(msg)) then
            mprintf(gettext("Output: %s\n"),msg);
        end
        mprintf(gettext("stderr: %s\n"),stderr);
    end

    if ierr <> 0 then
        if ( ilib_verbose() <> 0 ) then
            mprintf(gettext("%s: Error while modifying the reference Makefile:\n"),"ilib_gen_Make")
            mprintf(msg + " " + stderr);
        end
        return;
    end

    chdir(originPath);

endfunction


function generateConfigure(workingPath, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    cc)

    // We launch ./configure in order to produce a "generic" Makefile
    // for this computer

    if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("   %s: configure : Generate Makefile.\n"),"ilib_gen_Make");
    end
    cmd = gencompilationflags_unix(ldflags, cflags, fflags, cc, "configure")
    cmd = workingPath+"/compilerDetection.sh "+cmd

    [msg,ierr,stderr] = unix_g(cmd);

    if ( ilib_verbose() == 2 ) then
        mprintf(gettext("   %s: Command: %s\n"),"ilib_gen_Make",cmd);
        mprintf(gettext("   Output: %s\n"),msg);
        mprintf(gettext("   stderr: %s\n"),stderr);
    end

    if ierr <> 0 then
        if ( ilib_verbose() <> 0 ) then
            mprintf("%s\n", msg + " " + stderr);
        end
        error(msprintf(gettext("%s: An error occurred during the detection of the compiler(s). Set ilib_verbose(2) for more information.\n"), "ilib_gen_Make"));
        return;
    end

endfunction
//=============================================================================
