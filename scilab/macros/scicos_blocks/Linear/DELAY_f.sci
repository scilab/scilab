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
  register=x.model.rpar(4) //data structure of register block
  evtdly=x.model.rpar(5) //data structure of evtdly block
  register_exprs=register.graphics.exprs
  evtdly_exprs=evtdly.graphics.exprs
  exprs=[evtdly_exprs(1);register_exprs]
  while %t do
    [ok,dt,z0,exprs]=getvalue(['This block implements as a discretised delay';
		    'it is consist of a shift register and a clock';
		    'value of the delay is given by;'
		    'the discretisation time step multiplied by the';
		    'number-1 of state of the register'],..
			      ['Discretisation time step';
		    'Register initial state'],list('vec',1,'vec',-1),exprs)
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
    if ~ok then
      message(mess);
    else
      //Change the clock
      evtdly.graphics.exprs(1)=exprs(1);
      evtdly.model.firing=0; //initial delay firing date

      if evtdly.model.rpar<>dt then //Discretisation time step
	evtdly.model.rpar=dt
	newpar($+1)=5 // notify clock changes
      end
      x.model.rpar(5)=evtdly
      //Change the register
      register.graphics.exprs=exprs(2)
      if or(register.model.dstate<>z0(:)) then //Register initial state
	register.model.dstate=z0(:)
	newpar($+1)=4 // notify register changes
      end
      x.model.rpar(4)=register
      break
    end
  end
  needcompile=0
  y=needcompile
  typ=newpar
case 'define' then
  evtdly=EVTDLY_f('define')
    evtdly.graphics.orig=[243,296]
    evtdly.graphics.sz=[40,40]
    evtdly.graphics.flip=%t
    evtdly.graphics.exprs=['0.1';'1']
    evtdly.graphics.pein=11
    evtdly.graphics.peout=8
    evtdly.model.rpar=0.1
    evtdly.model.firing=1
  register=REGISTER_f('define')
    register.graphics.orig=[238,195]
    register.graphics.sz=[50,50]
    register.graphics.flip=%t
    register.graphics.exprs='0;0;0;0;0;0;0;0;0;0'
    register.graphics.pin=7
    register.graphics.pout=6
    register.graphics.pein=10
  input_port=IN_f('define')
    input_port.graphics.orig=[92,210]
    input_port.graphics.sz=[20,20]
    input_port.graphics.flip=%t
    input_port.graphics.exprs=['1';'1']
    input_port.graphics.pout=7
    input_port.model.ipar=1
  output_port=OUT_f('define')
    output_port.graphics.orig=[440,210]
    output_port.graphics.sz=[20,20]
    output_port.graphics.flip=%t
    output_port.graphics.exprs=['1';'1']
    output_port.graphics.pin=6
    output_port.model.ipar=1
  
  split=CLKSPLIT_f('define')
    split.graphics.orig=[263;271.2]
    output_port.graphics.pein=8,
    output_port.graphics.peout=[10;11]
   
  x=scicos_block()
  x.gui='DELAY_f'
  x.graphics.sz=[2,2]
  x.graphics.gr_i='xstringb(orig(1),orig(2),''Delay'',sz(1),sz(2),''fill'')'
  x.model.sim='csuper'
  x.model.in=1
  x.model.out=1
  x.model.blocktype='h'
  x.model.firing=%f
  x.model.dep_ut=[%f %f]
  x.model.rpar=empty_diagram();
  x.model.rpar(2)=input_port
  x.model.rpar(3)=output_port
  x.model.rpar(4)=register
  x.model.rpar(5)=evtdly
  x.model.rpar(6)=scicos_link(xx=[296.6;440],yy=[220;220],from=[4,1],to=[3,1])
  x.model.rpar(7)=scicos_link(xx=[112;229.4],yy=[220;220],from=[2,1],to=[4,1])
  x.model.rpar(8)=scicos_link(xx=[263;263],yy=[290.3;271.2],ct=[5,-1],..
			       from=[5,1],to=[9,1])
  x.model.rpar(9)=split
  x.model.rpar(10)=scicos_link(xx=[263;263],yy=[271.2;250.7],ct=[5,-1],..
			       from=[9,1],to=[4,1])
  x.model.rpar(11)=scicos_link(xx=[263;308.6;308.6;263;263],..
			       yy=[271.2;271.2;367;367;341.7],..
			       ct=[5,-1],from=[9,2],to=[5,1])
end
endfunction
