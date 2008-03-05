//** 1 Feb 2006
//**-------------------------- DIGITAL INPUT -----------------------
//** SCICOS "Hardware In the Loop"
//** Digital Input Scicos HIL block for Comedi Devices 
//** 
//** The code is based on the Comedi Digital Input from Roberto Bucher  
//
// Copyright roberto.bucher@supsi.ch / simone.mannori@inria.fr

function [x,y,typ] = comedi_dig_in(job,arg1,arg2)

x=[]; y=[]; typ=[];

select job //** main state machine switch

case 'plot' then                   //** plot the block 
  graphics=arg1.graphics; exprs=graphics.exprs;
  ch=exprs(1)(1);name=exprs(1)(2);
  standard_draw(arg1)

case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1) //** inputs 

case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1) //** outputs 

case 'getorigin' then
  [x,y]=standard_origin(arg1)      //** location 

case 'set' then
  x = arg1
  model = arg1.model; graphics = arg1.graphics;
  label = graphics.exprs;
  
  //** open a dialog box to SET the model pameters 
  while %t do
    [ok, ch, name,lab]=..
        getvalue('Set Comedi Dig.In block parameters',..
             ['Channel';
              'Device'],..
               list('vec',1,'str',1),label(1))
    
    if ~ok then break,end //** if the input fail the exit 
        
    label(1) = lab ; 
    
    funam = 'comedi_digital_input';
    funtyp = 4; //** valid numbers are 4->(C) 5->(Scilab) 
   
    xx = [];
    
    ng = [];
    z  = 0 ;
    nx = 0;
    nz = 0;
    i=[];
    
    o = 1; nout = 1;
    
    ci=1; nevin=1;
    
    co=[]; nevout=0;

    depu=%t;
    dept=%f;
    dep_ut=[depu dept];
    
    [model,graphics,ok] = check_io(model,graphics,i,o,ci,co)
    
    if ok then
      model.sim = list(funam, funtyp) //** simulating function 
      model.in = [] ; //** no input 
      model.out = o ; 
      model.evtin = ci ; //** event input 
      model.evtout=[]  ; //** no event output 
      model.state=[]   ; //** no state 
      model.dstate=0   ; //** no discrete state 
      model.rpar=[]    ; //** no real parameter 
      
      model.ipar=[ch]  ;
      
      model.firing=[]  ; //** no 
      model.dep_ut = dep_ut ; //** 
      model.nzcross = 0
      x.model = model
      graphics.exprs = label
      x.graphics = graphics
      break ; //** Ok, then exit from the while loop :) 
    end
    
  end //** of the while 
  
case 'define' then
  ch=0;
  name='Comedi Dig.In'

  model = scicos_model()
  funam = 'comedi_digital_input';
  funtyp = 4; //** valid numbers are 4->(C) 5->(Scilab) 
  model.sim = list(funam, funtyp) //** simulating function
  
  model.in=[]
  model.out=1
  
  model.evtin=1
  model.evtout=[]
  
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  
  model.ipar=[ch]
  
  model.blocktype='c'
  
  model.firing = []
  model.dep_ut = [%t %f]
  model.nzcross=0

  label = list([sci2exp(ch),name],[])

  gr_i = ['xstringb(orig(1),orig(2),[''Comedi Dig.In'';name+'' CH-''+string(ch)],sz(1),sz(2),''fill'');']
  
  x = standard_define([3 2], model, label, gr_i);

end

endfunction

