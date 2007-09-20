//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
SCI=getenv('SCI');
// genmacros necessaire pour le Bootstrap de la compilation des macros
exec('SCI/util/genmacros.sce');
//------------------------------------
SCI=getenv('SCI'); 
TMPDIR=getenv('TMPDIR');
//------------------------------------
stacksize(5000000);
genlib('utillib','SCI/macros/util');
//------------------------------------
