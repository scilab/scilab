//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ENPC
//
// This file is distributed under the same license as the Scilab package.
//

function [y]=Exp(m,n,lambda)
	// lambda exp(-lambda x) x>=0
	// ---------------------------
	y=(-1/lambda)* log(rand(m,n,'uniform'));
endfunction


function []=ExpT(n)
	// lambda exp(-lambda x) x>=0 
	// ---------------------------
	[lhs,rhs]=argn(0)
	if rhs <= 0 ; n=1000;end 
	lambda=3;
	y=Exp(1,n,lambda);
	drawlater();
	my_handle = scf(100001);
	clf(my_handle,"reset");
	histplot([0:0.1:10],y,[1,1],'051',' ',[0,0,5,3]);
	deff('[y]=f(x)','y=lambda*exp(-lambda*x);');
	x=[0:0.1:10]';plot2d(x,f(x),1,"000");
	titre= 'macro histplot : Histogram plot';
	xtitle(titre,'Classes','N(C)/Nmax');
	PolylineFillColorId = color(179,179,0);
	current_axe = gca();
	current_axe.title.font_size = 3;
	current_axe.background = color(232,230,200);
	current_axe.children(2).children.fill_mode = 'on';
	current_axe.children(2).children.background = PolylineFillColorId;
	current_axe.grid = [-1 PolylineFillColorId];
	drawnow();
endfunction

ExpT();
clear ExpT;
clear Exp;
