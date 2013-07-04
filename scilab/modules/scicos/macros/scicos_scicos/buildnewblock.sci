//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>  - 2007
//                      - Allan CORNET - 2008
//                      - Rachid Djenidi
//                      - Simone Mannori
//
//  Copyright (C) DIGITEO - Allan CORNET - 2010 - fully rewritten
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
//=============================================================================
function [ok]=buildnewblock(blknam, files, filestan, filesint, libs, rpat, ldflags, cflags)

    //** buildnewblock : generates Makefiles for
    //                   the generated C code of a scicos block,
    //                   compile and link it in Scilab
    //
    // Input : blknam : a prefix
    //         files : files to be compiled
    //         filestan : files to be compiled and included in
    //         resu           the standalone code
    //         filesint : files to be compiled and included in
    //                    the interfacing of standalone code
    //         libs : a vector of string of object files
    //                    to include in the building process
    //         rpat     : a target directory
    //         ldflags  : linker flags
    //         cflags   : C compiler flags
    //
    // Output :  ok : a flag to say if build is ok

    //** check rhs paramaters
    [lhs,rhs] = argn(0);

    if rhs <= 1 then files    = blknam, end
    if rhs <= 2 then filestan = "", end
    if rhs <= 3 then filesint = "", end
    if rhs <= 4 then libs     = "", end
    if rhs <= 5 then rpat     = TMPDIR, end
    if rhs <= 6 then ldflags  = "", end
    if rhs <= 7 then cflags   = "", end

    // Add .c extension if it is not already added
    exts_files = fileext(files);
    exts_filestan = fileext(filestan);
    exts_filesint = fileext(filesint);

    for i = 1:size(files, "*")
        if exts_files(i) == "" & files(i) <> "" then
            files(i) = files(i) + ".c";
        end
    end

    for i = 1:size(filestan, "*")
        if exts_filestan(i) == "" & filestan(i) <> "" then
            filestan(i) = filestan(i) + ".c";
        end
    end

    for i = 1:size(filesint, "*")
        if exts_filesint(i) == "" & ~isdir(filesint(i)) & filesint(i) <> "" then
            filesint(i) = filesint(i) + ".c";
        end
    end

    // add a external file if it exists
    // added for compatibility with 4.x
    // By RN ...
    if isfile(rpat + "/" + blknam + "f.f") then
        files=[files, blknam + "f.f"];
    end

    //** adjust path and name of object files
    //   to include in the building process
    if (libs ~= emptystr() & libs <> []) then
        libs = pathconvert(libs, %f, %t);
    end

    //** otherlibs treatment
    [ok, libs, for_link] = link_olibs(libs, rpat);
    if ~ok then
        ok = %f;
        message(["sorry compiling problem"; lasterror()]);
        return;
    end

    //** unlink if necessary
    [a, b] = c_link(blknam);
    while a
        ulink(b);
        [a, b] = c_link(blknam);
    end

    //** save path in case of error in ilib_compile
    oldpath = pwd();

    if getos() <> "Windows" then
        if isdir(SCI+"/../../include/scilab/scicos_blocks/") then
            //** Binary version
            cflags = cflags + " -I" + SCI + "/../../include/scilab/scicos/";
            cflags = cflags + " -I" + SCI + "/../../include/scilab/scicos_blocks/";
            cflags = cflags + " -I" + SCI + "/../../include/scilab/dynamic_link/";
        else
            //** it is a source version
            cflags = cflags + " -I" + SCI + "/modules/scicos/includes/";
            cflags = cflags + " -I" + SCI + "/modules/scicos_blocks/includes/";
            cflags = cflags + " -I" + SCI + "/modules/dynamic_link/includes/";
        end
    else
        // Load dynamic_link Internal lib if it's not already loaded
        if ~exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end
        cflags = cflags + strcat(" -I""" + dlwGetXcosIncludes() + """");
        ldflags = ldflags + strcat(" """ + dlwGetLibrariesPath() + dlwGetXcosLibraries() + """");
    end

    if (rpat == "" | rpat == []) & isdir(filesint) then
        // Prepare Block "Code Generation" from Menu
        chdir(filesint);
    else
        // generate Block in TMPDIR (default)
        chdir(TMPDIR);
    end

    // Since all files Xcos generated files of all diagrams are in the same TEMP directory
    // Here, we force to rebuild (not a simple incremental build on Windows)
    if getos() == "Windows" then
        VC_rebuild_mode = dlwForceRebuild();
        dlwForceRebuild(%t);
    end

    ierr = execstr("libn =ilib_for_link(blknam,files,"""",""c"","""",""loader.sce"","""",ldflags,cflags)","errcatch");

    // restore previous build mode
    if getos() == "Windows" then
        dlwForceRebuild(VC_rebuild_mode);
    end

    if ierr <> 0 then
        ok = %f;
        chdir(oldpath);
        disp(["sorry compiling problem"; lasterror()]);
        return;
    end

    if (rpat == "" | rpat == []) & isdir(filesint) then
        NAME_BLOCK_C_SCI = filesint + filesep() + blknam + "_c.sci";
        if isfile(NAME_BLOCK_C_SCI) then
            ierr = exec(NAME_BLOCK_C_SCI, "errcatch");
            if ierr <> 0 then
                ok = %f;
                chdir(oldpath);
                return
            end
        end
    end

    ierr = exec("loader.sce", "errcatch");
    if ierr <> 0 then
        ok = %f;
    else
        ok = %t;
    end

    chdir(oldpath);

endfunction

//=============================================================================
