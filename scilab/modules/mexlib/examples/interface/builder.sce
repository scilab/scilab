
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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

// [1] generate Path.incl
//  here we want SCI to be ../../
//  but in a contrib the next line should be
//  replaced by sci = SCI

sci="../../"
F=mopen("Path.incl","w");
mfprintf(F,"SCIDIR="+sci+"\n");
mfprintf(F,"SCIDIR1="+strsubst(sci,"/","\\")+"\n");
mclose(F);

// [2] building lib/libutil.xx

chdir lib
exec builder.sce
chdir ../

// [3] the part devoted to shared lib generation

ilib_name  = "libmex" 		// interface library name

// objects files (but do not give mexfiles here)

files = ["f4.o"];

// other libs needed for linking (must be shared library names)

libs  = ["lib/libutil"];

// table of (scilab_name,interface-name or mexfile-name, type)

table =["f1",		"fmex1",	"cmex";
"f2",		"fmex2",	"cmex";
"f3",		"foof",		"Fmex";
"f4",		"int_f4"	"csci"];

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs)
