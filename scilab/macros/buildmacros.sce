lines(0);
MSDOS=(getos()=='Windows');

CurrentDirectory=pwd();

 Directories=["util"];
 
Directories=[Directories;"SCI/modules/arnoldi/macros"];
Directories=[Directories;"SCI/modules/boolean/macros"];
Directories=[Directories;"SCI/modules/cacsd/macros"];
Directories=[Directories;"SCI/modules/core/macros"];
Directories=[Directories;"SCI/modules/data_structures/macros"];
Directories=[Directories;"SCI/modules/differential_equations/macros"];
Directories=[Directories;"SCI/modules/double/macros"];
Directories=[Directories;"SCI/modules/elementaries_functions/macros"];
Directories=[Directories;"SCI/modules/fileio/macros"];
Directories=[Directories;"SCI/modules/graphics/macros"];
Directories=[Directories;"SCI/modules/gui/macros"];
Directories=[Directories;"SCI/modules/helptools/macros"];
Directories=[Directories;"SCI/modules/incremental_link/macros"];
Directories=[Directories;"SCI/modules/integer/macros"];
Directories=[Directories;"SCI/modules/interpolation/macros"];
Directories=[Directories;"SCI/modules/intersci/macros"];
Directories=[Directories;"SCI/modules/io/macros"];
Directories=[Directories;"SCI/modules/Javasci/macros"];
Directories=[Directories;"SCI/modules/jvm/macros"];
Directories=[Directories;"SCI/modules/linear_algebra/macros"];
Directories=[Directories;"SCI/modules/localization/macros"];
Directories=[Directories;"SCI/modules/m2sci/macros"];
Directories=[Directories;"SCI/modules/maple2scilab/macros"];
Directories=[Directories;"SCI/modules/metanet/macros"];
Directories=[Directories;"SCI/modules/mexlib/macros"];
Directories=[Directories;"SCI/modules/optimization/macros"];
Directories=[Directories;"SCI/modules/others/macros"];
Directories=[Directories;"SCI/modules/overloading/macros"];
Directories=[Directories;"SCI/modules/polynomials/macros"];
Directories=[Directories;"SCI/modules/pvm/macros"];
Directories=[Directories;"SCI/modules/randlib/macros"];
Directories=[Directories;"SCI/modules/scilab2fortran/macros"];
Directories=[Directories;"SCI/modules/scipad/macros"];
Directories=[Directories;"SCI/modules/signal_processing/macros"];
Directories=[Directories;"SCI/modules/sound/macros"];
Directories=[Directories;"SCI/modules/sparse/macros"];
Directories=[Directories;"SCI/modules/special_functions/macros"];
Directories=[Directories;"SCI/modules/statistics/macros"];
Directories=[Directories;"SCI/modules/string/macros"];
Directories=[Directories;"SCI/modules/symbolic/macros"];
Directories=[Directories;"SCI/modules/tclsci/macros"];
Directories=[Directories;"SCI/modules/texmacs/macros"];
Directories=[Directories;"SCI/modules/tfds/macros"];
Directories=[Directories;"SCI/modules/time/macros"];
Directories=[Directories;"SCI/modules/wintools/macros"];
 
if %scicos then
Directories=[Directories;"SCI/modules/scicos/macros"];
end
Dim=size(Directories);


for i=1:Dim(1) do 
  chdir(Directories(i));
  disp('-- Creation of ['+Directories(i)+'] (Macros) --');
  exec('buildmacros.sce');
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
