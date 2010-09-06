// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- TEST WITH XCOS -->
//
// <-- Short Description -->
// White-box test for the xcosPal macro.

loadScicosLibs();

name = "My palette";
exec(SCI + "/modules/scicos/palettes/Lookup_Tables.cosf", -1); // getting an scs_m

pal = xcosPal()
pal = xcosPal(name)
pal = xcosPal(scs_m)
pal = xcosPal([], scs_m)
pal = xcosPal(name, scs_m)

