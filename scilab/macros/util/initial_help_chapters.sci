//================================
//
// Copyright INRIA 2005
// Scilab team
// Date : December 8th 2005
// 
//================================

function %helps=initial_help_chapters(language)
	
	[lhs,rhs]=argn(0)
	
	if rhs==0 then
		language="eng"
	else
		if rhs<>1 then error(39), end
	end
	
	dirs=["programming";
				"elementary";
				"fileio";
				"functions";
				"strings";
				"gui";
				"utilities";
				"linear";
				"polynomials";
				"control";
				"robust";
				"nonlinear";
				"signal";
				"arma";
				"translation";
				"tdcs";
				"statistics";
				"dcd";
				"identification";
				"sparse";];
	
	


	
	
	sep="/";
	if MSDOS then sep="\",end
	
	%helps=sep+"man"+sep+language+sep+dirs; 
	
	select language
	
	case "eng"
		helpstmp=["Programming";
			"Elementary Functions";
			"Input/Output Functions";
			"Handling of functions and libraries";
			"Character string manipulations";
			"GUI and Dialogs";
			"Utilities";
			"Linear Algebra";
			"Polynomial calculations";
			"General System and Control";
			"Robust control toolbox";
			"Optimization and simulation";
			"Signal Processing toolbox";
			"Arma modelisation and simulation toolbox";
			"Language or data translations";
			"TdCs";
			"Statistic basics";
			"Cumulative Distribution Functions; Inverses, grand";
			"Identification";
			"Sparse solvers";];
			
	
			
			%helps=[%helps,helpstmp];
			clear helpstmp;
			
			
	
	case "fr"
			helpstmp=["Programmation";
			"Fonctions élémentaires";
			"Entrées-sorties";
			"Manipulation des fonctions et des librairies";
			"Manipulations de chaînes de caractères";
			"IHM et Dialogues";
			"Utilitaires";
			"Algèbre linéaire";
			"Calculs sur les polynômes";
			"Contrôle et théorie des systèmes";
			"Contrôle robuste";
			"Optimisation et simulation";
			"Traitement du signal";
			"Modélisation et simulation ARMA";
			"Génération de code, traduction de données";
			"TdCs";
			"Statistiques";
			"Fonctions de distributions statistiques";
			"Identification";
			"Solveurs creux";
			];
		
			
			%helps=[%helps,helpstmp];
			clear helpstmp;
			
	end
	
	%helps=[SCI+%helps(:,1),%helps(:,2)];
	
endfunction
