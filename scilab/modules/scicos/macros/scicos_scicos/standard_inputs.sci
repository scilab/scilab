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

function [x,y,typ]=standard_inputs(o)
    //get position of inputs ports and clock inputs port for a standard block
    //  the input ports are located on the left (or right if tilded) vertical
    //    side of the block, regularly located from top to bottom
    //  the clock input ports are located on the top horizontal side
    //    side of the block, regularly located from left to right
    xf=60
    yf=40
    graphics=o.graphics
    model=o.model
    orig=graphics.orig;sz=graphics.sz;
    orient=%t;
    style=graphics.style;
    subStr=strstr(style,"flip=");
    if subStr<>"" then
        orient=%f;
    end
    inp=size(model.in,1);clkinp=size(model.evtin,1);
    if orient then
        xo=orig(1)
        dx=-xf/7
    else
        xo=orig(1)+sz(1)
        dx=yf/7
    end

    typ=[];

    //if o.model.sim=='valve' then pause,end
    if inp==0 then
        x=[];y=[];
    else
        y=orig(2)+sz(2)-(sz(2)/(inp+1))*(1:inp)
        x=(xo+dx)*ones(y)
        for k=1:inp
            if o.graphics.in_implicit==[] then
                typ=ones(x)
            else
                if o.graphics.in_implicit(k)=="E" then
                    typ=[typ ones(x(k))]
                elseif  o.graphics.in_implicit(k)=="I" then
                    typ=[typ 2*ones(x(k))]
                end
            end
        end
    end

    if clkinp<>0 then
        x=[x,orig(1)+(sz(1)/(clkinp+1))*(1:clkinp)]
        y=[y,(orig(2)+yf/7+sz(2))*ones(1,clkinp)]
        typ=[typ,-ones(1,clkinp)]
    end
endfunction








