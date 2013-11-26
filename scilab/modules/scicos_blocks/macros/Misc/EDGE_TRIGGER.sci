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

function [x,y,typ]=EDGE_TRIGGER(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        // look for the internal edge trigger block
        for i=1:length(arg1.model.rpar.objs) do
            o = arg1.model.rpar.objs(i);
            if typeof(o) == "Block" & o.gui == "EDGETRIGGER" then
                ppath = list(i);
                break;
            end
        end
        newpar=list();
        y=0;
        for path=ppath do
            np=size(path,"*")
            spath=list()
            for k=1:np
                spath($+1)="model"
                spath($+1)="rpar"
                spath($+1)="objs"
                spath($+1)=path(k)
            end
            xx=arg1(spath)// get the block
            execstr("xxn="+xx.gui+"(''set'',xx)")
            if diffobjs(xxn,xx) then
                model=xx.model
                model_n=xxn.model
                if ~is_modelica_block(xx) then
                    modified=or(model.sim<>model_n.sim)|..
                    ~isequal(model.state,model_n.state)|..
                    ~isequal(model.dstate,model_n.dstate)|..
                    ~isequal(model.rpar,model_n.rpar)|..
                    ~isequal(model.ipar,model_n.ipar)|..
                    ~isequal(model.label,model_n.label)
                    if or(model.in<>model_n.in)|or(model.out<>model_n.out) then
                        needcompile=1
                    end
                    if or(model.firing<>model_n.firing)  then
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
                        or(model.dep_ut<>model_n.dep_ut)  then
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
                // parameter or states changed
                arg1(spath)=xxn// Update
                newpar(size(newpar)+1)=path// Notify modification
                y=max(y,needcompile)
            end
        end
        x=arg1
        typ=newpar
    case "define" then
        scs_m_1=scicos_diagram();
        scs_m_1.objs(1) = EDGETRIGGER("define");
        scs_m_1.objs(2) = IFTHEL_f("define");
        scs_m_1.objs(3) = IN_f("define");
        scs_m_1.objs(4) = CLKOUTV_f("define");
        scs_m_1.objs(5) = scicos_link();
        scs_m_1.objs(6) = scicos_link();
        scs_m_1.objs(7) = scicos_link();

        // EDGETRIGGER
        blk = scs_m_1.objs(1);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [60 0];
        graphics.sz = [60 40];

        graphics.exprs = "0";
        model.ipar = 0;

        graphics.pin = 5;
        graphics.pout = 6;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(1) = blk;

        // IFTHEL_f
        blk = scs_m_1.objs(2);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [160 0];
        graphics.sz = [60 40];

        graphics.exprs = ["0";"0"];
        model.evtin = [];
        model.nzcross = 0;
        model.nmode = 0;

        graphics.pin = 6;
        graphics.peout = [7;0];

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(2) = blk;

        // IN_f
        blk = scs_m_1.objs(3);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [0 10];
        graphics.sz = [20 20];

        graphics.exprs = ["1"];
        model.ipar = 1;

        graphics.pout = 5;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(3) = blk;

        // CLKOUTV_f
        blk = scs_m_1.objs(4);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [170 -60];
        graphics.sz = [20 20];

        graphics.exprs = ["1"];
        model.ipar = 1;

        graphics.pein = 7;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(4) = blk;

        // IN_f -> EDGETRIGGER
        lnk = scs_m_1.objs(5);

        lnk.from = [3 1 0];
        lnk.to = [1 1 1];

        scs_m_1.objs(5) = lnk;

        // EDGETRIGGER -> IFTHEL_f
        lnk = scs_m_1.objs(6);

        lnk.from = [1 1 0];
        lnk.to = [2 1 1];

        scs_m_1.objs(6) = lnk;

        // IFTHEL_f -> CLKOUTV_f
        lnk = scs_m_1.objs(7);

        lnk.ct = [5 -1];
        lnk.from = [2 1 0];
        lnk.to = [4 1 1];

        scs_m_1.objs(7) = lnk;

        clear blk lnk

        model=scicos_model();
        model.sim = "csuper";
        model.in = 1;
        model.evtout = 1;
        model.rpar = scs_m_1;

        gr_i=[];
        x=standard_define([3 2],model,[],gr_i)
    end
endfunction

