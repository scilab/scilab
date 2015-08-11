// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 11411 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11411
//
// <-- Short Description -->
//  save function used unsigned char to store length of string, now it is an integer

oldMode = warning("query");
warning("off");
longstring = strcat(mgetl("SCI/COPYING-BSD")); //1639

//"figure".info_message
//"figure".igure_name
f = gcf();
f.info_message = longstring;
f.figure_name = longstring;
save(TMPDIR + "/savef.dat", "f");
delete(f);
clear f;
load(TMPDIR + "/savef.dat");
assert_checkequal(f.info_message, longstring);
assert_checkequal(f.figure_name, longstring);
delete(f);
clear f;

//"uimenu".callback
m = uimenu(gcf(),"label", "test", "callback", longstring); //no sense, just for test !
save(TMPDIR + "/savef.dat", "m");
delete(gcf());
clear m;
load(TMPDIR + "/savef.dat");
assert_checkequal(m.callback, longstring);
delete(gcf());
clear m;

//"uicontrol".String
//"uicontrol".callback
m = uicontrol(gcf(), "Style", "text", "string",longstring, "callback", longstring);
save(TMPDIR + "/savef.dat", "m");
delete(gcf());
clear m;
load(TMPDIR + "/savef.dat");
assert_checkequal(m.callback, longstring);
assert_checkequal(m.string, longstring);
delete(gcf());
clear m;
warning(oldMode);

