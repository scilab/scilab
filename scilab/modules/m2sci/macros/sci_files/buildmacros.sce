//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('m2scisci_fileslib','SCI/modules/m2sci/macros/sci_files',%f,%t);
//------------------------------------
