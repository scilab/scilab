function apropos(key)
  [lhs,rhs]=argn(0);
  if rhs<>1 then error(39), end
  help(key,'apropos');
endfunction 
