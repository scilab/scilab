// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function plot3d3(x,y,z,vect,theta,alpha,leg,flags,ebox)
    // mesh draw of a solid surface described
    // by a set of points
    // the mesh is drawn using the colums and rows of [x,y,z]
    //---------------------------------------------------------
    [lhs,rhs]=argn(0);
    if rhs<3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "plot3d3", 3) );
    end;
    if exists("vect","local")==0 then vect=-1,end
    if vect<>-1 then
        nobjs=prod(size(vect))+1;
        [rx,cx]=size(x);
        vect1=[0,vect,rx];
        xx=[],yy=[],zz=[];
        for i=1:nobjs;
            xx=[x(vect1(i)+1:vect1(i+1),:),xx]
            yy=[y(vect1(i)+1:vect1(i+1),:),yy]
            zz=[z(vect1(i)+1:vect1(i+1),:);zz]
        end
    else
        xx=x;yy=y;zz=z;
    end

    [n,p]=size(z);
    opts=[]
    if exists("theta","local")==1 then opts=[opts,"theta=theta"],end
    if exists("alpha","local")==1 then opts=[opts,"alpha=alpha"],end
    if exists("leg"  ,"local")==1 then opts=[opts,"leg=leg"]    ,end
    // set default flags
    // flag(1): color of outide lines
    // flag(2): color of inside lines
    // flag(3): scaling type
    // flag(4): box type
    if exists("flags" ,"local")==0 then flags=[3,4,2,4];  ,end
    if exists("ebox" ,"local")==1 then opts=[opts,"ebox=ebox"]  ,end


    execstr("param3d1(xx,yy,list(zz,flags(1)*ones(1,p)),"+..
    strcat([opts "flag=flags(3:4)"],",")+")")

    execstr("param3d1(xx'',yy'',list(zz'',flags(2)*ones(1,n)),"+..
    strcat([opts "flag=[0 flags(4)]"],",")+")")
endfunction
