function t=cat_code(a,b)
// Copyright INRIA
// Scilab Project - V. Couvert
// Catenate two parts of code (called by tree2code)
// Input:
// - a: original code
// - b: code to add to a
// Output:
// - t: catenation of a and b

if a==[] then 
  t=b
elseif b==[] then
  t=a
elseif stripblanks(b)=="" then // b=="" then add a new line
  t=[a;""];
else
  t=[a(1:$-1);a($)+b(1);b(2:$)]
end
endfunction
