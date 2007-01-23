lines(0);
CurrentDirectoryScicosBlocks=pwd();
 SubDirectoriesScicosBlocks=["Branching",
		    "Events",
		    "Linear",
		    "Misc",
		    "NonLinear",
		    "Sinks",
		    "Sources",
		    "Threshold",
		    "Hydraulics",
		    "Electrical",
		    "PDE"];
 
Dim=size(SubDirectoriesScicosBlocks);
for i=1:Dim(1) do 
  chdir(SubDirectoriesScicosBlocks(i));
  mprintf(" -- Creation of [%s] (Macros) for scicos_blocks --\n",SubDirectoriesScicosBlocks(i));
  exec('buildmacros.sce');
  chdir(CurrentDirectoryScicosBlocks);
end
clear Dim CurrentDirectoryScicosBlocks SubDirectoriesScicosBlocks
