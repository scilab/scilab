function path=cd(path)
  if argn(2)==1 then 
    path=stripblanks(path)
    if or(path==['SCI' '~']) path=path+'/',end
    chdir(path), 
  else 
    chdir(home);
  end
  path=pwd()
endfunction
