//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
function [res]=G_make(files,objects_or_dll)

  if ~haveacompiler() then
    error(msprintf(gettext('%s: A Fortran or C compiler is required.\n'),'G_make'))
  end
  
  [lhs,rhs] = argn(0);
  if rhs <> 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"G_make"));
    return
  end

  msg = '';
  
  if MSDOS then // WINDOWS

    if typeof(objects_or_dll)<>'string' then 
      error(sprintf(gettext('%s: Wrong type for input argument #%d: String expected.'),'G_make',2));
      return;
    end 
    
    OBJ = '';
    [ptmp,ftmp,fext] = fileparts(objects_or_dll);
    OBJ = ptmp + ftmp;
    
    if with_lcc() == %T then
      if ilib_verbose() > 1 then
        msg = unix_g('make -f Makefile.lcc '+OBJ);
      else
        host('make -f Makefile.lcc '+OBJ);
      end
    else
      // scilab uses Visual Studio C++ 
      if ilib_verbose() > 1 then
        msg = unix_g('nmake /Y /nologo /f Makefile.mak '+OBJ);
      else
        host('nmake /Y /nologo /f Makefile.mak '+OBJ);
      end
      
    end
    
    res = [OBJ];
    
  else // LINUX
  
    mk = [];
    for x = files(:)', if strindex(x,'-l')==[], mk=mk+' '+x ; end ;end 
    
    if ilib_verbose() > 1 then
      msg = unix_g('make '+ mk);
    else
      host('make '+ mk);
    end
    
    res = files ;
    
  end 
  
  if ilib_verbose() > 1 then
    disp(msg);
  end
  
endfunction
//==========================================
