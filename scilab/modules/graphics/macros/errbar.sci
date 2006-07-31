function []=errbar(x,y,em,ep)
// Rajoute des barres d'erreur sur un graphique 2D
// x et y decrivent les courbes (voir plot2d)
// em et ep sont deux matrices la barre d'erreur au point
// <x(i,j),y(i,j)> va de <x(i,j),y(i,j)-em(i,j)> a <x(i,j),y(i,j)+em(i,j)>
// x,y,em et ep sont donc des matrices (p,q), q courbes contenant chacunes
// p points.
// Exemple : taper errbar()
//      x=0:0.1:2*%pi;
//   y=<sin(x);cos(x)>';x=<x;x>';plot2d(x,y);
//   errbar(x,y,0.05*ones(x),0.03*ones(x));
//!
// Copyright INRIA

	[lhs,rhs]=argn(0)
	
	if rhs == 0 then   // demo
		
		title_demo = [
			'';
			'Demo of errbar()';
			'========================================';
			''];
		
		s_mat=[
			'x=0:0.1:2*%pi;';
			'y=[sin(x);cos(x)]'';x=[x;x]''';
			'plot2d(x,y);';
			'errbar(x,y,0.05*ones(x),0.03*ones(x));'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	end
   
xselect();
[n1,n2]=size(x);
y1=matrix(y-em,1,n1*n2);
x1=matrix(x,1,n1*n2);
y2=matrix(y+ep,1,n1*n2);
xset("clipgrf");
xsegs([x1;x1],[y1;y2]);
xclip();
endfunction
