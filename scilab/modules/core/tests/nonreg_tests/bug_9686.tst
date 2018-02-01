// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9686 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9686
//
// <-- Short Description -->
// error in an if expression inside a try makes Scilab froze
a=%f;b=%f;
try
  if error("test") then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  while error("test") then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  select error("test")
  case %t
  else  
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);

a=%f;b=%f;
try
  select %t 
  case  error("test") then
    a=%t;
  end
catch
  b=%t;
end
assert_checktrue(~a&b);


a=%f;b=%f;
try
  if %t then
    error("test")
    a=%t;  
  end
catch
  b=%t;
end
assert_checktrue(~a&b);


a=%f;b=%f;
try
  while %t then
    error("test")
    a=%t;  
  end
catch
  b=%t;
end
assert_checktrue(~a&b);
