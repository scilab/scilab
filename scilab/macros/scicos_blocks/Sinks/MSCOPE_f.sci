function [x,y,typ]=MSCOPE_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(o)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  if size(exprs)<10 then exprs(10)='0',end // compatibility
  model=arg1.model;
  dstate=model.in
  while %t do
    [ok,in,clrs,win,wpos,wdim,ymin,ymax,per,N,heritance,exprs]=getvalue(..
	'Set Scope parameters',..
	['Input ports sizes';
	'Drawing colors (>0) or mark (<0)';
	'Output window number';
	'Output window position';
	'Output window sizes';
	'Ymin vector';
	'Ymax vector';
	'Refresh period';
	'Buffer size';
        'Accept herited events 0/1'],..
	 list('vec',-1,'vec',-1,'vec',1,'vec',-1,'vec',-1,..
	 'vec','size(%1,''*'')','vec','size(%1,''*'')','vec',1,..
         'vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification
    mess=[]
    if size(in,'*')<=0 then
      mess=[mess;'Block must have at least one input port';' ']
      ok=%f
    end
    if mini(in)<=0 then
      mess=[mess;'Port sizes must be positive';' ']
      ok=%f
    end
    if size(clrs,'*')<sum(in) then 
      mess=[mess;'Not enough colors defined (at least '+string(sum(in))+')';' ']
      ok=%f
    end
    if size(wpos,'*')<>0 &size(wpos,'*')<>2 then
      mess=[mess;'Window position must be [] or a 2 vector';' ']
      ok=%f
    end
    if size(wdim,'*')<>0 &size(wdim,'*')<>2 then
      mess=[mess;'Window dim must be [] or a 2 vector';' ']
      ok=%f
    end
    if win<0 then
      mess=[mess;'Window number can''t be negative';' ']
      ok=%f
    end
    if per<=0 then
      mess=[mess;'Refresh period must be positive';' ']
      ok=%f
    end
    if N<2 then
      mess=[mess;'Buffer size must be at least 2';' ']
      ok=%f
    end
    if or(ymin>=ymax) then
      mess=[mess;'Ymax must be greater than Ymin';' ']
      ok=%f
    end
    if ~or(heritance==[0 1]) then
      mess=[mess;'Accept herited events must be 0 or 1';' ']
      ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';
	         ' ';mess])
    end
    if ok then
      [model,graphics,ok]=check_io(model,graphics,in,[],ones(1-heritance,1),[])
    end
    if ok then
      if wpos==[] then wpos=[-1;-1];end
      if wdim==[] then wdim=[-1;-1];end
      if ok then
	yy=[ymin(:)';ymax(:)']
	rpar=[0;per;yy(:)]
	clrs=clrs(1:sum(in))
	ipar=[win;size(in,'*');N;wpos(:);wdim(:);in(:);clrs(:);heritance]
	if prod(size(dstate))<>(sum(in)+1)*N+1 then 
	  dstate=-eye((sum(in)+1)*N+1,1),
	end
        model.evtin=ones(1-heritance,1)
	model.dstate=dstate;model.rpar=rpar;model.ipar=ipar
        model.dep_ut=[%t %f] //compatibility
	graphics.exprs=exprs;
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  win=1;
  in=[1;1]
  wdim=[-1;-1]
  wpos=[-1;-1]
  clrs=[1;3;5;7;9;11;13;15];
  N=2;

  ymin=[-1;-5];ymax=[1;5];per=30;
  yy=[ymin(:)';ymax(:)']

  model=scicos_model()
  model.sim='mscope'
  model.in=in
  model.evtin=1
  model.dstate=-eye((sum(in)+1)*N+1,1)
  model.rpar=[0;per;yy(:)]
  model.ipar=[win;size(in,'*');N;wpos(:);wdim(:);in(:);clrs(1:sum(in));0]
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[strcat(string(in),' ');
         strcat(string(clrs),' ');
	 string(win);
	 sci2exp([]);
	 sci2exp([]);
	 strcat(string(ymin),' ');
	 strcat(string(ymax),' ');
	 string(per);
	 string(N);
         string(0)];
   gr_i='xstringb(orig(1),orig(2),''MScope'',sz(1),sz(2),''fill'')'
   x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
