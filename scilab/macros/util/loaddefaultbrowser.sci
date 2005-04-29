// Allan CORNET
// INRIA 2005
function DefaultBrowser=loaddefaultbrowser()
	if MSDOS then
		FileBrowser=SCIHOME+"\Browser.ini";
	else
		FileBrowser=SCIHOME+"/Browser.ini";
	end
	
  [fd,err]=mopen(FileBrowser,"r");
  if (err<0) then
  	if with_tk() & ~with_gtk() then DefaultBrowser="Scilab Browser",end
  	if with_gtk() then DefaultBrowser="help widget",end
  else
  	if with_gtk() then
  		DefaultBrowser="help widget";
    else
    	DefaultBrowser=mgetl(fd,1);
    	if MSDOS then
    	  if ~or(DefaultBrowser==['Default Windows Browser','Scilab Browser','Old Scilab Browser']) then
    	 	  DefaultBrowser='Scilab Browser';
	      end
    	else
    		if ~or(DefaultBrowser==['Scilab Browser','Old Scilab Browser','nautilus','mozilla/netscape (gnome-moz-remote)','opera','quanta (kde)']) then
          DefaultBrowser='Scilab Browser';
	      end
    	end
    end
    mclose(fd);
  end
  
endfunction
