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

function [blocks] = bplatform2(blocks,flag)
    //** 07/01/2008 : update for Scilab 5.x
    win = 20000 + curblock();
    if flag<>4 then
        H = scf(win); //** recover the handle :)
    end
    xold=blocks.z
    rpar=blocks.rpar
    plen=rpar(1);
    csiz=rpar(2);
    phi=rpar(3);
    xmin=rpar(4);
    xmax=rpar(5);
    ymin=rpar(6);
    ymax=rpar(7);
    rcirc=1;
    tplat=0.2;

    //** INIT
    if flag==4 then
        H=scf(win)
        clf
        isoview(H, "on")
        Axe=H.children
        Axe.data_bounds=rpar(4:7)

        x0=(xmin+xmax)/2;
        y0=(ymin+ymax)/2;
        XY1=[x0-3, x0+3,x0+3,x0-3,x0-3]
        XY2=[y0, y0,y0-2*tplat,y0-2*tplat,y0]
        xset("color",5)
        xfpoly(XY1,XY2,0)

        xset("color",2)
        xfarc(x0-rcirc/2,y0+6,rcirc,rcirc,0,360*64)

        XY1=[xmin, xmax];
        XY2=[ymin, ymin];
        xpoly(XY1,XY2,"lines",0) //Ground

        xset("color",3)
        ym=y0-2*tplat;
        d0=ym/9;
        XY1=[x0,x0-0.2,x0+0.2 ,x0-0.2 ,x0+0.2, x0-0.2 ,x0+0.2 ,x0-0.2 , x0+0.2,x0     ];
        XY2=[ym,ym-d0 ,ym-2*d0,ym-3*d0,ym-4*d0,ym-5*d0,ym-6*d0,ym-7*d0,ym-8*d0,ym-9*d0];
        xpoly(XY1,XY2,"lines",0)

        xset("color",0)

        //** UPDATE
    elseif flag==2 then
        Axe=H.children
        yplat=blocks.inptr(1)(1)
        yball=blocks.inptr(2)(1)

        x0 = (xmin+xmax)/2;
        ym = yplat-2*tplat;
        d0 = ym/9;
        XY2 = [ym,ym-d0 ,ym-2*d0,ym-3*d0,ym-4*d0,ym-5*d0,ym-6*d0,ym-7*d0,ym-8*d0,ym-9*d0];
        Axe.children(1).data(:,2)=XY2';// Resort
        //Axe.children(2).data(2)=;// Ground
        Axe.children(3).data(2)=yball+rcirc;// Ball
        Axe.children(4).data(:,2)=[yplat, yplat,yplat-2*tplat,yplat-2*tplat,yplat]';// platform
    end
endfunction

