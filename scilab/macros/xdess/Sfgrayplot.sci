function []=Sfgrayplot(x,y,f,strf,rect,aaint)
// Like fgrayplot but the function fec is used to smooth the 
// result 
// f is evaluated on the grid x.*.y 
// anf the result is plotted assuming that f is linear on the triangles 
// built on the grid 
// 
// ______ 
// | /| /|
// |/_|/_|  
// | /| /|
// |/_|/_|
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs<=0,s_mat=['deff(''[z]=Surf(x,y)'',''z=x**3+y'');';
    'Sfgrayplot(-1:0.1:1,-1:0.1:1,Surf);'];
write(%io(2),s_mat);execstr(s_mat);
return;end;

//if rhs <= 3,strf="121";end
//if rhs <= 4,rect=[-1,-1,1,1];end
//if rhs <= 5,aaint=[10,2,10,2];end


// parsing the optionnal args
opt_arg_list = ["strf", "rect","nax","zminmax", "colminmax", "mesh", "colout"]
opt_arg_seq = []
for opt_arg = opt_arg_list
  if exists(opt_arg,"local") then
    opt_arg_seq = opt_arg_seq +","+ opt_arg + "=" + opt_arg
  end
end
   

if type(f)==11 then comp(f),end;
p=prod(size(x));
q=prod(size(y));
noe_x = ones(y).*.x;
noe_y = y.*.ones(x);
fun = feval(x,y,f);
fun = matrix(fun,p*q,1);
xxb=(1:p-1)';xx=[];
for i=0:q-2; xx=[ xx; xxb+p*i*ones(xxb)];end
[Ntr,vv]=size(xx);
trianl=[(1:Ntr)',xx,xx+ones(xx),xx+(p+1)*ones(xx),0*ones(xx)];
trianl=[trianl;(Ntr+1:2*Ntr)',xx,xx+(p)*ones(xx),xx+(p+1)*ones(xx),0*ones(xx)];

// then plot
if opt_arg_seq == [] then
  if rhs <= 3,strf="121";end
  if rhs <= 4,rect=[-1,-1,1,1];end
  if rhs <= 5,aaint=[10,2,10,2];end
  fec(noe_x',noe_y',trianl,fun,strf," ",rect,aaint)
else
  execstr("fec(noe_x'',noe_y'',trianl,fun,"+opt_arg_seq+")");
end
// fec(noe_x',noe_y',trianl,fun,strf," ",rect,aaint) 
endfunction
