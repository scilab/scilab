function apropos(key)
  if argn(2)<>1 then error(39), end
  help;
  help_apropos(key);
endfunction 
