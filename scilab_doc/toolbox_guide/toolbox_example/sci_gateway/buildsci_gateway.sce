// must be run from this directory
ilib_name  = 'libmytoolbox'     // interface library name
files = ['sci_fun.o', 'sci_sumab.o'];  // objects files
libs  = ["../src/libmytoolboxsrc"]                 // other libs needed for linking
table = [ 'fun', 'sci_fun';
        'sumab','sci_sumab'];        // table of (scilab_name,interface-name)
// do not modify below
ilib_build(ilib_name,table,files,libs)