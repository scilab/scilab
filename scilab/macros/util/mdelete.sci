function mdelete(filename)
// Copyright INRIA
// Scilab Project - V. Couvert
// Delete a file given by filename (or a set of files if meta-characters used in filename) 
rhs=argn(2)
if rhs<>1 then
  error(77);
end
if type(filename)<>10 | size(filename,"*")<>1 then
  error("Wrong type argument, expecting a character string");
end

if MSDOS then
  cmd="del """+pathconvert(filename,%F)+"""";
else
  cmd="rm -f """+pathconvert(filename,%F)+"""";
end
unix_w(cmd);
endfunction

