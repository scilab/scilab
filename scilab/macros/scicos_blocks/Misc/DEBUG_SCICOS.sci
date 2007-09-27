function [x,y,typ]=DEBUG_SCICOS(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    standard_draw(arg1)
   case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
   case 'getoutputs' then
    x=[];y=[];typ=[];
   case 'getorigin' then
    [x,y]=standard_origin(arg1)
   case 'set' then
    x=arg1;
    graphics=arg1.graphics;exprs=graphics.exprs
    textmp=exprs(2)
    ok=%t
    while 1==1
      [txt]=dialog(['Enter scilab instructions for debugging.';
		     ' Inputs are block and flag, output is block'],..
		     textmp);
  
      if txt<>[] then
	tt=['block=debug_scicos(block,flag)']

	if execstr('deff(tt,txt)','errcatch')==0 then
	  save(TMPDIR+'/debug_scicos',debug_scicos)
	  exprs(2)=txt
          if (scicos_debug()<>2 & scicos_debug()<>3) then
	   scicos_debug(2)
          end
	  break
	else
	  message(['Error in the instructions';lasterror()])
	end
      else
	ok=%f;break;
      end  
    end
    if ok then
      graphics.exprs=exprs;
      x.graphics=graphics;
    end
    
   case 'define' then
    model=scicos_model()
    model.sim=list('%debug_scicos',99)
    model.blocktype='d'
    
    exprs=list('','pause')
    gr_i=['xstringb(orig(1),orig(2),''Debug'',sz(1),sz(2),''fill'')']
    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction
