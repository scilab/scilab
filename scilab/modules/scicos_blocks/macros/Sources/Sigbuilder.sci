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

function [x,y,typ] = Sigbuilder(job,arg1,arg2)
    //** updated for Scilab 5.1 by Simone Mannori
    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        // look for the internal curve block
        ppath = list(0);
        for i=1:length(arg1.model.rpar.objs) do
            o = arg1.model.rpar.objs(i);
            if typeof(o) == "Block" & o.gui == "CURVE_c" then
                ppath(1) = i;
                break;
            end
        end
        newpar = list();
        y = 0;
        for path = ppath do
            np = size(path,"*")
            spath = list()
            for k=1:np
                spath($+1)="model"
                spath($+1)="rpar"
                spath($+1)="objs"
                spath($+1)=path(k)
            end
            xx=arg1(spath) // get the block
            execstr("xxn="+xx.gui+"(''set'',xx)")
            if diffobjs(xxn,xx) then
                model=xx.model
                model_n=xxn.model
                if ~is_modelica_block(xx) then
                    modified=or(model.sim<>model_n.sim)|..
                    ~isequal(model.state,model_n.state)|..
                    ~isequal(model.dstate,model_n.dstate)|..
                    ~isequal(model.odstate,model_n.odstate)|..
                    ~isequal(model.rpar,model_n.rpar)|..
                    ~isequal(model.ipar,model_n.ipar)|..
                    ~isequal(model.opar,model_n.opar)|..
                    ~isequal(model.label,model_n.label)
                    if or(model.in<>model_n.in)|or(model.out<>model_n.out)|..
                        or(model.in2<>model_n.in2)|or(model.out2<>model_n.out2)|..
                        or(model.outtyp<>model_n.outtyp)|or(model.intyp<>model_n.intyp) then
                        needcompile=1
                    end
                    if or(model.firing<>model_n.firing) then
                        needcompile=2
                    end
                    if (size(model.in,"*")<>size(model_n.in,"*"))|..
                        (size(model.out,"*")<>size(model_n.out,"*")) then
                        needcompile=4
                    end
                    if model.sim=="input"|model.sim=="output" then
                        if model.ipar<>model_n.ipar then
                            needcompile=4
                        end
                    end
                    if or(model.blocktype<>model_n.blocktype)|..
                        or(model.dep_ut<>model_n.dep_ut) then
                        needcompile=4
                    end
                    if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then
                        needcompile=4
                    end
                    if prod(size(model_n.sim))>1 then
                        if model_n.sim(2)>1000 then
                            if model.sim(1)<>model_n.sim(1) then
                                needcompile=4
                            end
                        end
                    end
                else
                    modified=or(model_n<>model)
                    eq=model.equations;eqn=model_n.equations;
                    if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..
                        or(eq.outputs<>eqn.outputs) then
                        needcompile=4
                    end
                end
                //parameter or states changed
                arg1(spath)=xxn// Update
                newpar(size(newpar)+1)=path// Notify modification
                y=max(y,needcompile)
            end
        end
        x=arg1
        typ=newpar

    case "define" then
        scs_m_1=scicos_diagram(..
        version="scicos4.2",..
        props=scicos_params(..
        wpar=[600,450,0,0,600,450],..
        Title=["Sigbuilder","./"],..
        tol=[0.0001;0.000001;1.000D-10;100001;0;0;0],..
        tf=100,..
        context=" ",..
        void1=[],..
        options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
        list([5,1],[4,1]),[0.8,0.8,0.8]),..
        void2=[],..
        void3=[],..
        doc=list()))
        scs_m_1.objs(1)=scicos_block(..
        gui="CURVE_c",..
        graphics=scicos_graphics(..
        orig=[329.63473,606.18517],..
        sz=[40,40],..
        exprs=["3";"[0,1,2]";"[10,20,-30]";"y";"n"],..
        pin=[],..
        pout=6,..
        pein=4,..
        peout=2,..
        gr_i=[],..
        id="",..
        in_implicit=[],..
        out_implicit="E"),..
        model=scicos_model(..
        sim=list("curve_c",4),..
        in=[],..
        in2=[],..
        intyp=1,..
        out=1,..
        out2=[],..
        outtyp=1,..
        evtin=1,..
        evtout=1,..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[0;1;2;10;20;-30],..
        ipar=[3;3;1],..
        opar=list(),..
        blocktype="c",..
        firing=0,..
        dep_ut=[%f,%t],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m_1.objs(2)=scicos_link(..
        xx=[349.63473;349.49528],..
        yy=[600.47089;565.10704],..
        id="drawlink",..
        thick=[0,0],..
        ct=[5,-1],..
        from=[1,1,0],..
        to=[3,1,1])
        scs_m_1.objs(3)=scicos_block(..
        gui="CLKSPLIT_f",..
        graphics=scicos_graphics(..
        orig=[349.49528;565.10704],..
        sz=[0.3333333,0.3333333],..
        exprs=[],..
        pin=[],..
        pout=[],..
        pein=2,..
        peout=[8;4],..
        gr_i=[],..
        id="",..
        in_implicit=[],..
        out_implicit=[]),..
        model=scicos_model(..
        sim="split",..
        in=[],..
        in2=[],..
        intyp=1,..
        out=[],..
        out2=[],..
        outtyp=1,..
        evtin=1,..
        evtout=[1;1],..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[],..
        ipar=[],..
        opar=list(),..
        blocktype="d",..
        firing=[%f,%f,%f],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m_1.objs(4)=scicos_link(..
        xx=[349.49528;266.69602;266.69602;270.35525;342.80795;342.80795;349.63473],..
        yy=[565.10704;565.10704;680.99483;680.99483;680.99483;651.89946;651.89946],..
        id="drawlink",..
        thick=[0,0],..
        ct=[5,-1],..
        from=[3,2,0],..
        to=[1,1,1])
        scs_m_1.objs(5)=scicos_block(..
        gui="OUT_f",..
        graphics=scicos_graphics(..
        orig=[398.20616,616.18517],..
        sz=[20,20],..
        exprs="1",..
        pin=6,..
        pout=[],..
        pein=[],..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit="E",..
        out_implicit=[]),..
        model=scicos_model(..
        sim="output",..
        in=-1,..
        in2=-2,..
        intyp=-1,..
        out=[],..
        out2=[],..
        outtyp=1,..
        evtin=[],..
        evtout=[],..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[],..
        ipar=1,..
        opar=list(),..
        blocktype="c",..
        firing=[],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m_1.objs(6)=scicos_link(..
        xx=[378.20616;398.20616],..
        yy=[626.18517;626.18517],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[1,1,0],..
        to=[5,1,1])
        scs_m_1.objs(7)=scicos_block(..
        gui="CLKOUTV_f",..
        graphics=scicos_graphics(..
        orig=[339.49528,505.10704],..
        sz=[20,30],..
        exprs="1",..
        pin=[],..
        pout=[],..
        pein=8,..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit=[],..
        out_implicit=[]),..
        model=scicos_model(..
        sim="output",..
        in=[],..
        in2=[],..
        intyp=1,..
        out=[],..
        out2=[],..
        outtyp=1,..
        evtin=1,..
        evtout=[],..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[],..
        ipar=1,..
        opar=list(),..
        blocktype="d",..
        firing=[],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m_1.objs(8)=scicos_link(..
        xx=[349.49528;349.49528],..
        yy=[565.10704;535.10704],..
        id="drawlink",..
        thick=[0,0],..
        ct=[5,-1],..
        from=[3,1,0],..
        to=[7,1,1])
        model=scicos_model(..
        sim="csuper",..
        in=[],..
        in2=[],..
        intyp=1,..
        out=-1,..
        out2=[],..
        outtyp=1,..
        evtin=[],..
        evtout=1,..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=scs_m_1,..
        ipar=[],..
        opar=list(),..
        blocktype="h",..
        firing=[],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list())
        gr_i=[]

        x=standard_define([3 2],model,[],gr_i)
    end
endfunction


//=========================================================
function [X,Y,orpar]=Do_Spline2(N,order,x,y)

    X=[];Y=[];orpar=[];
    METHOD=getmethod(order);

    if (METHOD=="zero order") then
        X=x(1);Y=y(1);
        for i=1:N-1
            X=[X;x(i);x(i+1);x(i+1)];
            Y=[Y;y(i);y(i);y(i+1)];
        end
        return;
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="linear") then
        X=[];
        for i=1:N
            X=[X;x(i)];
            Y=[Y;y(i)];
        end
        return;
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (N<20) then
        NP=4;
    else
        if (N<40) then
            NP=2;
        else
            if (N<100) then
                NP=1;
            else
                NP=0;
            end;
        end;
    end
    for i=1:N-1
        X=[X;linspace(x(i),x(i+1),NP+2)']; // pour tous sauf "linear" et "zero order"
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (N>2) & (METHOD=="order 2") then
        Z=ORDER2(x,y);
        A=Z(1:N-1);
        B=Z(N:2*N-2);
        C=Z(2*N-1:3*N-3);

        for j=1:size(X,"*")
            for i=N-1:-1:1
                if X(j)>=x(i) then,break;end
            end
            Y(j)=A(i)*(X(j)-x(i))^2+B(i)*(X(j)-x(i))+C(i);
        end
        orpar=matrix(Z,-1,1)
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="not_a_knot") then
        try
            d = splin(x, y, METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end

    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="periodic") then
        if y(1)<>y(N) then
            y(N)=y(1)
        end
        try
            d = splin(x, y,METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="monotone" ) then
        try
            d = splin(x, y, METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end

    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="fast") then
        try
            d = splin(x, y, METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE:  "+METHOD)
        end
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="clamped") then
        try
            d = splin(x, y, METHOD,[0;0]);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end
    end

endfunction

function METHOD=getmethod(order)
    select order
    case 0 then, METHOD="zero order"
    case 1 then, METHOD="linear"
    case 2 then, METHOD="order 2"
    case 3 then, METHOD="not_a_knot"
    case 4 then, METHOD="periodic"
    case 5 then, METHOD="monotone"
    case 6 then, METHOD="fast"
    case 7 then, METHOD="clamped"
    end
endfunction


function [Z]=ORDER2(x,y)
    N=size(x,"*")-1;
    A=zeros(3*N-1,N*3);
    B=zeros(3*N-1,1);
    for i=1:N
        j=3*(i-1)+1;
        A(j,i+2*N)=1;
        B(j)=y(i);
        A(j+1,i)=(x(i+1)-x(i))^2;
        A(j+1,i+N)=x(i+1)-x(i);
        A(j+1,i+2*N)=1;
        B(j+1)=y(i+1);
    end

    for i=1:N-1
        j=3*(i-1)+1;
        A(j+2,i)=2*(x(i+1)-x(i));
        A(j+2,i+N)=1;
        A(j+2,i+N+1)=-1;
    end

    Q=zeros(3*N,3*N);
    for i=1:N
        Q(i,i)=4*(x(i+1)-x(i))^2
        Q(i,i+N)=2*(x(i+1)-x(i))
        Q(i+N,i)=2*(x(i+1)-x(i))
        Q(i+N,i+N)=1;
    end

    At=[Q,A';A,zeros(3*N-1,3*N-1)]
    Bt=[zeros(3*N,1);B]
    Zt=At\Bt;
    Z=Zt(1:3*N,1)
endfunction
//===================================================






