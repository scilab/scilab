// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libtutorial'; 		// interface library name 
files = ['sci_view.c','displ.c','sci_matmul.c','matmul.c'];     // objects files 
					// 
libs  = []; 				// other libs needed for linking
table = [ 'matmul','sci_matmul';
          'view', 'sci_view'];		// table of (scilab_name,interface-name) 

// do not modify below 
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs);



