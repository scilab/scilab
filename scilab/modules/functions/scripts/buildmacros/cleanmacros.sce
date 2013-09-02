
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

lines(0);

CurrentDirectory=pwd();

chdir ("SCI/modules");
if getos() == "Windows" then
    dos("del *.bin /s 2>NUL","-echo");
    dos("del lib /s 2>NUL","-echo");
    dos("del names /s 2>NUL","-echo");
    dos("del *.moc /s 2>NUL","-echo");
else
    unix("rm -Rf *.bin");
    unix("rm -Rf lib");
    unix("rm -Rf names");
    unix("rm -Rf *.moc");
end

chdir (CurrentDirectory);

exit
// ------------------------------------