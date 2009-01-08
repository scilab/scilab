//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("dynamic_link.dem.gateway.sce");

subdemolist = ["call a C function"             , "call_c.sce" ;
               "call a fortran subroutine"             , "call_fortran.sce" ;];

subdemolist(:,2) = demopath + subdemolist(:,2)
