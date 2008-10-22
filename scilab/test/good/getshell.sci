// Allan CORNET INRIA 2005
function shell=getshell()
  if MSDOS then 
    [path,fname,extension]=fileparts(getenv('Comspec','UNKNOW'));
    shell=fname;
  else
    [path,fname,extension]=fileparts(getenv('SHELL','UNKNOW'));
    shell=fname;
  end
endfunction
