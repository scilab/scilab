function setlanguage(language)
// Allan CORNET INRIA 2004
// Set LANGUAGE  ======================================================
// used mainly for on-line help
global LANGUAGE
if (language=='fr' | language=='eng') & ~(LANGUAGE==language) then
	LANGUAGE=language;
//Scilab Help Chapters, ===============================================
//%helps is a two column matrix of strings
	global %helps
	%helps=initial_help_chapters(LANGUAGE);
	clear initial_help_chapters
	if with_tk()& ~with_gtk() then 
		RescanSciGUIHelp();
		winclose(winlist());
    		end
    	setlanguagemenu(LANGUAGE);
else
	if ~(LANGUAGE==language) then
	write(%io(2),"Unsupported language "+language+".");
	end
end
endfunction
//=====================================================================
function RescanSciGUIHelp()
  update='0';
  sciGUI_init();
  nch=eval(TCL_GetVar('sciGUITable(browsehelp,nchap)'));
  rescan_helps()
  TCL_SetVar('sciGUITable(browsehelp,nchap)',string(size(%helps,1)));
  update='1';
  tmpDir=strsubst(TMPDIR,'\','/');
  TCL_EvalStr('sciGUIBrowseHelp -1 '+update+' ""'+tmpDir+'/browsehelp.txt""');

endfunction
//=====================================================================
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
//=====================================================================
