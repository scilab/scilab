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

function source_draw_ports(o)

    nin=size(o.model.in,1);

    inporttype=o.graphics.in_implicit
    //inporttype=ones(nin,1)
    //if size(o.model.in,2)>1 then inporttype=o.model.in(:,2),end
    //
    nout=size(o.model.out,1);
    outporttype=o.graphics.out_implicit
    //outporttype=ones(nout,1)
    //if size(o.model.out,2)>1 then outporttype=o.model.out(:,2),end

    clkin=size(o.model.evtin,1);
    clkout=size(o.model.evtout,1);

    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    orient=%t;
    style=graphics.style;
    subStr=strstr(style,"flip=");
    if subStr<>"" then
        orient=%f;
    end
    xset("pattern",default_color(0));

    //xset('thickness',1)
    // draw input/output ports
    //------------------------

    //if o.model.sim=='inimpl'  then pause, end
    if orient then  //standard orientation
        // set port shape

        in1= [-1  1
        0   0
        1   1
        -1   1]*diag([xf/14,yf/7])

        in2= [-1  -1
        0   0
        -1   1
        -1  -1]*diag([xf/7,yf/14])

        in3= [1  -1
        0   0
        -1  -1
        1  -1]*diag([xf/14,yf/7])

        out2=[ 0  -1
        1  -1
        1   1
        0   1]*diag([xf/7,yf/14])

        dy=sz(2)/(nout+1)
        xset("pattern",default_color(1))
        xpoly(out2(:,1)+(orig(1)+sz(1)),..
        out2(:,2)+(orig(2)+sz(2)/2),"lines",1)

        dy=sz(2)/(nin+1)
        for k=1:nin
            if inporttype==[] then
                xfpoly(in1(:,1)+ones(4,1)*orig(1),..
                in1(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
            else
                if inporttype(k)=="E" then
                    if k==1 then
                        xfpoly(in1(:,1)+ones(4,1)*(orig(1)+2*sz(1)/7),..
                        in1(:,2)+ones(4,1)*(orig(2)+sz(2)),1)
                    elseif k==2 then
                        xfpoly(in2(:,1)+ones(4,1)*(orig(1)-1),..
                        in2(:,2)+ones(4,1)*(orig(2)+sz(2)/2),1)
                    elseif k==3 then
                        xfpoly(in3(:,1)+ones(4,1)*(orig(1)+2*sz(1)/7),..
                        in3(:,2)+ones(4,1)*(orig(2)),1)
                    end
                elseif inporttype(k)=="I" then
                    xfpoly(in2(:,1)+ones(4,1)*orig(1),..
                    in2(:,2)+ones(4,1)*(orig(2)+sz(2)/2),1)
                end
            end
        end
    else //tilded orientation

        in1= [-1  1
        0   0
        1   1
        -1   1]*diag([xf/14,yf/7])


        in2= [1  1
        0   0
        1  -1
        1   1]*diag([xf/7,yf/14])

        in3= [1  -1
        0   0
        -1  -1
        1  -1]*diag([xf/14,yf/7])

        out2=[0  -1
        -1  -1
        -1   1
        0   1]*diag([xf/7,yf/14])


        dy=sz(2)/(nout+1)
        xset("pattern",default_color(1))

        xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
        out2(:,2)+ones(4,1)*(orig(2)+sz(2)/2),"lines",1)

        dy=sz(2)/(nin+1)
        for k=1:nin
            if inporttype==[] then
                xfpoly(in1(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
                in1(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
            else
                if inporttype(k)=="E" then
                    if k==1 then
                        xfpoly(in1(:,1)+ones(4,1)*(orig(1)+5*sz(1)/7),..
                        in1(:,2)+ones(4,1)*(orig(2)+sz(2)),1)
                    elseif k==2 then
                        xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1)),..
                        in2(:,2)+ones(4,1)*(orig(2)+sz(2)/2),1)
                    elseif k==3 then
                        xfpoly(in3(:,1)+ones(4,1)*(orig(1)+5*sz(1)/7),..
                        in3(:,2)+ones(4,1)*(orig(2)),1)
                    end
                elseif inporttype(k)=="I" then
                    xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
                    in2(:,2)+ones(4,1)*(orig(2)+sz(2)/2),1)
                end
            end
        end
    end
endfunction
