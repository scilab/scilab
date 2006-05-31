// Non-regression test file for bug 871
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Date : 3 mai 2005

clear;

SCIFILE=TMPDIR+"/bug871.dia";

diary(SCIFILE);
rand;
diary(0);

SCIFILECONTENTS=mgetl(SCIFILE);

REFCONTENTS=[" ";
			"-->rand;";
			" ";
			"-->diary(0);"]

correct=%T

if or(REFCONTENTS<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,871);

clear
