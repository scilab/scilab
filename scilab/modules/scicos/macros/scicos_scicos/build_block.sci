//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - 2010 - Jérôme PICARD
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

function [model, ok] = build_block(o)

    // build the simulation function associated with the block if necessary

    model    = o.model;
    graphics = o.graphics;
    if model.sim(1)=="scifunc" then
        if model.ipar <> 0 then
            model.opar=model.ipar;
            model.ipar=0;
        end
        if isempty(model.opar) <> %f then
            messagebox(sprintf(gettext("%s: Error: A scifunc block has not been defined."), "build_block"),"modal","error");
            ok = %f
            return
        end
        model.sim = list(genmac(model.opar,size(model.in,"*"),size(model.out,"*")),3);
    elseif type(model.sim) == 15 then
        modsim = modulo(model.sim(2),10000)
        if int(modsim/1000) == 1 then   // Fortran Block
            funam = model.sim(1)
            if ~c_link(funam) then
                tt = graphics.exprs(2);
                ok = scicos_block_link(funam, tt, "f")
            end
        elseif int(modsim/1000) == 2 then   //  C Block
            [model,ok]=recur_scicos_block_link(o,"c")
        elseif model.sim(2) == 30004 then //modelica generic file type 30004
            //funam = model.sim(1); tt = graphics.exprs(2);
            if type(graphics.exprs) == 15 then // compatibility
                funam = model.sim(1);
                tt    = graphics.exprs(2);
            else
                funam = model.equations.model
                tt    = graphics.exprs.funtxt;
            end
            [dirF, nameF, extF] = fileparts(funam);
            [modelica_path, modelica_directory] = getModelicaPath();
            if (extF == "") then
                funam = modelica_directory + nameF + ".mo";
                mputl(tt, funam);
            end

            //next lines removed in ScicosLab
            //       //++ Check that modelica compiler is available
            //       //++ Otherwise, give some feedback and quit
            //       if ~with_modelica_compiler() then
            //         messagebox(sprintf(gettext("%s: Error: Modelica compiler (MODELICAC) is unavailable."), "build_block"),"modal","error");
            //         ok = %f
            //       end


            //       compilerpath = 'modelicac' //** thanks to automatic detection

            //       // build compilation command line, execute it and test for result
            //       strCmd = compilerpath + ' -c ' + funam + ' -o ' + fullfile(tarpath, nameF + '.moc')
            //       if execstr('unix_s(''' + strCmd + ''')', 'errcatch') <> 0 then
            //         error(sprintf(gettext("%s: Error : the following command line failed to execute: %s.\n"), "build_block", strCmd))
            //         ok = %f
            //       end
        end
    end
endfunction

