//================================
//
// Copyright INRIA 2005
// Scilab team
// Date : December 8th 2005
// 
//================================

function demolist=initial_demos_tables()
	demolist=[
		'Introduction to SCILAB','basic/intro/dem01.dem';
		'Graphics','graphics/graphics.dem';
		'Simulation','simulation/simulation.dem'
		'Control','control/control.dem'
		'Signal Processing','signal/signal.dem';
		'Optimization','optimization/optimization.dem';
		'Graph & Networks','metanet/meta.dem';
		'Scicos','scicos/scicos.dem';
		'Random','random/random.dem';
	];
	
	if with_tk() then
		demolist = cat(1,demolist,['TK/TCL demos','tk/tk.dem']);
	end
	
	if with_pvm() then
		demolist = cat(1,demolist,['Communications with PVM','pvm/pvm.dem']);
	end
		
	demolist(:,2)='SCI/demos/'+demolist(:,2);
	
endfunction
