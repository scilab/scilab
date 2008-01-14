// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================

names = ['fsum'];
files = ['fsum.o'];
libs = [];

ilib_for_link(names,files,libs,"f");
// ====================================================================
clear ilib_for_link;
clear ilib_link_gen_Make_lcc;
clear ilib_link_gen_Make_win32;
clear ilib_link_gen_Make_unix;
clear ilib_link_gen_Make;
clear ilib_link_gen_loader;
clear cd;
clear %dir_p;
clear dir;
clear names;
clear files;
clear libs;
// ====================================================================
