function [x,y,typ]=CBLOCK(job,arg1,arg2)
//
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
  x=arg1
 model=arg1.model;graphics=arg1.graphics;
 label=graphics.exprs;
  while %t do
    [ok,junction_name,impli,i,o,ci,co,xx,ng,z,rpar,ipar,auto0,depu,dept,lab]=..
        getvalue('Set C-Block2 block parameters',..
        ['simulation function';
	 'is block implicit? (y,n)';
        'input ports sizes';
        'output ports sizes';
        'input event ports sizes';
        'output events ports sizes';
        'initial continuous state';
	'number of zero crossing surfaces';
        'initial discrete state';
        'Real parameters vector';
        'Integer parameters vector';      
        'initial firing vector (<0 for no firing)';
        'direct feedthrough (y or n)';                                       
        'time dependence (y or n)'],..
         list('str',1,'str',1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,..
         'vec',-1,'vec',1,'vec',-1,'vec',-1,'vec',-1,'vec','sum(%7)',..
         'str',1,'str',1),label(1))
    if ~ok then break,end
    label(1)=lab
    funam=stripblanks(junction_name)
    xx=xx(:);z=z(:);rpar=rpar(:);ipar=int(ipar(:));
    nx=size(xx,1);nz=size(z,1);
    i=int(i(:));
    o=int(o(:));nout=size(o,1);
    ci=int(ci(:));nevin=size(ci,1);
    co=int(co(:));nevout=size(co,1);
    if part(impli,1)=='y' then funtyp=12004, else funtyp=2004,end
    if [ci;co]<>[] then
      if maxi([ci;co])>1 then message('vector event links not supported');ok=%f;end
    end
    depu=stripblanks(depu);if part(depu,1)=='y' then depu=%t; else depu=%f;end
    dept=stripblanks(dept);if part(dept,1)=='y' then dept=%t; else dept=%f;end
    dep_ut=[depu dept];
    

    if funam==' ' then break,end
    tt=label(2);
    if model.sim(1)<>funam|sign(size(model.state,'*'))<>sign(nx)|..
	  sign(size(model.dstate,'*'))<>sign(nz)|model.nzcross<>ng|..
    sign(size(model.evtout,'*'))<>sign(nevout) then
      tt=[]
    end
    [ok,tt]=CFORTR2(funam,tt)
    if ~ok then break,end
    [model,graphics,ok]=check_io(model,graphics,i,o,ci,co)
    if ok then
      model.sim=list(funam,funtyp)
      model.in=i
      model.out=o
      model.evtin=ci
      model.evtout=co
      model.state=xx
      model.dstate=z
      model.rpar=rpar
      model.ipar=ipar
      model.firing=auto0
      model.dep_ut=dep_ut
      model.nzcross=ng
      label(2)=tt
      x.model=model
      graphics.exprs=label
      x.graphics=graphics
      break
    end
  end
case 'define' then
  in=1
  out=1
  clkin=[]
  clkout=[]
  x0=[]
  z0=[]
  typ='c'
  auto=[]
  rpar=[]
  ipar=[]
  funam='toto'
  ng=0

model=scicos_model()
model.sim=list(' ',2004)
model.in=in
model.out=out
model.evtin=clkin
model.evtout=clkout
model.state=x0
model.dstate=z0
model.rpar=rpar
model.ipar=ipar
model.blocktype=typ
model.firing=auto
model.dep_ut=[%t %f]
model.nzcross=ng

label=list([funam,'n',sci2exp(in),sci2exp(out),sci2exp(clkin),sci2exp(clkout),..
	   sci2exp(x0),sci2exp(0),sci2exp(z0),sci2exp(rpar),sci2exp(ipar),..
	   sci2exp(auto),'y','n']',[])

  gr_i=['xstringb(orig(1),orig(2),''C block2'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction



function [ok,tt]=CFORTR2(funam,tt)
//
if tt==[] then
  
  textmp=[
	  '#include <math.h>';
	  '#include <stdlib.h>';
	  '#include <scicos/scicos_block.h>';
	  'void '+funam+'(scicos_block *block,int flag)';
	 ];
  ttext=[];
  textmp($+1)='{'
  textmp=[textmp;
	  '  /* ';
	  '  int block->nevprt;'
	  '  int block->nz;'
	  '  double* block->z;'
	  '  int block->nx;'
	  '  double* block->x;'
	  '  double* block->xd;'
	  '  double* block->res;'
	  '  int block->nin;'
	  '  int *block->insz;'
	  '  double **block->inptr;'
	  '  int block->nout;'
	  '  int *block->outsz;'
	  '  double **block->outptr;'
	  '  int block->nevout;'
	  '  int block->nrpar;'
	  '  double *block->rpar;'
	  '  int block->nipar;'
	  '  int *block->ipar;'
	  '  int block->ng;'
	  '  double *block->g;'
	  '  int *block->jroot;'
	  '  char block->label[41];'
	  '  */']
  
  textmp($+1)='  if (flag == 4) { /* initialization */'
  textmp($+1)='   '+funam+"_bloc_init(block,flag);"
    ttext=[ttext;'int '+funam+"_bloc_init(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  textmp($+1)=' '
  if nout<>0 then 
    textmp($+1)='  } else if(flag == 1) { /* output computation*/'
    textmp($+1)='   set_block_error('+funam+"_bloc_outputs(block,flag));"
    ttext=[ttext;'int '+funam+"_bloc_outputs(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  end
  
  if nx<>0 then 
    textmp($+1)='  } else if(flag == 0) { /* derivative or residual computation*/',
    textmp($+1)='   set_block_error('+funam+"_bloc_deriv(block,flag));"
    ttext=[ttext;'int '+funam+"_bloc_deriv(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  end
  
  if ng <>0 then
    textmp($+1)='  } else if(flag == 9) {/* zero crossing surface and mode computation*/',
    textmp($+1)='     set_block_error('+funam+"_bloc_zcross(block,flag));";
    ttext=[ttext;'int '+funam+"_bloc_zcross(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  end
  
  if nz<>0 then 
    textmp($+1)='  } else if(flag == 2) { /* computation of next discrte state*/ ',
    textmp($+1)='     set_block_error('+funam+"_bloc_states(block,flag));";
        ttext=[ttext;'int '+funam+"_bloc_states(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  elseif min(nx,ng+nevin)>0 then 
    textmp($+1)='  } else if(flag == 2) { /* computation of jumped state*/ ',
    textmp($+1)='     set_block_error('+funam+"_bloc_states(block,flag));";
        ttext=[ttext;'int '+funam+"_bloc_states(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  end
   
  if nevout<>0 then 
    textmp($+1)='  } else if(flag == 3) { /* computation of output event times*/',
    textmp($+1)='     set_block_error('+funam+"_bloc_evtout(block,flag));";
        ttext=[ttext;'int '+funam+"_bloc_evtout(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  end
  textmp($+1)='  } else  if (flag == 5) { /* ending */'
      textmp($+1)='     set_block_error('+funam+"_bloc_ending(block,flag));";
        ttext=[ttext;'int '+funam+"_bloc_ending(scicos_block *block,int flag)";
	   "{";
	   "return 0;}"];
  textmp($+1)='  }'
  textmp($+1)='}'
  textmp=[textmp;' '; ttext];
else
  textmp=tt;
end

while 1==1
  [txt]=x_dialog(['Function definition in C';
		  'Here is a skeleton of the functions which you shoud edit'],..
		 textmp);
  
  if txt<>[] then
    tt=txt
    [ok]=scicos_block_link(funam,tt,'c')
    if ok then
      textmp=txt;
    end
    break;
  else
    ok=%f;break;
  end  
end


endfunction
