function [palettes,windows]=do_load_as_palette(palettes,windows)
// Copyright INRIA
  [ok,scs_m,cpr,edited]=do_load()
  if ~ok then return,end


  maxpal=-mini([-200;windows(:,1)]) 
  kpal=maxpal+1

  lastwin=curwin

  curwin=get_new_window(windows)
  if or(curwin==winsid()) then
    xdel(curwin);
  end
  windows=[windows;[-kpal curwin]]
  palettes(kpal)=scs_m
  //
  xset('window',curwin),xselect();
  xset('alufunction',3)
  if ~MSDOS then
    delmenu(curwin,'3D Rot.')
    delmenu(curwin,'UnZoom')
    delmenu(curwin,'Zoom')
    delmenu(curwin,'Edit')
    delmenu(curwin,'Insert')
  else
        hidetoolbar(curwin)
     // French
  	delmenu(curwin,'&Editer')
  	delmenu(curwin,'&Outils')
  	delmenu(win,'&Inserer')
     // English
  	delmenu(curwin,'&Edit')
  	delmenu(curwin,'&Tools')
  	delmenu(curwin,'&Insert')
    
  end

  if pixmap then xset('pixmap',1),end,xbasc();
  rect=dig_bound(scs_m);

  if rect==[] then rect=[0 0 400,600],end
  %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
  //window size is limited to 400 x 300 ajust dimensions
  //to remain isometric.
  if %wsiz(1)<400 then 
    rect(1)=rect(1)-(400-%wsiz(1))/2
    rect(3)=rect(3)+(400-%wsiz(1))/2
    %wsiz(1)=400 
  end
  if %wsiz(2)<300 then 
    rect(2)=rect(2)-(300-%wsiz(2))/2
    rect(4)=rect(4)+(300-%wsiz(2))/2
    %wsiz(2)=300 
  end

  %zoom=1.2
  h=%zoom*%wsiz(2)
  w=%zoom*%wsiz(1)

  if ~MSDOS then h1=h+50,else h1=h,end
  xset('wresize',1)
  xset('wpdim',w,h1)
  xset('wdim',w,h)

  xsetech(wrect=[0 0 1 1],frect=rect,arect=[1 1 1 1]/32)
  if ~set_cmap(palettes(kpal).props.options('Cmap')) then 
    palettes(kpal).props.options('3D')(1)=%f //disable 3D block shape 
  end
  options=palettes(kpal).props.options
  set_background()
  drawobjs(palettes(kpal))
  if pixmap then xset('wshow'),end
  xinfo('Palette: may be used to copy  blocks or regions')
  xset('window',lastwin)
endfunction
