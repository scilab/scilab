//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
genlib('libscicos','SCI/modules/scicos/macros');
//------------------------------------
disp('-- Creation of [SCI/modules/scicos/macros/scicos] (Macros) --');
chdir('SCI/modules/scicos/macros/scicos'); 
exec('buildmacros.sce',-1);
//------------------------------------
disp('-- Creation of [SCI/modules/scicos/macros/scicos_blocks] (Macros) --');
chdir('SCI/modules/scicos/macros/scicos_blocks'); 
exec('buildmacros.sce',-1);
//------------------------------------