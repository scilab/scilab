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
    [ok,varnam,order,ZC,OutEnd,exprs]=getvalue('Set From_Workspace block parameters',..
	['Variable name';'Interpolation order';'Enable zero crossing(0:No, 1:Yes)?';'Output at end(0:Zero, 1:Hold, 2:Repeat)'],...
	list('str',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if order<0 | order>1 then 
      x_message('Interpolation order should be either 0 or 1');
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

    
    if length(varnam)==0  then
      message(["Invalid variable name.";
	       "Please choose another variable name."] );
      ok=%f
    else
      ierr=execstr(varnam+'=0','errcatch')
      if ierr <> 0 then
	message(["Invalid variable name.";
		 "Please choose another variable name."] );
	ok=%f
      end
    end

    if ok then
      model.ipar=[length(varnam);str2code(varnam);order;ZC;OutEnd;];
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
 order=1;
 ZC=1;
 OutEnd=0;
 
 model=scicos_model();
 model.sim=list('fromws_c',4);
 model.out=-1;
 model.out2=1
 model.outtyp=-1
 model.ipar=[length(varnam);str2code(varnam);order;ZC;OutEnd;];
 model.evtin=[1];
 model.evtout=[1];
 model.firing=[0];
 model.blocktype='d';
 model.dep_ut=[%f %f];
 gr_i=['txt=[''From workspace''];'..
       ;'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']   
 exprs=[string(varnam),string(order),string(ZC),string(OutEnd)];
 x=standard_define([3.5 2],model,exprs,gr_i)
end
endfunction

