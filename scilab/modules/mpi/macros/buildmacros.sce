if (isdef('genlib') == %f) then
   exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end

genlib('mpilib','SCI/modules/mpi/macros',%f,%t); 

