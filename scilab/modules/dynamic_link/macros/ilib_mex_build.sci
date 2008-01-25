//==========================================
// INRIA 2008
//==========================================
function ilib_mex_build(ilib_name,table,files,libs,makename,ldflags,cflags,fflags)
  
  if ~havecompiler() then
  	error(_('A Fortran or C compiler is required.'))  
  end
  
  [lhs,rhs]=argn(0);
  
  if rhs <= 4 then makename = 'Makelib';end
  if rhs <= 5 then ldflags = ''; end 
  if rhs <= 6 then cflags  = ''; end 
  if rhs <= 7 then fflags  = ''; end 
  
  warningmode = warning('query');
  
  // generate the gateway file
  if (warningmode == 'on') then
    write(%io(2),_('   generate a gateway file'));
  end    
  ilib_gen_gateway(ilib_name,table)
  
  // generate a loader file
  if (warningmode == 'on') then
    write(%io(2),_('   generate a loader file'));
  end
  ilib_gen_loader(ilib_name,table);
  
  // generate a Makefile
  if (warningmode == 'on') then
    write(%io(2),sprintf(_('   generate a Makefile: %s'),'Makelib'));
  end
  ilib_gen_Make(ilib_name,table,files,libs,makename,%t,ldflags,cflags,fflags);
  
  // we call make
  if (warningmode == 'on') then
    write(%io(2),_('   running the makefile'));
  end
  ilib_compile(ilib_name,makename,files);
  
endfunction
//==========================================

