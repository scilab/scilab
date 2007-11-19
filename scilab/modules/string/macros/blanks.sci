//=============================================
// blanks macro
// blanks(n)
// Create string of blank characters
// Allan CORNET
// INRIA 2007
//=============================================
function str = blanks(n)
  if type(n) <> 1 then
    error(42)
  end
  if (n(1) > 0) then
    BLANK = ' ';
    str = '';
    for i=1:n,  str = str + BLANK; end;
  else
    error(21);
  end
endfunction
//=============================================
