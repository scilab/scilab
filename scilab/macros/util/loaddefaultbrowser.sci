function DefaultBrowser=loaddefaultbrowser()
// Allan CORNET
// INRIA 2005
  if with_gtk() then
      DefaultBrowser="help widget";
      return
  end
  if MSDOS then
    FileBrowser=SCIHOME+"\Browser.ini";
  else
    FileBrowser=SCIHOME+"/Browser.ini";
  end
  if with_tk() then Default="Scilab Browser",else Default=[],end
  
  [fd,err]=mopen(FileBrowser,"r");
  if (err<0) then //ini file does not exist
    DefaultBrowser=Default
  else //ini file  exist
    DefaultBrowser=mgetl(fd,1);
    if MSDOS then
      KnownBrowsers=['Default Windows Browser';'Scilab Browser';
		     'Old Scilab Browser']
    else
      KnownBrowsers=['Scilab Browser';'Old Scilab Browser';'nautilus';
		     'mozilla/netscape (gnome-moz-remote)';'opera';
		     'quanta (kde)']
    end
    if ~or(DefaultBrowser==KnownBrowsers) then 
      //invalid DefaultBrowser in Browser.ini
      DefaultBrowser=Default
    else
      if DefaultBrowser=='Scilab Browser' & ~with_tk() then
	//DefaultBrowser in Browser.ini does not apply to current scilab
	DefaultBrowser=Default
      end
    end
    mclose(fd);
  end
endfunction
