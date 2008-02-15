
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
