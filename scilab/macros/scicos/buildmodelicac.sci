function buildmodelicac(path)
  old=pwd()  
  chdir(path)
  if ~MSDOS then
    stat=unix('make depend;make')
    if stat<>0 then 
      disp('Error when building modelica compiler')
    else
      unix_s('cp modelicac ../bin/')
    end
  else
    disp('build of modelica compiler not yet implemented')
  end
  chdir(old)
endfunction
