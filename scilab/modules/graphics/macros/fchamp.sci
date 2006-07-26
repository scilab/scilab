function []=fchamp(macr_f,fch_t,fch_xr,fch_yr,arfact,rect,strf)
//   Draw vector field in R^2,
//   Vector field defined by:
//   y=f(x,t,[u]), for compatibility with ode function
//    f : vector field. CAn be either :
//	 a function y=f(t,x,[u])
//       a list such as  list(f1,u1) with f1 a function
//       y=f1(t,x,u) and u1=current value of u
//    t : instants vector
//    xr,yr: two vectors defining the gridding
//    arfact : optional arg to control size of arrow heads,
//    strf : string ( same as  plot2d )
//Example : enter fchamp()
//!
// Copyright INRIA
	
	[lhs,rhs]=argn(0)
	if rhs <= 0 then   // demo
		
		title_demo = [
			'';
			'Demo of fchamp()';
			'========================================';
			''];
		
		s_mat=[
			'deff(''[xdot] = derpol(t,x)'',[''xd1 = x(2)'';';
			'''xd2 = -x(1) + (1 - x(1)**2)*x(2)'';';
			'''xdot = [ xd1 ; xd2 ]'']);';
			'fchamp(derpol,0,-1:0.1:1,-1:0.1:1,1);'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	end
  
  if rhs <= 2,fch_xr=-1:0.1:1;end
  if rhs <= 3,fch_yr=-1:0.1:1;end

  opts=[]
  if exists('arfact','local')==1 then opts=[opts,'arfact=arfact'],end
  if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
  if exists('strf','local')==1 then opts=[opts,'strf=strf'],end

  if type(macr_f) <> 15,
    if type(macr_f)==11 then comp(macr_f),end;
    deff('[yy]=mmm(x1,x2)',['xx=macr_f(fch_t,[x1;x2])';'yy=xx(1)+%i*xx(2);']);
  else
    mmm1=macr_f(1)
    if type(mmm1)==11 then comp(mmm1),end;
    deff('[yy]=mmm(x1,x2)',['xx=mmm1(fch_t,[x1;x2],macr_f(2));';
		    'yy=xx(1)+%i*xx(2);']);
  end
  fch_v=feval(fch_xr,fch_yr,mmm);


  execstr('champ(fch_xr,fch_yr,real(fch_v),imag(fch_v),'+strcat(opts,',')+')')
endfunction
