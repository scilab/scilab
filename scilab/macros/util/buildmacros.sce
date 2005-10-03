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
genlib('utillib','SCI/macros/util');
//------------------------------------
