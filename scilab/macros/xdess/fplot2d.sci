function []=fplot2d(xr,f,style,strf,leg,rect,nax,void)
// 2D plot of function f : a Scilab function or the name (as a string)
//  of a dynamically linked function.
// Enter fplot2d() for an example.
// deff('<y>=f(x)','y=sin(x)+cos(x)');
// fplot2d(f,0:0.1:%pi);
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=0,
  s_mat=['deff(''[y]=f(x)'',''y=sin(x)+cos(x)'');';
         'fplot2d(0:0.1:%pi,f);'];
  write(%io(2),s_mat);execstr(s_mat);
  return;
end;
if type(xr)=='10' then // logflag passed first
  error('First argument must be the discretization of x')
elseif rhs < 2 then 
  error('at least 2 input arguments required')
end
//handling optionnal arguments
opts=[]
if exists('style','local')==1 then opts=[opts,'style=style'],end
if exists('strf','local')==1 then opts=[opts,'strf=strf'],end
if exists('leg','local')==1 then opts=[opts,'leg=leg'],end
if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
if exists('logflag','local')==1 then opts=[opts,'logflag=logflag'],end
if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end
if size(opts,2)<rhs-2 then  error('invalid named arguments'),end
execstr('plot2d(xr,feval(xr,f),'+strcat(opts,',')+')')
endfunction
