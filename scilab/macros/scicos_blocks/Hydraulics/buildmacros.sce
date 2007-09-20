//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
MSDOS=(getos()=='Windows');
SCI=getenv('SCI'); 
TMPDIR=getenv('TMPDIR');
//------------------------------------
genlib('scshydraulicslib','SCI/macros/scicos_blocks/Hydraulics'); 
//------------------------------------
//if MSDOS then
//  unix("dir /B *.mo >models");
//else
//  unix("ls *.mo >models");
//end
if with_ocaml() then 
  models=['Bache.mo';'PerteDP.mo';'PortPHQ1.mo';'PortPHQ2.mo ';
          'Puits.mo';'Source.mo';'ThermoCarre.mo';'VanneReglante.mo'];
  exec("../../../util/genmoc.sce");
end;
//------------------------------------
