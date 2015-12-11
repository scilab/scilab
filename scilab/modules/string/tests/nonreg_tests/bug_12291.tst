// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 12291 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12291
//
// <-- Short Description -->
// Unable to display a list of handle as a handle userData
//
set(get(0), "usedeprecatedskin", "on");
f=figure("figure_name","UIcontrol with an handle as a user_data","figure_id",0,"auto_resize","off");
frame=uicontrol("parent", f,"style","frame");
child=uicontrol("parent",frame,"style","text","string","OK");
set(frame,"userdata",list(child));
set(f,"figure_position",[201 228]);
set(f,"figure_size", [500 500]);
set(f,"axes_size",[490 400]);
res="uicontrol(''ForegroundColor'', [-1 -1 -1] ,''Relief'', ''ridge'', ''String'', '''' ,''Style'', ''frame'', ''Userdata'', list(uicontrol(''ForegroundColor'', [-1 -1 -1] ,''Relief'', ''flat'', ''String'', ''OK'' ,''Style'', ''text'')))";
assert_checkequal(sci2exp(frame,0),res);
assert_checktrue(execstr("frame2="+sci2exp(frame),"errcatch")==0);
xdel(winsid())
