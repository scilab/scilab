// This is the builder.sce
// must be run from this directory

lines(0);

ilib_name  = "lib_create_list";

files = ["create_list.c"];

libs  = [];

table =["create_list", "sci_create_list"];

ldflags = "";
cflags  = "";
fflags  = "";

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs,"",ldflags,cflags,fflags);
