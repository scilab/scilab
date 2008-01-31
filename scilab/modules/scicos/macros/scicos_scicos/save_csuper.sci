//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function path=save_csuper(scs_m,fpath,gr_i,sz)
// given a super block definition x save_super creates a file which contains
// this super block  handling  macro definition
// 19/07/07 : Alan added two rhs : gr_i and sz for
//                                 initial values
//            gr_i a vector of string (matrix size nx1)
//            sz a vector of real (matrix size 1x2)

  [lhs,rhs]=argn(0)
  if rhs<4 then sz=[],end
  if rhs<3 then gr_i='',end

  path=[]
  scs_m=do_purge(scs_m)
  nam=scs_m.props.title(1);
  nam=strsubst(nam,' ','_')
  in=[];out=[];clkin=[];clkout=[];
  bl='  '
  com='/'+'/'
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      model=o.model
      select o.gui
	case 'IN_f' then
	in=[in;model.out]
	case 'OUT_f' then
	out=[out;model.in]
	case 'CLKIN_f' then
	clkin=[clkin;model.evtout]
	case 'CLKOUT_f' then
	clkout=[clkout;model.evtin];
	case 'CLKINV_f' then
	clkin=[clkin;model.evtout]
	case 'CLKOUTV_f' then
	clkout=[clkout;model.evtin];
      end
    end
  end

  model=scicos_model()
  model.sim='csuper'
  model.in=in
  model.out=out
  model.evtin=clkin
  model.evtout=clkout
  model.rpar=scs_m
  model.blocktype='h'
  model.dep_ut=[%f %f]

  ppath=getparpath(scs_m,[])

  // form text of the macro
  txt=['function [x,y,typ]='+nam+'(job,arg1,arg2)';
       'x=[];y=[],typ=[]';
       'select job';
       'case ''plot'' then';
       '  standard_draw(arg1)';
       'case ''getinputs'' then';
       '  [x,y,typ]=standard_inputs(arg1)';
       'case ''getoutputs'' then';
       '  [x,y,typ]=standard_outputs(arg1)';
       'case ''getorigin'' then';
       '  [x,y]=standard_origin(arg1)';
       'case ''set'' then']
  if size(ppath)>0 then
    t1=sci2exp(ppath,'ppath')
    txt=[txt;
	 '  '+com+'paths to updatable parameters or states'
	 bl(ones(size(t1,1),1))+t1;
	 '  newpar=list();';
	 '  y=0;';
	 '  for path=ppath do'
	 '    np=size(path,''*'')'
	 '    spath=list()'
	 '    for k=1:np'
	 '      spath($+1)=''model'''
	 '      spath($+1)=''rpar'''
	 '      spath($+1)=''objs'''
	 '      spath($+1)=path(k)'
	 '    end'
	 '    xx=arg1(spath)'+com+' get the block';
	 '    execstr(''xxn=''+xx.gui+''(''''set'''',xx)'')'
	 '    if ~isequalbitwise(xxn,xx) then '
	 '      model=xx.model'
	 '      model_n=xxn.model'
	 '      if ~is_modelica_block(xx) then'
	 '        modified=or(model.sim<>model_n.sim)|..'
	 '                 ~isequal(model.state,model_n.state)|..'
	 '                 ~isequal(model.dstate,model_n.dstate)|..'
	 '                 ~isequal(model.odstate,model_n.odstate)|..'
	 '                 ~isequal(model.rpar,model_n.rpar)|..'
	 '                 ~isequal(model.ipar,model_n.ipar)|..'
	 '                 ~isequal(model.opar,model_n.opar)|..'
	 '                 ~isequal(model.label,model_n.label)'
	 '        if or(model.in<>model_n.in)|or(model.out<>model_n.out)|..'
	 '           or(model.in2<>model_n.in2)|or(model.out2<>model_n.out2)|..'
         '           or(model.outtyp<>model_n.outtyp)|or(model.intyp<>model_n.intyp) then'
	 '          needcompile=1'
	 '        end'
	 '        if or(model.firing<>model_n.firing) then'
	 '          needcompile=2'
	 '        end'
	 '        if (size(model.in,''*'')<>size(model_n.in,''*''))|..'
	 '          (size(model.out,''*'')<>size(model_n.out,''*''))|..'
	 '          (size(model.evtin,''*'')<>size(model_n.evtin,''*'')) then'
	 '          needcompile=4'
	 '        end'
	 '        if model.sim==''input''|model.sim==''output'' then'
	 '          if model.ipar<>model_n.ipar then'
	 '            needcompile=4'
	 '          end'
	 '        end'
	 '        if or(model.blocktype<>model_n.blocktype)|..'
	 '           or(model.dep_ut<>model_n.dep_ut) then'
	 '          needcompile=4'
	 '        end'
	 '        if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then'
	 '          needcompile=4'
	 '        end'
	 '        if prod(size(model_n.sim))>1 then'
	 '          if model_n.sim(2)>1000 then'
	 '            if model.sim(1)<>model_n.sim(1) then'
	 '              needcompile=4'
	 '            end'
	 '          end'
	 '        end'
	 '      else'
	 '        modified=or(model_n<>model)'
	 '        eq=model.equations;eqn=model_n.equations;'
	 '        if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..'
	 '           or(eq.outputs<>eqn.outputs) then'
	 '          needcompile=4'
	 '        end'
	 '      end'
	 '     '+com+'parameter or states changed'
	 '      arg1(spath)=xxn'+com+' Update'
	 '      newpar(size(newpar)+1)=path'+com+' Notify modification'
	 '      y=max(y,needcompile)';
	 '    end'
	 '  end';
	 '  x=arg1'
	 '  typ=newpar']
  end

  //t1=sci2exp(model,'model');
  txt=[txt;
       'case ''define'' then']

  path=stripblanks(fpath)+'/'+nam+'.sci'
  [u,err]=file('open',path,'unknown')
  if err<>0 then
    message(path+': Directory or file write access denied')
    return
  end
  write(u,txt,'(a)')
  cos2cosf(u,model.rpar,0)
  model.rpar='%scs_m_1'

  //t1=sci2exp(model,'model');
  //scicos_model
  tt1=[];
  fields=getfield(1,model);
  for i=1:lstsize(model)-1
    field_nam=fields(i+1);
    tt=sci2exp(getfield(i+1,model));
    tt(1)=field_nam+'='+tt(1);
    if i<>lstsize(model)-1 then
      tt($)=tt($)+',';
    end
    tt1=[tt1;tt];
  end
  //my_strcat
  for i=1:size(tt1,1)
   if length(tt1(i))<>0 then
     if part(tt1(i),length(tt1(i)))==',' then
       tt1(i)=tt1(i)+'..';
     end
   end
  end
  //final work
  t1=['model=scicos_model(..';
      '         '+tt1(1)];
  for i=2:size(tt1,1)
     t1=[t1;'         '+tt1(i)];
  end
  t1($)=t1($)+')';

  t1=[strsubst(t1,sci2exp('%scs_m_1'),'scs_m_1')];
  if gr_i == '' then
    t1=[t1
        '  gr_i=''xstringb(orig(1),orig(2),'''''+nam+''''',sz(1),sz(2),''''fill'''')'';'];
  else
    gr_i_tmp = sci2exp(gr_i);
    if size(gr_i_tmp,1)<>1 then
      t1=[t1
          '  gr_i='+gr_i_tmp(1)
          '       '+gr_i_tmp(2:$)];
    else
      t1=[t1
          '  gr_i='+gr_i_tmp];
    end
  end
  if sz<>[] then
      t1=[t1
          '  x=standard_define(['+string(sz(1))+...
                                ' '+string(sz(2))+'],model,[],gr_i)';];
  else
      t1=[t1
          '  x=standard_define([2 2],model,[],gr_i)';];
  end
  t1=[t1
      'end'
      'endfunction'];
  write(u,t1,'(a)')
  file('close',u)
endfunction
