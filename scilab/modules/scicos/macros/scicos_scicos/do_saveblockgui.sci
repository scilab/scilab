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

function fname=do_saveblockgui(o)
  tit = ["Use .sci extension because GUI is a Scilab function"];
  fname = savefile('*.sci',emptystr(),tit)

  if fname==emptystr() then
    return
  end

  [path,bname,ext] = splitfilepath_cos(fname)
  [u,err]=file('open',fname,'unknown')

  if err<>0 then
    messagebox(path+': Directory or file write access denied','modal')
    return
  end

  graphics=o.graphics
  exprs0=graphics.exprs(2)(1)
  btitre=graphics.exprs(2)(2)(1)
  bitems=graphics.exprs(2)(2)(2:$)
  if exprs0==[] then 
     txtset='x=arg1,return'
  else
     tt='scicos_context.'+exprs0(1);
     for i=2:size(exprs0,1)
       tt=tt+',scicos_context.'+exprs0(i),
     end
     ss=graphics.exprs(2)(3)

     txtset=[
          '  y=needcompile'
          '  typ=list()'
          '  graphics=arg1.graphics;'
          '  exprs=graphics.exprs'
          '  Btitre=..'
          '    '+sci2exp(btitre)
          '  Exprs0=..'
          '    '+sci2exp(exprs0)
          '  Bitems=..'
          '    '+sci2exp(bitems)
          '  Ss=..'
          '    '+sci2exp(ss)
          '  scicos_context=struct()'
          '     x=arg1'
	  '  ok=%f'
	  '  while ~ok do'
          '    [ok,'+tt+',exprs]=scicos_getvalue(Btitre,Bitems,Ss,exprs)'
	  '    if ~ok then return;end'
	  '     %scicos_context=scicos_context'
          '     sblock=x.model.rpar'
          '     [%scicos_context,ierr]=script2var(sblock.props.context,%scicos_context)'
          '     if ierr==0 then'
	  '       [sblock,%w,needcompile2,ok]=do_eval(sblock,list(),%scicos_context)'
	  '	  if ok then'
          '          y=max(2,needcompile,needcompile2)'
          '          x.graphics.exprs=exprs'
          '          x.model.rpar=sblock'
	  '          break'
	  '	  end'
	  '     else'
	  '       messagebox(lasterror(),''modal'')'
	  '	  ok=%f'
	  '     end'
	  '  end']
  end



  txt=['function [x,y,typ]='+bname+'(job,arg1,arg2)'
       '//Generated from '+o.model.rpar.props.title(1)+' on '+date()
       'x=[];y=[];typ=[];'
       'select job'
       'case ''plot'' then'
       '  standard_draw(arg1)'
       'case ''getinputs'' then'
       '  [x,y,typ]=standard_inputs(arg1)'
       'case ''getoutputs'' then'
       '  [x,y,typ]=standard_outputs(arg1)'
       'case ''getorigin'' then'
       '  [x,y]=standard_origin(arg1)'
       'case ''set'' then'
       txtset
       'case ''define'' then'
       ]

  ierr=execstr('write(u,txt,''(a)'')','errcatch','n')
  if ierr<>0 then 
    messagebox('Impossible to write in this file; possibly locked.','modal')
    file('close',u)
  fname=emptystr()
  end
  dimen=o.graphics.sz/20
  dimen=dimen(:)'

  textdef=['  model=scicos_model()']
  model=o.model
  model.ipar=1;
  cc=getfield(1,model)
  cos2cosf(u,model.rpar,0)
  for ch=cc(2:$)
     if ch=='rpar' then
        textdef=[textdef;
                 '  model.rpar=scs_m_1']
     else
        chval=sci2exp(evstr('model.'+ch))
        textdef=[textdef;
                 '  model.'+ch+'='+chval(1);
                 chval(2:$)]
     end
  end

  for i=1:size(exprs0,1)
    ierr=execstr('strtmp=sci2exp(evstr(exprs0(i)),0)','errcatch')
    if ierr<>0 then strtmp='[]',disp('Cannot evaluate '+exprs0(i)),end
    textdef=[textdef;
             '  '+exprs0(i)+'='+strtmp
            ];
  end


  textdef=[textdef;
           '  exprs=[..';
           '       sci2exp('+exprs0+',0)'
           '        ]'
          ]

  gr_i_tmp = sci2exp(o.graphics.gr_i);
  if size(gr_i_tmp,1)<>1 then
      textdef=[textdef;
          '  gr_i='+gr_i_tmp(1)
          '       '+gr_i_tmp(2:$)];
  else
      textdef=[textdef;
          '  gr_i='+gr_i_tmp];
  end
  
  textdef=[textdef;
              '  x=standard_define('+sci2exp(dimen)+',model,exprs,gr_i)']
  

  txt=[ textdef
       'end'
       'endfunction'
       ]

  write(u,txt,'(a)')
  file('close',u)
endfunction
