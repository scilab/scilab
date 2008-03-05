//** 24 Jan 2006
//**--------------------------------------- OUTPUT -----------------------
//** SCICOS "Hardware In the Loop"
//** Output Scicos block for Measurement Computing USB-1208FS USB I/O Card
//** 
//** The code is based on the Comedi Analog Input from Roberto Bucher  
//
// Copyright roberto.bucher@supsi.ch

function [x,y,typ] = usb_dataout(job,arg1,arg2)
//
// Copyright roberto.bucher@supsi.ch
x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object 
  graphics=arg1.graphics; 
  exprs=graphics.exprs;
  ch=exprs(1)(1);
  name=exprs(1)(2);
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
       [ok,ch,name,com_range,aref,lab]=..
         getvalue('Set USB-1208FS block parameters',..
         [ 'Channel';
           'Device';
	          'Range';
            'Aref'],..
             list('vec',1,'str',1,'vec',1,'vec',1'),label(1))

    if ~ok then break,end
    
    label(1)=lab
    
    funam= 'analog_output';
    funtyp=4;
    
    xx=[];ng=[];z=0;
    nx=0;nz=0;
    o=[];
    i=1;nin=1;
    ci=1;nevin=1;
    co=[];nevout=0;
    
    depu=%t;
    dept=%f;
    dep_ut=[depu dept];

    //** [ok,tt]=getCode(funam)
    
    if ~ok then break,end
         [model,graphics,ok] = check_io(model,graphics,i,o,ci,co)
    
    if ok then
      model.sim = list(funam,funtyp) ; //** compuatation function 
      model.in=i
      model.out=[]
      model.evtin=ci
      model.evtout=[]
      model.state=[]
      model.dstate=0
      model.rpar=[]
      model.ipar=[ch; com_range; aref]; 
      model.firing=[]
      model.dep_ut=dep_ut
      model.nzcross=0
      //** label(2)=tt
      x.model=model
      graphics.exprs=label
      x.graphics=graphics
      break
    end
  end

case 'define' then      //** the standard define  
  ch=0; com_range=0; aref=0; 
  name='USB-1208FS'

  model=scicos_model()
  funam = 'analog_output';
  funtyp = 4 ;
  model.sim=list(funam, funtyp) //** simulating function

  model.in=1
  model.out=[]
  model.evtin=1
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[ch; com_range; aref]; 
  model.blocktype='c'
  model.firing=[]
  model.dep_ut=[%t %f]
  model.nzcross=0

  label=list([sci2exp(ch),name,sci2exp(com_range),sci2exp(aref)],[])

  gr_i=['xstringb(orig(1),orig(2),[''USB D/A'';name+'' CH-''+string(ch)],sz(1),sz(2),''fill'');']
  x = standard_define([4 2],model,label,gr_i)
end

endfunction


