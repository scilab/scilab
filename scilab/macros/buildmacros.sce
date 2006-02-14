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
 "metanet",
 "mtlb",
 "optim",
 "robust",
 "sci2for",
 "signal",
 "sound",
 "statistics",
 "tdcs",
 "texmacs",
 "tksci",
 "xdess",
 "sparse",
 "scicos",
 "m2sci",
 "scicos_blocks",
 "gui"];
 
Dim=size(Directories);
timer();

for i=1:Dim(1) do 
  chdir(Directories(i));
  disp('-------- Creation of '+Directories(i)+' (Macros) --------');
  exec('buildmacros.sce');
  chdir(CurrentDirectory);
end
clear CurrentDirectory Dim Directories
exit
