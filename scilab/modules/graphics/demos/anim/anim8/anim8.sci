//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function demo_riemann()
	
	//demo_help demo_riemann
	my_handle             = scf(100001);my_handle
	clf(my_handle,"reset");
	
	my_handle.pixmap     = "on";
	C                   = hotcolormap(200);
	C                   = C(1:$-40,:);
	my_handle.color_map = C;
	my_handle.axes_size = [597,634];
	[z,s]               = cplxroot(4,35); //compute
	cplxmap(z,s,163,69);  //draw
	
	color_management();
	
	show_pixmap();
	
	realtimeinit(0.03)
	for k=1:300
		realtime(k);
		if modulo(k,10)==0 then
			clear_pixmap();
			drawlater();
			cplxmap(z,s,163+k/10,69+k/20);  //draw
			// color_management();
			drawnow();
			show_pixmap();
		end
	end
	
endfunction

function color_management()
	
	my_handle            = gcf();
	my_axe               = gca();
	my_fac3d             = gce();
	
	my_handle.background = -1; // black
	my_axe.background    = -1; // black
	my_axe.foreground    = -1; // black
	my_fac3d.color_mode  = -1; // no lines
	
	
endfunction

function cplxmap(z,w,varargin)
	
	//cplxmap(z,w,T,A,leg,flags,ebox)
	//cplxmap Plot a function of a complex variable.
	//       cplxmap(z,f(z))
	
	x            = real(z);
	y            = imag(z);
	u            = real(w);
	v            = imag(w);
	
	my_handle    = gcf();
	my_color_map = my_handle.color_map;
	ncols        = size(my_color_map,:);
	
	[X,Y,U]      = nf3d(x,y,u);
	[X,Y,V]      = nf3d(x,y,v);
	Colors       = sum(V,'r');
	Colors       = Colors - min(Colors);
	Colors       = int((ncols-1)*Colors/max(Colors)+1);
	
	plot3d(X,Y,list(U,Colors),varargin(:));
	
endfunction

function [z,s]=cplxroot(n,m)
	
	//cplxroot(n,m,T,A,leg,flags,ebox)
	//CPLXROOT Riemann surface for the n-th root.
	//       CPLXROOT(n) renders the Riemann surface for the n-th root.
	//       CPLXROOT, by itself, renders the Riemann surface for the cube root.
	//       CPLXROOT(n,m) uses an m-by-m grid.  Default m = 20.
	// Use polar coordinates, (r,theta).
	// Cover the unit disc n times.
	// Copyright INRIA
	
	[lhs,rhs]=argn(0)
	
	if rhs  < 1, n = 3; end
	if rhs  < 2, m = 20; end
	
	r     = (0:m)'/m;
	theta = - %pi*(-n*m:n*m)/m;
	z     = r * exp(%i*theta);
	s     = r.^(1/n) * exp(%i*theta/n);
	
endfunction
