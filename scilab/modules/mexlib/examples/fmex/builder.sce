
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// This is the builder.sce
// must be run from this directory

// interface library name
ilib_name  = 'libmex'

// objects files (but do not give mexfiles here)
files = [];

// other libs needed for linking (must be shared library names)
libs  = [];

// table of (scilab_name,interface-name or mexfile-name, type) 

table =['mexf1','mexfunction1','fmex'];

ldflags = "";
cflags  = "";
fflags  = "";

// do not modify below
// ----------------------------------------------
ilib_mex_build(ilib_name,table,files,libs,'Makelib',ldflags,cflags,fflags)
