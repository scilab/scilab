// This is the builder.sce
// must be run from this directory

lines(0);

ilib_name  = "lib_check_properties";

files = ["check_properties.c"];

libs  = [];

table =["check_properties_1", "sci_check_properties_1"; ...
"chprop2",            "sci_check_properties_2"];

// We must be careful when we choose a scilab function name in case of overloading.
// We Scilab name function must be 8 char max.

ldflags = "";
cflags  = "";
fflags  = "";

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs,"",ldflags,cflags,fflags);
