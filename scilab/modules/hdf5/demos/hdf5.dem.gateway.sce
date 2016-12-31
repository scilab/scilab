// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre Ledru
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("hdf5.dem.gateway.sce");
    _("HDF5");  // lets gettext() harvesting it
    add_demo("HDF5", demopath + "hdf5.dem.gateway.sce");

    subdemolist = [_("Ring resonator (HDF5 data source)")   ,"resonator.dem.sce" ]

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
