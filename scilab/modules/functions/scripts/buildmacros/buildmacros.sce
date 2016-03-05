
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

// ------------------------------------
//warning('off'); // disable warnings
// ------------------------------------

// ------------------------------------
//lines(0);
Directories=[];
CurrentDirectory=pwd();

// because scilab.start not called when we build macros
// Previously (wrong place), it was defined in buildmacros.sce of metanet module

modules=getmodules();
index=size(modules);

for i=1:index(1) do
    Directories=[Directories;"SCI/modules/"+modules(i)+"/macros"];
end
clear modules index i

Dim=size(Directories);


for i=1:Dim(1) do
    cd(Directories(i));
    //if (fileinfo('buildmacros.sce')<>[]) then
    //disp(Directories(i));
    exec("buildmacros.sce", 0);
    //end
    cd(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
// ------------------------------------
