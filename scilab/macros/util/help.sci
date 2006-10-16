function help(key)
  global %browsehelp
  
  nwniarg=find(sciargs()=="-nwni");
  texmacsarg=find(sciargs()=="--texmacs");
  noguiarg=find(sciargs()=="-nogui");
    
  if (%browsehelp<>[])&(nwniarg == [])&(texmacsarg == [])&(noguiarg == []) then
  
   //for compatibility with toolboxes making use of old cat files
   change_old_man()
   INDEX=make_help_index()

   if argn(2)==0 then
     browsehelp(INDEX,"index");
     return
   end
 
   key=stripblanks(key)
   if or(part(key,1)==['(',')','[',']','{','}','%','''','""',':','*','/', ...
		      '\','.','<','>','&','^','|','~','+','-']) then
     key="symbols";
   end
   path=gethelpfile(key)
   if path<>[] then
     browsehelp(path,key)
   else
     apropos(key)
   end
  else
   if (%browsehelp == []) then 
    warning('%browsehelp not correctly defined. help browser disabled. ');
    return;
   end
   
   errmsg='help browser disabled in this mode : ';
   if ( nwniarg <> []) then
    errmsg=errmsg+'-nwni';
    warning(errmsg);
    return;
   end
   if ( texmacsarg <> []) then
    errmsg=errmsg+ '--texmacs';
    warning(errmsg);
    return;
   end
   if ( noguiarg <> [] ) then
    errmsg=errmsg+ '-nogui';
    warning(errmsg);
    return;
   end
  
  end
endfunction









