//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2011 Bernard DUJARDIN <bernard.dujardin@scilab.contrib.org>
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

function [x,y,typ]=WRITEC_f(job,arg1,arg2)
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
        fname=exprs(2)
        frmt=exprs(3)
        while %t do
            [ok,in,fname1,frmt1,N,swap,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "WRITEC_f");" "; ..
            gettext("Write to C binary file")], [gettext("Input Size"); gettext("Output File Name"); ..
            gettext("Output Format"); gettext("Buffer Size"); gettext("Swap Mode (0:No, 1:Yes)")], ..
            list("vec",1,"str",1,"str",1,"vec",1,"vec",1),exprs)

            if ~ok then
                break,
            end //user cancel modification

            in=int(in)
            nin=in

            fname1 = pathconvert(stripblanks(fname1), %f, %t)
            frmt1=stripblanks(frmt1)
            fmts=["s","l","d","f","c","us","ul","uc","ull","uls","ubl","ubs", "dl","fl","ll","sl","db","fb","lb","sb"]

            if and(frmt1 <> fmts) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Input Format"), frmt1), ..
                gettext("Valid formats are: " + strcat(fmts,", ")));
                ok=%f
            elseif alreadyran & fname1 <> fname then
                block_parameter_error(msprintf(gettext("You cannot modify ''%s'' when running"), gettext("Input Format")), ..
                gettext("End current simulation first."));
                ok=%f
            elseif alreadyran & N <> ipar(5) then
                block_parameter_error(msprintf(gettext("You cannot modify ''Buffer Size'' when running."), gettext("Buffer Size")), ..
                gettext("End current simulation first"));
                ok=%f
            elseif fname1 == "" then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Output File Name")), gettext("You must provide a filename."));
                //Check if directory exist
            elseif fileparts(fname1) ~= "" then
                [pa, fn, ex] = fileparts(fname1)
                if ~isdir(pa) then
                    block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Output File Name")), ..
                    msprintf(gettext("Directory ''%s'' does not exist"), pa ));
                    ok=%f
                end
            elseif N < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Buffer Size"), N), ..
                gettext("Strictly positive integer expected."));
                ok=%f
            elseif in <= 0 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Input Size"), in), ..
                gettext("Strictly positive integer expected."));
                ok=%f
            elseif swap <> 0 & swap <> 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Swap Mode"), swap), ..
                msprintf(gettext("Must be in the interval %s."), "[0, 1]"));
                ok=%f
            end

            frmt1=part(frmt1,1:3)

            if ok then
                ipar=[length(ascii(fname1));ascii(frmt1)';N;swap;ascii(fname1)']
                if prod(size(dstate))<>(nin+1)*N+2 then
                    dstate=[-1;lunit;zeros((nin+1)*N,1)]
                end
                model.in=nin
                model.dstate=dstate;
                model.ipar=ipar
                //      model.firing=[] //compatibility
                //      model.dep_ut=[%t %f] //compatibility
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        in=1;
        nin=sum(in)
        frmt="c  "
        fname="foo"
        swap=0
        lunit=0
        N=2;
        model=scicos_model()
        model.sim=list("writec",2)
        model.in=in
        model.evtin=1
        model.dstate=[-1;lunit;zeros((nin+1)*N,1)]
        model.ipar=[length(ascii(fname));ascii(frmt)';N;swap;ascii(fname)']
        model.blocktype="d"
        model.dep_ut=[%t %f]

        exprs=[sci2exp(in);
        fname;
        frmt;
        string(N)
        string(swap)]
        gr_i=[]
        x=standard_define([4 2],model,exprs,gr_i)
    end
endfunction
