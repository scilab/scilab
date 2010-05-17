//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

my_path = SCI+"/modules/graphics/demos/misc";
funcprot(0);
exec(my_path+"/misc.dem.sci");funcprot(1);
demo_misc();
f=mopen(my_path+"/misc.dem.sci");
demo_viewCode("misc.dem.sci");
mclose(f);
