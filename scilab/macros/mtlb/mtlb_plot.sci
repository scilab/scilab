function h=mtlb_plot(X1,X2,X3,X4,X5,X6,X7,X8,X9)
global mtlb_log_mod
// Copyright INRIA
if mtlb_log_mod==[] then 
  log_mod='nn'
else
  log_mod=mtlb_log_mod
end
h=[]
[lhs,rhs]=argn(0)
global ('%MTLBHOLD')
if %MTLBHOLD==[] then %MTLBHOLD=%f,end
if ~%MTLBHOLD then
  strf='061'
  xbasc()
else
  strf='000'
end 
  
if rhs==1 then
  mtlb_plt1(X1,'k-')
elseif rhs==2 then
  if type(X2)==10 then
    mtlb_plt1(X1,X2)
  else   
    mtlb_plt2(X1,X2)
  end
elseif rhs==3&type(X3)==10 then
  mtlb_plt2(X1,X2,X3)
else
  k=1
  kc=[],cstyl=[]
  while k<=rhs-1 
    kc=[kc;[k k+1]]
    if k+2>rhs then
      cstyl=[cstyl,'k-']
    else
      execstr('tp=type(X'+string(k+2)+')')
      if tp==10 then
        execstr('st=X'+string(k+2))
        cstyl=[cstyl,st]
        k=k+3
      else
        cstyl=[cstyl,'k-']
        k=k+2
      end
    end
  end
  nc=size(cstyl,'*')
  xmx=-10^20
  xmn=10^20
  ymx=-10^20
  ymn=10^20
  for k=1:nc
    kx=kc(k,1)
    ky=kc(k,2)
    execstr('xmx=max(xmx,max(X'+string(kx)+'))')
    execstr('xmn=min(xmn,min(X'+string(kx)+'))')
    execstr('ymx=max(ymx,max(X'+string(ky)+'))')
    execstr('ymn=min(ymn,min(X'+string(ky)+'))')
  end
  if  ~%MTLBHOLD then
    xsetech([0,0,1.0,1.0],[xmn,ymn,xmx,ymx])
    strf='001'
  else
    strf='000'
  end

  for k=1:nc
    kx=kc(k,1)
    ky=kc(k,2)
    execstr('mtlb_plt2(X'+string(kx)+',X'+string(ky)+',cstyl(k))')
  end
end
mtlb_log_mod=[]

function mtlb_plt1(X1,mtlb_style)
[lhs,rhs]=argn(0)
p=xget('pattern')
mclrs=['y','m','c','r','g','b','w','k']
sclrs=[33 ,22 ,17 ,5  ,11 ,12 ,33 ,1  ]
mltyp=['.','o','x','+','-','*',':','-.','--']
sltyp=[0  ,9  ,2,   1,  -1  3   -1  -1   -1]
clr=sclrs(find(part(mtlb_style,1)==mclrs))
ltyp=find(mltyp==part(mtlb_style,size(clr,'*')+1:length(mtlb_style)))
if clr==[] then clr=1,end
if ltyp>0 then
  style=-ltyp+1
else
  style=clr
end
if norm(imag(X1),1)==0 then
  if min(size(X1))==1 then
    plot2d1('e'+log_mod,0,X1(:),style,strf)
  else
    plot2d1('e'+log_mod,0,X1,style*ones(1,size(X1,2)),strf)
  end
else
  if min(size(X1))==1 then
    plot2d1('g'+log_mod,real(X1(:)),imag(X1(:)),style,strf)
  else
    plot2d1('g'+log_mod,real(X1),imag(X1),style*ones(1,size(X1,2)),strf)
  end
end
xset('pattern',p)

function mtlb_plt2(X1,Y1,mtlb_style)
[lhs,rhs]=argn(0)
p=xget('pattern')
if rhs==3 then
  mclrs=['y','m','c','r','g','b','w','k']
  sclrs=[33 ,22 ,17 ,5  ,11 ,12 ,33 ,1  ]
  mltyp=['.','o','x','+','-','*',':','-.','--']
  sltyp=[0  ,9  ,2,   1,  -1  3   -1  -1   -1]
  for k=1:length(mtlb_style)
    clr=sclrs(find(part(mtlb_style,k)==mclrs))
    if clr<>[] then 
      mtlb_style=part(mtlb_style,1:k-1)+part(mtlb_style,k+1:length(mtlb_style))
      break,
    end
  end
  ltyp=sltyp(find(mtlb_style==mltyp))
  if clr==[] then clr=1,end
  if ltyp>0 then
    xset('pattern',clr)
    style=-ltyp+1
  else
    style=clr
  end
else
  style=[]
end
[mx1,nx1]=size(X1)
[my1,ny1]=size(Y1)
if min(size(X1))==1&min(size(Y1))==1 then
  X1=X1(:);Y1=Y1(:)
  n=min(size(X1,1),size(Y1,1))
  if style==[] then style=1,end
  plot2d1('g'+log_mod,X1(1:n),Y1(1:n),style,strf)
elseif min(size(X1))==1 then
  nx1=mx1*nx1
  if my1<>nx1&ny1==nx1 then Y1=Y1.', end
  if style==[] then style=1:size(Y1,2),else style=style*ones(1,size(Y1,2)),end
  plot2d1('o'+log_mod,X1(:),Y1,style,strf)  
elseif min(size(Y1))==1 then
  if style==[] then style=1,end
  plot2d1('o'+log_mod,X1(:),Y1,style*ones(1,size(Y1,2)),strf)  
else
  if style==[] then 
    plot2d1('g'+log_mod,X1,Y1,1:size(Y1,2),strf)
  else
    plot2d1('g'+log_mod,X1,Y1,style*ones(1,size(Y1,2)),strf)
  end
end
xset('pattern',p)
