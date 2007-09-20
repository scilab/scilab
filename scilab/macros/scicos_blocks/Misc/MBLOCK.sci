function [x,y,typ]=MBLOCK(job,arg1,arg2)
// 
// Copyright INRIA Oct 2006
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
  x=arg1
 model=arg1.model;graphics=arg1.graphics;
 label=graphics.exprs;
 while %t do
   [ok,Tin,Tintype,Tout,Touttype,Tparam,Tparamv,Tfunam,lab]=..
       getvalue('Set Modelica generic block parameters',..
       ['Input variables:       ';..
        'Input variables types: ';..
        'Output variables:      ';..
	'Output variables types:';..
    	'Parameters in Modelica:';..
	'Parameter  values:     ';..
        'Function name:         '],..
       list('str',-1,'str',-1,'str',-1,'str',-1,'str',-1,'str',-1,'str',-1),label(1))
   if ~ok then break,end
   ierr=execstr("in=stripblanks(evstr(Tin));..
   intype=stripblanks(evstr(Tintype));..
   out=stripblanks(evstr(Tout));..
   outtype=stripblanks(evstr(Touttype));..
   param=stripblanks(evstr(Tparam));..
   paramv=evstr(Tparamv);..
   funam=stripblanks(Tfunam)",'errcatch') 
   if ierr<>0  then x_message("Error, try again please!");break,end

   // type checking
   typeok=%t;
   for i=1:size(intype,'*')
     if intype(i)<>'E'&intype(i)<>'I' then 
       typeok=%f;
       x_message("Input type should be ''E'' or ''I''!");break;
     end
   end
   if ~typeok then break, end

   for i=1:size(outtype,'*')
     if outtype(i)<>'E'&outtype(i)<>'I' then 
       typeok=%f;
       x_message("Output type should be ''E'' or ''I''!");break;
     end
   end
   if ~typeok then break, end

   if or(size(intype)<>size(in)) then
     x_message("Input variables are not well defined!");break;
   end
   
   if or(size(outtype)<>size(out)) then
     x_message("Output variables are not well defined!");break;
   end
   
   if or(size(param)<>size(paramv)) then
     x_message("Parameters are not well defined!");break;
   end
   
   if funam=='' then x_message("The filename is not defined!");break,end
   [dirF,nameF,extF]=fileparts(funam);
   if (extF<>'' & extF<>'.mo')|(dirF<>'' & extF<>'.mo') then
     x_message("Filename extention should be ''.mo '' !"); break
   end
   
   label(1)=lab

   intypex=find(intype=='I'); outtypex=find(outtype=='I');
   [model,graphics,ok]=check_io(model,graphics,ones(in),ones(out),[],[],intypex,outtypex)

   //============================---------MBLOCK2
   if extF=='.mo' & fileinfo(funam)<>[] then  
     tt=mgetl(funam);
   else
     tt=label(2);
     if model.sim(1)<>nameF then
       tt=[]
     end
   end
   //============================
   if ok  then
     [ok,tt]=MODCOM(funam,tt,in,out);  
     if ~ok then break,end   
   end 

   if ok then
     //------------------
     mo=modelica()
     mo.model=nameF
     mo.inputs=in;
     mo.outputs=out;
     mo.parameters=list(param,paramv)
     model.equations=mo
     //------------------
     model.rpar=paramv;
     model.sim(1)=funam
     label(2)=tt
     x.model=model
	 graphics.gr_i(1)(1)='txt=[''Modelica'';'' ' + nameF + ' ''];'
     graphics.in_implicit =intype
     graphics.out_implicit=outtype
     graphics.exprs=label
     x.graphics=graphics
     break
   end
 end

case 'define' then
  in=['u1']
  intype=['I']
  out=['y1';'y2']
  outtype=['I';'E']
  param=['R';'L'];
  paramv=[0.1;0.0001];
  nameF='generic'

  label=list([sci2exp(in),sci2exp(intype),sci2exp(out),sci2exp(outtype),sci2exp(param),sci2exp(paramv),nameF]',[])
    
  model=scicos_model()
  model.blocktype='c';
  model.dep_ut=[%f %t];
  model.rpar=paramv;
  
  mo=modelica();
  mo.model=nameF;
  mo.parameters=list(param,paramv)
  model.sim=list(mo.model,30004);
  mo.inputs=in;
  mo.outputs=out;
  model.in=ones(size(mo.inputs,'r'),1)
  model.out=ones(size(mo.outputs,'r'),1)
  model.equations=mo
  gr_i=['txt=[''Modelica'';'' '+nameF+' ''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
  x.graphics.in_implicit =intype
  x.graphics.out_implicit=outtype
end
endfunction

