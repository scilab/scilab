function histplot(n,data,style,strf,leg,rect,nax)
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
  p=prod(size(data))
  data=matrix(data,p,1);
//
  q=prod(size(n))
  if q==1 then 
     dmin=mini(data)
     dmax=maxi(data)
     x=(0:n)';
     x=(1/n)*( dmax*x + dmin*(n*ones(x)-x));
   else
     x=matrix(n,q,1)
   end,
   n=prod(size(x));
   x1=x;x1(1)=x1(1)-1
   deff('[y]=f_hist(k)',['y=prod(size(find(data>x1(k)&data<=x1(k+1))))';
       'y=y/(p*(x(k+1)-x(k)));']);
                    
   y=feval((1:n-1)',f_hist);
   y=[y;y(n-1)];
   nx=maxi(min(15,prod(size(x))-1),1);
   if rhs <= 5 then 
     rect=[mini(x),0,maxi(x),maxi(y)];
     if rect(2)==rect(4) then rect(2)=0.0;rect(4)=1.1; end
//     if q==1 then 
//  	rect(1)= rect(1)-abs(x(2)-x(1));rect(3)=rect(3)+abs(x(2)-x(1));nx=nx+2;
//     end
end
  select rhs
   case 7 then plot2d2("gnn",x,y,style,strf,leg,rect,nax);
               plot2d3("gnn",x,y,style,"000");
   case 6 then plot2d2("gnn",x,y,style,strf,leg,rect,[1,nx,2,10]);
               plot2d3("gnn",x,y,style,"000");
   case 5 then plot2d2("gnn",x,y,style,strf,leg,rect,[1,nx,2,10]);
               plot2d3("gnn",x,y,style,"000");
   case 4 then plot2d2("gnn",x,y,style,strf,' ',rect,[1,nx,2,10]);
               plot2d3("gnn",x,y,style,"000");
   case 3 then plot2d2("gnn",x,y,style,"051",' ',rect,[1,nx,2,10]);
               plot2d3("gnn",x,y,style,"000");
   case 2 then plot2d2("gnn",x,y,[1],"051",' ',rect,[1,nx,2,10]);
               plot2d3("gnn",x,y,[1,-1],"000");
  end



