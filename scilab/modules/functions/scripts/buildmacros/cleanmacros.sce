
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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