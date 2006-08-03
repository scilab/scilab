lines(0);
MSDOS=(getos()=='Windows');

CurrentDirectory=pwd();
 Directories=["util",
 "percent",
 "algebre",
 "arma",
 "auto",
 "calpol",
 "elem",
 "int",
 "optim",
 "robust",
 "sci2for",
 "signal",
 "statistics",
 "tdcs",
 "texmacs",
 "sparse",
 "gui",
  ];
  
 Directories=[Directories;"SCI/modules/help-tools/macros"];   
 Directories=[Directories;"SCI/modules/scipad/macros"];  
 Directories=[Directories;"SCI/modules/maple/macros"];
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
