function [x,y,typ]=DELAY_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  // paths to updatable parameters or states
  x=arg1
  ppath = list(4,5)
  newpar=list();
  register_label=x(3)(8)(4)(2)(4)
  evtdly_label=x(3)(8)(5)(2)(4)
  label=[evtdly_label(1);register_label]
  while %t do
    [ok,dt,z0,label]=getvalue(['This block implements as a discretised delay';
	'it is consist of a shift register and a clock';
	'value of the delay is given by;'
	'the discretisation time step multiplied by the';
	'number-1 of state of the register'],..
	    ['Discretisation time step';
	    'Register initial state'],list('vec',1,'vec',-1),label)
    if ~ok then break,end
    mess=[]
    if prod(size(z0))<2 then
      mess=[mess;'Register length must be at least 2';' ']
      ok=%f
    end    
    if dt<=0 then
      mess=[mess;'Discretisation time step must be positive';' ']
      ok=%f
    end
    if ~ok then message(mess);end
    if ok then
      //Change the clock
      x(3)(8)(5)(2)(4)(1)=label(1);
      x(3)(8)(5)(3)(11)=0; //initial delay firing date
      if x(3)(8)(5)(3)(8)<>dt then //Discretisation time step
	x(3)(8)(5)(3)(8)=dt
	newpar($+1)=5 // notify clock changes
      end
      //Change the register
      x(3)(8)(4)(2)(4)=label(2)
      if or(x(3)(8)(4)(3)(7)<>z0(:)) then //Register initial state
	x(3)(8)(4)(3)(7)=z0(:)
	newpar($+1)=4 // notify register changes
      end
      break
    end
  end

  needcompile=0
  y=needcompile
  typ=newpar
case 'define' then
  model = list('csuper',1,1,[],[],[],[],..
  list(list([600,400],'DELAY_f',[],[]),..
  list('Block',list([92,210],[20,20],%t,['1';'1'],[],7,[],[],[]),..
  list('input',[],1,[],[],[],[],[],1,'c',[],[%f,%f],' ',list()),' ','IN_f'),..
  list('Block',list([440,210],[20,20],%t,['1';'1'],6,[],[],[],[]),..
  list('output',1,[],[],[],[],[],[],1,'c',[],[%f,%f],' ',list()),' ','OUT_f'),..
  list('Block',list([238,195],[50,50],%t,'0;0;0;0;0;0;0;0;0;0',7,6,10,[],..
  ['dly=model(8);';'txt=[''Shift'';''Register'';string(dly)];';
  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']),..
  list('delay',1,1,1,[],[],zeros(10,1),[],[],'d',[],[%f,%f],' ',list()),' ','REGISTER_f'),..
  list('Block',list([243,296],[40,40],%t,['0.1';'1'],[],[],11,8,..
  ['dt=model(8);';'txt=[''Delay'';string(dt)];';
  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']),..
  list('evtdly',[],[],1,1,[],[],0.1,[],'d',1,[%f,%f],' ',list()),' ','EVTDLY_f'),..
  list('Link',[296.6;440],[220;220],'drawlink',' ',[0,0],[1,1],[4,1],[3,1]),..
  list('Link',[112;229.4],[220;220],'drawlink',' ',[0,0],[1,1],[2,1],[4,1]),..
  list('Link',[263;263],[290.3;271.2],'drawlink',' ',[0,0],[5,-1],[5,1],[9,1]),..
  list('Block',list([263;271.2],[1,1],%t,' ',[],[],8,[10;11]),..
  list('lsplit',[],[],1,[1;1],[],[],[],[],'d',[-1,-1],[%t,%f],' ',list()),' ','CLKSPLIT_f'),..
  list('Link',[263;263],[271.2;250.7],'drawlink',' ',[0,0],[5,-1],[9,1],[4,1]),..
  list('Link',[263;308.6;308.6;263;263],[271.2;271.2;367;367;341.7],'drawlink',..
  ' ',[0,0],[5,-1],[9,2],[5,1])),[],'h',%f,[%f,%f])
  gr_i='xstringb(orig(1),orig(2),''Delay'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,'DELAY_f',gr_i)
end
