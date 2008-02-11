//  Scicos
//
//  Copyright (C) INRIA - Allan CORNET
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
CurrentDirScicos=pwd();
 SubDirsScicos = ["scicos_auto",
		    "scicos_utils",
		    "scicos_scicos",
		 "scicos_menus"];

Dim=size(SubDirsScicos);
for i=1:Dim(1) do
  chdir(SubDirsScicos(i));
  exec('buildmacros.sce');
  chdir(CurrentDirScicos);
end
clear Dim CurrentDirScicos SubDirsScicos
