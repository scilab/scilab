//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
MSDOS=(getos()=='Windows');
SCI=getenv('SCI'); 
TMPDIR=getenv('TMPDIR');
//------------------------------------
genlib('scselectricallib','SCI/macros/scicos_blocks/Electrical');
//------------------------------------
if MSDOS then
  unix("dir /B *.mo >models");
else
  unix("ls *.mo >models");
end
if with_ocaml() then exec("../../../util/genmoc.sce");end;
//------------------------------------
