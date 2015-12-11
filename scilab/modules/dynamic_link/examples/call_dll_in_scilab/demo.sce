// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

if (findmsvccompiler()<>"unknown") then
    dos("nmake /f Makefile.mak");
    link("libtdll.dll","doit","c");
    [a,b]=call("doit",1,1,"d",2,2,"d","sort",1,2);
end


