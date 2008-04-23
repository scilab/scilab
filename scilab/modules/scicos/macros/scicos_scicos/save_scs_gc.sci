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

function scs_gc=save_scs_gc()
// save the current scicos graphic context
// and the curent scale ( xsetech)
vals=["alufunction";"pattern";"thickness";
    "pixmap";"background";"foreground";"dashes"];
[r1,r2]=xgetech()
scs_gc=list(xget('window'),list(r1,r2),driver());
for i=1:prod(size(vals)),
  scs_gc($+1)=list(vals(i),xget(vals(i)));
end
endfunction
