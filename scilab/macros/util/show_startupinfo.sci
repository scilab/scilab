function show_startupinfo()
  //used to inform users 
  settings_file=pathconvert('~/.scilab_settings',%f,%t)
  w=fileinfo(settings_file);
  global LANGUAGE
  if  LANGUAGE=='fr' then
    DialogNewGraphic=['Attention:'
		      'Cette version de Scilab utilise par defaut le nouveau "
		      "mode graphique."
		      "Ce nouveau mode est pour l''essentiel compatible avec "
		      "l''ancien.'
		      ' '
		      'Cependant les animations doivent être adaptées ...'
		      " "
		      "Vous pouvez revenir à l''ancien mode par l''instruction:'
		      "set old_style on"
		      " "
		      "Cliquez ""Oui"" si vous ne désirez plus voir cet avertissement"]
    Buttons=['Oui','Non']
  else
    DialogNewGraphic=['Warning:'
		      'This Scilab version uses, by  default, the new graphics mode. ';
		      'This new graphic mode is mostly upward compatible.'
		      ' '
		      'Note however that the animations should be adapted to "
		      "the new graphic."
		      " "
		      "You can go back to the old graphic entering the instruction:"
		      "set old_style on"
		      " "
		      "Click ""Yes"" if you do not want to see this warning any more']
    Buttons=['Yes','No']
  end
  if ~MSDOS  then
    show=%t
    if w <> [] then
      show=grep(mgetl('~/.scilab_settings'),'displayDialogNewGraphic=no')==[]
    end
    if show
      if x_message(DialogNewGraphic, Buttons) == 1 then
	mputl('displayDialogNewGraphic=no','~/.scilab_settings')
      end
    end
  end
endfunction
