function browsevar()
// Variable Browser
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.2 2004/06/24
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  
  sciGUI_init();

  [%_nams,%_vol]=who('get');
  p=predef();
  sz=stacksize();
  u=file('open',TMPDIR+'/browsevar.txt','unknown');
  for %_k=1:size(%_nams,1)
    %_sz=' ';
    execstr('%_typ=type('+%_nams(%_k)+')')
    if (%_typ<=10) then execstr('%_sz=size('+%_nams(%_k)+');'),end
    if ((%_typ==17)|(%_typ==16)) then
      execstr('%_tt=getfield(1,'+%_nams(%_k)+')');%_tt=%_tt(1);
      ierr=execstr('%_sz=%'+%_tt+'_size('+%_nams(%_k)+')','errcatch'); // ?
      if ierr<>0 then %_sz='?',end
    end
    
    fprintf(u,"<d>%d {%s} %d {%s}",%_typ,strcat(string(%_sz),' by '),%_vol(%_k),%_nams(%_k));
  end
  fprintf(u,"<m>%d %d %d",sz(1),sz(2),sz(2)/sz(1)*100);
  fprintf(u,"<p>%d",p);
  file('close',u);

  tmpDir=strsubst(TMPDIR,'\','/');
  TCL_EvalStr('sciGUIBrowseVar ""'+tmpDir+'""');
endfunction

