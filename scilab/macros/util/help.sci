function help(key)
  //for compatibility with toolboxes making use of old cat files
  change_old_man()
  INDEX=make_help_index()

  if argn(2)==0 then
    browsehelp(INDEX,"index");
    return
  end
 
  key=stripblanks(key)
  if or(part(key,1)==['(',')','[',']','{','}','%','''','""',':','*','/', ...
		      '\','.','<','>','&','^','|','~','+','-']) then
    key="symbols";
  end
  path=gethelpfile(key)
  if path<>[] then
    browsehelp(path,key)
  else
    apropos(key)
  end
endfunction









