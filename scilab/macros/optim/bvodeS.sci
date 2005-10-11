function z=bvodeS(x,m,n,a,b,fsub,gsub,zeta,...
    ystart,dfsub,dgsub,fixpnt,ndimf,ndimi,ltol,tol,ntol,nonlin,...
    collpnt,subint,iprint,ireg,ifail)
//
// PURPOSE
//   Simplified call of bvode. (Version 08.05.2005)
//   List of arguments: see help bvode in Scilab.
//   The arguments from ystart to ifail are optional. 
//   They may be called by name in the form argument=name 
//   in any order.
//
// AUTHOR
//   Rainer von Seggern

ms=sum(m);ipar=zeros(1,11);

if n>20, error('More than 20 equations are not allowed'); end
[lhs,rhs]=argn()
if rhs<8 | rhs>23, error('Wrong number of arguments'); end
if min(m)<1 | max(m)>4, error('1<=m(i)<=4'); end
if ~exists('ystart','local'), %ys=%ystart1; iguess=0; 
                              else %ys=%ystart;iguess=1;end
if ~exists('dfsub','local'), dfsubA=%dfsub; 
                              else dfsubA=%dfsub1; end
if ~exists('dgsub','local'), dgsubA=%dgsub; 
                              else dgsubA=%dgsub1; end
if ~exists('ltol','local'), ltol=1:ms; end
if ~exists('tol','local'), tol=1e-4*ones(1,ms); end
if ~exists('ntol','local'), ntol=length(tol); end
if ~exists('nonlin','local'), nonlin=1; end
if ~exists('collpnt','local'), collpnt=0; end
if ~exists('subint','local'), subint=0; end
if ~exists('iprint','local'), iprint=-1; end
if ~exists('ireg','local'), ireg = 0; end
if ~exists('ndimf','local'), 
   ndimf=1000*(4+ms+(5+n*collpnt)*(ms+n*collpnt)+4*ms^2); end
if ~exists('ndimi','local'), ndimi=1000*(3+ms+n*collpnt); end
if ~exists('fixpnt','local'), fixpnt=[]; 
                    else ipar(11)=length(fixpnt); end
if ~exists('ifail','local'), ifail=0; else ifail=1; end

ipar(1)=nonlin;
ipar(2)=collpnt;
ipar(3)=subint;
ipar(4)=ntol;
ipar(5)=ndimf;
ipar(6)=ndimi;
ipar(7)=iprint;
ipar(9)=iguess;
ipar(10)=ireg;

if ifail==1 then
disp(ltol,'ltol= ');
disp(tol,'tol= ');
disp(fixpnt,'fixpnt= ');
disp(ipar','ipar= ');
end

z=bvode(x,n,m,a,b,zeta,ipar,ltol,tol,fixpnt,...
        %fsub,dfsubA,%gsub,dgsubA,%ys);
endfunction

function y=%R(f_,x,z)
  if type(f_)==15 then
    R_=f_(1); y=R_(x,z,f_(2:$));
  elseif type(f_)==13 then
    y=f_(x,z)
  else
    error('argument has wrong type')
  end
endfunction

function y=%fsub(x,z)
  y=%R(fsub,x,z); sy=size(y);
  if sy(1)==1, y=y', end
endfunction
  
function RS=%fsubT(z,x)
RS=%fsub(x,z)
endfunction

function J=%dfsub(x,z)
   sz=size(z);if sz(1)==1, z=z'; end
    J=derivative(list(%fsubT,x),z)
endfunction

function J=%dfsub1(x,z)
  J=%R(dfsub,x,z)
endfunction

function g=%gsub(i,z)
  g=%R(gsub,i,z)
endfunction

function g=%gsubT(z,i)
  g=%gsub(i,z)
endfunction

function dg=%dgsub(i,z)
  sz=size(z);if sz(1)==1, z=z'; end
  dg=derivative(list(%gsubT,i),z)
endfunction

function dg=%dgsub1(i,z)
  dg=%R(dgsub,i,z)
endfunction

function [z,LhS]=%RR(f_,x)
  if type(f_)==15 then
    RR_=f_(1); [z,LhS]=RR_(x,f_(2:$));
  elseif type(f_)==13 then
    [z,LhS]=f_(x)
  else
    error('argument has wrong type')
  end
endfunction

function [z,LhS]=%ystart(x)
   [z,LhS]=%RR(ystart,x)
endfunction

function [z,LS]=%ystart1(x), 
  z=0; LS=0; 
endfunction
