// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()
  demopath = get_absolute_file_path("toolbox_skeleton.dem.gateway.sce");

  subdemolist = ["sum with Scilab"             ,"scilab_sum.dem.sce"; ..
                 "A c sum "                    ,"c_sum.dem.sce"     ; ..
                 "The sum with fortran"        ,"fortran_sum.dem.sce" ; ..
                 "A find in cpp"               ,"cpp_find.dem.sce" ; ];

  subdemolist(:,2) = demopath + subdemolist(:,2);
  
endfunction

subdemolist = demo_gateway();
clear demo_gateway; // remove demo_gateway on stack
