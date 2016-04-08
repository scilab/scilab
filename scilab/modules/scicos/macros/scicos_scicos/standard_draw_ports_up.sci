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

function standard_draw_ports_up(o)

    nin=size(o.model.in,1);

    inporttype=o.graphics.in_implicit
    nout=size(o.model.out,1);
    outporttype=o.graphics.out_implicit
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    xset("pattern",default_color(0));

    // set port shape

    out2=[0  -1
    -1  -1
    -1   1
    0   1]*diag([xf/7,yf/14])

    in2= [1  -1
    0  -1
    0   1
    1   1]*diag([xf/7,yf/14])


    dy=sz(2)/3
    xset("pattern",default_color(1))
    xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*2),"lines",1)
    xpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
    in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*2),"lines",1)
    xfpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1)
    xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
    in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1)

endfunction
