//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
SCI=getenv('SCI'); 
TMPDIR=getenv('TMPDIR');
//------------------------------------
genlib('libm2sci','SCI/modules/m2sci/macros');
//------------------------------------
disp('-- Creation of [SCI/modules/m2sci/macros/m2sci] (Macros) --');
chdir('SCI/modules/m2sci/macros/m2sci'); 
exec('buildmacros.sce',-1);
//------------------------------------
disp('-- Creation of [SCI/modules/m2sci/macros/mtlb] (Macros) --');
chdir('SCI/modules/m2sci/macros/mtlb'); 
exec('buildmacros.sce',-1);
//------------------------------------