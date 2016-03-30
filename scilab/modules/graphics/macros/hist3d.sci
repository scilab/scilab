// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function hist3d(f,theta,alpha,leg,flags,ebox)
    //!
    nep=8

    if ~isdef("theta","local") then theta = 35; end;
    if ~isdef("alpha","local") then alpha = 45; end;
    if ~isdef("leg","local") then leg = "X@Y@Z"; end;
    if ~isdef("flags","local") then flags = [2 1 4]; end;

    def=list(theta,alpha,leg,flags);
    [lhs,rhs]=argn(0)
    if rhs<=0 then  //demo
        towns = ["Agen" "Bastia" "Chamonix" "Cognac" "Hyères" "Le Mans" "Le Puy" ..
        "Lille" "Lorient" "Mende" ]
        months = [_("January") _("Febuary") _("March") _("April") _("May") _("June")..
        _("July") _("August") _("September") _("October") _("November") ..
        _("December") _("Average") ]
        months = "$\scalebox{1.3}{\rotatebox{90}{\mbox{" + months + "}}}$"

        T = [5.7 5.9 9.8 12.1 16.0 19.4 21.8 21.5 18.4 14.7 9.3 5.2 13.5
        9.4 9.4 11.2 13.3 17.3 21.0 24.1 24.5 21.2 17.6 13.3 10.4 16.1
        -2.3 -0.8 3.0 6.6 11.2 14.4 15.5 15.9 12.5 8.6 2.7 -1.6 7.2
        6.1 5.9 9.7 11.9 15.8 19.0 21.1 20.9 17.9 14.4 9.3 5.6 13.3
        8.7 8.9 11.0 13.1 16.7 20.3 23.2 23.3 20.2 16.8 12.6 9.8 15.4
        5.0 5.5 8.3 10.7 14.5 17.8 19.9 19.7 18.5 12.8 8.1 5.4 12.8
        1.0 1.3 4.8 7.2 11.3 14.3 17.6 17.1 13.5 9.9 4.6 1.8 8.8
        3.7 4.1 7.1 9.8 13.5 16.2 18.6 18.5 15.5 11.7 7.2 4.2 10.8
        6.6 6.7 8.6 10.3 13.6 16.2 18.1 18.0 16.0 13.0 9.4 7.0 12.0
        0.6 1.3 3.7 5.5 9.6 13.1 16.2 16.0 12.8 8.8 3.8 1.8 7.8
        ];
        hist3d(T)
        ax = gca()
        ax.y_ticks = tlist(["ticks" "locations" "labels"], (0:12)+0.5, months)
        ax.x_ticks = tlist(["ticks" "locations" "labels"], (0:9)+0.5, towns)
        zlabel("T [°C]")
        xtitle(_("Average monthly temperatures in french cities"),"","")
        ax.title.font_size = 3
        ax.rotation_angles = [28 19]
        return;
    end
    if typeof(f)=="list" then
        [f,x,y]=f(1:3);
        sx=prod(size(x));
        sy=prod(size(y));
        if [sx-1,sy-1]<>size(f) then
            write(%io(2),"f: Incompatible length ");
            return;
        end
        dx=(x(2)-x(1))/nep;
        dy=(y(2)-y(1))/nep;
        bnds=[x(1) x($), y(1) y($),min(0,min(f)) max(0,max(f))]
        x=(x(1:sx-1)+x(2:sx))/2;
        y=(y(1:sy-1)+y(2:sy))/2;
        [nl,nc]=size(f);
    else
        [nl,nc]=size(f);
        x=(1:nl)-(0.5)*ones(1,nl);
        y=(1:nc)-(0.5)*ones(1,nc);
        dx=1/nep; dy=1/nep;
        bnds=[0 nl,0 nc,min(0,min(f)) max(0,max(f))]
    end
    x=x.*.[1,1] + dx*ones(x).*.[0,1] - dx*ones(x).*.[1,0];
    y=y.*.[1,1] + dy*ones(y).*.[0,1] - dy*ones(y).*.[1,0];
    a=[0;0;1;1]
    b=[0;1;1;0]
    c=[0;0;0;0]
    d=[1;1;1;1]
    ix=[b,b,a,a,c,d];
    iy=[a,a,c,d,b,b];
    indx=ones(1,nc) .*. (ones(1,nl).*.ix +(1:2:2*nl-1).*.ones(ix));
    iy=matrix(iy,24,1);
    //indy=(ones(1,nl).*.iy+(1:2:2*nl-1).*.ones(iy)) .*. ones(1,nc);
    indy=(ones(1,nc).*.iy+(1:2:2*nc-1).*.ones(iy)) .*. ones(1,nl);
    indy=matrix(indy,4,6*nc*nl);
    [nnl,nnc]=size(indx);

    xx=matrix(x(matrix(indx,1,nnl*nnc)),nnl,nnc);
    yy=matrix(y(matrix(indy,1,nnl*nnc)),nnl,nnc);
    zz=matrix(f,1,nl*nc).*.[c,d,b,b,a,a];

    if ~isdef("ebox","local") then ebox = bnds; else "ebox = ebox"; end;
    plot3d(xx,yy,zz,def(1),def(2),def(3),def(4),ebox)

endfunction

