function help_menu(i)
//function associated with the help button in Scilab main window
  browsehelp=browsehelp;// load browsehelp subfunctions
  change_old_man()
  INDEX=make_help_index()
  if i==1 then
    browsehelp(INDEX,"index");
  elseif  i==2 then
    key=x_dialog('Select a keyword','')
    if key==[] then return,end
    key=stripblanks(key(1))
    if key=='' then return,end
    apropos(key)
  elseif   i==3 then
    browsehelp_configure('set')
  end
endfunction
