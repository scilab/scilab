function path=cd(path)
  if argn(2)==1 then 
    path=stripblanks(path)
    if or(path==['SCI' '~']) path=path+'/',end
    if or(path==['/' '\']) then
      chdir(path)
    else
      if ~isdir(path) then
        error('Can''t go to directory '+path,998);
      end
      chdir(path)
    end 
  else 
    chdir(home);
  end
  path=pwd();
endfunction
