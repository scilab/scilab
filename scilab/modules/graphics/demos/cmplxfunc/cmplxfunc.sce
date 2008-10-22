//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - Bruno PINCON
// Copyright (C) 2005-2007 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file is distributed under the same license as the Scilab package.
//

// demo pour visualisation des fonctions elementaires complexes

// Adapted for new graphic by Pierre MARECHAL ( 16/12/2005 )

demolist = [
	"log","func1()";
	"exp","func2()";
	"tan","func3()";
	"atan","func4()";
	"sin","func5()";
	"asin","func6()";
	"cos","func7()";
	"acos","func8()";
	"sinh","func9()";
	"asinh","func10()";
	"cosh","func11()";
	"acosh","func12()";
	"tanh","func13()";
	"atanh","func14()";
	"custom","func15()"];

Title = [
	"visualisation of  complex";
	"elementary  functions  on";
	"a centered disk or square";
	"";
	"    Choose a function    "];

rep_init = ["(1)./((z+2).*(2-z))";"Disk";"1.9";"No"];
resp = rep_init;

while %t
	
	[num]=x_choose(demolist(:,1),Title);
	
	if num == 0 then	
		break
	else
	  if num == 15
	     if fileinfo(TMPDIR + filesep() + "cmplxfunc_dial.bin") == [] then
	       exec("SCI/modules/graphics/demos/cmplxfunc/cmplxfunc_dial.sce");
	     end
	     [resp]=demo_mdialog(TMPDIR + filesep() +"cmplxfunc_dial.bin");
		  if resp == [] then
		    resp = rep_init;
		  else
			 select resp(4)
				  case "Ox" ; e = 0.001; TypeCut = "Ox";
				  case "Oy" ; e = 0.001; TypeCut = "Oy";
				  else e=0  ; TypeCut = "Ox" ; resp(4) = "No" ;
			 end
			 deff("Z=f(z)","Z="+resp(1))
			 theta = -110; alpha = 75;
			 R = evstr(resp(3));
			 StrFunc="f";
			 select resp(2)
				  case "Square"
					 PlotCmplxFunc(R,e,"Square",TypeCut,41,"f",theta,alpha,[0;0]);
				  case "Disk"
					 PlotCmplxFunc(R,e,"Disk",TypeCut,[40 20],"f",theta,alpha,[0;0]);
			 end
		  end
		 else
		  execstr(demolist(num,2));
		 end
	end
end
