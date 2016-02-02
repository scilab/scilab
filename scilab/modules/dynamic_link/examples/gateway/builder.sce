// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
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

ilib_name  = "libtutorial"; 		// interface library name
files = ["sci_view.c","displ.c","sci_matmul.c","matmul.c"];     // objects files
//
libs  = []; 				// other libs needed for linking
table = [ "matmul","sci_matmul";
"view", "sci_view"];		// table of (scilab_name,interface-name)

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs);



