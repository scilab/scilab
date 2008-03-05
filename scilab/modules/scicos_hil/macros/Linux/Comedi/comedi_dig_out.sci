//** 31 Jan 2006
//**----------------------------- DIGITAL OUTPUT -----------------------
//** SCICOS "Hardware In the Loop"
//** Output Scicos HIL block for Comedi Devices 
//** 
//** The code is based on the Comedi Analog Output from Roberto Bucher  
//
// Copyright roberto.bucher@supsi.ch / simone.mannori@inria.fr

function [x,y,typ] = comedi_dig_out(job,arg1,arg2)
//
// Copyright roberto.bucher@supsi.ch
x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object 
  graphics=arg1.graphics; exprs=graphics.exprs;
  ch=exprs(1)(1);name=exprs(1)(2);
  standard_draw(arg1)

case 'getinputs' then //** inputs 
  [x,y,typ]=standard_inputs(arg1) //** 

case 'getoutputs' then //** 
  [x,y,typ]=standard_outputs(arg1)

case 'getorigin' then  //** 
  [x,y]=standard_origin(arg1)

case 'set' then //** set parameters 
  x = arg1 ; //** 
  model = arg1.model; graphics=arg1.graphics;
  label = graphics.exprs;
  
  while %t do
       [ok,ch,name,thresh,lab]=..
        getvalue('Set Comedi Dig.Out block parameters',..
        ['Channel';
        'Device';
        'Threshold'],..
         list('vec',1,'str',1,'vec',1),label(1))

    if ~ok then break,end
    
    label(1)=lab
    
    funam  = 'comedi_digital_output';
    funtyp = 4;
    
    xx=[];ng=[];z=0;
    nx=0;nz=0;
    o=[];
    i=1;nin=1;
    ci=1;nevin=1;
    co=[];nevout=0;
    
    depu=%t;
    dept=%f;
    dep_ut=[depu dept];
    
    [model,graphics,ok] = check_io(model,graphics,i,o,ci,co)
    
    if ok then
    
      model.sim = list(funam,funtyp) ; //** computational function 
      model.in=i
      model.out=[]
      model.evtin=ci
      model.evtout=[]
      model.state=[]
      model.dstate=0
      model.rpar=[]
  
      model.rpar = [thresh] ;
      model.ipar = [ch]   ;
      
      model.firing=[]
      model.dep_ut=dep_ut
      model.nzcross=0
      x.model=model
      graphics.exprs=label
      x.graphics=graphics
      break
    end //** -> exit point 
  
  end //** end of while(1) 

case 'define' then      //** the standard define  
  ch=0; thresh=0 ; // default values 

  name='Comedi Dig.Out' ; //** Show in the block 

  model=scicos_model()
  
  funam= 'comedi_digital_output';
  funtyp=4;
  
  model.sim=list(funam, funtyp) //** simulating function

  model.in=1
  model.out=[]
  model.evtin=1
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  
  model.rpar = [thresh] ;
  model.ipar = [ch]   ;
  
  model.blocktype='c'
  model.firing=[]
  model.dep_ut=[%t %f]
  model.nzcross=0

  label=list([sci2exp(ch),name,sci2exp(thresh)],[])
  
  gr_i=['xstringb(orig(1),orig(2),[''Comedi Dig.Out'';name+'' CH-''+string(ch)],sz(1),sz(2),''fill'');']
  x = standard_define([3 2],model,label,gr_i)
end

endfunction


