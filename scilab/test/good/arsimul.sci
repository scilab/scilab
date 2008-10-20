function z=arsimul(x1,x2,x3,x4,x5,x6,x7,x8)
// function z=arsimul(a,b,d,sig,u,up,yp,ep)
// Copyright INRIA
[lhs,rhs]=argn(0)
// switch to ar representation 
if type(x1)<>15&type(x1)<>16 then 
   if rhs < 5, write(%io(2),'arsimul: error expecting at least 5 arguments');
	return;end;
   ar=armac(x1,x2,x3,size(x1,'r'),size(x5,'r'),x4);
   if rhs==5,z=arsimul(ar,x5);return;end
   if rhs==6,z=arsimul(ar,x5,x6);return;end
   if rhs==7,z=arsimul(ar,x5,x6,x7);return;end;
   if rhs==8,z=arsimul(ar,x5,x6,x7,x8);return;end;
end
// Here the call is always arsimul(ar,....)
a=x1('a');b=x1('b');d=x1('d');sig=x1('sig');
u=x2;
[bl,bc]=size(b);[al,ac]=size(a);[dl,dc]=size(d);
[mmu,Nu]=size(u);
if mmu<>x1('nu') then 
  write(%io(2),'number of rows of u are incompatible with arma object');
  return;
end;
// X = [y_{n-1},y_{n-2},...y_{n-ka},u_{n-1},....,u_{n-kb},e_{n-1},....,e_{n-kd}
a1=a(:,al+1:$);al1=size(a1,'c');
b1=b(:,mmu+1:$);bl1=size(b1,'c');
d1=d(:,al+1:$);dl1=size(d1,'c');
A=[-a1,b1,d1];
// y_{n} = A*X + b(:,1:mmu)*u_{n}+d(:,1:al)*e_{n} 
// in the system fff x=[y_n;X];
if A==[] then 
  deff('[xkp1]=fff(k,x)',...
	['ukp1=u(:,k+1);dkp1=br(:,k+1);';
	 'xkp1= b(:,1:mmu)*ukp1+d(:,1:al)*dkp1']);
else 
  deff('[xkp1]=fff(k,x)',...
	['x=x(al+1:$);ukp1=u(:,k+1);dkp1=br(:,k+1);';
	 'ykp1= A*x + b(:,1:mmu)*ukp1+d(:,1:al)*dkp1';
	 'xkp1=[];'
	 'if al1>0; xkp1=[ykp1;x(1:al1-al)];end;';
	 'if bl1>0; xkp1=[xkp1;ukp1;x(al1+1:al1+bl1-mmu)];end;';
	 'if dl1>0; xkp1=[xkp1;dkp1;x(al1+bl1+1:al1+bl1+dl1-al)];end;';
	 'xkp1=[ykp1;xkp1];'	]);
end
// Noise simulation.
br=sig*rand(al,Nu,'normal');
//br=[-2,1,0.5]
// Initial condition 
// the first call to fff will be fff(0,x) 
// x must be set to 
// [ y_{0},...y{-ak},u_{0},...u_{-bk},d_{0},...d_{-dk}
// where ak= al1/al -1; bk= bl1/mmu -1 ; dk = dl1/al-1
// past conditions for up 
//--------------------------
if rhs <=2,
   up=0*ones(bl1,1);
else
   up=x3;
   if bl1==0 then 
      if up<>[] then write(%io(2),"up should be set to []");
		return ;
      end;
   else 
      up_s=size(up)
      if up_s(1)<>mmu|up_s(2)<>(bl1/mmu) then
      	write(%io(2)," up=[u(0),u(-1),..,] must be of dimension ("...
         +string(mmu)+','+string(bl1/mmu)+')');
        return 
      end
      up=matrix(up,bl1,1);
   end
end
// past conditions for yp
//--------------------------
if rhs <=3,
   yp=0*ones(al1,1)
else
   yp=x4;
   if al1==0 then 
      if yp<>[] then write(%io(2),"yp should be set to []");
		return ;
      end;
   else 
     yp_s=size(yp);
     if yp_s(1)<>al|yp_s(2)<>(al1/al) then 
      write(%io(2)," yp=[y(0),y(-1),..,] must be of dimension ("...
      +string(al)+','+string(al1/al)+')');
      return;
     end
     yp=matrix(yp,al1,1);
   end
end
// past conditions for ep
//--------------------------
if rhs <=4,
  ep=0*ones(dl1,1);
else
   ep=x5
   if dl1==0 then 
      if ep<>[] then write(%io(2),"ep should be set to []");
		return ;
      end;
   else 
      ep_s=size(ep);
      if ep_s(1)<>al|ep_s(2)<>(dl1/al) then
         write(%io(2)," ep=[e(0),e(-1),..,] must be of dimension ("...
           +string(al)+','+string(dl1/al)+')');
         return;
      end
      ep=matrix(ep,dl1,1);
   end;
end;
xi=[yp;up;ep];
// If A=[] it is a degenerate case which also work 
// but xi must be set to a scalar value to provide proper 
// result dimensions.
// 
xi=[0*ones(al,1);xi];
z=ode('discret',xi,0,1:Nu,fff);
// Now z contains y_{1},.....y_{Nu};
z=z(1:al,:)
endfunction
