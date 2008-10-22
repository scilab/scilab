//-----------------------------------------------------------------------------
function [res]=G_make(files,objects_or_dll)
// Copyright INRIA
// Updated by Allan CORNET INRIA 2006
// call make for target files or objects depending 
// on OS and compilers
//-----------------------------------------------------------------------------
  if getenv('WIN32','NO')=='OK' then 
    if typeof(objects_or_dll)<>'string' then 
      error('G_make: objects must be a string');
      return;
    end 
    if with_lcc() ==%T then
      host('make -f Makefile.lcc '+objects_or_dll);
    elseif COMPILER=='VC++' then 
      // scilab was build with VC++ 
      vcvompilerversion=findmsvccompiler();
      if (vcvompilerversion=='msvc80express') | (vcvompilerversion=='msvc80pro') | (vcvompilerversion=='msvc80std') then
        host('nmake /Y /nologo /f Makefile.mak '+objects_or_dll);
      else
        host('nmake /nologo /f Makefile.mak '+objects_or_dll);
      end
      
    else  
      // Scilab was built with gcwin32 
      host('make '+objects_or_dll);
    end
    res=[objects_or_dll];
  else 
    mk=[]
    for x=files(:)', if strindex(x,'-l')==[], mk=mk+' '+x ; end ;end 
    host('make '+ mk);
    res=files ;
  end 
  
endfunction
//-----------------------------------------------------------------------------
