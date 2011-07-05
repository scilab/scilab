// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

  demopath = get_absolute_file_path("pvm.dem.gateway.sce");
  add_demo(_("PVM parallel Toolbox"), demopath+"pvm.dem.gateway.sce");

  subdemolist = [_("Master and slave")  ,"pvm.dem.sce"];

  subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
