//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef("genlib") == %f) then
    exec(SCI+"/modules/functions/scripts/buildmacros/loadgenlib.sce");
end
//------------------------------------
genlib("optimizationlib","SCI/modules/optimization/macros",%f,%t);
genlib("neldermeadlib","SCI/modules/optimization/macros/neldermead",%f,%t);
genlib("optimbaselib","SCI/modules/optimization/macros/optimbase",%f,%t);
genlib("optimsimplexlib","SCI/modules/optimization/macros/optimsimplex",%f,%t);
//------------------------------------
