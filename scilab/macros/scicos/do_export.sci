function do_export(scs_m,fname) 
  [lhs,rhs]=argn(0)
  
    
  rect=dig_bound(scs_m)
  if rect==[] then
    enablemenus()
    return
  end
  
  driv='Pos';num=1
  dr=driver()
  wpar=scs_m.props.wpar
  winc=xget('window');
  if rhs==1 then

    num=choose(['Postscript file';..
		  'Graphics window'],'How do you want to export?')
    if num==0 then return;end
    if num<>2 then  fname= savefile('*');end
  end
  // remove blanks
  if num<>2 then
    fname=stripblanks(fname)
    ff=str2code(fname);ff(find(ff==40|ff==53))=36;fname=code2str(ff)
    if fname==emptystr() then return;end   
  end
  //
  disablemenus()
  if num==2 then
    xset('window',max(winsid())+1)
    driv='Rec'
  end
  colmap=xget('colormap')
  driver(driv)
  options=scs_m.props.options

  
  wa=(rect(3)-rect(1))
  ha=(rect(4)-rect(2))
  
  if ~exists('%scicos_lr_margin') then %scicos_lr_margin=.1;end
  if ~exists('%scicos_ud_margin') then %scicos_ud_margin=.1;end
  
  rect(1)=rect(1)-wa*%scicos_lr_margin
  rect(3)=rect(3)+wa*%scicos_lr_margin
  rect(2)=rect(2)-ha*%scicos_ud_margin
  rect(4)=rect(4)+ha*%scicos_ud_margin
  
  wa=(rect(3)-rect(1))
  ha=(rect(4)-rect(2))
  
  if num<>2 then
    set_posfig_dim(wa*%zoom/1.8,ha*%zoom/1.8)
    //xset('wdim',wa,ha)
    xinit(fname),
    xsetech(wrect=[0 0 1 1],frect=rect,arect=[0,0,0,0])
    cmap=options.Cmap
    for k=1:size(cmap,1) 
      [mc,kk]=mini(abs(colmap-ones(size(colmap,1),1)*cmap(k,:))*[1;1;1])
      if mc>.0001 then
	colmap=[colmap;cmap(k,:)]
      end
    end

   xset('colormap',colmap)
  else
    //xset('wdim',wa,ha)
    window_set_size()
    if ~set_cmap(options('Cmap')) then // add colors if required
      options('3D')(1)=%f //disable 3D block shape
    end
  end
  
  if ~exists('%scicos_with_background') then %scicos_with_background=1;end
  if %scicos_with_background then set_background(),end
  
  
  


  pat=xget('pattern')
  xset('pattern',default_color(0));
  width=(rect(3)-rect(1))/3;
  height=(rect(4)-rect(2))*%scicos_ud_margin;
//  alu=xget('alufunction')
  xset('alufunction',3)

  xstringb(rect(1)+width,rect(4)-height,scs_m.props.title(1),width,height, ...
	   'fill')

  xset('pattern',pat)
  scs_m.props.title(1)='Scilab Graphics of '+scs_m.props.title(1)
  drawobjs(scs_m),
  if pixmap then xset('wshow'),end

  if num<>2 then 
    set_posfig_dim(0,0),
    xend();
  end
  
  
//  xset('alufunction',alu)
  driver(dr)
  xset('window',winc)
  
  if ~exists('%scicos_landscape') then %scicos_landscape=1;end
  opt=""
  if %scicos_landscape then opt=" -landscape ";end
  
  if num==1 then
    if MSDOS then
      fname=pathconvert(fname,%f,%t,'w')
      comm=pathconvert(SCI+'\bin\BEpsf',%f,%f,'w')
      rep=unix_g(''"'+comm+''" '+opt+fname)
    else
      rep=unix_g(SCI+'/bin/BEpsf '+opt+fname)
    end
    if rep<>[] then 
      message(['Problem generating ps file.';..
		 'perhaps directory not writable'] )
    end
  end
  enablemenus()
endfunction
