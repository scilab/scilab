function sciGUIhelp(key)
// Browse help
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.2 2004-06-24
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  update='0';
  
  sciGUI_init();
  nch=eval(TCL_GetVar('sciGUITable(browsehelp,nchap)'));
  if (size(%helps,1)~=nch) then	
    rescan_helps()
    TCL_SetVar('sciGUITable(browsehelp,nchap)',string(size(%helps,1)));
    update='1';
  end
  tmpDir=strsubst(TMPDIR,'\','/');
  if (argn(2)==0|key=='index') then
    TCL_EvalStr('sciGUIBrowseHelp -1 '+update+' ""'+tmpDir+'/browsehelp.txt"" """"');
  elseif (argn(2)==1) then
    key=stripblanks(key)
    if or(key==['(',')','[',']','{','}','%','''','""',':','*','/', ...
			'\','.','<','>','&','^','|','~','+','-']) then
      key="symbols";
    end
    nn=string(key);
    TCL_EvalStr('sciGUIBrowseHelp -1 '+update+' ""'+tmpDir+'/browsehelp.txt"" '""+nn+'""');
  else
    error(42);
  end
endfunction

function rescan_helps()
  nfil=size(%helps,1);
  fid=mopen(TMPDIR+'/browsehelp.txt','w');
  cwindow=progressionbar('Parsing help files ...');
  cdir=pwd();
  for j=1:nfil,
    procFile=pathconvert(%helps(j,1),%f);
    chdir(procFile);
    procFile=pwd();
    procFile=pathconvert(%helps(j,1),%f);
    mfprintf(fid,"%d\n%s\n%s/%s\n%s\n",0,%helps(j,2),procFile,"whatis.htm","");
    dta=mgetl('whatis.htm');
    progressionbar(cwindow);
    for w=1:size(dta,1)
      p1=strindex(dta(w),'<A HREF=""');
      if p1~=[] then
	p2=strindex(dta(w),'</A');
	fli=part(dta(w),(p1:(p2-1)));
	p3=strindex(fli,'""')+[1 -1];
	lnk=part(fli,p3(1):p3(2));
	nam=part(fli,(p3(2)+3):length(fli));
	extra=part(dta(w),(p2+4):length(dta(w)))
	extra=strsubst(extra,'</dd>','');
	extra=strsubst(extra,'</DD>','');
	mfprintf(fid,"%d\n%s\n%s/%s\n%s\n",1,nam,procFile,lnk,extra);
      end
    end
  end
  chdir(cdir);
  mclose(fid);
  winclose(cwindow);
endfunction
