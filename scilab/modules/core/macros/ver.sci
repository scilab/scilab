//===============================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//===============================
//Version information for Scilab
//===============================
function res = ver()

    sciver = getversion("scilab");
    res(1,1) = gettext("Scilab Version: ");
    res(1,2) = sprintf("%d.%d.%d.%d",sciver(1),sciver(2),sciver(3),sciver(4));

    if ( with_module("jvm") )  & ( getscilabmode()<> "NWNI" ) then
        res(2,1) = gettext("Operating System: ");
        res(2,2) = sprintf("%s %s",system_getproperty("os.name"), ..
        system_getproperty("os.version"));

        res(3,1) = gettext("Java version: ");
        res(3,2) = system_getproperty("java.version");

        res(4,1) = gettext("Java runtime information: ");
        res(4,2) = sprintf("%s (build %s)\n",system_getproperty("java.runtime.name"), ..
        system_getproperty("java.runtime.version"));

        res(5,1) = gettext("Java Virtual Machine information: ");
        res(5,2) = sprintf("%s (build %s, %s)\n",system_getproperty("java.vm.name"), ..
        system_getproperty("java.vm.version"), ..
        system_getproperty("java.vm.info"));

        res(6,1) = gettext("Vendor specification: ");
        res(6,2) = system_getproperty("java.vm.specification.vendor");
    end
endfunction
//===============================
