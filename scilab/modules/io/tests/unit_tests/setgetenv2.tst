//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"프로그램",
"תוכנית"];

for i=1:size(tab_ref,"*")
	var = "test" + string(i);
	setenv(var, tab_ref(i));
	if(getenv(var) <> tab_ref(i)) then pause,end
	setenv(var, "");
end

for i=1:size(tab_ref,"*")
	val = "test" + string(i);
	setenv(tab_ref(i), val);
	if(getenv(tab_ref(i)) <> val) then pause,end
	setenv(var, "");
end

for i=1:size(tab_ref,"*")
	setenv(tab_ref(i), tab_ref(i));
	if(getenv(tab_ref(i)) <> tab_ref(i)) then pause,end
	setenv(var, "");
end
