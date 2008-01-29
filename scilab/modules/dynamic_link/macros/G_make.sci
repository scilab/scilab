//==========================================
// INRIA 2008
//==========================================
function [res]=G_make(files,objects_or_dll)

  if ~haveacompiler() then
  	error(_('A Fortran or C compiler is required.'));
  end

  if MSDOS then // WINDOWS
  
    if typeof(objects_or_dll)<>'string' then 
      error(sprintf(_('%s: Wrong type for input argument: String expected.'),'G_make'));
      return;
    end 
    
    if with_lcc() == %T then
      host('make -f Makefile.lcc '+objects_or_dll);
    else
      // scilab uses Visual Studio C++ 
      host('nmake /Y /nologo /f Makefile.mak '+objects_or_dll);
    end
    
    res=[objects_or_dll];
    
  else // LINUX
  
    mk=[]
    for x=files(:)', if strindex(x,'-l')==[], mk=mk+' '+x ; end ;end 
    host('make '+ mk);
    res=files ;
    
  end 
  
endfunction
//==========================================
