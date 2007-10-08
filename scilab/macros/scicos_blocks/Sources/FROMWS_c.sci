function [x,y,typ]=FROMWS_c(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
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
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,varnam,Method,ZC,OutEnd,exprs]=getvalue('Set From_Workspace block parameters',..
	['Variable name';'Interpolation Method';'Enable zero crossing(0:No, 1:Yes)?';'Output at end(0:Zero, 1:Hold, 2:Repeat)'],...
	list('str',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if ~(Method==0 | Method==1| Method==2| Method==3) then 
      x_message('Interpolation method should be chosen in [0,1,2,3]');
      ok=%f;
    end
    
    if ~(ZC==0 | ZC==1) then 
      x_message('Zero crossing should be either 0 or 1');
      ok=%f;
    end
    
    if ~(OutEnd==0 | OutEnd==1| OutEnd==2) then 
      x_message('Output at end option should be either 0 or 1');
      ok=%f;
    end

    //check for valid name variable
    r=%f;
    ierr=execstr('r=validvar(varnam)','errcatch')
    if ~r then
      message(["Invalid variable name.";
               "Please choose another variable name."]);
      ok=%f
    end

    if ok then
      model.ipar=[length(varnam);str2code(varnam);Method;ZC;OutEnd;];
      [model,graphics,ok]=set_io(model,graphics,list(),list([-1,1],-1),1,1);      
      if ok then
	graphics.exprs=exprs;
	x.graphics=graphics;x.model=model
	break
      end
    end
  end

case 'define' then
 varnam='V';// V.tume=0; V.value=1;
 Method=1;
 ZC=1;
 OutEnd=0;
 
 model=scicos_model();
 model.sim=list('fromws_c',4);
 model.out=-1;
 model.out2=1
 model.outtyp=-1
 model.ipar=[length(varnam);str2code(varnam);Method;ZC;OutEnd;];
 model.evtin=[1];
 model.evtout=[1];
 model.firing=[0];
 model.blocktype='d';
 model.dep_ut=[%f %t];
 gr_i=['txt=[''From workspace''];'..
       ;'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']   
 exprs=[string(varnam),string(Method),string(ZC),string(OutEnd)];
 x=standard_define([3.5 2],model,exprs,gr_i)
end
endfunction

