//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

titl=[
	"---BE CAREFUL NOTHING IS PROTECTED---";
	"                                     ";
	" To draw your own complex  function :";
	"                                     ";
	"1/ define the  function by a correct ";
	"   string where  the complex var must";
	"   be z ; also  as  the function will";
	"   be evaluated  on  a matrix  don''t ";
	"   forget the . to operate in element";
	"   wize meaning ; examples :         ";
	"   z.^2     (z+1).*(z-1)   (1)./(z+2)";
	"   sqrt(z)  (z+%i).*(z-%i) 1+2*z+z.^2";
	"                                     ";
	"2/ define the type of the domain     ";
	"   string Square or Disk             ";
	"                                     ";
	"3/ define the ""radius"" R of the domain";
	"                                     ";
	"4/ may be your function has a kind of";
	"   discontinuity on Ox  or  Oy => put";
	"   the string Ox or Oy or No if not  "];

namevar= [
	"1/ string     ";
	"2/ Type Domain";
	"3/ R          ";
	"4/ Cut on Axe "];
	
value=["(1)./((z+2).*(2-z))";"Disk";"1.9";"No"];

save(TMPDIR+filesep()+"cmplxfunc_dial.bin",titl,namevar,value);
