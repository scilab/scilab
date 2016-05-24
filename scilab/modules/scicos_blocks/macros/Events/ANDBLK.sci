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

function [x,y,typ]=ANDBLK(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
    case "define" then
        andlog=ANDLOG_f("define")
        andlog.graphics.orig=[194,133]
        andlog.graphics.sz=[60,60]
        andlog.graphics.pout=9
        andlog.graphics.pein=[4;11]

        input_port1=CLKIN_f("define")
        input_port1.graphics.orig=[149,287]
        input_port1.graphics.sz=[20,20]
        input_port1.graphics.exprs="1"
        input_port1.graphics.peout=4
        input_port1.model.ipar=1

        output_port=CLKOUT_f("define")
        output_port.graphics.orig=[450,83]
        output_port.graphics.sz=[20,20]
        output_port.graphics.exprs="1"
        output_port.graphics.pein=8
        output_port.model.ipar=1

        input_port2=CLKIN_f("define")
        input_port2.graphics.orig=[141,330]
        input_port2.graphics.sz=[20,20]
        input_port2.graphics.exprs="2"
        input_port2.graphics.peout=6
        input_port2.model.ipar=2

        ifthel=IFTHEL_f("define")
        ifthel.graphics.orig=[331,137]
        ifthel.graphics.sz=[60,60]
        ifthel.graphics.pin=9
        ifthel.graphics.pein=12
        ifthel.graphics.peout=[8;0]

        split=CLKSPLIT_f("define")
        split.graphics.orig=[234;275.78348]
        split.graphics.pein=6,
        split.graphics.peout=[11;12]


        diagram=scicos_diagram()
        diagram.objs(1)=andlog
        diagram.objs(2)=input_port1
        diagram.objs(3)=output_port
        diagram.objs(4)=scicos_link(xx=[169;214;214],yy=[297;297;198.71],..
        ct=[5,-1],from=[2,1],to=[1,1])
        diagram.objs(5)=input_port2
        diagram.objs(6)=scicos_link(xx=[161;234;234],yy=[340;340;275.78],..
        ct=[5,-1],from=[5,1],to=[10,1])
        diagram.objs(7)=ifthel
        diagram.objs(8)=scicos_link(xx=[351;351;450],yy=[131.29;93;93],..
        ct=[5,-1],from=[7,1],to=[3,1])
        diagram.objs(9)=scicos_link(xx=[262.57;322.43],yy=[163;167],..
        ct=[1,1],from=[1,1],to=[7,1])
        diagram.objs(10)=split
        diagram.objs(11)=scicos_link(xx=[234;234],yy=[275.78;198.71],..
        ct=[5,-1],from=[10,1],to=[1,2])
        diagram.objs(12)=scicos_link(xx=[234;361;361],yy=[275.78;275.78;202.71],..
        ct=[5,-1],from=[10,2],to=[7,1])
        x=scicos_block()
        x.gui="ANDBLK"
        x.graphics.sz=[2,2]
        x.graphics.gr_i=[];
        x.graphics.pein=[0;0]
        x.graphics.peout=0
        x.model.sim="csuper"
        x.model.evtin=[1;1]
        x.model.evtout=1
        x.model.blocktype="h"
        x.model.firing=%f
        x.model.dep_ut=[%f %f]
        x.model.rpar=diagram;

    end
endfunction
