// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function savedefaultbrowser(browser)
	
  DefaultBrowser=browser;
	if MSDOS then
		FileBrowser=SCIHOME+"\Browser.ini";
    if ~or(DefaultBrowser==['Default Windows Browser)','Scilab Browser','Old Scilab Browser']) then
      DefaultBrowser='Scilab Browser';
	  end
  else
  	FileBrowser=SCIHOME+"/Browser.ini";
  	if ~or(DefaultBrowser==['Scilab Browser','Old Scilab Browser','nautilus','mozilla/netscape (gnome-moz-remote)','opera','quanta (kde)']) then
    DefaultBrowser="Scilab Browser";
    end
	
  end
  
  [fd,err]=mopen(FileBrowser,"w");
	mputl(DefaultBrowser,FileBrowser);
	mclose(fd);
	
endfunction
