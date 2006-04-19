// must be run from this directory
ilib_name  = 'libtoolbox_example'     // interface library name
files = ['intfun1.o', 'intsumab.o'];  // objects files
libs  = ["../src/libtoolboxsrc"]                 // other libs needed for linking
table = [ 'scifun1', 'intfun1';
               'scisum','intsumab'];        // table of (scilab_name,interface-name)
// do not modify below
ilib_build(ilib_name,table,files,libs)
