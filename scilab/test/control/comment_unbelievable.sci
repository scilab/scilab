//---
// Comment in function declaration breack
function result=identity(arg) // comment here
  result=arg
endfunction

//---
// Comment in IF control structure
if %t then // comment here
  disp("true")
else // another comment here
  disp("false")
end

//---
//
try // comment here
identity(20)
catch // comment here
disp("Something caught")
end

//---
// Comment in while instruction
while %t // comment here
  disp(pouet)
end

//---
// Comment in while instruction
while %t do // comment here
  disp(pouet)
end

//---
// Comment in while instruction
while %t , // comment here
  disp(pouet)
end

//---
// Comment in while instruction
while %t then // comment here
  disp(pouet)
end