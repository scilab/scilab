//  Scicos
//
//  Copyright (C) INRIA - Alan Layec <alan.layec@inria.fr>
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

function [x, y, typ] = TOWS_c(job, arg1, arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x = arg1;
        graphics = arg1.graphics;
        model = arg1.model;
        exprs = graphics.exprs;

        while %t do
            [ok, nz, varnam, herit, exprs] = scicos_getvalue("Set Xcos buffer block", ...
            ["Size of buffer";
            "Scilab variable name";
            "Inherit (no:0, yes:1)"], ...
            list("vec", 1, "str", 1, "vec", 1), exprs);

            if ~ok then
                break,
            end;

            if (nz <= 0) then
                message("Size of buffer must be positive");
                ok = %f;
            end

            //check for valid name variable
            r = %f;
            ierr = execstr("r = validvar(varnam)", "errcatch");
            if ~r | ierr <> 0 | length(varnam) > 19 then
                message(["Invalid variable name."; "Please choose another variable name."]);
                ok = %f;
            end
            // If varnam already exists, then it must be of type struct (17) with fields "values" and "names".
            // Otherwise, it is considered as a protected variable, an error is raised and user will be asked to enter a new name.
            execstr("if type("+varnam+") <> 17 | or(fieldnames("+varnam+") <> [""values""; ""time""]) then" + ...
            " message([""Protected variable name.""; ""Please choose another variable name.""]);" + ...
            " ok = %f;" + ...
            " end", "errcatch");

            if ok then
                [model, graphics, ok] = set_io(model, graphics, ...
                list([-1, -2], -1), list(), ...
                ones(1-herit, 1), []);
                if herit == 1 then
                    model.blocktype = "x";
                else
                    model.blocktype = "d";
                end
                model.ipar = [nz; length(ascii(varnam)); ascii(varnam)'];
                graphics.exprs = exprs;
                x.graphics = graphics;
                x.model = model;
                break
            end
        end

    case "define" then
        nu     = -1;
        nz     = 128;
        varnam = "A";
        herit  = 0;

        model           = scicos_model();
        model.sim       = list("tows_c", 4);
        model.in        = [nu];
        model.in2       = -2;
        model.intyp     = -1;
        model.out       = [];
        model.evtin     = [1];
        model.evtout    = [];
        model.rpar      = [];
        model.ipar      = [nz; length(ascii(varnam)); ascii(varnam)'];
        model.blocktype = "d";
        model.firing    = [];
        model.dep_ut    = [%f %f];
        gr_i = [];
        exprs = [string(nz); string(varnam); string(herit)];
        x = standard_define([4 2], model, exprs, gr_i);
    end
endfunction

