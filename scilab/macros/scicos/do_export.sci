function do_export(scs_m,fname) 
  [lhs,rhs]=argn(0)
  driv='Pos';num=1
  dr=driver()
  wpar=scs_m.props.wpar
  winc=xget('window');
  if rhs==1 then

    num=x_choose(['Postscript file';..
		  'Graphics window'],'How do you want to export?')
    if num==0 then return;end
    if num<>2 then  fname= xgetfile('*');end
  end
  // remove blanks and apostrophe
  if num<>2 then
    ff=str2code(fname);ff(find(ff==40|ff==53))=[];fname=code2str(ff)
    if fname==emptystr() then return;end
  end
  //
  disablemenus()
  if num==2 then
    xset('window',max(winsid())+1)
    driv='Rec'
  end

  driver(driv)
  if num<>2 then xinit(fname);end

  options=scs_m.props.options

  set_background()


  xset('wdim',600,400);
  rect=dig_bound(scs_m)

  wa=(rect(3)-rect(1))
  ha=(rect(4)-rect(2))
  aa=wa/ha
  rr=600/400

  if aa<rr then 
    wa2=wa*rr/aa;rect(1)=rect(1)-(wa2-wa)/2;rect(3)=rect(1)+wa2
  else
    ha2=ha*aa/rr;rect(2)=rect(2)-(ha2-ha)/2;rect(4)=rect(2)+ha2
  end

  if num==1 then
    xsetech([0 0 1 1],rect)
    if options('Background')<>[] then 
      ll=6
      wp=rect(3)-rect(1);hp=rect(4)-rect(2);
      rr=[rect(1)-wp/ll;rect(4)+wp/ll;wp+2*wp/ll;hp+2*hp/ll];
      xrects(rr,options('Background')(1));
    end



  else
    dxx=(rect(3)-rect(1))/20;
    dyy=(rect(4)-rect(2))/20;
    rect(1)=rect(1)-dxx;rect(3)=rect(3)+dxx;
    rect(2)=rect(2)-dyy;rect(4)=rect(4)+dyy;
    xsetech([-1 -1 8 8]/6,rect)
  end

  pat=xget('pattern')
  xset('pattern',default_color(0));
  width=(rect(3)-rect(1))/3;
  height=(rect(4)-rect(2))/12;
  alu=xget('alufunction')
  xset('alufunction',3)
  xstringb(rect(1)+width,rect(4),scs_m.props.title(1),width,height,'fill')
  xset('pattern',pat)
  scs_m.props.title(1)='Scilab Graphics of '+scs_m.props.title(1)
  drawobjs(scs_m),
  if pixmap then xset('wshow'),end

  if num<>2 then xend();end
  xset('alufunction',alu)
  driver(dr)
  xset('window',winc)
  if num==1 then
    if MSDOS then
      rep=unix_g(''"'+SCI+'/bin/BEpsf'" -landscape '+fname)
    else
      rep=unix_g(SCI+'/bin/BEpsf -landscape '+fname)
    end
    if rep<>[] then x_message(['Problem generating ps file.';..
		    'perhaps directory not writable'] );end
    end
    enablemenus()
endfunction
