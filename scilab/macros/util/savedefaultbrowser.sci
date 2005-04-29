// Allan CORNET
// INRIA 2005
function savedefaultbrowser(browser)
	
  DefaultBrowser=browser;
	if MSDOS then
		FileBrowser=SCIHOME+"\Browser.ini";
    if ~or(DefaultBrowser==['Default Windows Browser','Scilab Browser','Old Scilab Browser']) then
      DefaultBrowser='Scilab Browser';
	  end
  else
  	FileBrowser=SCIHOME+"/Browser.ini";
  	if ~or(DefaultBrowser==['Scilab Browser','Old Scilab Browser','nautilus','mozilla/netscape (gnome-moz-remote)','opera','quanta (kde)']) then
  		if with_gtk() then
  		  DefaultBrowser="help widget";
      else
        DefaultBrowser="Scilab Browser";
	    end
    end
	
  end
  
  [fd,err]=mopen(FileBrowser,"w");
	mputl(DefaultBrowser,FileBrowser);
	mclose(fd);
	
endfunction