// This is the builder.sce
// must be run from this directory
            
ilib_name  = 'libmex'           // interface library name

// objects files (but do not give mexfiles here)
 
files = ['temptst2.o'];
 
// other libs needed for linking (must be shared library names)
 
libs  = []
 
// table of (scilab_name,interface-name or mexfile-name, type)
 
table =['square','temptst','cmex'];

if ~MSDOS then 
  if part(getenv('OSTYPE','no'),1:6)=='darwin' then 
	  cflags = ""
	  fflags = ""; 
	  ldflags= ""; 
	  cc = "g++";
  else 
    // Since linking is done by gcc and not g++ 
    // we must add the libstdc++ to cflags 
    // an other possibility would be to use cflags="" and cc="
    cflags = " -lstdc++" 
    fflags = ""; 
    ldflags= ""; 
    cc="";
  end	
else 
  cflags = "" 
  fflags = ""; 
  ldflags= ""; 
  cc = "";
end
 
// do not modify below
// ----------------------------------------------
ilib_mex_build(ilib_name,table,files,libs,'Makelib',ldflags,cflags,fflags)




