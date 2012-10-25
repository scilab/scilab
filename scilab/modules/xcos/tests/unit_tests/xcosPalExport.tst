// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// White-box test for the xcosPalExport macro.

path = TMPDIR + "/palette.sod";

pal = xcosPal();

xcosPalExport(pal, path)
if ~isfile(path) then pause, end
deletefile(path);

[status, msg] = xcosPalExport(pal, path)
if ~isfile(path) then pause, end
deletefile(path);

savedPal = pal;
xcosPalExport(pal, path)

clear pal;
import_from_hdf5(path);
if ~and(savedPal == pal) then pause, end

