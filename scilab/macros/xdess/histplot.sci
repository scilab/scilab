function histplot(n,data,style,strf,leg,rect,nax,x1,x2,x3,x4,x5)
// histplot(n,data,[style,strf,leg,rect,nax])
// draws histogram of entries in  data put into n classes
// 
// histplot(xi,data,[style,strf,leg,rect,nax])
// generates the histogram of entries in data put into classes
// ]xi(k) xi(k+1)] .
// xi's are assumed st. increasing
//
// [style,strf,leg,rect,nax] same as plot2d
// Example : enter histplot()
//! 
// Copyright INRIA
// modif to use dsearch (Bruno le 10/12/2001)
[lhs,rhs]=argn(0)
if rhs<=0, s_mat=['histplot([-6:0.2:6],rand(1,2000,''n''),[1,-1],''011'','' '',[-6,0,6,0.5],[2,12,2,11]);';
	 'deff(''[y]=f(x)'',''y=exp(-x.*x/2)/sqrt(2*%pi);'');';
         'x=-6:0.1:6;x=x'';plot2d(x,f(x),1,'"000'");';
         'titre= ''macro histplot : Histogram plot'';';
         'xtitle(titre,''Classes'',''N(C)/Nmax'');'];
         write(%io(2),s_mat);execstr(s_mat);
         return;end;
if rhs<2 , write(%io(2),'histplot : Wrong number of arguments');
  return;
end;
if exists('normalization','local')==0 then  normalization=%t,end
opts=[]

if exists('style','local')==1 then 
  opts=[opts,'style=style'],
else
  opts=[opts,'style=1']
end
if exists('strf','local')==1 then opts=[opts,'strf=strf'],end
if exists('leg'  ,'local')==1 then opts=[opts,'leg=leg']    ,end
isrect=exists('rect' ,'local')==1
if exists('nax' ,'local')==1 then 
  opts=[opts,'nax=nax']  ,
else
  opts=[opts,'nax=[1,nx,2,10]']
end
if exists('logflag' ,'local')==1 then opts=[opts,'logflag=logflag']  ,end
if exists('frameflag' ,'local')==1 then opts=[opts,'frameflag=frameflag']  ,end
if exists('axesflag' ,'local')==1 then opts=[opts,'axesflag=axesflag']  ,end

if size(opts,2)+bool2s(isrect)+2 <rhs then  error('invalid named arguments'),end

  p=size(data,'*')
  data=data(:)
  //

  if size(n,'*')==1 then 
     x = linspace(min(data), max(data), n+1)';
  else
     x=n(:)
  end,
  n=prod(size(x));
  
  [ind , y] = dsearch(data, x);
  
  if normalization then y=y ./ (p*(x(2:$)-x(1:$-1))),end //normalization
  
  y=[y;y(n-1)];
  nx=maxi(min(15,prod(size(x))-1),1);
  if ~isrect then 
     rect=[mini(x),0,maxi(x),maxi(y)];
     if rect(2)==rect(4) then rect(2)=0.0;rect(4)=1.1; end
  end
  opts=[opts,'rect=rect']
  execstr('plot2d2(x,y,'+strcat(opts,',')+')')
  execstr('plot2d3(x,y,'+strcat([opts(1),'strf='"000"''],',')+')')
   

endfunction
