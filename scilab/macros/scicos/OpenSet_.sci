function OpenSet_()
xinfo('Click to open block or make a link')
%kk=[]
while %t 
  if %pt==[] then
    [btn,%xc,%yc,%win_1,Cmenu]=cosclick()

    if Cmenu<>[] then
      %pt=[];break
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[%xc;%yc];%win=%win_1
      if Cmenu==emptystr() then 
	Cmenu=[];%pt=[];
      end 
      break
    end
  else
    %xc=%pt(1);%yc=%pt(2);%win_1=%win;%pt=[]
  end
  
  disablemenus()
  if windows(find(%win_1==windows(:,2)),1)==100000 then
    //click in navigator
    [%Path,%kk,ok]=whereintree(%Tree,%xc,%yc)
    if ok&%kk<>[] then %Path($)=null();%Path($)=null();end
      if ~ok then %kk=[],end
  else
    %kk=getobj(scs_m,[%xc;%yc])
    %Path=%kk
  end
  if %kk<>[] then
    super_path=[super_path,%kk] 
    [o,modified,newparametersb,needcompileb,editedb]=clickin(scs_m(%Path))
    if Cmenu=='Link' then
      %pt=[%xc,%yc];
      enablemenus()
      break
    end
    // in case previous window has been destroyed
    if ~or(curwin==winsid()) then
      xset('window',curwin);
      xset('default')
      xbasc();
      if pixmap then xset('pixmap',1); end
      xset('pattern',1)
      xset('dashes',1)
      if ~set_cmap(options('Cmap')) then // add colors if required
	options('3D')(1)=%f //disable 3D block shape
      end
      if pixmap then xset('wwpc');end
      xbasc();xselect()
      %dr=driver();driver('Rec');
      set_background()
      
      window_set_size()
      
      xset('alufunction',6)
      drawobjs(scs_m)
      
      if pixmap then xset('wshow'),end
      
      menu_stuff()
      
      
      if ~super_block then
	delmenu(curwin,'stop')
	addmenu(curwin,'stop',list(1,'haltscicos'))
	unsetmenu(curwin,'stop')
      else
	unsetmenu(curwin,'Simulate')
      end
      //
    end
    //end of redrawing deleted parent  
    
    if needcompileb==4 then
      %kw=find(windows(:,1)==100000)
      if %kw<>[] then
	xdel(windows(%kw,2))
	%Tree=list()
      end
    end
    edited=edited|editedb
    super_path($-size(%kk,2)+1:$)=[]
    if editedb then
      scs_m_save=scs_m;nc_save=needcompile
      if ~pal_mode then
	needcompile=max(needcompile,needcompileb)
      end
      scs_m=update_redraw_obj(scs_m,%Path,o)
      
    end
    
    //note if block parameters have been modified
    if modified&~pal_mode  then
      //model=o(3)
      newparameters=mark_newpars(%kk,newparametersb,newparameters)
    end
  end
  enablemenus()
end
xinfo(' ')

