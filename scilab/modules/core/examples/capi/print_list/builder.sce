// This is the builder.sce
// must be run from this directory

lines(0);

ilib_name  = "lib_print_list";

files = ["print_list.c"];

libs  = [];

table =["print_list", "sci_print_list"];

ldflags = "";
cflags  = "";
fflags  = "";

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs,"",ldflags,cflags,fflags);
