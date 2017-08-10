//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ] = SRFLIPFLOP(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        // look for the 1/z block
        for i=1:length(arg1.model.rpar.objs) do
            o = arg1.model.rpar.objs(i);
            if typeof(o) == "Block" & o.gui == "DOLLAR_m" then
                path = i;
                break;
            end
        end

        newpar=list()
        xx=arg1.model.rpar.objs(path)// get the 1/z block
        exprs=xx.graphics.exprs(1)
        model=xx.model;
        init_old= model.odstate(1)
        while %t do
            [ok,init,exprs0]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "SRFLIPFLOP" );" "; gettext("SR flip-flop"); " "; ..
            gettext("The ''Initial Value'' must be 0 or 1 of type int8"); gettext("&nbsp;- Negative values are considered as int8(0)"); ..
            gettext("&nbsp;- Positive values are considered as int8(1)"); " "], ..
            gettext("Initial Value"), ..
            list("vec",1), exprs)

            if ~ok then
                break,
            end
            if init<=0 then
                init=int8(0);
            elseif init >0 then
                init=int8(1);
            end
            if ok then
                xx.graphics.exprs(1)=exprs0
                model.odstate(1)=init
                xx.model=model
                arg1.model.rpar.objs(path)=xx// Update
                break
            end
        end
        needcompile=0
        if init_old<>init then
            // parameter  changed
            newpar(size(newpar)+1)=path// Notify modification
            needcompile=2
        end
        x=arg1
        y=needcompile
        typ=newpar
    case "define" then
        scs_m=scicos_diagram(..
        version="scicos4.2",..
        props=scicos_params(..
        wpar=[600,450,0,0,600,450],..
        Title=["SRFLIPFLOP"],..
        tol=[0.0001;0.000001;1.000E-10;100001;0;0;0],..
        tf=60,..
        context=" ",..
        void1=[],..
        options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
        list([5,1],[4,1]),[0.8,0.8,0.8]),..
        void2=[],..
        void3=[],..
        doc=list()))
        scs_m.objs(1)=scicos_block(..
        gui="LOGIC",..
        graphics=scicos_graphics(..
        orig=[298.504,201.45067],..
        sz=[40,40],..
        exprs=["[0 1;1 0;1 0;1 0;0 1;0 1;0 0;0 0]";"1"],..
        pin=[4;10;12],..
        pout=[3;8],..
        pein=[],..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit=["E";"E";"E"],..
        out_implicit=["E";"E"]),..
        model=scicos_model(..
        sim=list("logic",4),..
        in=[1;1;1],..
        in2=[1;1;1],..
        intyp=[5;5;5],..
        out=[1;1],..
        out2=[1;1],..
        outtyp=[5;5],..
        evtin=[],..
        evtout=[],..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[],..
        ipar=[],..
        opar=list(..
        int8([0,1;
        1,0;
        1,0;
        1,0;
        0,1;
        0,1;
        0,0;
        0,0])),..
        blocktype="c",..
        firing=%f,..
        dep_ut=[%t,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m.objs(2)=scicos_block(..
        gui="DOLLAR_m",..
        graphics=scicos_graphics(..
        orig=[299.23733,254.25067],..
        sz=[40,40],..
        exprs=["int8(0)";"1"],..
        pin=6,..
        pout=4,..
        pein=[],..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit="E",..
        out_implicit="E"),..
        model=scicos_model(..
        sim=list("dollar4_m",4),..
        in=1,..
        in2=1,..
        intyp=5,..
        out=1,..
        out2=1,..
        outtyp=5,..
        evtin=[],..
        evtout=[],..
        state=[],..
        dstate=[],..
        odstate=list(int8(0)),..
        rpar=[],..
        ipar=[],..
        opar=list(),..
        blocktype="d",..
        firing=[],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m.objs(3)=scicos_link(..
        xx=[347.07543;363.03733;363.03733],..
        yy=[228.11733;228.11733;248.584],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[1,1,0],..
        to=[5,1,1])
        scs_m.objs(4)=scicos_link(..
        xx=[290.6659;272.104;272.104;289.93257],..
        yy=[274.25067;274.25067;231.45067;231.45067],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[2,1,0],..
        to=[1,1,1])
        scs_m.objs(5)=scicos_block(..
        gui="SPLIT_f",..
        graphics=scicos_graphics(..
        orig=[363.03733,248.584],..
        sz=[0.3333333,0.3333333],..
        exprs=[],..
        pin=3,..
        pout=[6;14],..
        pein=[],..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit="E",..
        out_implicit=["E";"E";"E"]),..
        model=scicos_model(..
        sim="lsplit",..
        in=-1,..
        in2=[],..
        intyp=1,..
        out=[-1;-1;-1],..
        out2=[],..
        outtyp=1,..
        evtin=[],..
        evtout=[],..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[],..
        ipar=[],..
        opar=list(),..
        blocktype="c",..
        firing=[],..
        dep_ut=[%t,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m.objs(6)=scicos_link(..
        xx=[363.03733;363.03733;344.95162],..
        yy=[248.584;274.25067;274.25067],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[5,1,0],..
        to=[2,1,1])
        scs_m.objs(7)=scicos_block(..
        gui="OUT_f",..
        graphics=scicos_graphics(..
        orig=[367.07543,204.784],..
        sz=[20,20],..
        exprs="2",..
        pin=8,..
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
        in2=[],..
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
        ipar=2,..
        opar=list(),..
        blocktype="c",..
        firing=[],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m.objs(8)=scicos_link(..
        xx=[347.07543;367.07543],..
        yy=[214.784;214.784],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[1,2,0],..
        to=[7,1,1])
        scs_m.objs(9)=scicos_block(..
        gui="IN_f",..
        graphics=scicos_graphics(..
        orig=[249.93257,211.45067],..
        sz=[20,20],..
        exprs="1",..
        pin=[],..
        pout=10,..
        pein=[],..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit=[],..
        out_implicit="E"),..
        model=scicos_model(..
        sim="input",..
        in=[],..
        in2=[],..
        intyp=1,..
        out=-1,..
        out2=[],..
        outtyp=-1,..
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
        scs_m.objs(10)=scicos_link(..
        xx=[269.93257;289.93257],..
        yy=[221.45067;221.45067],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[9,1,0],..
        to=[1,2,1])
        scs_m.objs(11)=scicos_block(..
        gui="IN_f",..
        graphics=scicos_graphics(..
        orig=[249.93257,201.45067],..
        sz=[20,20],..
        exprs="2",..
        pin=[],..
        pout=12,..
        pein=[],..
        peout=[],..
        gr_i=[],..
        id="",..
        in_implicit=[],..
        out_implicit="E"),..
        model=scicos_model(..
        sim="input",..
        in=[],..
        in2=[],..
        intyp=1,..
        out=-1,..
        out2=[],..
        outtyp=-1,..
        evtin=[],..
        evtout=[],..
        state=[],..
        dstate=[],..
        odstate=list(),..
        rpar=[],..
        ipar=2,..
        opar=list(),..
        blocktype="c",..
        firing=[],..
        dep_ut=[%f,%f],..
        label="",..
        nzcross=0,..
        nmode=0,..
        equations=list()),..
        doc=list())
        scs_m.objs(12)=scicos_link(..
        xx=[269.93257;289.93257],..
        yy=[211.45067;211.45067],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[11,1,0],..
        to=[1,3,1])
        scs_m.objs(13)=scicos_block(..
        gui="OUT_f",..
        graphics=scicos_graphics(..
        orig=[383.03733,238.584],..
        sz=[20,20],..
        exprs="1",..
        pin=14,..
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
        in2=[],..
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
        scs_m.objs(14)=scicos_link(..
        xx=[363.03733;383.03733],..
        yy=[248.584;248.584],..
        id="drawlink",..
        thick=[0,0],..
        ct=[1,1],..
        from=[5,2,0],..
        to=[13,1,1])

        model=scicos_model()
        model.sim="csuper"
        model.in=[1;1]
        model.in2=[1;1]
        model.out=[1;1]
        model.out2=[1;1]
        model.intyp=[5 5]
        model.outtyp=[5 5]
        model.blocktype="h"
        model.firing=%f
        model.dep_ut=[%t %f]
        model.rpar=scs_m
        gr_i=[]
        x=standard_define([2 3],model,[],gr_i)
    end
endfunction

