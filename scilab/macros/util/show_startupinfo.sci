function show_startupinfo()
  //used to inform users on first start after installation
  
  // If in No Window mode then returns...
  if ~isempty(grep(sciargs(),["-nw","-nwni"])) then
    return
  end
  
  settings_file=pathconvert(SCIHOME+'/.scilab_settings',%f,%t)
  w=fileinfo(settings_file);
  global LANGUAGE
  if  LANGUAGE=='fr' then
    DialogNewGraphic=["Attention:"
		      " "
		      "Les versions Scilab 4.x seront les dernières à fonctionner"
		      "avec l''ancien mode graphique (dit ''old'')."
		      "Voir http://www.scilab.org/product/man/html/eng/graphics/graphics_entities.htm"
		      " "
		      "Veuillez lire le fichier release_notes.txt pour plus de détails."]
    Buttons=['Rappel','Ok']
  else
    DialogNewGraphic=["Warning:"
 		      " "
		      "The Scilab 4.x versions are the last to work with the ''old'' graphics mode."
		      "See http://www.scilab.org/product/man/html/eng/graphics/graphics_entities.htm"
		      " "
		      "Please read release_notes.txt for more details."]
    Buttons=['Remind Me','Ok']
  end
  if ~MSDOS  then
    show=%t
    if w <> [] then
      show=grep(mgetl(SCIHOME+'/.scilab_settings'),'displayDialogNewGraphic=no')==[]
    end
    if show
      if x_message(DialogNewGraphic, Buttons) == 2 then
	mputl('displayDialogNewGraphic=no',SCIHOME+'/.scilab_settings')
      end
    end
  end
endfunction
