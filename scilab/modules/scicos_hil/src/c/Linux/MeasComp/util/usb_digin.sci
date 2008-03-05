//** 24 Jan 2006
//** ------------------------------------ INPUT ---------------------------------
//** SCICOS "Hardware In the Loop"
//** Input Scicos block for Measurement Computing USB-1208FS USB I/O Card
//** 
//** The code is based on the Comedi Analog Input from Roberto Bucher (roberto.bucher@supsi.ch)
//** 


function [x,y,typ] = usb_digin(job,arg1,arg2)

x=[]; y=[]; typ=[];

select job //** main state machine switch

case 'plot' then                  //** plot the block 
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

case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)  //** inputs 

case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1) //** outputs 

case 'getorigin' then
  [x,y]=standard_origin(arg1)      //** location 

case 'set' then
  x = arg1 ; //** 
  model = arg1.model;
  graphics = arg1.graphics;
  label = graphics.exprs; //** "label" are the default values BEFORE the user input 
                          //** "lab" are the user AFTER the user input
  
  //** open a dialog box to SET the model pameters 
  while %t do
    [ok, ch, name, portname, lab]=..
         getvalue('Set USB-1208FS block parameters',..
                   [ 'Channel';
                     'Device' ;
                     'Port Name  '],..
                     list('vec',1,'str',1,'str',1),label)
         //** Channel->ch  Device->name  Port Sel->portsel  Port Dir->portdir               
    
    if ~ok then break,end //** if the input fail the exit 
     
    //** check if the user input is valid 
    if 	or(portname==["A" "B"]) //** + other tests 
       
       if portname=="A"
           portsel = 0
       elseif portname=="B"
           portsel = 1
       end      
            
       label = lab ; 
    
       funam = 'digital_input';
       funtyp = 4; //** valid numbers are 4->(C) 5->(Scilab) 
       
       xx = []; ng = [];
       z  = 0 ; nx = 0;
       nz = 0; i=[]; o = 1;
       nout = 1; ci=1; nevin=1;
       co=[]; nevout=0;
       
       depu=%t; dept=%f;
       dep_ut=[depu dept];
    
       if ~ok then break,end
    
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
         portdir = 0      ; //** 0->ALL Inputs, 1->ALL Outputs  
	 model.ipar=[ch; portsel; portdir]  ; 
         model.firing=[]  ; //** no 
         model.dep_ut = dep_ut ; //** 
         model.nzcross = 0
         x.model = model
         graphics.exprs = label
         x.graphics = graphics
         break ; //** Ok, then exit from the while loop :) 
       end
  
    else
  
     message ("User Input Not Valid");   
    
    end //** of the user input validation   
    
  end //** of the while 
  
case 'define' then
  ch = 0      ; //** bit zero PA_B0 , PB_B0
  portsel = 0 ; //** 0->Port A, 1->Port B
  portname = "A"  ;   
  portdir = 0 ; //** 0->ALL Inputs, 1->ALL Outputs  
  name='USB-1208FS' ; //** device name 

  model = scicos_model()
  funam = 'digital_input';
  funtyp = 4; //** numbers are 4->(C) 5->(Scilab) 
  model.sim=list(funam, funtyp) //** simulating function
  
  model.in=[]
  model.out=1
  
  model.evtin=1
  model.evtout=[]
  
  model.state=[]
  model.dstate=[]
  
  model.rpar = [] ; //** no real parameter here 
  model.ipar = [ch; portsel; portdir]  ; 
  
  model.blocktype='c'
  
  model.firing = []
  model.dep_ut = [%t %f]
  model.nzcross=0
    
  label = [sci2exp(ch), name, portname] ;
  
  gr_i = ['xstringb(orig(1), orig(2), [''USB DigIn.Port-''+portname; name+'' CH-''+string(ch)],sz(1),sz(2),''fill'');']
  
  x = standard_define([4 2], model, label, gr_i)

end

endfunction

