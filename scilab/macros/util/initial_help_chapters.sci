function %helps=initial_help_chapters()
%helps =['/man/programming'	,'Scilab Programming';
	'/man/graphics'		,'Graphic Library';
	'/man/elementary'	,'Elementary Functions';
	'/man/fileio'	        ,'Input/Output Functions';
	'/man/functions'	,'Handling of functions and libraries'
	'/man/strings'          ,'Character string manipulations';
	'/man/gui'              ,'Dialogs'
	'/man/utilities'        ,'Utilities'
	'/man/time-date'        ,'Time and date'
	'/man/linear'		,'Linear Algebra functions';
	'/man/polynomials'	,'Polynomial calculations';
	'/man/dcd'		,'Cumulative Distribution Functions; Inverses, grand';
	'/man/control'		,'General System and Control functions';
	'/man/robust'		,'Robust control toolbox';
	'/man/nonlinear'	,'Non-linear tools (optimization and simulation)';
	'/man/signal'		,'Signal Processing toolbox';
	'/man/arma'		,'Arma modelisation and simulation toolbox';
	'/man/metanet'		,'Metanet: graph and network toolbox';
	'/man/scicos'		,'Scicos: Bloc diagram editor and simulator';
	'/man/sound'		,'Sound file handling';
	'/man/translation'	,'Language or data translations';
	'/man/pvm'		,'PVM parallel toolbox';
	'/man/comm'		,'GECI communication toolbox';
	'/man/tksci'		,'TCL/Tk interface';
	'/man/identification'	,'Identification toolbox';
	'/man/tdcs'		,'TdCs']

%helps=[ SCI+%helps(:,1),%helps(:,2)];
endfunction
