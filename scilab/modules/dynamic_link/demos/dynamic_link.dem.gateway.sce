// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("dynamic_link.dem.gateway.sce");

subdemolist = ["call a C function"             , "call_c.sce" ;
               "call a fortran subroutine"             , "call_fortran.sce" ;];

subdemolist(:,2) = demopath + subdemolist(:,2)
clear demopath;