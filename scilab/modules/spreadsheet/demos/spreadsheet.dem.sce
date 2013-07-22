// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_spreadsheet()

    l = lines();
    lines(0);

    m = mode();
    mode(1);

    mprintf("\n// Decode ole file, extract and open Excel stream\n");
    mprintf("[fd, SST, Sheetnames, Sheetpos] = xls_open(''SCI/modules/spreadsheet/demos/xls/Test1.xls'')\n");

    [fd, SST, Sheetnames, Sheetpos] = xls_open("SCI/modules/spreadsheet/demos/xls/Test1.xls")

    mprintf("\n// Read first data sheet\n");
    mprintf("[Value, TextInd] = xls_read(fd, Sheetpos(1))\n");

    [Value, TextInd] = xls_read(fd, Sheetpos(1))

    //close the spreadsheet stream
    mclose(fd);

    mprintf("\n");

    mode(m);
    lines(l(2), l(1));

endfunction

demo_spreadsheet()
clear demo_spreadsheet;
