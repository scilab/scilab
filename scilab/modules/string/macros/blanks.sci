//=============================================
// blanks macro
// blanks(n)
// Create string of blank characters
// Allan CORNET
// INRIA 2007
//=============================================
function str = blanks(n)
  if ~or(type(n)==[1,8]) then
    error(44,1)
  end
  if n>0 then
    str=part(" ",ones(1,n))
  elseif n==0
    str=""
  else
    error(116,1);
  end
endfunction
//=============================================



