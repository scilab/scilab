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

function [x,y,typ]=WRITEAU_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        ipar=model.ipar;
        dstate=model.dstate
        lunit=dstate(2)
        while %t do
            [ok,N,swap,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "WRITEAU_f");" "; ..
            gettext("Write ''.au'' sound file on audio device")],[gettext("Buffer Size"); gettext("Swap Mode (0:No, 1:Yes)")], ..
            list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification

            nin=1

            fname1="/dev/audio"
            frmt1="uc "

            if alreadyran & (N <> ipar(5)) then
                block_parameter_error(msprintf(gettext("You cannot modify ''%s'' when running."), gettext("Buffer Size")), ..
                gettext("End current simulation first"));
                ok=%f
            elseif N < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Buffer Size"), N), ..
                gettext("Strictly positive integer expected."));
                ok=%f
            end
            if swap <> 0 & swap <> 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Swap Mode"), swap), ..
                msprintf(gettext("Must be in the interval %s."), "[0, 1]"));
                ok=%f
            end

            if ok then
                ipar=[length(ascii(fname1));ascii(frmt1)';N;swap;ascii(fname1)']
                if prod(size(dstate))<>(nin+1)*N+2 then
                    dstate=[-1;lunit;zeros((nin+1)*N,1)]
                end
                model.in=1
                model.dstate=dstate;
                model.ipar=ipar
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end

        end
    case "define" then
        in=1;
        nin=sum(in)
        frmt="uc "
        fname="/dev/audio"
        swap=0
        lunit=0
        N=2;

        model=scicos_model()
        model.sim=list("writeau",2)
        model.in=in
        model.evtin=1
        model.dstate=[-1;lunit;zeros((nin+1)*N,1)]
        model.ipar=[length(ascii(fname));ascii(frmt)';N;swap;ascii(fname)']
        model.blocktype="d"
        model.dep_ut=[%t %f]

        exprs=[string(N)
        string(swap)]
        gr_i=[]
        x=standard_define([4 2],model,exprs,gr_i)
    end
endfunction
