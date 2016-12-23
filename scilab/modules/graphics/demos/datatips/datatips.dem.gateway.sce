// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
demopath = get_absolute_file_path("datatips.dem.gateway.sce");

subdemolist=[
_("2D curves")              , "datatip1.sce";
_("3D curve")               , "datatip2.sce";
_("Detach")                 , "datatip_detach.sce"];


subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
