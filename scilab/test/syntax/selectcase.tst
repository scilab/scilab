// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Tests with comments
c = 1;
select c // Test value of c
case 1 then// A comment just after then
  res = %T; // Return %T
case 2 then // Is c equal to 2?
  res = %F; // Return %F
else // Default case
  res = %F;  // Return %F
end // End of select/case
//if ~res then pause;end
//clear res

// Tests with numerical value
c = 28.2;
select c
case 12
  res = %F;
case 28.2
  res = %T;
case 42
  res = %F;
else
  res = %F;
end
//if ~res then pause;end
//clear res

// Tests with character strings
c = "testme";
select c
case 12
  res = %F;
case "wrong"
  res = %T;
case "testme"
  res = %F;
else
  res = %F;
end
//if ~res then pause;end
//clear res

// Tests with cells
c = 28;
select c
case 12
  res = %F;
case 42
  res = %F;
case {28, 15}
  res = %T;
else
  res = %F;
end
//if ~res then pause;end
//clear res

c = "testme";
res = %F;
select c
case 12
  res = %F;
case "wrong"
  res = %F;
case {"testme", "anothervalue"}
  res = %T;
  // No else case!
end
//if ~res then pause;end
//clear res

