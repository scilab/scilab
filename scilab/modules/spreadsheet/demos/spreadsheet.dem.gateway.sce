// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("spreadsheet.dem.gateway.sce");
    gettext("Spreadsheet");   // Lets gettext() harvesting it
    add_demo("Spreadsheet", demopath + "spreadsheet.dem.gateway.sce");

    subdemolist = [_("Read a xls file")  ,"spreadsheet.dem.sce"];

    subdemolist(:,2) = demopath + subdemolist(:,2);

endfunction

subdemolist = demo_gateway();
clear demo_gateway;