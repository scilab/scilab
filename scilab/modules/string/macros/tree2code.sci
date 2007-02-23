function C=tree2code(P,prettyprint)
// Copyright INRIA
// Scilab Project - V. Couvert
// Convert a tree returned by macr2tree() to a macro code
// Input: 
// - P: 'program' tlist
// - prettyprint: boolean value, if FALSE (default value), generated code is not formated else it is
// Output:
// - C: character string matrix (colum vector)

// Verify good type for input
if typeof(P)<>"program" then
  error("Input variable is not a program tree in tree2code !")
end

// Default value
rhs = argn(2);
if rhs==1 then
  prettyprint=%F
end

C=""

// Function prototype
lhsstr=[]
rhsstr=[]
if P.name<>"" then // Not a batch file
  for k=1:size(P.outputs)
    lhsstr=[lhsstr,expression2code(P.outputs(k))]
  end
  lhsstr="["+strcat(lhsstr,",")+"]"
  
  for k=1:size(P.inputs)
    rhsstr=[rhsstr,expression2code(P.inputs(k))]
  end
  rhsstr="("+strcat(rhsstr,",")+")"
  
  C="function "+lhsstr+" = "+P.name+rhsstr;
end

I=P.statements

// To avoid to have some instructions on the first line (function prototype)
if I(1)<>list("EOL") then
  C=cat_code(C,instruction2code(list("EOL"),%T));
end

// For each statement, generate corresponding code
for i=1:size(I)-2 // -2 to ignore last return+EOL
  C=cat_code(C,instruction2code(I(i),prettyprint));
  C = format_txt(C,I(i),prettyprint,I(i+1));
end

if P.name<>"" then // Not a batch file
  C=cat_code(C,"");
  C=cat_code(C,"endfunction"); // Replace last return
  C=cat_code(C,"");
end
endfunction

