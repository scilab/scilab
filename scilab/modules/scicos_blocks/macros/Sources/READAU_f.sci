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

function [x,y,typ]=READAU_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        out=model.out
        dstate=model.dstate
        ipar=model.ipar

        imask=9+ipar(1)
        tmask=ipar(imask)
        lunit=dstate(3)
        fname=exprs(1)

        while %t do
            [ok,fname1,N,swap,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "READAU_f"); " "; ..
            gettext("(Read Audio File)");" "; gettext("Read is done on a binary ''.au'' file")], ..
            [gettext("Input File Name"); gettext("Buffer size"); gettext("Swap Mode (0:No, 1:Yes)")], ..
            list("str",1,"vec",1,"vec",1), exprs);
            tmask1=[];
            outmask=1;
            frmt1="uc";
            M=1;
            offset=1;
            if ~ok then
                break,
            end //user cancel modification
            fname1=stripblanks(fname1)
            frmt1=stripblanks(frmt1)
            if alreadyran&fname1<>fname then
                block_parameter_error(gettext("Simulation running !!! You cannot modify Input file name"), ..
                gettext("End current simulation first."));
                // Remove this test user can't modify time in dialog
                //     elseif alreadyran&size(tmask1)<>size(tmask) then
                //       message(['You cannot modify time management when running';'End current simulation first'])
            elseif fname1 == "" then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Input File Name")), gettext("You must provide a filename."));
            elseif N < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Buffer size"), N), msprintf(gettext("Must be greater than %d."), 1));
            elseif alreadyran & (N <> ipar(6)) then
                block_parameter_error(msprintf(gettext("You cannot modify ''%s'' when running."), gettext("Buffer Size")), gettext("End current simulation first."));
            elseif swap <> 0 & swap <> 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Swap Mode"), swap), msprintf(gettext("Must be in the interval %s."),"[0, 1]"));
            else
                [model,graphics,ok]=check_io(model,graphics,[],1,1,[])
                frmt1=part(frmt1,1:3);
                if ok then
                    ipar=[length(ascii(fname1));
                    ascii(frmt1)';
                    0;
                    N;
                    M;
                    swap;
                    offset
                    ascii(fname1)';
                    tmask1
                    outmask(:)]
                    if prod(size(dstate))<>(N*M)+3 then
                        dstate=[-1;-1;lunit;zeros(N*M,1)]
                    end
                    model.dstate=dstate;
                    model.ipar=ipar
                    graphics.exprs=exprs;
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        frmt="uc "
        fname="test.au"
        lunit=0
        N=20;
        M=1
        tmask=[]
        swap=0
        offset=1
        outmask=1
        ievt=0
        nout=size(outmask,"*")

        model=scicos_model()
        model.sim=list("readau",2)
        model.out=nout
        model.evtin=1
        model.dstate=[1;1;lunit;zeros(N*M,1)]
        model.ipar=[length(ascii(fname));ascii(frmt)';ievt;N;M;swap;offset;ascii(fname)';
        tmask;outmask]
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=[fname;
        string(N);
        string(swap)]
        gr_i=[]
        x=standard_define([5 2],model,exprs,gr_i)
    end
endfunction
