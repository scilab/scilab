// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Simple wrapper around ilib_for_link

function tbx_build_src(names,       ..
    files,       ..
    flag,        ..
    src_path,    ..
    libs,        ..
    ldflags,     ..
    cflags,      ..
    fflags,      ..
    cc,          ..
    libname,     ..
    loadername,  ..
    makename)

    oldpath = pwd();

    if exists("src_path","local") then
        chdir(src_path);
    end

    if ~exists("libs", "local")       then libs       = []; end
    if ~exists("ldflags", "local")    then ldflags    = ""; end
    if ~exists("cflags", "local")     then cflags     = ""; end
    if ~exists("fflags", "local")     then fflags     = ""; end
    if ~exists("cc", "local")         then cc         = ""; end
    if ~exists("libname", "local")    then libname    = ""; end
    if ~exists("loadername", "local") then loadername = "loader.sce"; end
    if ~exists("makename", "local")   then makename   = ""; end

    ilib_for_link(names,      ..
    files,      ..
    libs,       ..
    flag,       ..
    makename,   ..
    loadername, ..
    libname,    ..
    ldflags,    ..
    cflags,     ..
    fflags,     ..
    cc);

    chdir(oldpath);

endfunction
