// Allan CORNET
// INRIA 2007

cd 'SCI/modules/mexlib/examples/mexdll'

if (findmsvccompiler() <> 'unknown') then
 unix_w('lib /def:xtimesy.def');
 unix_w('nmake /f makelib.mak all');
 
 addinter('libtst.dll','libtst','xtimesy')
 xtimesy(2,3)
  
end
