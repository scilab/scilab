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

function PlaceinBrowser_()
  Cmenu=[]
  %superpath='root,'+strcat(string(super_path),',')
  %superpath1='root,'+strcat(string(super_path(1)),',')
  TCL_EvalStr('.scsTree.t selection clear')  
  TCL_EvalStr('.scsTree.t opentree '+%superpath1)
  TCL_EvalStr('.scsTree.t selection add '+%superpath)
  TCL_EvalStr('.scsTree.t see '+%superpath)
endfunction
