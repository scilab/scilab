lines(0);
CurrentDirScicosBlocks=pwd();
 SubDirsScicosBlocks=["Branching",
		    "Events",
		    "Linear",
		    "Misc",
		    "NonLinear",
		    "Sinks",
		    "Sources",
		    "MatrixOp",
		    "Threshold",
		    "Hydraulics",
		    "Electrical",
		    "PDE",
		    "IntegerOp"];
 
Dim=size(SubDirsScicosBlocks);
for i=1:Dim(1) do 
  chdir(SubDirsScicosBlocks(i));
  disp('-------- Creation of '+SubDirsScicosBlocks(i)+' (Macros) for scicos_blocks --------');
  exec('buildmacros.sce');
  chdir(CurrentDirScicosBlocks);
end
clear Dim CurrentDirScicosBlocks SubDirsScicosBlocks