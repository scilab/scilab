function path=save_super(scs_m,fpath,gr_i,sz)
// given a super block definition scs_m save_super creates 
// this super block's interfacing function
// 19/07/07 : Alan added two rhs : gr_i and sz for
//                                 initial values
//            gr_i a vector of string (matrix size nx1)
//            sz a vector of real (matrix size 1x2)
// Copyright INRIA
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
  model.sim='super'
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
       'case ''set'' then'
       '  y=needcompile'
       '  while %t do'
       '    [x,newparameters,needcompile]=scicos(arg1.model.rpar)'
       '    arg1.model.rpar=x'
       '    [ok,arg1]=adjust_s_ports(arg1)'
       '    if ok then'
       '      x=arg1'
       '      y=needcompile'
       '      typ=newparameters'
       '      %exit=resume(%f)'
       '    else'
       '      %r=2'
       '      %r=message([''SUPER BLOCK needs to be edited;'';''Edit or exit by removing all edition''],[''Edit'';''Exit''])'
       '      if %r==2 then typ=list(),%exit=resume(%t),end'
       '    end'
       '  end']

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

  t1=[strsubst(t1,sci2exp('%scs_m_1'),'scs_m_1')]
  if gr_i == '' then
    t1=[t1
        '  gr_i=''xstringb(orig(1),orig(2),'''''+nam+''''',sz(1),sz(2),''''fill'''')'';']
  else
    gr_i_tmp = sci2exp(gr_i);
    if size(gr_i_tmp,1)<>1 then
      t1=[t1
          '  gr_i='+gr_i_tmp(1)
          gr_i_tmp(2:$)]
    else
      t1=[t1
          '  gr_i='+gr_i_tmp]
    end
  end
  if sz<>[] then
      t1=[t1
          '  x=standard_define(['+string(sz(1))+...
                                ' '+string(sz(2))+'],model,[],gr_i)';]
  else
      t1=[t1
          '  x=standard_define([2 2],model,[],gr_i)';]
  end
  t1=[t1
      'end'
      'endfunction']

  write(u,t1,'(a)')
  file('close',u)
endfunction
