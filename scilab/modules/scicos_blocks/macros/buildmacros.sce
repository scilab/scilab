//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

lines(0);

if MSDOS then
  // Add SCI/bin to current PATH env
  // not defined by default used to find modelicac
  PATH_env = getenv('PATH');
  setenv('PATH',strsubst(getshortpathname(SCI+'/bin'),'/','\') + pathsep() + PATH_env);
end

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
  exec('buildmacros.sce');
  chdir(CurrentDirScicosBlocks);
end
clear Dim CurrentDirScicosBlocks SubDirsScicosBlocks
