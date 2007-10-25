//------------------------------------
// Allan CORNET INRIA 2007
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('spreadsheetlib','SCI/modules/spreadsheet/macros',%f,%t);
//------------------------------------
