
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13683 -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13683
//
// <-- Short Description -->
// Stack error using unsetmenu for a dockable="off" figure
//

f = figure("dockable", "off");
unsetmenu(f.figure_id, _("File"), 2);
