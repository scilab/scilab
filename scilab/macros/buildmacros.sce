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
 "mtlb",
 "optim",
 "robust",
 "sci2for",
 "signal",
 "statistics",
 "tdcs",
 "texmacs",
 "sparse",
 "gui",
 "m2sci",
 "pvm"];
 
 if MSDOS then
   Directories=[Directories;"SCI/modules/graphics/macros"];
   Directories=[Directories;"SCI/modules/metanet/macros"];
   Directories=[Directories;"SCI/modules/sound/macros"];
   Directories=[Directories;"SCI/modules/tclsci/macros"];
   Directories=[Directories;"SCI/modules/time/macros"];
 else
   Directories=[Directories;"xdess"];
   Directories=[Directories;"metanet"];
   Directories=[Directories;"sound"];
   Directories=[Directories;"time"];
   Directories=[Directories;"tksci"];
 end
 
if %scicos then
Directories=[Directories;
 "scicos";
 "scicos_blocks"];

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
