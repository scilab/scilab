function  C=instruction2code(I,prettyprint)
// Copyright INRIA
// Scilab Project - V. Couvert
// Translate an instruction tlist to Scilab code (called by tree2code)
// Input:
// - I: instruction 'tree'
// - prettyprint: boolean value, if FALSE (default value), generated code is not formated else it is
// Output:
// - C: Scilab code corresponding to I

// Default value
rhs=argn(2)
if rhs==1 then
  prettyprint=%F
end

C=[]

// -----------
// Empty lines
// -----------
if I==list("EOL") then
  C=""
  return
end

// ---------------------------------------------
// Generate code corresponding to a IF-THEN-ELSE
// ---------------------------------------------
if typeof(I)=="ifthenelse" then

  // IF
  C="if "+expression2code(I.expression)+" then"
  [C,indent_space] = format_txt(C,I.then(1),prettyprint); // Add EOL after then if needed and returns indent_space
  for k=1:size(I.then)
    C=cat_code(C,indent_space+instruction2code(I.then(k)))
    if k<size(I.then) then // Add EOL between then statements if needed
      C = format_txt(C,I.then(k),prettyprint,I.then(k+1));
    end
  end
  C = format_txt(C,I.then($),prettyprint); // Add EOL after last then statement if needed

  // ELSEIF
  if size(I.elseifs)<>0 then
    for k=1:size(I.elseifs)
      C=cat_code(C,"elseif "+expression2code(I.elseifs(k).expression)+" then")
      [C,indent_space] = format_txt(C,I.elseifs(k).then(1),prettyprint); // Add EOL after then if needed and returns indent_space
      for l=1:size(I.elseifs(k).then)
        C=cat_code(C,indent_space+instruction2code(I.elseifs(k).then(l)))
        if l<size(I.elseifs(k).then) then // Add EOL between then statements
          C = format_txt(C,I.elseifs(k).then(l),prettyprint,I.elseifs(k).then(l+1));
        end
      end
      C = format_txt(C,I.elseifs(k).then($),prettyprint); // Add EOL after last then statement if needed
    end
  end
  
  // ELSE
  if size(I.else)<>0 then
    C=cat_code(C,"else")
    [C,indent_space] = format_txt(C,I.else(1),prettyprint); // Add EOL after else if needed and returns indent_space
    for k=1:size(I.else)
      C=cat_code(C,indent_space+instruction2code(I.else(k)))
      if k<size(I.else) then // Add EOL between else statements if needed
        C = format_txt(C,I.else(k),prettyprint,I.else(k+1));
      end
    end
    C = format_txt(C,I.else($),prettyprint); // Add EOL after last else statement if needed
  end
  C=cat_code(C,"end")
  C($)=C($)+";"
  return
end

// --------------------------------------------
// Generate code corresponding to a SELECT-CASE
// --------------------------------------------
if typeof(I)=="selectcase" then
  
  // SELECT
  C="select "+expression2code(I.expression(1))
  if size(I.expression)==2 | prettyprint then // Add EOL after select if needed 
    C = cat_code(C,"")
  end

  // CASES
  if size(I.cases)<>0 then
    for k=1:size(I.cases)
      C=cat_code(C,"  case "+expression2code(I.cases(k).expression)+" then")
      [C,indent_space] = format_txt(C,I.cases(k).then(1),prettyprint); // Add EOL after then if needed and returns indent_space
      if indent_space=="  " then // indent_space is modified because indentation differs from others control instructions
	indent_space="    "
      end
      for l=1:size(I.cases(k).then)
        C=cat_code(C,indent_space+instruction2code(I.cases(k).then(l)))
        if l<size(I.cases(k).then) then // Add EOL between then statements if needed
          C = format_txt(C,I.cases(k).then(l),prettyprint,I.cases(k).then(l+1));
        end
      end
      C = format_txt(C,I.cases(k).then($),prettyprint); // Add EOL after last then statement if needed
    end
  end

  // ELSE
  if size(I.else)<>0 then
    C=cat_code(C,"  else")
    [C,indent_space] = format_txt(C,I.else(1),prettyprint); // Add EOL after else if needed and returns indent_space
    if indent_space=="  " then // indent_space is modified because indentation differs from others control instructions
      indent_space="    "
    end
    for k=1:size(I.else)
      C=cat_code(C,indent_space+instruction2code(I.else(k)))
      if k<size(I.else) then // Add EOL between else statements if needed
        C = format_txt(C,I.else(k),prettyprint,I.else(k+1));
      end
    end
    C = format_txt(C,I.else($),prettyprint); // Add EOL after last else statement if needed
  end
  C=cat_code(C,"end")
  C($)=C($)+";"
  return
end

// --------------------------------------
// Generate code corresponding to a WHILE
// --------------------------------------
if typeof(I)=="while" then

  C="while "+expression2code(I.expression)
  [C,indent_space] = format_txt(C,I.statements(1),prettyprint); // Add EOL after while if needed and returns indent_space
  for k=1:size(I.statements)
    C=cat_code(C,indent_space+instruction2code(I.statements(k)))
    if k<size(I.statements) then // Add EOL between statements if needed
      C = format_txt(C,I.statements(k),prettyprint,I.statements(k+1));
    end
  end
  C = format_txt(C,I.statements($),prettyprint); // Add EOL after last statement if needed
  C=cat_code(C,"end")
  C($)=C($)+";"
  return
end

// ------------------------------------
// Generate code corresponding to a FOR
// ------------------------------------
if typeof(I)=="for" then

  C="for "+instruction2code(I.expression)
  [C,indent_space] = format_txt(C,I.statements(1),prettyprint); // Add EOL after while if needed and returns indent_space
  for k=1:size(I.statements)
    C=cat_code(C,indent_space+instruction2code(I.statements(k)))
    if k<size(I.statements) then // Add EOL between statements if needed
      C = format_txt(C,I.statements(k),prettyprint,I.statements(k+1));
    end
  end
  C = format_txt(C,I.statements($),prettyprint); // Add EOL after last statement if needed
  C=cat_code(C,"end")
  C($)=C($)+";"
  return
end

// --------------------------------------
// Generate code corresponding to a EQUAL
// --------------------------------------
if typeof(I)=="equal" then
  
  // Comments
  if typeof(I.expression)=="funcall" then
    if I.expression.name=="comment" then
      I.expression.rhs(1).value=strsubst(I.expression.rhs(1).value,"""""","""")
      I.expression.rhs(1).value=strsubst(I.expression.rhs(1).value,"''''","''")
      C="//"+I.expression.rhs(1).value
      //C($)=C($)+";"
      return
    end
  end

  // Other EQUAL instruction
  if size(I.lhs)==1 then
    if typeof(I.lhs(1))=="variable" then
      if I.lhs(1).name=="ans" then // expression
	C=rhs2code(I.expression)
      else
	RHS=rhs2code(I.expression)
	if size(RHS,"*")==1 then
	  C=I.lhs(1).name+" = "+rhs2code(I.expression)
	else // Multi-line definition
	  C=[I.lhs(1).name+" = "+RHS(1);"     "+RHS(2:$)]
	end
      end
    else // Insertion...
      C=expression2code(I.lhs(1))+" = "+rhs2code(I.expression)
    end
  else
    lhsnames=[]
    for lhsind=1:size(I.lhs)
      lhsnames=[lhsnames,expression2code(I.lhs(lhsind))]
    end
    if strcat(lhsnames,",")<>"" then
      C="["+strcat(lhsnames,",")+"] = "+rhs2code(I.expression)
    else
      C=rhs2code(I.expression)
    end
  end
  C($)=C($)+";"
  return
end

// -------
// Command
// -------
if typeof(I)<>"funcall" then
  disp("instruction2code: bug in macr2tree() !");
  pause
end
C=expression2code(I); 
C($)=C($)+";"
endfunction

