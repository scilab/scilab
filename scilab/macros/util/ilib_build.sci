function ilib_build(ilib_name,table,files,libs,makename)
[lhs,rhs]=argn(0);
info=1
if rhs <= 4 then makename = 'Makelib';end
// generate the gateway file
if info==1 then write(%io(2),'   generate a gateway file');end
ilib_gen_gateway(ilib_name,table)
// generate a loader file
if info==1 then write(%io(2),'   generate a loader file');end
ilib_gen_loader(ilib_name,table);
// generate a Makefile
if info==1 then write(%io(2),'   generate a Makefile: Makelib');end
ilib_gen_Make(ilib_name,files,libs,makename);
// we call make
if info==1 then write(%io(2),'   running the makefile');end
ilib_compile(ilib_name,makename);














