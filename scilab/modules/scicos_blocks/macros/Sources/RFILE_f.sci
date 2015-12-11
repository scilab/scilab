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

function [x, y, typ] = RFILE_f(job,arg1,arg2)

    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        x = arg1;
        graphics = arg1.graphics;
        exprs = graphics.exprs
        model = arg1.model;
        dstate = model.dstate
        ipar = model.ipar
        ievt = ipar(3);
        N = ipar(4);
        imask = 5+ipar(1)+ipar(2)
        tmask = ipar(imask)
        lunit = dstate(3)
        fname = exprs(3)
        frmt = exprs(4)
        //for backward compatibility
        if size(exprs,"*")>5 then
            exprs(6) = []

        end

        while %t do
            [ok,tmask1,outmask,fname1,frmt1,N,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "RFILE_f");" "; ..
            gettext("Read from an input file"); " "; gettext("Read is done on:"); gettext("&nbsp; - A binary file if no format given"); ..
            gettext("&nbsp; - A formatted text file if a  format (fortran type) is given")], ..
            [gettext("Time Record Selection"); gettext("Outputs Record Selection"); gettext("Input File Name"); gettext("Input Format"); gettext("Buffer Size")], ..
            list("vec",-1,"vec",-1,"str",1,"str",1,"vec",1), exprs);

            if ~ok then
                break
            end //user cancel modification

            fname1 = pathconvert(stripblanks(fname1), %f, %t)
            frmt1 = stripblanks(frmt1)
            //out=int(out)
            //nout=out
            nout = size(outmask, "*")

            if prod(size(tmask1)) > 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Time Record Selection"), strcat(string(tmask1(:))," ")),..
                gettext("Empty matrix or scalar expected."));

            elseif tmask1 ~= [] & tmask1 < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d"), gettext("Time Record Selection"), tmask1), ..
                gettext("Strictly positive integer expected."));

            elseif lunit > 0 & min(length(frmt),1) <> min(length(frmt1), 1) then
                block_parameter_error([gettext("Simulation running !!! You cannot switch <br />between formatted and unformatted")], ..
                gettext("End current simulation first."));

            elseif lunit > 0 & fname1 <> fname then
                block_parameter_error(gettext("Simulation running !!! You cannot modify ''Input File Name''"), gettext("End current simulation first."));

            elseif lunit > 0 & size(tmask1) <> size(tmask) then
                block_parameter_error(gettext("Simulation running !!! You cannot modify ''Time Record Selection''"), gettext("End current simulation first."));

            elseif fname1 == "" then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s"), gettext("Input File Name"), fname1), gettext("You must provide a filename."));
                // Simple check for including of the format's string  in parenthesis
            elseif frmt1 ~= "" &  (part(frmt1, 1) ~= "(" |  part(frmt1, length(frmt1)) ~= ")")
                block_parameter_error(msprintf(gettext("Wrong format for ''%s'' parameter: %s."), gettext("Input Format"), frmt1), ..
                gettext("You must enclose the format''s string between parentheses."));
            elseif N < 2 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Buffer Size"), N), ..
                gettext("Buffer size must be at least 2."));
            elseif nout == 0 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Outputs Record Selection"), nout), ..
                gettext("You must read at least one field in record."));

            elseif min(outmask) < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s"), gettext("Outputs Record Selection"), ..
                strcat(string(outmask(:))," ")), gettext("Strictly positive indexes expected."));
            else
                if tmask1 == [] then
                    ievt = 0;
                    cout = [];
                    tmask1 = 0;
                else
                    ievt = 1
                    cout = 1;
                end
                [model,graphics,ok] = check_io( model, graphics, [], nout, 1, cout)
                if ok then
                    if ievt == 0 then
                        model.firing = []
                    else
                        model.firing = 0
                    end
                    ipar = [ ...
                    length(ascii(fname1)); ...
                    length(ascii(frmt1)); ...
                    ievt; ...
                    N; ...
                    ascii(fname1)'; ...
                    ascii(frmt1)'; ...
                    tmask1; ...
                    outmask(:) ...
                    ];

                    if prod(size(dstate)) <> (nout+ievt)*N + 3 then
                        dstate = [-1 ; -1; lunit; zeros((nout + ievt) * N, 1)]
                    end
                    model.dstate = dstate;
                    model.ipar = ipar
                    graphics.exprs = exprs;
                    x.graphics = graphics;
                    x.model = model
                    break
                end
            end
        end

    case "define" then
        out = 1;
        nout = sum(out)
        frmt = "(7(e10.3,1x))"
        fname = "foo"
        lunit = 0
        N = 2;
        rpar = []
        tmask = 0
        outmask = 1
        ipar = [ ...
        length(ascii(fname));length(ascii(frmt));0;N; ...
        ascii(fname)';ascii(frmt)'; ...
        tmask;outmask ...
        ];

        dstate = [1; 1; lunit; zeros((nout)*N, 1)]
        model = scicos_model()
        model.sim = "readf"
        model.out = nout
        model.evtin = 1
        model.dstate = dstate
        model.ipar = [ ...
        length(ascii(fname));length(ascii(frmt));0;N; ...
        ascii(fname)';ascii(frmt)'; ...
        tmask;outmask ...
        ];

        model.blocktype = "d"
        model.dep_ut = [%f %f]

        exprs = [sci2exp([]);
        sci2exp(outmask);
        fname;
        frmt;
        string(N);
        sci2exp(out)]
        gr_i = [];

        x = standard_define([3 2],model,exprs,gr_i)
    end

endfunction
