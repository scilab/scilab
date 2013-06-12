//------------------------------------------------------------------------------------
// Allan CORNET
// DIGITE 2008 - 2010
// generates .iss for scilab 5.x (no jre)
//------------------------------------------------------------------------------------
global ISS_WITHOUT_JRE;
curpathsce = get_absolute_file_path("Create_ISS_nojre.sce");
ISS_WITHOUT_JRE = %t;
exec(curpathsce + "Create_ISS.sce");
//------------------------------------------------------------------------------------
