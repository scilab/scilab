// ============================================================
// Allan cornet 2005
// Modified by Pierre MARECHAL
// Scilab team
// Copyright INRIA
// 
// Test file association for scicos ( cos, cosf )
// ============================================================
global LANGUAGE;
// ============================================================
v=getversion('scilab');

if (v(4)<>0) then
 VERSTRING='Scilab '+string(v(1))+'.'+string(v(2))+'.'+string(v(3))+'.'+string(v(4));
else
 if (v(3)<>0) then
	VERSTRING='Scilab '+string(v(1))+'.'+string(v(2))+'.'+string(v(3));
 else
	VERSTRING='Scilab '+string(v(1))+'.'+string(v(2));
 end
end
// ============================================================

function [PathOut,OptionsOut]=GetPathAndOptionsInThisString(StringIn)
	IndexGuillemets=strindex(StringIn,'""');
	PathOut=part(StringIn,[IndexGuillemets(1)+1:IndexGuillemets(2)-1]); 
	PathOut=convstr(getlongpathname(PathOut),'u');
	OptionsOut=part(StringIn,[IndexGuillemets(2):IndexGuillemets(4)]); 
	clear IndexGuillemets;
endfunction;

PATHREF=convstr(getlongpathname(strsubst(SCI,'/','\'))+'\bin\wscilex.exe','u');
NBRTESTOK=0;

// ============================================================

if (LANGUAGE=='fr') then
	SCICOS1DIA=winqueryreg('HKEY_CLASSES_ROOT','COS_scilab_file\shell\Executer avec '+VERSTRING+'\command');
else
	SCICOS1DIA=winqueryreg('HKEY_CLASSES_ROOT','COS_scilab_file\shell\Run with '+VERSTRING+'\command');
end
[PATHSCICOS1DIA,OPTIONSSCICOS1DIA]=GetPathAndOptionsInThisString(SCICOS1DIA);
OPTIONSSCICOS1REF='""'+' -X ""%1""'; 
if ( (PATHSCICOS1DIA==PATHREF) & (OPTIONSSCICOS1DIA==OPTIONSSCICOS1REF) ) then
	disp('cos execute OK');
	NBRTESTOK=NBRTESTOK+1;
else
	disp('cos execute NOK');
	pause;
end;
clear SCICOS1DIA PATHSCICOS1DIA OPTIONSSCICOS1DIA OPTIONSSCICOS1REF;

// ============================================================

SCICOS2DIA=winqueryreg('HKEY_CLASSES_ROOT','COSF_scilab_file\shell\open\command');
[PATHSCICOS2DIA,OPTIONSSCICOS2DIA]=GetPathAndOptionsInThisString(SCICOS2DIA);
OPTIONSSCICOS2REF='""'+' -O ""%1""'; 
if ( (PATHSCICOS2DIA==PATHREF) & (OPTIONSSCICOS2DIA==OPTIONSSCICOS2REF) ) then
	disp('cosf open OK');
	NBRTESTOK=NBRTESTOK+1;
else
	disp('cosf open NOK');
	pause;
end;
clear SCICOS2DIA PATHSCICOS2DIA OPTIONSSCICOS2DIA OPTIONSSCICOS2REF;

// ============================================================

if (LANGUAGE=='fr') then
	SCICOS3DIA=winqueryreg('HKEY_CLASSES_ROOT','COSF_scilab_file\shell\Executer avec '+VERSTRING+'\command');
else
	SCICOS3DIA=winqueryreg('HKEY_CLASSES_ROOT','COSF_scilab_file\shell\Run with '+VERSTRING+'\command');
end  
[PATHSCICOS3DIA,OPTIONSSCICOS3DIA]=GetPathAndOptionsInThisString(SCICOS3DIA);
OPTIONSSCICOS3REF='""'+' -X ""%1""'; 
if ( (PATHSCICOS3DIA==PATHREF) & (OPTIONSSCICOS3DIA==OPTIONSSCICOS3REF) ) then
	disp('cosf execute OK');
	NBRTESTOK=NBRTESTOK+1;
else
	disp('cosf execute NOK');
	pause;
end;
clear SCICOS3DIA PATHSCICOS3DIA OPTIONSSCICOS3DIA OPTIONSSCICOS3REF;
