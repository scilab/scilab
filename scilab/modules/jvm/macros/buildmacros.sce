//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
if (isdef("genlib") == %f) then
    exec(SCI+"/modules/functions/scripts/buildmacros/loadgenlib.sce");
end
//------------------------------------
genlib("jvmlib","SCI/modules/jvm/macros",%f,%t);
//------------------------------------
