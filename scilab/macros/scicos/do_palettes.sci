function [palettes,windows]=do_palettes(palettes,windows)
// Copyright INRIA

  kpal=x_choose(scicos_pal(:,1),'Choose a Palette')
  if kpal==0 then return,end

  lastwin=curwin
  winpal=find(windows(:,1)==-kpal) 
  if winpal<>[] then
    if ~or(windows(winpal,2)==winsid()) then
      windows(winpal,:)=[]
      winpal=[]
    end
  end

  if winpal==[] then  //selected palettes isnt loaded yet
    curwin=get_new_window(windows)
    if or(curwin==winsid()) then
      xdel(curwin);
      //xset('window',curwin)
    end
    windows=[windows;[-kpal curwin]]
    palettes=add_palette(palettes,scicos_pal(kpal,2),kpal)
    if palettes(kpal)==list() then return,end
  else //selected palettes is already loaded 
    curwin=windows(winpal,2)
  end
  //
  xset('window',curwin),
  if ~MSDOS then
    delmenu(curwin,'3D Rot.')
    delmenu(curwin,'UnZoom')
    delmenu(curwin,'Zoom')
    //  delmenu(curwin,'File')
  else
    delmenu(curwin,'3D &Rot.')
    delmenu(curwin,'&UnZoom')
    delmenu(curwin,'&Zoom')
    //  delmenu(curwin,'&File')
  end
  
  xselect();
  xset('alufunction',3)
  if pixmap then xset('pixmap',1),end,xbasc();
  rect=dig_bound(palettes(kpal));
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


  graph=TMPDIR+'/'+scicos_pal(kpal,1)+'.pal'

  //Check if the graph file exists
  [u,ierr]=file('open',graph,'old')
  if u<>[] then file('close',u);end

  if ierr<>0 then
    options=palettes(kpal).props.options
    set_background()
    if ~set_cmap(palettes(kpal).props.options('Cmap')) then 
	palettes(kpal).props.options('3D')(1)=%f //disable 3D block shape 
    end
    drawobjs(palettes(kpal))
    if pixmap then xset('wshow'),end

    xsave(graph)
  else
    xload(graph)
    xname(palettes(kpal).props.title(1))
  end
  xinfo('Palette: may be used to copy  blocks or regions')  
  if pixmap then xset('wshow'),end
  xset('window',lastwin)
endfunction
