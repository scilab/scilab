// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH ONLY -->

// Testing warning modes
warning("on");
assert_checkequal(warning("query"), "on");
warning("off");
assert_checkequal(warning("query"), "off");
warning("stop");
assert_checkequal(warning("query"), "stop");

// Testing warning stop mode
warning("stop");
str = "";
try
    warning("this is a warning");
catch
    str = lasterror();
end
str //display str
assert_checktrue(str <> "");


//Testing waring on mode
warning("on");
str = "";
try
    warning("this is a warning");
catch
    str = lasterror();
end
str //display str
assert_checktrue(str == "");

//Testing waring off mode
warning("on");
str = "";
try
    warning("this is a warning");
catch
    str = lasterror();
end
str //display str
assert_checktrue(str == "");
