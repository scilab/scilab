function contourf(x,y,z,nv,style,strf,leg,rect,nax)
	
	[nout,nin]=argn(0);
	
	newstyle = get('figure_style')=='new'
	
	if nin == 0 then   // demo
		
		title_demo = [
			'';
			'Demo of contourf()';
			'========================================';
			''];
		
		s_mat=['t=-%pi:0.1:%pi;m=sin(t)''*cos(t);contourf(t,t,m);'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	end

if nin <= 8 then nax=[1,10,1,10];end 
if nin <= 7 then rect=[0,0,1,1];end
if nin <= 6 then leg=" ";end 
if nin <= 5 then strf="121";end 
if nin <= 3 then zmin=mini(z);zmax=maxi(z);nv = zmin + (1:10)*(zmax-zmin)/(11);end 
if nin <= 2 then z=rand(size(x,'*'),size(y,'*'));end 
if nin <= 1 then y=1:10;end 
if nin <= 0 then x=1:10;end 
if x==[] then x=1:size(z,'r');end 
if y==[] then y=1:size(z,'c');end 

nvs=size(nv,'*') ;
if nvs==1 then nvs=nv;zmin=mini(z);zmax=maxi(z);nv = zmin + (1:nvs)*(zmax-zmin)/(nvs+1);end;
if nin <= 4 then style = -1*ones(1,nvs);end
if nin <= 7 then rect=[mini(x),mini(y),maxi(x),maxi(y)]; end 
nv1=nv
[mz,nz] = size(z);
minz = min(z);
maxz = max(z);
// Surround the matrix by a very low region to get closed contours, and
// replace any NaN with low numbers as well.
zz=[ %nan*ones(1,nz+2); %nan*ones(mz,1),z,%nan*ones(mz,1);%nan*ones(1,nz+2)];
kk=find(isnan(zz(:)));
zz(kk)=minz-1e4*(maxz-minz)+zeros(kk);

xx = [2*x(1)-x(2); x(:); 2*x(mz)-x(mz-1)];
yy = [2*y(1)-y(2); y(:); 2*y(nz)-y(nz-1)];

// Internal call to get the contours 
[x1,y1]=contour2di(xx,yy,zz,nv);
CS=[x1;y1];
// Find the indices of the curves in the c matrix, and get the
// area of closed curves in order to draw patches correctly. 
ii = 1;
ncurves = 0;
I = [];
Area=[];

while (ii < size(CS,2)),
  nl=CS(2,ii);
  ncurves = ncurves + 1;
  I(ncurves) = ii;
  xp=CS(1,ii+(1:nl));  // First patch
  yp=CS(2,ii+(1:nl));
  Area(ncurves)=sum( mtlb_diff(xp).*(yp(1:nl-1)+yp(2:nl))/2 );
  ii = ii + nl + 1;
end

lp=xget('lastpattern');

if size(nv,'*') > 1 // case where nv is a vector defining the level curve values
  if  size(nv,'*') > lp ; write(%io(2),'Colormap too small');return ;end 
else
  if nv > lp ; write(%io(2),'Colormap too small');return ;end 
end

min_nv=mini(nv);
max_nv=maxi(nv);

plot2d([mini(xx);maxi(xx)],[mini(yy);maxi(yy)],0,strf,leg,rect,nax);

// Plot patches in order of decreasing size. This makes sure that
// all the lev1es get drawn, not matter if we are going up a hill or
// down into a hole. When going down we shift levels though, you can
// tell whether we are going up or down by checking the sign of the
// area (since curves are oriented so that the high side is always
// the same side). Lowest curve is largest and encloses higher data
// always.

draw_min=1;
H=[];
[FA,IA]=sort(abs(Area));

if newstyle then
  drawlater(); // postpon the drawing here
  a=gca();
  old_foreground = a.foreground;
  pat=xget('pattern');
  for jj=IA',
    nl=CS(2,I(jj));
    lev1=CS(1,I(jj));
    if (lev1 ~= minz | draw_min),
      xp=CS(1,I(jj)+(1:nl));  
      yp=CS(2,I(jj)+(1:nl)); 
      pat=size(find( nv <= lev1),'*');
      xset("pattern",pat);
      xfpoly(xp,yp)
    end;
  end
  
  if style(1)<>-1 then 
    contour2d(xx,yy,zz,nv,style,"000",leg,rect,nax);
  end
  a.foreground = old_foreground;
  drawnow(); // draw all now!
else
  pat=xget('pattern');
  for jj=IA',
    nl=CS(2,I(jj));
    lev1=CS(1,I(jj));
    if (lev1 ~= minz | draw_min),
      xp=CS(1,I(jj)+(1:nl));  
      yp=CS(2,I(jj)+(1:nl)); 
      pat=size(find( nv <= lev1),'*');
      xset("pattern",pat);
      xfpoly(xp,yp)
    end;
  end
  
  xset('pattern',pat);
  if style(1)<>-1 then 
    contour2d(xx,yy,zz,nv,style,"000",leg,rect,nax);
  end
end

endfunction
