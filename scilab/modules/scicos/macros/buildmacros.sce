//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
if %scicos then
// load some libraries 
if ~exists('corelib') then load('SCI/modules/core/macros/lib'),end
if ~exists('functionslib') then load('SCI/modules/functions/macros/lib'),end
if ~exists('fileiolib') then load('SCI/modules/fileio/macros/lib'),end
if ~exists('stringlib') then load('SCI/modules/string/macros/lib'),end
if ~exists('overloadinglib') then load('SCI/modules/overloading/macros/lib'),end
if ~exists('iolib') then load('SCI/modules/io/macros/lib'),end
//------------------------------------
genlib('libscicos','SCI/modules/scicos/macros');
//------------------------------------
mprintf(" -- Creation of [SCI/modules/scicos/macros/scicos] (Macros) --\n");
chdir('SCI/modules/scicos/macros/scicos');
exec('buildmacros.sce',-1);
//------------------------------------
mprintf(" -- Creation of [SCI/modules/scicos/macros/scicos_blocks] (Macros) --\n");
chdir('SCI/modules/scicos/macros/scicos_blocks');
exec('buildmacros.sce',-1);
//------------------------------------
end // if %scicos then
//------------------------------------