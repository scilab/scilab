// This is the builder.sce
// must be run from this directory

lines(0);

ilib_name  = "lib_optional_parameters";

files = ["optional_parameters.c"];

libs  = [];

table =["optional_parameters", "sci_optional_parameters"];

ldflags = "";
cflags  = "";
fflags  = "";

// do not modify below
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs,"",ldflags,cflags,fflags);
