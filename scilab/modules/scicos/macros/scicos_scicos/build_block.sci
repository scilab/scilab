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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [model, ok] = build_block(o)

// build the simulation function associated with the block if necessary

  model    = o.model;
  graphics = o.graphics;

  if model.sim(1)=='scifunc' then
    if model.ipar==0 then
      messagebox(sprintf(gettext("%s: Error: A scifunc block has not been defined."), "build_block"),"modal","error");
      ok = %f
      return
    end
    model.sim = list(genmac(model.ipar,size(model.in,'*'),size(model.out,'*')),3);

  elseif type(model.sim) == 15 then

    modsim = modulo(model.sim(2),10000)

    if int(modsim/1000) == 1 then   // Fortran Block
      funam = model.sim(1)
      if ~c_link(funam) then
        tt = graphics.exprs(2);
        ok = scicos_block_link(funam, tt, 'f')
      end

    elseif int(modsim/1000) == 2 then   // C Block
      funam = model.sim(1)
      if ~c_link(funam) then //** if the function is not already linked
        tt = graphics.exprs(2);
        ok = scicos_block_link(funam, tt, 'c');
      end

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
      
      // MODELICAC on windows does not support '\' at the end of path
      // TO DO: Fix MODELICAC
      if MSDOS then
        tarpath = pathconvert(fullfile(TMPDIR,'Modelica'), %f, %t);
      else
        tarpath = pathconvert(fullfile(TMPDIR,'Modelica'), %t, %t);
      end

      if (extF == '') then
        funam = fullfile(tarpath, nameF + '.mo');
        mputl(tt, funam);
      end

      //++ Check that modelica compiler is available
      //++ Otherwise, give some feedback and quit
      if ~with_modelica_compiler() then
        messagebox(sprintf(gettext("%s: Error: Modelica compiler (MODELICAC) is unavailable."), "build_block"),"modal","error");
        ok = %f
      end

      compilerpath = getmodelicacpath() + "modelicac"; //** thanks to automatic detection

      // build compilation command line, execute it and test for result
      strCmd = compilerpath + ' -c ' + funam + ' -o ' + fullfile(tarpath, nameF + '.moc')
      if execstr('unix_s(''' + strCmd + ''')', 'errcatch') <> 0 then
        error(sprintf(gettext("%s: Error : the following command line failed to execute: %s.\n"), "build_block", strCmd))
        ok = %f
      end
    end
  end
endfunction

