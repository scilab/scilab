function y=mtlb_logic(a,op,b)
// Copyright INRIA
// Emulation function for Matlab logical operators
// V.C.

// Because isreal function does not work with Boolean values
  if or(type(a)==[4,6]) then
    a=bool2s(a)
  end
  if or(type(b)==[4,6]) then
    b=bool2s(b)
  end

  // Because <, >, <=, and >= operators do not work with complex values
  if or(op==["<",">","<=",">="]) then
    if ~isreal(a) then
      a=real(a)
    end
    if ~isreal(b) then
      b=real(b)
    end
  end

  if a==[] | b==[] then
    y=[]
  else
    if op=="==" then
      y=a==b
    elseif op=="<" then
      y=a<b
    elseif op==">" then
      y=a>b
    elseif op=="<=" then
      y=a<=b
    elseif op==">=" then
      y=a>=b
    else
      y=a~=b
    end
  end
endfunction
