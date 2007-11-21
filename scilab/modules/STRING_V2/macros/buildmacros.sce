//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//------------------------------------
genlib('stringlib','SCI/modules/STRING_V2/macros',%f,%t);
//------------------------------------
