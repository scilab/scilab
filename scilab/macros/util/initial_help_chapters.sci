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
				"graphics";
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
				"mtlb";
				"sparse";];
	
	
	if %scicos then dirs=[dirs;"scicos"];,end;
	if %pvm then 	dirs=[dirs;"pvm"];,end;
	
	
	sep="/";
	if MSDOS then sep="\",end
	
	%helps=sep+"man"+sep+language+sep+dirs; 
	
	select language
	
	case "eng"
		helpstmp=["Programming";
			"Graphics Library";
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
			"Matlab to Scilab conversion tips";
			"Sparse solvers";];
			
	
			if %scicos then helpstmp=[helpstmp;"Scicos: Bloc diagram editor and simulator"];,end;
			if %pvm then helpstmp=[helpstmp;"PVM parallel toolbox"];,end;
			
			%helps=[%helps,helpstmp];
			clear helpstmp;
			
			
	
	case "fr"
			helpstmp=["Programmation";
			"Librairie graphique";
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
			"Aide à la conversion Matlab vers Scilab";
			"Solveurs creux";
			];
			
			if %scicos then helpstmp=[helpstmp;"Scicos : éditeur et simulateur de blocs diagrammes"];,end;
			if %pvm then helpstmp=[helpstmp;"Calcul parallèle avec PVM"];,end;
			
			%helps=[%helps,helpstmp];
			clear helpstmp;
			
	end
	
	%helps=[SCI+%helps(:,1),%helps(:,2)];
	
endfunction
