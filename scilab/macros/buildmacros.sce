lines(0);
MSDOS=(getos()=='Windows');

CurrentDirectory=pwd();

 Directories=["util",
 "percent",
 "int",
 "sci2for",
 "tdcs",
  ];
 
 Directories=[Directories;"SCI/modules/linear_algebra/macros"];        
 Directories=[Directories;"SCI/modules/elementaries_functions/macros"];        
 Directories=[Directories;"SCI/modules/gui/macros"];        
 Directories=[Directories;"SCI/modules/statistics/macros"];      
 Directories=[Directories;"SCI/modules/cacsd/macros"];    
 Directories=[Directories;"SCI/modules/signal_processing/macros"];  
 Directories=[Directories;"SCI/modules/sparse/macros"];
 Directories=[Directories;"SCI/modules/polynomials/macros"];      
 Directories=[Directories;"SCI/modules/string/macros"];      
 Directories=[Directories;"SCI/modules/optimization/macros"];    
 Directories=[Directories;"SCI/modules/helptools/macros"];   
 Directories=[Directories;"SCI/modules/scipad/macros"];  
 Directories=[Directories;"SCI/modules/maple2scilab/macros"];
 Directories=[Directories;"SCI/modules/m2sci/macros"];
 Directories=[Directories;"SCI/modules/m2sci/macros/m2sci"];
 Directories=[Directories;"SCI/modules/m2sci/macros/mtlb"];
 Directories=[Directories;"SCI/modules/pvm/macros"];
 Directories=[Directories;"SCI/modules/graphics/macros"];
 Directories=[Directories;"SCI/modules/metanet/macros"];
 Directories=[Directories;"SCI/modules/sound/macros"];
 Directories=[Directories;"SCI/modules/tclsci/macros"];
 Directories=[Directories;"SCI/modules/time/macros"];
 
 
if %scicos then
Directories=[Directories;
 "SCI/modules/scicos/macros";
 "SCI/modules/scicos/macros/scicos";
 "SCI/modules/scicos/macros/scicos_blocks";];
end

Dim=size(Directories);


for i=1:Dim(1) do 
  chdir(Directories(i));
  disp('-------- Creation of '+Directories(i)+' (Macros) --------');
  exec('buildmacros.sce');
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
