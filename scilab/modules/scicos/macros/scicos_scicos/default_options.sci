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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function options = default_options()

    options=scsoptlist()
    col3d=[0.8 0.8 0.8]
    //if xget('use color')==1 then   //suppose always color screen
    options("3D")=list(%t,33)   // we add gray to color map
    //else
    //  options('3D')=list(%f,0)
    //  col3d=[]
    //end
    options("Background")=[8 1] //white,black
    options("Link")=[1,5] //black,red
    options("ID")=list([4 1 10 1],[4 1 2 1])
    options("Cmap")=col3d

endfunction

function options = scsoptlist(varargin)
    lt = ["scsopt","3D","Background","Link","ID","Cmap"]
    options = tlist(lt,varargin(:))
endfunction

