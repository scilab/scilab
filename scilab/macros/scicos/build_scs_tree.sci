function [xx,yy,lp,larg]=build_scs_tree(scs_m,flag)
[lhs,rhs]=argn(0) 
if rhs<2 then
  flag='super'
end
xx=[];yy=[];
lp=list()
n=size(scs_m)
blks=[]
if flag=='super' then
  for k=2:n
    if scs_m(k)(1)=='Block' then 
      if scs_m(k)(3)(1)=='super' then blks=[blks,k],end
    end
  end
else
  for k=2:n
    if scs_m(k)(1)=='Block' then blks=[blks,k],end
  end
end
//
nl=size(blks,2)
xlk=x0
for k=blks
  path=[path k];
  xx=[xx,[x0;xlk]];yy=[yy,[y0;y0-1]]
//  xsegs([x0;xlk],[y0;y0-1],1)
  larg=maxi(xlk,larg)
  lp($+1)=path
  if scs_m(k)(3)(1)=='super' then
    y0s=y0;x0s=x0;x0=xlk;y0=y0-1
    largs=larg;
    [xxl,yyl,lpl,larg]=build_scs_tree(scs_m(k)(3)(8))
    xx=[xx,xxl];yy=[yy,yyl]
    lp=lstcat(lp,lpl)

    xlk=xlk+(larg-largs)+1/(nl*(1+abs(y0)))
    y0=y0s;x0=x0s
  end
  path($)=[]
end
