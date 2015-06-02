// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests convstr
//===============================

if convstr("ABC")<>"abc" then pause,end
if convstr("ABC","l")<>"abc" then pause,end
if convstr("ABC","u")<>"ABC" then pause,end
if convstr(["ABC";"x"])<>["abc";"x"] then pause,end
if convstr(["ABC";"x"],"l")<>["abc";"x"] then pause,end
if convstr(["ABC";"x"],"u")<>["ABC";"X"] then pause,end
if convstr(["ABC";"x"],"L")<>["abc";"x"] then pause,end
if convstr(["ABC";"x"],"U")<>["ABC";"X"] then pause,end
if or(convstr(["";""],"u")<>["";""]) then pause,end
//===============================
if convstr("ABC")<>"abc" then pause,end
if convstr("ABC","l")<>"abc" then pause,end
if convstr("ABC","u")<>"ABC" then pause,end
if convstr("ABC","L")<>"abc" then pause,end
if convstr("ABC","U")<>"ABC" then pause,end
if convstr(["ABC","x"])<>["abc","x"] then pause,end
if convstr(["ABC","x"],"l")<>["abc","x"] then pause,end
if convstr(["ABC","x"],"u")<>["ABC","X"] then pause,end
if convstr(emptystr())<>emptystr() then pause,end
//===============================
if convstr("") <> "" then pause,end
if convstr("","u") <> "" then pause,end
if convstr("","U") <> "" then pause,end
if convstr("","l") <> "" then pause,end
if convstr("","L") <> "" then pause,end
//===============================
if convstr([]) <> [] then pause,end
if convstr([],"u") <> [] then pause,end
if convstr([],"U") <> [] then pause,end
if convstr([],"l") <> [] then pause,end
if convstr([],"L") <> [] then pause,end
//===============================
if ( convstr("SCILAB","l") <> convstr("scilab","l") ) then pause,end
if ( convstr("SCILAB","u") <> convstr("scilab","u") ) then pause,end
//===============================
if convstr("0123456789","u") <> "0123456789" then pause,end
if convstr("0123456789","l") <> "0123456789" then pause,end
//===============================
a=[""];
if length(convstr(a,"u")) <> 0 then pause,end
if length(convstr(a,"l")) <> 0 then pause,end
//===============================
a=["";""];
if length(convstr(a,"u")) <> 0 then pause,end
if length(convstr(a,"l")) <> 0 then pause,end
//===============================
STR = ["this","is";"my","matrix"];
if convstr(STR,"u") <> ["THIS" "IS";"MY" "MATRIX"] then pause,end
if convstr(STR,"l") <> STR then pause,end
//===============================

//===============================
// interface tests
//===============================

if execstr("convstr(''abc'',''i'')","errcatch") <= 0 then pause,end
if execstr("convstr(''abc'',''i'')","errcatch") <= 0 then pause,end
