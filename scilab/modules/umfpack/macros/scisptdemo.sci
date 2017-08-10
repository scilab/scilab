//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [] = scisptdemo()
    // demo pour l'interface umf
    mode(-1)

    oldln = lines();
    lines(0)

    deff("[]=demoex(num)","exec(scisptdemolist(num,2),-1)")
    a = gda();
    a.title.font_size = 3;
    a;title.font_style = 6;


    scisptdemolist = [gettext("how to use this stuff")           , SCI+"/modules/umfpack/demos/scisptdem1.dem";
    gettext("display a speed comparison test") , SCI+"/modules/umfpack/demos/scisptdem2.dem";
    gettext("small tests for condestsp")       , SCI+"/modules/umfpack/demos/scisptdem3.dem"];
    while %t
        num=x_choose(scisptdemolist(:,1), gettext("Click to choose a demo"));
        if num==0 then
            lines(oldln(1)) , break
        else
            demoex(num)
        end
    end

    sda()
endfunction
