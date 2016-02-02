//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//
function [x,y,typ]=SELF_SWITCH(job,arg1,arg2)
    function diagram = genSwitchInnerDiagram(stateOpen)
        diagram = scicos_diagram();
        // Input forward
        diagram.objs(1) = IN_f("define");

        // Output's forward
        diagram.objs(2)=OUT_f("define");

        diagram.objs(3)=CONST_m("define");
        diagram.objs(3).graphics.exprs = "0";
        diagram.objs(3).model.rpar = 0;

        diagram.objs(4)=SWITCH_f("define");
        if stateOpen == %t
            diagram.objs(4).model.ipar = 1;
            diagram.objs(4).graphics.exprs = ["2";"2"];
        else
            diagram.objs(4).model.ipar = 0;
            diagram.objs(4).graphics.exprs = ["2";"1"];
        end

        // IN_f <-> SWITCH_f
        diagram.objs(5) = scicos_link(xx=[0, 0], yy=[0, 0], from=[1, 1], to=[4, 1]);
        // CONST_m <-> SWITCH_f
        diagram.objs(6) = scicos_link(xx=[0, 0], yy=[0, 0], from=[3, 1], to=[4, 2]);
        // SWITCH_f <-> OUT_f
        diagram.objs(7) = scicos_link(xx=[0, 0], yy=[0, 0], from=[4, 1], to=[2, 1]);
    endfunction
    select job
    case "set" then
        x = arg1;
        stateOpen = x.model.opar(1);
        x.model.rpar = genSwitchInnerDiagram(~stateOpen);
        x.model.opar = list(~stateOpen);
        y = 0;
        typ=list();
        if stateOpen == %f
            x.graphics.style="SELF_SWITCH_OFF"
        else
            x.graphics.style="SELF_SWITCH_ON"
        end

    case "define"
        // By default SWITCH is open
        stateOpen = %t;
        x = scicos_block()
        x.gui="SELF_SWITCH"
        x.graphics.sz=[2,2]
        x.graphics.gr_i=[]
        x.graphics.pin=0
        x.graphics.pout=0
        x.model.sim="csuper"
        x.model.in = 1
        x.model.out = 1
        x.model.blocktype="h"
        x.model.dep_ut=[%f %f]
        x.model.rpar = genSwitchInnerDiagram(stateOpen);
        x.model.opar = list(stateOpen);
        x.graphics.in_implicit=["E"]
        x.graphics.in_style=""
        x.graphics.out_implicit=["E"];
        x.graphics.out_style=""
        x.graphics.style="SELF_SWITCH_OFF"
    end
endfunction
