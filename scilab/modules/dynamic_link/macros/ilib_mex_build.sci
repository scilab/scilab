//==========================================
// INRIA 2008
//==========================================
function ilib_mex_build(ilib_name,table,files,libs,makename,ldflags,cflags,fflags)
  
  if ~haveacompiler() then
  	error(_('A Fortran or C compiler is required.'))  
  	return;
  end
  
  [lhs,rhs]=argn(0);
  
  if rhs <= 4 then makename = 'Makelib';end
  if rhs <= 5 then ldflags = ''; end 
  if rhs <= 6 then cflags  = ''; end 
  if rhs <= 7 then fflags  = ''; end 
  
  ilib_build(ilib_name,table,files,libs,makename,ldflags,cflags,fflags,%t);
  
  
endfunction
//==========================================

