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

function [x,y,typ]=WFILE_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        warnobsolete("WRITEC_f", "6.0.0");
        warnMessage = msprintf(_("Feature %s is obsolete."), "WFILE_f");
        warnAdvise = msprintf(_("Please use %s instead."), "WRITEC_f");
        warnXcosMessage = msprintf("%s %s", warnMessage, warnAdvise);
        warnBlockByUID(arg1.model.label, warnXcosMessage);
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        dstate=model.dstate
        lunit=dstate(2)
        fname=exprs(2)
        frmt=exprs(3)
        while %t do
            [ok,in,fname1,frmt1,N,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "WFILE_f");" "; gettext("Write to output file"); " "; ..
            gettext("Write is done on:"); gettext("&nbsp; - A binary file if no format given"); ..
            gettext("&nbsp; - A formatted text file if a  format (Fortran type) is given")], [gettext("Input Size"); gettext("Output File Name"); ..
            gettext("Output Format"); gettext("Buffer Size")], list("vec",1,"str",1,"str",1,"vec",1),exprs)

            if ~ok then
                break,
            end //user cancel modification
            in=int(in)

            nin=in

            fname1 = pathconvert(stripblanks(fname1), %f, %t) // File name expansion
            frmt1=stripblanks(frmt1)

            if lunit > 0 & min(length(frmt),1) <> min(length(frmt1),1) then
                block_parameter_error(gettext("Simulation running !!! You cannot switch<br />between formatted and unformatted when running"), gettext("End current simulation first."));
                ok=%f
            elseif lunit > 0 & fname1 <> fname then
                block_parameter_error(gettext("You cannot modify ''Output File Name'' when running."), gettext("End current simulation first."));
                ok=%f
            elseif fname1 == "" then
                block_parameter_error(gettext("Wrong value for ''Output File Name'' parameter"), gettext("You must provide a filename."));
                ok=%f
                //Check if directory exist
            elseif fileparts(fname1) ~= "" then
                [pa, fn, ex] = fileparts(fname1)
                if ~isdir(pa) then
                    block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Output File Name")), ..
                    msprintf(gettext("Directory ''%s'' does not exist"), pa ));
                    ok=%f
                end
                // Simple check for including of the format's string  in parenthesis
            elseif frmt1 ~= "" &  (part(frmt1, 1) ~= "(" | part(frmt1, length(frmt1)) ~= ")")
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Input Format"), frmt1), ..
                gettext("You must enclose the format''s string between parentheses."));
                ok=%f
            elseif N < 2 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Buffer Size"), N), ..
                gettext("Must be greater than 1."));
                ok=%f
            elseif in <= 0 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Input Size"), in), ..
                gettext("Strictly positive integer expected."));
                ok=%f
            end

            if ok then
                ipar=[length(ascii(fname1));length(ascii(frmt1));0;N;ascii(fname1)';ascii(frmt1)']
                if prod(size(dstate))<>(nin+1)*N+2 then
                    dstate=[-1;lunit;zeros((nin+1)*N,1)]
                end
                model.in=nin
                model.dstate=dstate;model.ipar=ipar
                model.dep_ut=[%t %f] //compatibility
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        in=1;
        nin=sum(in)
        frmt="(7(e10.3,1x))"
        fname="foo"
        lunit=0
        N=2;

        model=scicos_model()
        model.sim="writef"
        model.in=in
        model.evtin=1
        model.dstate=[-1;lunit;zeros((nin+1)*N,1)]
        model.ipar=[length(ascii(fname));length(ascii(frmt));0;N;ascii(fname)';ascii(frmt)']
        model.blocktype="d"
        model.dep_ut=[%t %f]

        exprs=[sci2exp(in);
        fname;
        frmt;
        string(N)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
