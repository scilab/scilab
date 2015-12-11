// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
