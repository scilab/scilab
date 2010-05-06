// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ENPC
//
// This file is released into the public domain
function [y]=RndInt(m,n,imin,imax) 
	// discrete uniform random number
	//-------------------------------
	y=rand(m,n,'uniform')
	y=int(floor(y*(imax+1-imin)+ imin ));
endfunction

function [z]=RndIntT(n)
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=10000;end
	imin=-10;
	imax=10;
	y=RndInt(1,n,-10,10);
	i=imin-2:imax+2;
	z=[];
	for i1=i, z=[z,prod(size(find(y==i1)))],end
	drawlater();
	drawlater();
	my_handle = scf(100001);
	clf(my_handle,"reset");
	plot2d3("onn",i',z'/n,[1,2],,,[-12,0,12,0.1]);
	i1=(imin:imax)';
	plot2d1("onn",i1,ones(i1)/prod(size(imin:imax)),[-2,5]);
	
	xtitle('Simulation of a discrete uniform random variable');
	current_axe = gca();
	current_axe.title.font_size = 3;
	current_axe.background = color(232,230,200);
	legend(['Simulation';'Theory']);
	drawnow();
endfunction

RndIntT();
clear RndIntT;
clear RndInt;
