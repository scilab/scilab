// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function comet3d(varargin)
    //Comet-like trajectory.
    //   comet(y) displays an animated comet plot of the vector y.
    //   comet(x,y) displays an animated comet plot of vector y vs. x.
    //   comet(x,y,p) uses a comet of length p*size(y,'*').  Default is p = 0.1.

    //   Example:
    //       t = linspace(-%pi,%pi,500);
    //       clf();comet3d(sin(5*t),sin(t),t^2)
    //
    //       function z=traj(x,y),z=1.5*sin(x^2)*cos(y),endfunction
    //       clf();comet3d(cos(t),sin(t),traj)
    //
    nv=size(varargin)
    if nv>=3&varargin(nv-1)=="colors" then
        c=round(varargin(nv))
        if type(c)<>1|~isreal(c) then
            error(msprintf(_("%s: Wrong type for argument #%d: Real vector expected.\n"),"comet3d",nv))
        end
        varargin=list(varargin(1:$-2))
    else
        c=[]
    end
    select size(varargin)

    case 1 then //z
        z=varargin(1)
        if or(size(z)==1) then
            x=1:size(z,"*")
        else
            x=1:size(z,1)
        end
        y=x
        p=0.1
    case 3 then  //x,y,z
        [x,y,z]=varargin(1:3)
        p=0.1
    case 4 then  //x,y,z,p
        [x,y,z,p]=varargin(1:4)
    else
        error(msprintf(_("%s: Wrong number of input arguments: %d or %d to %d expected.\n"),"comet3d",1,3,4))
    end


    if type(x)<>1|~isreal(x) then
        error(msprintf(_("%s: Wrong type for argument #%d: Real vector expected.\n"),"comet3d",1))
    end
    if type(y)<>1|~isreal(x) then
        error(msprintf(_("%s: Wrong type for argument #%d: Real vector expected.\n"),"comet3d",1))
    end

    if (type(z)<>1|~isreal(z))&and(type(z)<>[11 13]) then
        error(msprintf(_("%s: Wrong type for argument #%d: Real vector expected.\n"),"comet3d",3))
    end


    if or(type(z)==[11 13]) then
        x=x(:);y=y(:)
        n=size(x,"*")
        m=1
        if n<>size(y,"*") then
            error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"comet3d",1,2))
        end
        prot=funcprot();funcprot(0)
        zz=z;
        z=zeros(n,1);
        for i=1:n
            z(i)=zz(x(i),y(i))
        end
        funcprot(prot)
    else
        if or(size(z)==1) then
            m=1
            z=z(:)
            n=size(z,"*")
        else
            [n,m]=size(z)
        end
        if or(size(x)==1) then
            x=x(:)
            if size(x,"*")<>n then
                error(msprintf(_("%s: Wrong size for argument #%d: %d expected.\n"),"comet3d",1,n))
            end
            x=x*ones(1,m)
        else
            if or(size(x)<>size(z)) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"comet3d",1,3))
            end
        end
        if or(size(y)==1) then
            y=y(:)
            if size(y,"*")<>n then
                error(msprintf(_("%s: Wrong size for argument #%d: %d expected.\n"),"comet3d",2,n))
            end
            y=y*ones(1,m)
        else
            if or(size(y)<>size(z)) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"comet3d",2,3))
            end

        end
    end


    if type(p)<>1|~isreal(p)|size(p,"*")>1 then
        error(msprintf(_("%s: Wrong type for argument #%d: Real scalar expected.\n"),"comet3d",3))
    end
    if p<0|p>=1 then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"),"comet3d",3,"[0 1["))
    end
    fig=gcf();
    if c==[] then
        c=1:m
    else
        if size(c,"*")<>m then
            error(msprintf(_("%s: Wrong size for argument #%d: %d expected.\n"),"comet",nv,m))
        end
        if min(c)<1|max(c)>size(fig.color_map,1) then
            error(msprintf(_( "%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"comet",nv,"1,...,"+string(size(fig.color_map,1))))
        end
    end
    axes=gca();
    axes.view="3d"


    if axes.children==[] then
        axes.data_bounds=[min(x) min(y) min(z);max(x) max(y) max(z)];
        axes.axes_visible="on";
        axes.box="on";
    else
        axes.data_bounds=[min(axes.data_bounds(1,:), [min(x) min(y) min(z)]);
        max(axes.data_bounds(2,:), [max(x) max(y) max(z)])];
    end
    //create the head, body and tail polylines
    drawlater()
    for l=1:m
        xpoly([],[]);tail(l)=gce();
        tail(l).foreground=c(l);

        xpoly([],[]);body(l)=gce();
        body(l).foreground=c(l); body(l).thickness=2;
        xpoly([],[],"marks");head(l)=gce();
        head(l).mark_size_unit="point";
        head(l).mark_size=6;
        head(l).mark_style=9;
        head(l).mark_foreground=c(l);
    end
    show_window();

    function anim()
        //animation loop
        k = round(p*n);
        step=ceil(n/200); //used to speed up the drawing

        for i=1:n
            for l=1:m
                head(l).data=[x(i,l),y(i,l),z(i,l)];
                if i<=k then
                    body(l).data= [body(l).data;[x(i,l),y(i,l),z(i,l)]];
                else
                    body(l).data= [body(l).data(2:$,:);[x(i,l),y(i,l),z(i,l)]];
                    tail(l).data=[ tail(l).data;[x(i-k,l),y(i-k,l),z(i-k,l)]];
                end
            end
            if modulo(i,step)==0 then
                fig.immediate_drawing = "on"
                fig.immediate_drawing = "off"
            end
        end
        drawnow(),drawlater()

        for i=n:n+k
            for l=1:m
                body(l).data= body(l).data(2:$,:);
                tail(l).data=[ tail(l).data;[x(i-k,l),y(i-k,l),z(i-k,l)]];
            end
            if modulo(i,step)==0 then
                fig.immediate_drawing = "on"
                fig.immediate_drawing = "off"
            end
        end
        delete(body)
        drawnow()
    endfunction
    //not to generate an error message if the window is closed
    exec(anim, "errcatch", -1)
    //exec(anim,-1)
endfunction
