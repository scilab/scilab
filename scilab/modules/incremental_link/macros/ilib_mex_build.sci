function ilib_mex_build(ilib_name,table,files,libs,makename,ldflags,cflags,fflags)
// Copyright Enpc 
// 
  [lhs,rhs]=argn(0);
  info=1
  if rhs <= 4 then makename = 'Makelib';end
  if rhs <= 5 then ldflags = ""; end 
  if rhs <= 6 then cflags  = ""; end 
  if rhs <= 7 then fflags  = ""; end 
  // generate the gateway file
  if info==1 then write(%io(2),'   generate a gateway file');end
  ilib_gen_gateway(ilib_name,table)
  // generate a loader file
  if info==1 then write(%io(2),'   generate a loader file');end
  ilib_gen_loader(ilib_name,table);
  // generate a Makefile
  if info==1 then write(%io(2),'   generate a Makefile: Makelib');end
  ilib_gen_Make(ilib_name,table,files,libs,makename,%t,ldflags,cflags,fflags);
  // we call make
  if info==1 then write(%io(2),'   running the makefile');end
  ilib_compile(ilib_name,makename,files);
endfunction

