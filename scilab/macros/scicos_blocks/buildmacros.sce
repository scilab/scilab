lines(0);
CurrentDirectoryScicosBlocks=pwd();
 SubDirectoriesScicosBlocks=["Branching",
 "Electrical",
 "Events",
 "Hydraulics",
 "Linear",
 "Misc",
 "NonLinear",
 "Sinks",
 "Sources",
 "Threshold"];
 
Dim=size(SubDirectoriesScicosBlocks);
for i=1:Dim(1) do 
  chdir(SubDirectoriesScicosBlocks(i));
  disp('-------- Creation of '+SubDirectoriesScicosBlocks(i)+' (Macros) for scicos_blocks --------');
  exec('buildmacros.sce');
  chdir(CurrentDirectoryScicosBlocks);
end
clear Dim CurrentDirectoryScicosBlocks SubDirectoriesScicosBlocks