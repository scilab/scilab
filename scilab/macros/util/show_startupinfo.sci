function show_startupinfo()
  //used to inform users 
  
  // In in No Window mode then returns...
  if ~isempty(grep(sciargs(),["-nw","-nwni"])) then
    return
  end
  
  settings_file=pathconvert(SCIHOME+'/.scilab_settings',%f,%t)
  w=fileinfo(settings_file);
  global LANGUAGE
  if  LANGUAGE=='fr' then
    DialogNewGraphic=["Attention:"
		      "Cette version de Scilab utilise par défaut le nouveau "
		      "mode graphique."
		      " "
		      "Vous pouvez revenir à l''ancien mode par l''instruction :"
		      "    set old_style on"
		      " "
		      "Cependant les animations doivent être adaptées au "
		      "nouveau mode graphique."
		      " "
		      "Pour plus d''information, consulter l''aide en ligne de "
		      """graphics_entities""."
		      " "
		      " "
		      "Veuillez noter que l''ancien mode sera obsolète dans les futures "
		      "versions."
		      " "
		      "Cliquez ""Oui"" si vous ne désirez plus voir cet avertissement"]
    Buttons=['Oui','Non']
  else
    DialogNewGraphic=["Warning:"
		      "This Scilab version uses, by default, the new graphics mode.";
		      " "
		      "You can go back to the old graphic entering the instruction:"
		      "    set old_style on"
		      " "
		      "Note however that the animations should be adapted to "
		      "the new graphics."
		      " "
		      "For more information, see ""graphics_entities"" help page."
		      " "
		      " "
		      "Please notice that the old graphics mode will be obsolete "
		      "in future releases."
		      " "
		      "Click ""Yes"" if you do not want to see this warning any more"]
    Buttons=['Yes','No']
  end
  if ~MSDOS  then
    show=%t
    if w <> [] then
      show=grep(mgetl(SCIHOME+'/.scilab_settings'),'displayDialogNewGraphic=no')==[]
    end
    if show
      if x_message(DialogNewGraphic, Buttons) == 1 then
	mputl('displayDialogNewGraphic=no',SCIHOME+'/.scilab_settings')
      end
    end
  end
endfunction
