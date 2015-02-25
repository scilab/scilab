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

function [x,y,typ] = READC_f(job,arg1,arg2)

    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        x = arg1;
        model = x.model;
        graphics = arg1.graphics;
        exprs = graphics.exprs
        out = model.out
        dstate = model.dstate
        ipar = model.ipar
        imask = 9+ipar(1)
        tmask = ipar(imask)
        lunit = dstate(3)
        fname = exprs(3)
        frmt = exprs(4)

        while %t do
            [ok,tmask1,outmask,fname1,frmt1,M,N,offset,swap,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "READC_f" );
            " "; gettext("Read from C binary file")], [gettext("Time Record Selection"); gettext("Outputs Record Selection"); ..
            gettext("Input File Name"); gettext("Input Format"); gettext("Record Size"); gettext("Buffer Size"); ..
            gettext("Initial Record Index"); gettext("Swap Mode (0:No, 1:Yes)")], ..
            list("vec", -1, "vec", -1, "str", 1, "str", 1, "vec", 1, "vec", 1,"vec", 1, "vec", 1), exprs);

            if ~ok then
                break
            end //user cancel modification

            fname1 = pathconvert(stripblanks(fname1), %f, %t)
            frmt1 = stripblanks(frmt1)
            fmts = [ "s","l","d","f","c","us","ul","uc","ull","uls","ubl","ubs","dl","fl","ll","sl","db","fb","lb","sb"];

            nout  =  size(outmask,"*")

            if prod(size( tmask1 )) > 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Time Record Selection")), ..
                gettext("Must be a scalar or an empty matrix."))

            elseif and(frmt1 <> fmts) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Input Format"), frmt1), ..
                gettext("Valid formats are: " + strcat(fmts,", ")));

            elseif alreadyran & fname1 <> fname then
                block_parameter_error(msprintf(gettext("You cannot modify ''%s'' when running"), gettext("Input File Name")), ..
                gettext("End current simulation first."));

            elseif N <> ipar(6) & alreadyran then
                block_parameter_error(msprintf(gettext("You cannot modify ''%s'' when running."), gettext("Buffer Size")), ..
                gettext("End current simulation first"));

            elseif alreadyran & size(tmask1) <> size(tmask) then
                block_parameter_error(msprintf(gettext("You cannot modify ''%s'' when running."), gettext("Time Record Selection")), ..
                gettext("End current simulation first."));

            elseif fname1 == "" then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Input File Name")), ..
                gettext("You must provide a file name."));
            elseif M < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Record Size"), M), ..
                gettext("Strictly positive integer expected."));

            elseif tmask1 ~= [] & (tmask1 < 1 | tmask1 > M) then
                block_parameter_error(msprintf(gettext("Wrong value for  ''%s'' parameter: %d."), gettext("Time Record Selection"), tmask1), ..
                msprintf(gettext("Must be in the interval %s."), gettext("[1, Record Size = ") + string (M)+"]"));

            elseif nout == 0 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Outputs Record Selection"), nout), ..
                gettext("Strictly positive integer expected."));

            elseif nout > M then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Outputs Record Selection"), nout), ..
                msprintf(gettext("Must be in the interval %s."), gettext("[1, Record Size = ") + string (M)+"]"));

            elseif max(outmask) > M | min(outmask) < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for indexes in ''%s'' parameter: %s."), gettext("Outputs Record Selection"),  strcat(string(outmask(:))," ")), ..
                msprintf(gettext("Must be in the interval %s."), gettext("[1, Record Size = ") + string (M)+"]"));

            elseif N < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Buffer Size"), N), ..
                gettext("Strictly positive integer expected."));

            elseif swap <> 0 & swap <> 1 then
                block_parameter_error(msprintf(gettext("Wrong value for  ''%s'' parameter: %d."), gettext("Swap Mode"), swap), ..
                msprintf(gettext("Must be in the interval %s."), "[0, 1]"));

            elseif offset < 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Initial Record Index"), offset), ..
                gettext("Strictly positive integer expected."));
            else
                if tmask1 == [] then
                    ievt = 0;
                    tmask1 = 0;
                    outpt = [];
                else
                    ievt = 1;
                    outpt = 1;
                end

                out = size(outmask,"*")
                [model,graphics,ok] = check_io(model,graphics,[],out,1,outpt)
                frmt1 = part(frmt1,1:3);

                if ok then
                    if ievt == 0 then
                        model.firing = -1
                    else
                        model.firing = 0
                    end

                    ipar = [ ...
                    length(ascii(fname1)); ...
                    ascii(frmt1)'; ...
                    ievt; ...
                    N; ...
                    M; ...
                    swap; ...
                    offset; ...
                    ascii(fname1)'; ...
                    tmask1; ...
                    outmask(:) ...
                    ];

                    if prod(size(dstate)) <> (N*M) + 3 then
                        dstate = [-1; -1; lunit; zeros(N*M, 1)]
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
        frmt = "d  "
        fname = "foo"
        lunit = 0
        N = 20;
        M = 1
        rpar = []
        tmask = 0
        swap = 0
        offset = 1
        outmask = 1
        ievt = 0
        nout = size(outmask,"*")

        ipar = [ ...
        length(ascii(fname)); ascii(frmt)'; ievt; N; M; ...
        swap;offset; ascii(fname)'; tmask; outmask ...
        ];

        model = scicos_model()
        model.sim = list("readc",2)
        model.out = nout
        model.evtin = 1
        model.evtout = []
        model.dstate = [1; 1; lunit; zeros(N*M,1)]
        model.ipar = [ ...
        length(ascii(fname)); ascii(frmt)'; ievt; N; M; ...
        swap; offset;ascii(fname)'; ...
        tmask; outmask ...
        ];

        model.blocktype = "d"
        model.firing = -1
        model.dep_ut = [%f %f]

        exprs = [ ...
        "[]"; sci2exp(outmask); fname; frmt; string(M); ...
        string(N); string(offset);string(swap) ...
        ];

        gr_i = [];

        x = standard_define([4 2],model,exprs,gr_i)
    end

endfunction
