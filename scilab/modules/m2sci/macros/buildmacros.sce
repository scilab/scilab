//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
genlib('libm2sci','SCI/modules/m2sci/macros');
//------------------------------------
mprintf(" -- Creation of [SCI/modules/m2sci/macros/m2sci] (Macros) --\n");
chdir('SCI/modules/m2sci/macros/m2sci');
exec('buildmacros.sce',-1);
//------------------------------------
mprintf(" -- Creation of [SCI/modules/m2sci/macros/mtlb] (Macros) --\n");
chdir('SCI/modules/m2sci/macros/mtlb');
exec('buildmacros.sce',-1);
//------------------------------------
