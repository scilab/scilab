// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function t=cat_code(a,b)

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
