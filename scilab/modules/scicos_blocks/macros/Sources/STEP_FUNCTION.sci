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

function [x,y,typ]=STEP_FUNCTION(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        // look for the internal edge trigger block
        for i=1:length(arg1.model.rpar.objs) do
            o = arg1.model.rpar.objs(i);
            if typeof(o) == "Block" & o.gui == "STEP" then
                ppath = list(i);
                break;
            end
        end
        newpar=list();
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
                        // input or output port sizes changed
                        needcompile=1
                    end
                    if or(model.firing<>model_n.firing)  then
                        // initexe changed
                        needcompile=2
                    end
                    if model.sim=="input"|model.sim=="output" then
                        if model.ipar<>model_n.ipar then
                            needcompile=4
                        end
                    end
                    if or(model.blocktype<>model_n.blocktype)|..
                        or(model.dep_ut<>model_n.dep_ut)  then
                        // type 'c','d','z','l' or dep_ut changed
                        needcompile=4
                    end
                    if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then
                        // size of zero cross changed
                        needcompile=4
                    end
                    if prod(size(model_n.sim))>1 then
                        if model_n.sim(2)>1000 then  // Fortran or C Block
                            if model.sim(1)<>model_n.sim(1) then  //function name has changed
                                needcompile=4
                            end
                        end
                    end
                else //implicit block
                    //force compilation if an implicit block has been edited
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
            end
        end
        x=arg1
        y=needcompile
        typ=newpar
    case "define" then
        scs_m_1=scicos_diagram();
        scs_m_1.objs(1) = STEP("define");
        scs_m_1.objs(2) = OUT_f("define");
        scs_m_1.objs(3) = scicos_link();
        scs_m_1.objs(4) = scicos_link();

        // STEP
        blk = scs_m_1.objs(1);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [0 0];
        graphics.sz = [40 40];

        graphics.pein = 4;
        graphics.peout = 4;
        graphics.pout = 3;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(1) = blk;

        // OUT_f
        blk = scs_m_1.objs(2);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [80 10];
        graphics.sz = [20 20];

        graphics.exprs = ["1"];
        model.ipar = 1;

        graphics.pin = 3;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(2) = blk;

        // STEP -> OUT_f
        lnk = scs_m_1.objs(3);

        lnk.from = [1 1 0];
        lnk.to = [2 1 1];

        scs_m_1.objs(3) = lnk;

        // STEP -> STEP
        lnk = scs_m_1.objs(4);

        lnk.xx = [0 20  -20 -20 20 1];
        lnk.yy = [0 -20 -20  60 60 1];

        lnk.ct = [5 -1];
        lnk.from = [1 1 0];
        lnk.to = [1 1 1];

        scs_m_1.objs(4) = lnk;

        clear blk lnk

        model=scicos_model();
        model.sim = "csuper";
        model.out = 1;
        model.out2=1;
        model.outtyp=1;

        model.rpar = scs_m_1;

        gr_i=[]
        x=standard_define([2 2],model,[],gr_i)
    end
endfunction

