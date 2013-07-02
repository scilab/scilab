//------------------------------------
// Allan CORNET INRIA 2007
//------------------------------------
if (isdef("genlib") == %f) then
    exec(SCI+"/modules/functions/scripts/buildmacros/loadgenlib.sce");
end
//------------------------------------
genlib("output_streamlib","SCI/modules/output_stream/macros",%f,%t);
//------------------------------------
