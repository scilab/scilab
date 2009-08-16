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

function [scs_m, cpr, needcompile, ok] = do_eval(scs_m, cpr)
// This function (re)-evaluates blocks in the scicos data structure scs_m

  ok = %t
  needcompile1 = max(2,needcompile)
  %mprt = funcprot()
  funcprot(0)
  getvalue = setvalue;
  
  function message(txt)
    messagebox(['In block ' + o.gui + ': ';
		txt;
	       'current parameter value kept'],'error','modal'); 
    %scicos_prob = resume(%t)
  endfunction

  %scicos_prob = %f
  deff('[ok, tt] = FORTR(funam, tt, i, o)','ok = %t')
  deff('[ok, tt] = CFORTR2(funam, tt, i, o)','ok = %t')
  deff('[ok, tt] = CFORTR(funam, tt, i, o)','ok = %t')
  deff('[x, y, ok, gc] = edit_curv(x, y, job, tit, gc)','ok = %t')
  deff('[ok,tt,dep_ut] = genfunc1(tt, ni, no, nci, nco, nx, nz, nrp, type_)',..
       'dep_ut = model.dep_ut; ok = %t')
  deff('result = dialog(labels, valueini)','result = valueini')
  %nx = lstsize(scs_m.objs)
  deff('[ok, tt] = MODCOM(funam, tt, vinp, vout, vparam, vparamv, vpprop)',..
       ['[dirF, nameF, extF]=fileparts(funam);'
	'tarpath = pathconvert(TMPDIR + ''/Modelica/'', %f, %t);'
	'if (extF == '''')  then'
	'  funam1 = tarpath + nameF + ''.mo'';'
	'elseif fileinfo(funam) == [] then'
	'  funam1 = funam;'
	'end;'
	'mputl(tt, funam1);'
	'compilerpath = pathconvert(SCI + ''/bin/'', %f, %t);'
	'if MSDOS then'
	'  compilerpath = compilerpath + ''modelicac.exe'';'
	'else'
	'  compilerpath = compilerpath + ''modelicac'';'
	'end'
	'if execstr(''unix_s(compilerpath + '''' -c '''' + funam1 + '''' -o '''' + tarpath + nameF + ''''.moc'''')'',''errcatch'') <> 0 then'
	'  ok = %f;'
	'else'
	'  ok=%t;'
	'end;'])
  
  funcprot(%mprt)
  
  for %kk = 1:%nx
    o = scs_m.objs(%kk)
    if typeof(o) == 'Block' then
      model = o.model
      if model.sim == 'super' | (model.sim == 'csuper' & model.ipar <> 1) then  //exclude mask
	sblock = model.rpar
	context = sblock.props.context
	
	[%scicos_context, ierr] = script2var(context, %scicos_context)
	
	if ierr <> 0 then
	  %now_win = xget('window')
	  message(['Cannot evaluate a context '; lasterror()])
	  xset('window', %now_win)
	else
	  
	  [sblock, %w, needcompile2, ok] = do_eval(sblock, list())
	  needcompile1 = max(needcompile1, needcompile2)
	  if ok then
	    scs_m.objs(%kk).model.rpar = sblock
	  end
	end
      else
	model = o.model
	// should we generate a message here ?
	ier = execstr('o = ' + o.gui + '(''set'',o)', 'errcatch')
	if ier == 0 then //++ no error
	  needcompile1 = max(needcompile1, needcompile) // for scifunc_block
	  model_n = o.model
	  if or(model.blocktype <> model_n.blocktype) | .. // type 'c','d','z','l'
		or(model.dep_ut <> model_n.dep_ut) | ..
		(model.nzcross <> model_n.nzcross) | ..
		(model.nmode <> model_n.nmode) then
	    needcompile1 = 4
	  end
	  if (size(model.in,'*')    <> size(model_n.in,'*')) | ..
		(size(model.out,'*')   <> size(model_n.out,'*')) | ..
		(size(model.evtin,'*') <> size(model_n.evtin,'*')) then
	    // number of input (evt or regular ) or output  changed
	    needcompile1 = 4
	  end
	  if model.sim == 'input' | model.sim == 'output' then
	    if model.ipar <> model_n.ipar then
	      needcompile1 = 4
	    end
	  end
	  scs_m.objs(%kk) = o
	end // message for else here
      end
    end
  end
  
  needcompile = needcompile1
  
  if needcompile == 4 then
    cpr = list()
  end
endfunction
  
  
