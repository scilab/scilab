// This is the builder.sce
// must be run from this directory

lines(0);

ilib_name  = "lib_multiply_by_two";

files = ["multiply_by_two.c"];

libs  = [];

table =["multiply_by_two", "sci_multiply_by_two"];

ldflags = "";
cflags  = "";
fflags  = "";

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs,"",ldflags,cflags,fflags);
