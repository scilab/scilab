
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//============================
// Example How to do and use a librarie
//============================
// Create a librarie with functions in SCI+'/modules/functions/examples directory
// to do once time
genlib('demofunctionslib',SCI+'/modules/functions/examples');
//============================
// load demofunctionslib
load(SCI+'/modules/functions/examples/lib');
//============================
y=3;
x=f(y);
z=g(x,y);
//============================
disp(y);
disp(x);
disp(z);
//============================
// remove all librarie and functions ref.
clear demofunctionslib y x z f g;
//============================

