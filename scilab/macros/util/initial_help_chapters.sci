function %helps=initial_help_chapters(language)
[lhs,rhs]=argn(0)
if rhs==0 then
  language="eng"
else
  if rhs<>1 then error(39), end
end
dirs=["programming";"graphics";"elementary";"fileio";"functions";"strings";
      "gui";"utilities";"linear";"polynomials";"control";"robust";
      "nonlinear";"signal";"arma";"metanet";"scicos";"sound";"translation";
      "pvm";"tdcs";"tksci";"statistics";"dcd";"identification"];
sep="/";if MSDOS then sep="\",end
%helps=sep+"man"+sep+language+sep+dirs; 
select language
case "eng"
  %helps=[%helps,..
	  ["Programming";"Graphics Library";"Elementary Functions";
	   "Input/Output Functions";"Handling of functions and libraries";
	   "Character string manipulations";"Dialogs";"Utilities";
	   "Linear Algebra";"Polynomial calculations";
	   "General System and Control";"Robust control toolbox";
	   "Optimization and simulation";
	   "Signal Processing toolbox";
	   "Arma modelisation and simulation toolbox";
	   "Metanet: graph and network toolbox";
	   "Scicos: Bloc diagram editor and simulator";"Sound file handling";
	   "Language or data translations";"PVM parallel toolbox";"TdCs";
	   "TCL/Tk interface";
	   "Statistics";
	   "Cumulative Distribution Functions; Inverses, grand";
           "Identification"]];
  case "fr"
  %helps=[%helps,..
	  ["Programmation";"Librairie graphique";"Fonctions élémentaires";
	   "Entrées-sorties";"Manipulation des fonctions et des librairies";
	   "Manipulations de chaînes de caractères";"Dialogues";"Utilitaires";
	   "Algèbre linéaire";"Calculs sur les polynômes";
	   "Contrôle et théorie des systèmes";"Contrôle robuste";
	   "Optimisation et simulation";
	   "Traitement du signal";"Modélisation et simulation ARMA";
	   "Metanet : graphes et réseaux";
	   "Scicos : éditeur et simulateur de blocs diagrammes";
	   "Manipulation de fichiers sons";
	   "Génération de code, traduction de données";
	   "Calcul parallèle avec PVM";"TdCs";
	   "Interface TCL/Tk";
	   "Statistiques";
	   "Fonctions de distributions statistiques";
           "Identification"]];
end
%helps=[SCI+%helps(:,1),%helps(:,2)];
