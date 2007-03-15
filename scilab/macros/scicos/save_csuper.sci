function path=save_csuper(scs_m,fpath)
// given a super block definition x save_super creates a file which contains
// this super block  handling  macro definition
// Copyright INRIA
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
  txt=[
      'function [x,y,typ]='+nam+'(job,arg1,arg2)';
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
	 '  '+com+' paths to updatable parameters or states'
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
	 '    if diffobjs(xxn,xx) then '
	 '	model=xx.model'
	 '	 model_n=xxn.model'
	 '	 if ~is_modelica_block(xx) then'
	 '             modified=or(model.sim<>model_n.sim)|..'
	 '	     ~isequal(model.state,model_n.state)|..'
	 '	     ~isequal(model.dstate,model_n.dstate)|..'
	 '	     ~isequal(model.rpar,model_n.rpar)|..'
	 '	     ~isequal(model.ipar,model_n.ipar)|..'
	 '	     ~isequal(model.label,model_n.label)'
	 '      if or(model.in<>model_n.in)|or(model.out<>model_n.out) then  '
	 '	  needcompile=1'
	 '	end'
	 '	if or(model.firing<>model_n.firing)  then '
	 '	  needcompile=2'
	 '	end'
	 '	if (size(model.in,''*'')<>size(model_n.in,''*''))|..'
	 '        (size(model.out,''*'')<>size(model_n.out,''*'')) then  '
	 '         needcompile=4  '
	 '       end  '
	 '	if model.sim==''input''|model.sim==''output'' then'
	 '	  if model.ipar<>model_n.ipar then'
	 '	    needcompile=4'
	 '	  end'
	 '	end'
	 '	if or(model.blocktype<>model_n.blocktype)|..'
	 '	      or(model.dep_ut<>model_n.dep_ut)  then '
	 '	  needcompile=4'
	 '	end'
	 '	if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then '
	 '	  needcompile=4'
	 '	end'
	 '	if prod(size(model_n.sim))>1 then'
	 '	  if model_n.sim(2)>1000 then '
	 '	    if model.sim(1)<>model_n.sim(1) then'
	 '	      needcompile=4'
	 '	    end'
	 '	  end'
	 '	end'
	 '      else '
	 '	modified=or(model_n<>model)'
	 '	eq=model.equations;eqn=model_n.equations;'
	 '	if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..'
	 '				      or(eq.outputs<>eqn.outputs) then  '
	 '	  needcompile=4'
	 '	end'
	 '      end'
	 '      '+com+' parameter or states changed'
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
  t1=sci2exp(model,'model');
  t1=[strsubst(t1,sci2exp('%scs_m_1'),'scs_m_1')
      '  gr_i=''xstringb(orig(1),orig(2),'''''+nam+''''',sz(1),sz(2),''''fill'''')'';'
      '  x=standard_define([2 2],model,[],gr_i)';
      'end'
      'endfunction']
  write(u,t1,'(a)')
  file('close',u)
endfunction
