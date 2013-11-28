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

function [x,y,typ]=Extract_Activation(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
    case "define" then
        scs_m_1=scicos_diagram();
        scs_m_1.objs(1) = IFTHEL_f("define");
        scs_m_1.objs(2) = CLKSOMV_f("define");
        scs_m_1.objs(3) = IN_f("define");
        scs_m_1.objs(4) = CLKOUTV_f("define");
        scs_m_1.objs(5) = scicos_link();
        scs_m_1.objs(6) = scicos_link();
        scs_m_1.objs(7) = scicos_link();
        scs_m_1.objs(8) = scicos_link();

        // IFTHEL_f
        blk = scs_m_1.objs(1);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [80 0];
        graphics.sz = [60 40];

        graphics.exprs = ["0";"0"];
        model.evtin = [];
        model.nzcross = 0;
        model.nmode = 0;

        graphics.pin = 7;
        graphics.peout = [5;6];

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(1) = blk;

        // CLKSOMV_f
        blk = scs_m_1.objs(2);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [80 -80];
        graphics.sz = [80 40];

        graphics.pein = [5;6];
        graphics.peout = 8;

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

        graphics.pout = 7;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(3) = blk;

        // CLKOUTV_f
        blk = scs_m_1.objs(4);
        graphics = blk.graphics;
        model = blk.model;

        graphics.orig = [110 -140];
        graphics.sz = [20 20];

        graphics.exprs = ["1"];
        model.ipar = 1;

        graphics.pein = 8;

        blk.graphics = graphics;
        blk.model = model;
        scs_m_1.objs(4) = blk;

        // IFTHEL_f -> CLKSOMV_f
        lnk = scs_m_1.objs(5);

        lnk.ct = [5 -1];
        lnk.from = [1 1 0];
        lnk.to = [2 1 1];

        scs_m_1.objs(5) = lnk;

        // IFTHEL_f -> CLKSOMV_f
        lnk = scs_m_1.objs(6);

        lnk.ct = [5 -1];
        lnk.from = [1 2 0];
        lnk.to = [2 2 1];

        scs_m_1.objs(6) = lnk;

        // IN_f -> IFTHEL_f
        lnk = scs_m_1.objs(7);

        lnk.from = [3 1 0];
        lnk.to = [1 1 1];

        scs_m_1.objs(7) = lnk;

        // CLKSOMV_f -> CLKOUTV_f
        lnk = scs_m_1.objs(8);

        lnk.ct = [5 -1];
        lnk.from = [2 1 0];
        lnk.to = [4 1 1];

        scs_m_1.objs(8) = lnk;

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

