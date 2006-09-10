//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
genlib('scshydraulicslib','SCI/modules/scicos/macros/scicos_blocks/Hydraulics'); 
//------------------------------------
if MSDOS then
  unix("dir /B *.mo >models");
else
  unix("ls *.mo >models");
end
if with_ocaml() then exec("../../genmoc.sce");end;
//------------------------------------
