//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
genlib('scselectricallib','SCI/modules/scicos/macros/scicos_blocks/Electrical');
//------------------------------------
if MSDOS then
  unix("dir /B *.mo >models");
else
  unix("ls *.mo >models");
end
if with_modelicac() then exec("../../genmoc.sce");end;
//------------------------------------
