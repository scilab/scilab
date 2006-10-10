function [x,y,typ]=SUPER_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[]
select job
case 'plot' then
  standard_draw(arg1)
  s_port_names(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
 case 'set' then
  y=needcompile // in case leaving with Exit
  while %t do
    [x,newparameters,needcompile,edited]=scicos(arg1.model.rpar)
    arg1.model.rpar=x
//    x=arg1
    [ok,arg1]=adjust_s_ports(arg1)
    if ok then
      x=arg1
      y=needcompile
      typ=newparameters
      %exit=resume(%f)
    else
      %r=2
      %r=message(['SUPER BLOCK needs to be edited;';
		  'Edit or exit by removing all edition'],['Edit'; ...
		    'Exit'])
      if %r==2 then 
	typ=list()
	%exit=resume(%t),
      else
	if ~or(curwin==winsid()) then  // in case super block is closed
	  xset("window",curwin)
	end
      end
    end
  end
  
case 'define' then
  scs=scicos_diagram()
  scs.props.title='Super Block'
  model=scicos_model()
  model.sim='super'
  model.in=1
  model.out=1
  model.rpar=scs
  model.blocktype='h'
  model.dep_ut=[%f %f]

  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+      [2 4 4]*(sz(1)/7);';
    'yy=orig(2)+sz(2)-[2 2 6]*(sz(2)/10);';
    'xrects([xx;yy;[sz(1)/7;sz(2)/5]*ones(1,3)]);';
    'xx=orig(1)+      [1 2 3 4 5 6 3.5 3.5 3.5 4 5 5.5 5.5 5.5]*sz(1)/7;';
    'yy=orig(2)+sz(2)-[3 3 3 3 3 3 3   7   7   7 7 7   7   3  ]*sz(2)/10;';
    'xsegs(xx,yy,0);';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,[],gr_i)
end
endfunction
