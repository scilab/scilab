
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

// This is the builder.sce
// must be run from this directory

// interface library name
ilib_name  = "libmex"

// objects files (but do not give mexfiles here)
files = ["temptst2.cpp"];

// other libs needed for linking (must be shared library names)
libs  = [];

// table of (scilab_name,interface-name or mexfile-name, type)
table =["square","temptst","cmex"];

if getos() <> "Windows" then
    if part(getenv("OSTYPE","no"),1:6)=="darwin" then
        cflags = ""
        fflags = "";
        ldflags= "";
        cc = "g++";
    else
        // Since linking is done by gcc and not g++
        // we must add the libstdc++ to cflags
        // an other possibility would be to use cflags="" and cc="
        cflags = " -lstdc++"
        fflags = "";
        ldflags= "";
        cc="";
    end
else
    cflags = ""
    fflags = "";
    ldflags= "";
    cc = "";
end

// do not modify below
// ----------------------------------------------
ilib_mex_build(ilib_name,table,files,libs,"",ldflags,cflags,fflags)
