// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
demopath = get_absolute_file_path("datatips.dem.gateway.sce");

subdemolist=[
_("2D curves")              , "datatip1.sce";
_("3D curve")               , "datatip2.sce"];


subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
