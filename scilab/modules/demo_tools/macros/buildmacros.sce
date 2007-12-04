// =============================================================================
// Author : Pierre MARECHAL
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end

genlib('demo_toolslib','SCI/modules/demo_tools/macros',%f,%t);
