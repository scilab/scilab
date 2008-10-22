//---------------------------------------------------------------------------------
// Allan CORNET INRIA 2005
// Tests files Association by Inno Setup in registry
// Tests for Default Installation
//---------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------
PATHREF=convstr(getlongpathname(strsubst(SCI,'/','\'))+'\bin\wscilex.exe','u');
NBRTESTOK=0;
//---------------------------------------------------------------------------------
SCE_ENTRY = ".sce File (Scilab 5.x)";
SCE1DIA=winqueryreg('HKEY_CLASSES_ROOT',SCE_ENTRY+'\shell\open\command');
[PATHSCE1DIA,OPTIONSSCE1DIA]=GetPathAndOptionsInThisString(SCE1DIA);
OPTIONSSCE1REF='""'+' -O ""%1""'; 
if ( (PATHSCE1DIA==PATHREF) & (OPTIONSSCE1DIA==OPTIONSSCE1REF) ) then
  disp('SCE open OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('SCE open NOK');
  pause;
end;
clear SCE1DIA PATHSCE1DIA OPTIONSSCE1DIA OPTIONSSCE1REF;
//---------------------------------------------------------------------------------
if (getlanguage() == 'fr_FR') then
  SCE2DIA=winqueryreg('HKEY_CLASSES_ROOT',SCE_ENTRY+'\shell\Executer avec '+VERSTRING+'\command');
else
  SCE2DIA=winqueryreg('HKEY_CLASSES_ROOT',SCE_ENTRY+'\shell\Run with '+VERSTRING+'\command');
end  
[PATHSCE2DIA,OPTIONSSCE2DIA]=GetPathAndOptionsInThisString(SCE2DIA);
OPTIONSSCE2REF='""'+' -X ""%1""'; 
if ( (PATHSCE2DIA==PATHREF) & (OPTIONSSCE2DIA==OPTIONSSCE2REF) ) then
  disp('SCE execute OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('SCE execute NOK');
  pause;
end;
clear SCE2DIA PATHSCE2DIA OPTIONSSCE2DIA OPTIONSSCE2REF;
//---------------------------------------------------------------------------------
SCE3DIA=winqueryreg('HKEY_CLASSES_ROOT',SCE_ENTRY+'\shell\print\command');
[PATHSCE3DIA,OPTIONSSCE3DIA]=GetPathAndOptionsInThisString(SCE3DIA);
OPTIONSSCE3REF='""'+' -P ""%1""'; 
if ( (PATHSCE3DIA==PATHREF) & (OPTIONSSCE3DIA==OPTIONSSCE3REF) ) then
  disp('SCE print OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('SCE print NOK');
  pause;
end;
clear SCE3DIA PATHSCE3DIA OPTIONSSCE3DIA OPTIONSSCE3REF;
//---------------------------------------------------------------------------------
SCI_ENTRY = ".sci File (Scilab 5.x)";
SCI1DIA=winqueryreg('HKEY_CLASSES_ROOT',SCI_ENTRY+'\shell\open\command');
[PATHSCI1DIA,OPTIONSSCI1DIA]=GetPathAndOptionsInThisString(SCI1DIA);
OPTIONSSCI1REF='""'+' -O ""%1""'; 
if ( (PATHSCI1DIA==PATHREF) & (OPTIONSSCI1DIA==OPTIONSSCI1REF) ) then
  disp('SCI open OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('SCI open NOK');
  pause;
end;
clear SCI1DIA PATHSCI1DIA OPTIONSSCI1DIA OPTIONSSCI1REF;
//---------------------------------------------------------------------------------
if (getlanguage() == 'fr_FR') then
  SCI2DIA=winqueryreg('HKEY_CLASSES_ROOT',SCI_ENTRY+'\shell\Executer avec '+VERSTRING+'\command');
else
  SCI2DIA=winqueryreg('HKEY_CLASSES_ROOT',SCI_ENTRY+'\shell\Run with '+VERSTRING+'\command');
end
[PATHSCI2DIA,OPTIONSSCI2DIA]=GetPathAndOptionsInThisString(SCI2DIA);
OPTIONSSCI2REF='""'+' -X ""%1""'; 
if ( (PATHSCI2DIA==PATHREF) & (OPTIONSSCI2DIA==OPTIONSSCI2REF) ) then
  disp('SCI execute OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('SCI execute NOK');
  pause;
end;
clear SCI2DIA PATHSCI2DIA OPTIONSSCI2DIA OPTIONSSCI2REF;
//---------------------------------------------------------------------------------
SCI3DIA=winqueryreg('HKEY_CLASSES_ROOT',SCI_ENTRY+'\shell\print\command');
[PATHSCI3DIA,OPTIONSSCI3DIA]=GetPathAndOptionsInThisString(SCI3DIA);
OPTIONSSCI3REF='""'+' -P ""%1""'; 
if ( (PATHSCI3DIA==PATHREF) & (OPTIONSSCI3DIA==OPTIONSSCI3REF) ) then
  disp('SCI print OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('SCI print NOK');
  pause;
end;
clear SCI3DIA PATHSCI3DIA OPTIONSSCI3DIA OPTIONSSCI3REF;
//---------------------------------------------------------------------------------
TST_ENTRY = ".tst File (Scilab 5.x)";
TST1DIA=winqueryreg('HKEY_CLASSES_ROOT',TST_ENTRY+'\shell\open\command');
[PATHTST1DIA,OPTIONSTST1DIA]=GetPathAndOptionsInThisString(TST1DIA);
OPTIONSTST1REF='""'+' -O ""%1""'; 
if ( (PATHTST1DIA==PATHREF) & (OPTIONSTST1DIA==OPTIONSTST1REF) ) then
  disp('TST open OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('TST open NOK');
  pause;
end;
clear TST1DIA PATHTST1DIA OPTIONSTST1DIA OPTIONSTST1REF;
//---------------------------------------------------------------------------------
if (getlanguage() == 'fr_FR') then
  TST2DIA=winqueryreg('HKEY_CLASSES_ROOT',TST_ENTRY+'\shell\Executer avec '+VERSTRING+'\command');
else
  TST2DIA=winqueryreg('HKEY_CLASSES_ROOT',TST_ENTRY+'\shell\Run with '+VERSTRING+'\command');
end  
[PATHTST2DIA,OPTIONSTST2DIA]=GetPathAndOptionsInThisString(TST2DIA);
OPTIONSTST2REF='""'+' -X ""%1""'; 
if ( (PATHTST2DIA==PATHREF) & (OPTIONSTST2DIA==OPTIONSTST2REF) ) then
  disp('TST execute OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('TST execute NOK');
  pause;
end;
clear TST2DIA PATHTST2DIA OPTIONSTST2DIA OPTIONSTST2REF;
//---------------------------------------------------------------------------------
TST3DIA=winqueryreg('HKEY_CLASSES_ROOT',TST_ENTRY+'\shell\print\command');
[PATHTST3DIA,OPTIONSTST3DIA]=GetPathAndOptionsInThisString(TST3DIA);
OPTIONSTST3REF='""'+' -P ""%1""'; 
if ( (PATHTST3DIA==PATHREF) & (OPTIONSTST3DIA==OPTIONSTST3REF) ) then
  disp('TST print OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('TST print NOK');
  pause;
end;
clear TST3DIA PATHTST3DIA OPTIONSTST3DIA OPTIONSTST3REF;
//---------------------------------------------------------------------------------
DEM_ENTRY = ".dem File (Scilab 5.x)";
DEM1DIA=winqueryreg('HKEY_CLASSES_ROOT',DEM_ENTRY+'\shell\open\command');
[PATHDEM1DIA,OPTIONSDEM1DIA]=GetPathAndOptionsInThisString(DEM1DIA);
OPTIONSDEM1REF='""'+' -O ""%1""'; 
if ( (PATHDEM1DIA==PATHREF) & (OPTIONSDEM1DIA==OPTIONSDEM1REF) ) then
  disp('DEM open OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('DEM open NOK');
  pause;
end;
clear DEM1DIA PATHDEM1DIA OPTIONSDEM1DIA OPTIONSDEM1REF;
//---------------------------------------------------------------------------------
if (getlanguage() == 'fr_FR') then
  DEM2DIA=winqueryreg('HKEY_CLASSES_ROOT',DEM_ENTRY+'\shell\Executer avec '+VERSTRING+'\command');
else
  DEM2DIA=winqueryreg('HKEY_CLASSES_ROOT',DEM_ENTRY+'\shell\Run with '+VERSTRING+'\command');
end  
[PATHDEM2DIA,OPTIONSDEM2DIA]=GetPathAndOptionsInThisString(DEM2DIA);
OPTIONSDEM2REF='""'+' -X ""%1""'; 
if ( (PATHDEM2DIA==PATHREF) & (OPTIONSDEM2DIA==OPTIONSDEM2REF) ) then
  disp('DEM execute OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('DEM execute NOK');
  pause;
end;
clear DEM2DIA PATHDEM2DIA OPTIONSDEM2DIA OPTIONSDEM2REF;
//---------------------------------------------------------------------------------
DEM3DIA=winqueryreg('HKEY_CLASSES_ROOT',DEM_ENTRY+'\shell\print\command');
[PATHDEM3DIA,OPTIONSDEM3DIA]=GetPathAndOptionsInThisString(DEM3DIA);
OPTIONSDEM3REF='""'+' -P ""%1""'; 
if ( (PATHDEM3DIA==PATHREF) & (OPTIONSDEM3DIA==OPTIONSDEM3REF) ) then
  disp('DEM print OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('DEM print NOK');
  pause;
end;
clear DEM3DIA PATHDEM3DIA OPTIONSDEM3DIA OPTIONSDEM3REF;
//---------------------------------------------------------------------------------  
GRAPH_ENTRY = ".graph File (Scilab 5.x)";
GRAPH1DIA=winqueryreg('HKEY_CLASSES_ROOT',GRAPH_ENTRY+'\shell\open\command');
[PATHGRAPH1DIA,OPTIONSGRAPH1DIA]=GetPathAndOptionsInThisString(GRAPH1DIA);
OPTIONSGRAPH1REF='""'+' -X ""%1""'; 
if ( (PATHGRAPH1DIA==PATHREF) & (OPTIONSGRAPH1DIA==OPTIONSGRAPH1REF) ) then
  disp('graph open OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('graph open NOK');
  pause;
end;
clear GRAPH1DIA PATHGRAPH1DIA OPTIONSGRAPH1DIA OPTIONSGRAPH1REF;
//---------------------------------------------------------------------------------  
GRAPHB_ENTRY = ".graphb File (Scilab 5.x)";
GRAPH2DIA=winqueryreg('HKEY_CLASSES_ROOT',GRAPHB_ENTRY+'\shell\open\command');
[PATHGRAPH2DIA,OPTIONSGRAPH2DIA]=GetPathAndOptionsInThisString(GRAPH2DIA);
OPTIONSGRAPH2REF='""'+' -X ""%1""'; 
if ( (PATHGRAPH2DIA==PATHREF) & (OPTIONSGRAPH2DIA==OPTIONSGRAPH2REF) ) then
  disp('graphb open OK');
  NBRTESTOK=NBRTESTOK+1;
else
  disp('graphb open NOK');
  pause;
end;
clear GRAPH2DIA PATHGRAPH2DIA OPTIONSGRAPH2DIA OPTIONSGRAPH2REF;
//---------------------------------------------------------------------------------
SAV_ENTRY = ".sav File (Scilab 5.x)";
execstr('SAVDIA=winqueryreg(''HKEY_CLASSES_ROOT'',SAV_ENTRY+''\shell\open\command'');','errcatch');
if isdef('SAVDIA') then
  [PATHSAVDIA,OPTIONSSAVDIA]=GetPathAndOptionsInThisString(SAVDIA);
  OPTIONSSAVREF='""'+' -X ""%1""'; 
  if ( (PATHSAVDIA==PATHREF) & (OPTIONSSAVDIA==OPTIONSSAVREF) ) then
    disp('sav execute OK');
    NBRTESTOK=NBRTESTOK+1;
  else
    disp('sav execute NOK');
    pause;
  end;
else
  disp('sav not associate (Default)');
end;
//---------------------------------------------------------------------------------  
BIN_ENTRY = ".bin File (Scilab 5.x)";
execstr('BINDIA=winqueryreg(''HKEY_CLASSES_ROOT'',BIN_ENTRY+''\shell\open\command'');','errcatch');
if isdef('BINDIA') then
  [PATHBINDIA,OPTIONSBINDIA]=GetPathAndOptionsInThisString(BINDIA);
  OPTIONSBINREF='""'+' -X ""%1""'; 
  if ( (PATHBINDIA==PATHREF) & (OPTIONSBINDIA==OPTIONSBINREF) ) then
    disp('bin execute OK');
    NBRTESTOK=NBRTESTOK+1;
  else
    disp('bin execute NOK');
    pause;
  end;
else
  disp('bin not associate (Default)');  
end;
//---------------------------------------------------------------------------------  
if ( (NBRTESTOK==14) | (NBRTESTOK==15) | (NBRTESTOK==16) ) then
  if (NBRTESTOK==14) then
      disp('Entries in Regitry OK (Default Installation)');
    else
      disp('Entries in Regitry OK (Not Default Installation)');
  end
else
    disp('Entries in Regitry NOK');
end
//---------------------------------------------------------------------------------  
clear GetPathAndOptionsInThisString PATHREF;
//---------------------------------------------------------------------------------
