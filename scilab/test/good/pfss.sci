function elts=pfss(S,rmax,cord)
//Syntax : elts=pfss(S)
//Partial fraction decomposition of the linear system S (in state-space form):
// elts is the list of linear systems which add up to S
// i.e. elts=list(S1,S2,S3,...,Sn) with S1 + S2 +... +Sn = S
// Each Si contains some poles of S according to the block-diagonalization
// of the A matrix of S.
// If S is given in transfer form, it is first converted into state-space
// and each subsystem is then converted in transfer form.
//!
// Copyright INRIA
flag=0;
[LHS,RHS]=argn(0);
FLAG=S(1)
if FLAG(1)=='r' then flag=1;S=tf2ss(S);end
if RHS==1 then rmax=[];cord=[];end
if RHS==2 then 
  if type(rmax)==10 then cord=rmax;end
  if type(rmax)==1 then cord=[];end
end
if type(S)<>16 then error(91,1),end
[t,f,g,h,dd,dom]=S([1:5,7]);
if t(1)<>'lss' then error(91,1),end;
[n,n]=size(f);
if rmax==[] then
[f,x,bs]=bdiag(f);
else
[f,x,bs]=bdiag(f,rmax);
end
h=h*x;g=x\g;
k=1;ll=0;
elts=list();
for l=bs',
          ind=k:k+l-1;
          f1l=f(ind,ind);
          gl=g(ind,:);
          hl=h(:,ind);
          elts(ll+1)=syslin('c',f1l,gl,hl)
          ll=ll+1;k=k+l;
end;
if RHS==2  then
select cord
case 'c'
  nb=size(bs,'*');
  class=[];
    for k=1:nb
     oneortwo=bs(k);ss=elts(k);A=ss(2);
     if oneortwo==1 then 
        class=[class,real(spec(A))];end
     if oneortwo>1 then 
        class=[class,mini(real(spec(A)))];end
    end;
  [cl,indi]=sort(-class);
  elts1=elts;
     for k=1:size(elts);
        elts(k)=elts1(indi(k));end
case 'd'
  nb=size(bs,'*');
  class=[];
    for k=1:nb
     oneortwo=bs(k);ss=elts(k);A=ss(2);
     if oneortwo==1 then 
        class=[class,abs(spec(A))];end
     if oneortwo>1 then 
        class=[class,maxi(abs(spec(A)))];end
    end;
  [cl,indi]=sort(-class);
  elts1=elts;
     for k=1:size(elts);
        elts(k)=elts1(indi(k));end
end
end
if type(dd)==1 then
if norm(dd,'fro')<>0 then elts(ll+1)=dd,end
end
if type(dd)==2 then 
if norm(coeff(dd),1) > %eps then elts(ll+1)=dd,end
end
if flag==1 then
   k=size(elts);
   for kk=1:k,elts(kk)=ss2tf(elts(kk));end
 end
endfunction
