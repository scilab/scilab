function res=m2sci_gui()
// Copyright INRIA
// Scilab Project - V. Couvert
// Call m2sci.tcl to get file or path to convert using M2SCI and user defined options

if ~with_tk() then
  error('Tcl/Tk interface not defined')
end

// Default Options and Values
TK_SetVar("ePathToConvert",getcwd());
TK_SetVar("eFileToConvert","<enter a file name>");
TK_SetVar("eResDirName",getcwd());
TK_SetVar("rRecMode","NO");
TK_SetVar("rOnlyDouble","NO");
TK_SetVar("rVerbMode","3");
TK_SetVar("rPrettyPrint","NO");
TK_SetVar("rFlagWhatToConvert","0");
TK_SetVar("rToDo","0");
TK_SetVar("rWindowKilled","0");

TK_EvalFile(SCI+"/tcl/m2sci.tcl");

rWindowKilled=TK_GetVar("rWindowKilled");
rToDo=TK_GetVar("rToDo");
while rToDo=="0" & rWindowKilled=="0"
  rWindowKilled=TK_GetVar("rWindowKilled");
  rToDo=TK_GetVar("rToDo");
end

if rToDo=="1" then
  DirName=TK_GetVar("ePathToConvert");
  FileName=TK_GetVar("eFileToConvert");
  ResDirName=TK_GetVar("eResDirName");
  RecMode=TK_GetVar("rRecMode");
  OnlyDouble=TK_GetVar("rOnlyDouble");
  VerbMode=TK_GetVar("rVerbMode");
  PrettyPrint=TK_GetVar("rPrettyPrint");
  FlagWhatToConvert=TK_GetVar("rFlagWhatToConvert");
  
  // Modify Tcl/Tk values
  if RecMode=="YES" then
    RecMode=%T
  else  
    RecMode=%F
  end    
  if OnlyDouble=="YES" then
    OnlyDouble=%T
  else
    OnlyDouble=%F
  end    
  if PrettyPrint=="YES" then
    PrettyPrint=%T
  else
    PrettyPrint=%F
  end    
  VerbMode=evstr(VerbMode);
  
  if FlagWhatToConvert=="0" then // Convert a whole directory
    translatepaths(DirName,ResDirName);
    res=0
  else
    res=mfile2sci(FileName,ResDirName,RecMode,OnlyDouble,VerbMode,PrettyPrint);
  end
elseif rToDo=="-1" then
  // Cancel button
  res=0
else
  // Window killed
  res=0
end
endfunction
