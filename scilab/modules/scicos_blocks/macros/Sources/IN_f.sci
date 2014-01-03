//  Xcos
//
// Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
// Copyright (C) 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ]=IN_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs;
        if size(exprs,"*")==2 then
            exprs=exprs(1),
        end //compatibility
        if size(exprs,"*")==1 then
            exprs=[exprs(1);"[-1 -2]";"-1"],
        end //compatibility
        while %t do
            [ok,prt,otsz,ot,exprs]=getvalue(_("Set Input block parameters"),...
            [_("Port number");
            _("Outport size ([-1 -2] for inherit)");
            _("Outport Type (-1 for inherit)")],...
            list("vec",1,"vec",-1,"vec",1),exprs)
            if ~ok then
                break,
            end
            prt=int(prt)
            if prt<=0 then
                message(_("Port number must be a positive integer"))
            elseif ~isequal(size(otsz,"*"),2) then
                message(_("Outport Size must be a 2 elements vector"))
            elseif ((ot<1|ot>9)&(ot<>-1)) then
                message(_("Outport type must be a number between 1 and 9, or -1 for inheritance."))
            else
                if model.ipar<>prt then
                    needcompile=4;
                    y=needcompile,
                end
                model.ipar=prt
                model.firing=[];
                model.out=otsz(1)
                model.out2=otsz(2)
                model.outtyp=ot;
                graphics.exprs=exprs
                x.graphics=graphics
                x.model=model
                break
            end
        end
    case "define" then
        prt=1
        model=scicos_model()
        model.sim="input"
        model.out=-1
        model.out2=-2
        model.outtyp=-1
        model.ipar=prt
        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=sci2exp(prt)
        gr_i=[]
        x=standard_define([1 1],model,exprs,gr_i)
    end
endfunction
