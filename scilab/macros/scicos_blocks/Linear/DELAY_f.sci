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
  if x.model.rpar.objs(1)==mlist('Deleted') then
      ppath = list(4,5)  //compatibility with translated blocks
    else
      ppath = list(3,4)
    end
  newpar=list();
  register=x.model.rpar.objs(ppath(1)) //data structure of register block
  evtdly=x.model.rpar.objs(ppath(2)) //data structure of evtdly block
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
//      evtdly.model.firing=0; //initial delay firing date

      if evtdly.model.rpar<>dt then //Discretisation time step
	evtdly.model.rpar=dt
	newpar($+1)=ppath(2) // notify clock changes
      end
      x.model.rpar.objs(ppath(2))=evtdly
      
      //Change the register
      register.graphics.exprs=exprs(2)
      if or(register.model.dstate<>z0(:)) then //Register initial state
	register.model.dstate=z0(:)
	newpar($+1)=ppath(1) // notify register changes
      end
      x.model.rpar.objs(ppath(1))=register
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
    evtdly.graphics.pein=10
    evtdly.graphics.peout=7
    evtdly.model.rpar=0.1
    evtdly.model.firing=0
    
  register=REGISTER_f('define')
    register.graphics.orig=[238,195]
    register.graphics.sz=[50,50]
    register.graphics.flip=%t
    register.graphics.exprs='0;0;0;0;0;0;0;0;0;0'
    register.graphics.pin=6
    register.graphics.pout=5
    register.graphics.pein=9
    
  input_port=IN_f('define')
    input_port.graphics.orig=[92,210]
    input_port.graphics.sz=[20,20]
    input_port.graphics.flip=%t
    input_port.graphics.exprs=['1';'1']
    input_port.graphics.pout=6
    input_port.model.ipar=1
    
  output_port=OUT_f('define')
    output_port.graphics.orig=[440,210]
    output_port.graphics.sz=[20,20]
    output_port.graphics.flip=%t
    output_port.graphics.exprs=['1';'1']
    output_port.graphics.pin=5
    output_port.model.ipar=1
  
  split=CLKSPLIT_f('define')
    split.graphics.orig=[263;271.2]
    split.graphics.pein=7,
    split.graphics.peout=[9;10]
    
  diagram=scicos_diagram();
    diagram.objs(1)=input_port
    diagram.objs(2)=output_port
    diagram.objs(3)=register
    diagram.objs(4)=evtdly
    diagram.objs(5)=scicos_link(xx=[296.6;440],yy=[220;220],..
				from=[3,1],to=[2,1])
    diagram.objs(6)=scicos_link(xx=[112;229.4],yy=[220;220],..
				from=[1,1],to=[3,1])
    diagram.objs(7)=scicos_link(xx=[263;263],yy=[290.3;271.2],ct=[5,-1],..
				from=[4,1],to=[8,1])
    diagram.objs(8)=split
    diagram.objs(9)=scicos_link(xx=[263;263],yy=[271.2;250.7],ct=[5,-1],..
				from=[8,1],to=[3,1])
    diagram.objs(10)=scicos_link(xx=[263;308.6;308.6;263;263],..
				 yy=[271.2;271.2;367;367;341.7],..
				 ct=[5,-1],from=[8,2],to=[4,1]) 
  x=scicos_block()
    x.gui='DELAY_f'
    x.graphics.sz=[2,2]
    x.graphics.gr_i=list('xstringb(orig(1),orig(2),''Delay'',sz(1),s"+...
			 "z(2),''fill'')',8) 
    x.graphics.pin=0
    x.graphics.pout=0
    x.model.sim='csuper'
    x.model.in=1
    x.model.out=1
    x.model.blocktype='h'
    x.model.dep_ut=[%f %f]
    x.model.rpar=diagram
end
endfunction
