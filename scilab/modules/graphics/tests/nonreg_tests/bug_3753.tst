// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3753 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3753
//
// <-- Short Description -->
// Using draw axes in range mode ('r') produces labels are completely unreadable and show strange characters.
//

SC=3.2;
square(-SC,-SC,SC,SC)
drawaxis(x=[SC,-SC,8],y=-SC,dir="d",tics="r",format_n="%1.1f");

axis = gce();
if (axis.tics_labels <> ["3.2"  "2.4"  "1.6"  "0.8"  "0.0"  "-0.8"  "-1.6"  "-2.4"  "-3.2"]) then pause; end



