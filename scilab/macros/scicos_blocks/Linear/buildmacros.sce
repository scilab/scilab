//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
MSDOS=(getos()=='Windows');
SCI=getenv('SCI'); 
TMPDIR=getenv('TMPDIR');
//------------------------------------
if MSDOS then
  unix('if EXIST *.bin del /F *.bin');
  unix('if EXIST names del /F names');
  unix('if EXIST lib del /F lib');  
else
  unix('rm -f *.bin');
  unix('rm -f names');
  unix('rm -f lib');  
end
//------------------------------------
genlib('scslinearlib','SCI/macros/scicos_blocks/Linear');
//------------------------------------
