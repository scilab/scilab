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

function [x,y,typ]=vanne_outputs(o)
    xf=60
    yf=40
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    orient=%t;
    style=graphics.style;
    subStr=strstr(style,"flip=");
    if subStr<>"" then
        orient=%f;
    end
    out=size(o.model.out,1);clkout=size(o.model.evtout,1);
    if orient then
        x1=orig(1)
        dx1=-xf/7
        x2=orig(1)+sz(1)
        dx2=xf/7
    else
        x1=orig(1)+sz(1)
        dx1=yf/7
        x2=orig(1)
        dx2=-xf/7
    end

    y=[orig(2)+2*sz(2)/10]
    x=[(x2+dx2) ]
    typ=[2]

endfunction
