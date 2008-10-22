function fcontour2d(xr,yr,f,nz,style,strf,leg,rect,nax,void)
	
	// deff('[z]=Surf(x,y)','z=x**2+y**2');
	// fcontour(Surf,-1:0.1:1,-1:0.1:1,10);
	
	// Copyright INRIA
	
	[lhs,rhs]=argn(0);
	
	if rhs == 0 then   // demo
		
		title_demo = [
			'';
			'Demo of fcontour2d()';
			'========================================';
			''];
		
		s_mat=[
			'deff(''[z]=Surf(x,y)'',''z=x**3+y'');';
			'fcontour2d(-1:0.1:1,-1:0.1:1,Surf,10,style=1:10,rect=[-1,-1,1,1]*1.5,strf='"011"');'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	end

if rhs<3,
  error(' I need at least 3 arguments or zero to have a demo');
end

opts=[]
if exists('nz','local')==0 then nz=10,rhs=rhs+1,end
if exists('style','local')==1 then opts=[opts,'style=style'],end
if exists('strf','local')==1 then opts=[opts,'strf=strf'],end
if exists('leg','local')==1 then opts=[opts,'leg=leg'],end
if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end

if size(opts,2)<rhs-4 then  error('invalid named arguments'),end

if type(f)==11 then comp(f),end;
execstr('contour2d(xr,yr,feval(xr,yr,f),nz,'+strcat(opts,',')+')')
endfunction
