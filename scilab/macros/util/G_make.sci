function [res]=G_make(files,objects_or_dll)
// Copyright INRIA
// call make for target files or objects depending 
// on OS and compilers
if getenv('WIN32','NO')=='OK' then 
  if typeof(objects_or_dll)<>'string' then error('G_make: objects must be a string');
    return;
  end 
  if with_lcc() ==%T then
  	host('make -f Makefile.lcc '+objects_or_dll);
 else if COMPILER=='VC++' then 
    // scilab was build with VC++ 
    host('nmake /nologo /f Makefile.mak '+objects_or_dll);
  else if COMPILER=='ABSOFT' then 
      // scilab was build with ABSOFT 
      host('amake /f Makefile.amk '+objects_or_dll); 
    else  
      // Scilab was built with gcwin32 
      host('make '+objects_or_dll);
    end
  end
  end
  res=[objects_or_dll];
else 
  mk=[]
  for x=files(:)', if strindex(x,'-l')==[], mk=mk+' '+x ; end ;end 
  host('make '+ mk);
  res=files ;
end 

endfunction
