//** 31 Jan 2006
//**----------------------------- DIGITAL OUTPUT -----------------------
//** SCICOS "Hardware In the Loop"
//** Output Scicos HIL block for Comedi Devices 
//** 
//** The code is based on the Comedi Analog Output from Roberto Bucher  
//
// Copyright roberto.bucher@supsi.ch / simone.mannori@inria.fr

function [x,y,typ] = usb_digout(job,arg1,arg2)
//
// Copyright roberto.bucher@supsi.ch
x=[];y=[];typ=[];

select job //** main state machine switch

case 'plot' then  //** plot the object 
  graphics = arg1.graphics;
  model    = arg1.model;
  
  ch = model.ipar(1)       ; 
  name = graphics.exprs(2) ;
  portsel = model.ipar(2)  ;

  if portsel==0
     portname="A"
  elseif portsel==1
     portname="B"
  end 
  
  standard_draw(arg1)

case 'getinputs' then //** inputs 
  [x,y,typ]=standard_inputs(arg1) //** 

case 'getoutputs' then //** 
  [x,y,typ]=standard_outputs(arg1)

case 'getorigin' then  //** 
  [x,y]=standard_origin(arg1)

case 'set' then //** set parameters 
  x = arg1 ; //** 
  model = arg1.model;
  graphics = arg1.graphics;
  label = graphics.exprs; //** "label" are the default values BEFORE the user input 
                          //** "lab" are the user AFTER the user input 
  
  while %t do
        [ok, ch, name, portname, threshold, lab]=..
              getvalue('Set USB-1208FS block parameters',..
              [ 'Channel';
                'Device' ;
                'Port Name  ';
                'Threshold  '],..
                 list('vec',1,'str',1,'str',1,'vec',1),label)
                 //** Channel->ch  Device->name  Port Sel->portsel  Port Dir->portdir               
		 
    if ~ok then break,end

    //** check if the user input is valid 
    if 	or(portname==["A" "B"]) //** + other tests 
       
       if portname=="A"
           portsel = 0
       elseif portname=="B"
           portsel = 1
       end      
       
       label = lab ; //** update the structure 
       funam  = 'digital_output';
       funtyp = 4; //** C code 
    
       xx=[]; ng=[]; z=0;
       nx=0;  nz=0;
       o=[];
       i=1; nin=1;
       ci=1; nevin=1;
       co=[];nevout=0;
       
       depu=%t; dept=%f;
       dep_ut = [depu dept];
    
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
  
          portdir = 1 ; //** 1->ALL Outputs  
          model.rpar = [threshold] ;
          model.ipar = [ch; portsel; portdir] ; 
      
          model.firing=[]
          model.dep_ut = dep_ut ;
          model.nzcross=0
          x.model = model ;
          graphics.exprs=label
          x.graphics=graphics
          break //** -> exit point 
      end 

  else
  
     message ("User Input Not Valid");   
    
  end //** of the user input validation   
    
      
  end //** end of while(1) 

case 'define' then      //** the standard define  
  ch = 0          ; //** bit zero PA_B0 , PB_B0
  portsel = 0     ; //** 0->Port A, 1->Port B
  portname = "A"  ;  
  portdir = 1 ; //** 0->ALL Inputs, 1->ALL Outputs  
  threshold = 0 ; //** threshold level 
  name='USB-1208FS' ; //** device name 

  model=scicos_model()
  
  funam= 'digital_output';
  funtyp=4;
  
  model.sim=list(funam, funtyp) //** simulating function

  model.in=1
  model.out=[]
  model.evtin=1
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  
  model.rpar = [threshold] ;
  model.ipar = [ch; portsel; portdir]  ; 
  
  model.blocktype='c'
  model.firing=[]
  model.dep_ut=[%t %f]
  model.nzcross=0

  label =[sci2exp(ch), name, portname, sci2exp(threshold)] ;
 
  gr_i = ['xstringb(orig(1), orig(2), [''USB DigOut.Port-''+portname; name+'' CH-''+string(ch)],sz(1),sz(2),''fill'');']
  
  x = standard_define([4 2], model, label, gr_i)
end

endfunction


