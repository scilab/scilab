function show_startupinfo()
  return
  //used to inform users on first start after installation
  
  // If in No Window mode then returns...
  if ~isempty(grep(sciargs(),["-nw","-nwni"])) then
    return
  end
  
  settings_file=pathconvert(SCIHOME+'/.scilab_settings',%f,%t)
  w=fileinfo(settings_file);
  
  if (getlanguage() == 'fr' ) then
    DialogNewGraphic=["Attention:"
		      " "
		      "Les versions Scilab 4.x etaient les dernières à fonctionner"
		      "avec l''ancien mode graphique."
		      " "
		      "Veuillez lire le fichier RELEASE_NOTES pour plus de détails."]
    Buttons=['Rappel','Ok']
  else
    DialogNewGraphic=["Warning:"
 		      " "
		      "Scilab 4.x versions family were the last versions working"
		      "with the old graphics mode."
		      " "
		      "Please read the RELEASE_NOTES file for more details."]
    Buttons=['Remind Me','Ok']
  end
  if ~MSDOS  then
    show=%t
    if w <> [] then
      show=grep(mgetl(SCIHOME+'/.scilab_settings'),'displayDialogNewGraphic=no')==[]
    end
    if show

	mprintf('\n');
	for i=1:size(DialogNewGraphic,'*')
	  mprintf('%s\n',DialogNewGraphic(i));
	end
	mputl('displayDialogNewGraphic=no',SCIHOME+'/.scilab_settings')
// 	x_message disabled (x11 ref removed)
//      if x_message(DialogNewGraphic, Buttons) == 2 then
//	mputl('displayDialogNewGraphic=no',SCIHOME+'/.scilab_settings')
//      end
    end
  end
endfunction
