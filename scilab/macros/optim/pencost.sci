function [fpen,gpen,ind]=pencost(x,ind,fncts,ne,nc,cpen);
// pencost=template of external function for penalized problem:
//
// min f(x) 
//  g_i(x)  = 0    i=1:ne
//  g_i(x) <= 0    i=ne+1:nc     (0<=ne<=nc)
//
//function pencost defines a penalized cost function and its gradient
//which is passed to scilab's optim function.
//
//A penalized problem is defined through the scilab function fncts
//which defines the constraints g_1,..,g_nc and the cost function f.
//
//fncts = scilab external with calling sequence:
//[gisandf,theirgrads,indic]=fncts(x,indic)
//which returns 
//     in row vector gisandf [g_1(x),...,g_nc(x),f(x)]
//
//     in column 1  of matrix theirgrads grad(g_1)_x
//.......... 
//     in column i  of matrix  theirgrads grad(g_i)_x 
//...........
//     in column nc of matrix theirgrads  grad(g_nc)_x
//     in column nc+1 of matrix theirgrads grad(g_nc)_x
//
// ne = number of equality constraints
// nc = total number of constraints
//                 
//EXAMPLE:
//
//minimize f(x)=0.5*(x(1)^2+x(2)^2)
//   g_1(x)= x(1)+x(2)-1 <=0
//   g_2(x)=- x(1) <= 0
//
//   no equality constraint (ne=0)
//   two inequality constraints (-> nc=2 constraints)
//   
// 1-define your problem through a function (usually in a .sci file):
//deff('[gsf,grds,indic]=myproblem(x,indic)',...
//       'gsf=[x(1)+x(2)-1,-x(1),norm(x)^2];...
//        grds=[1,-1,x(1);...
//              1,0,x(2)]')
//
// 2-call scilab's optim function (after loading pencost.sci (this file));
// getf('pencost.sci','c') 
// ne=0;nc=2;x0=[4;7];
// penalizing constant cpen=100;
// [f,x]=optim(list(pencost,myproblem,ne,nc,cpen),x0)
//
// or (passing myproblem,ne,nc,cpen by context)
// fncts=myproblem;[f,x]=optim(pencost,x0)
//
// Copyright INRIA
[f,gradf,indic]=fncts(x,ind);
if indic < 0 then ind=indic, return, end;
if nc >ne then
    for i=ne+1:nc, f(i)=max([0 f(i)]); end;
end;
fpen=f(nc+1) + 0.5*cpen*norm(f(1:nc))^2;
if ind==2 then return, end;
gpen=gradf(:,nc+1);
if ne >0 then
    for i=1:ne, gpen=gpen + cpen*f(i)*gradf(:,i),end;
end;
if nc > ne
  for i=ne+1:nc,
    if f(i)>0 then gpen=gpen + cpen*f(i)*gradf(:,i),end;
  end;
end;
endfunction
