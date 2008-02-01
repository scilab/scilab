// =============================================================================
// Copyright (c) INRIA - Pierre MARECHAL - 2008
// =============================================================================

if ~isdef("genlib") then
	exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end

lines(0);

CurrentDirectory_metanet = pwd();
Directories_metanet      = ["editor","graph_tools"];
Dim                      = size(Directories_metanet,"*");

stacksize(5000000);

for i=1:Dim do
	chdir(Directories_metanet(i));
	exec("buildmacros.sce");
	chdir(CurrentDirectory_metanet);
end

clear CurrentDirectory_metanet Directories_metanet Dim
