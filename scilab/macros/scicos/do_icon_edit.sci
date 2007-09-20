function scs_m=do_icon_edit(%pt,scs_m)
// do_block - edit a block icon
// Copyright INRIA
  if Select==[] then
    win=%win;
    xc=%pt(1);yc=%pt(2);%pt=[];
    K=getblock(scs_m,[xc;yc])
    if K==[] then Cmenu=[];%pt=[];return,end
  else
    K=Select(:,1)';%pt=[]
  end
  if size(K,'*')>1|%win<>Select(1,2) then
    message("Only one block can be selected in current window for this operation.")
    Cmenu=[];%pt=[];return
  end    
  
  gr_i=scs_m.objs(K).graphics.gr_i

  if type(gr_i)<>15 then  gr_i=list(gr_i,[]);end

  gh_curwin=gcf();//Preserve current Window

  coli=gr_i(2);
  
  //Create the edition window  
  win=max(winsid())+1;
  fig=scf(win);xselect();
  fig.figure_size(1)=fig.figure_size(2)
  
  //set axes properties
  ax=gca();
  if coli<>[] then ax.background=coli;end//background
  ax.isoview='on';
  ax.margins=zeros(4,1)
  
  // draw the current icon (creates the graphical objects)
  sz=[1 1];orig=[0 0];
  deff('c=scs_color(c)',' ')
  if execstr(gr_i(1),"errcatch")<>0 then
    message(['The current icon depends on block parameter'
	     'part of the icon cannot be imported here'
	     'Use icon menu to check the content'])
  end
  //create and start the menus
  delmenu(win,'Edit');delmenu(win,'3D Rot.'); //remove unused default menus
  exec('SCI/macros/scicos/entity_menu.sce',-1);
  add_entity_menu() //add the edition menu
  ged(6,win)//start entity picker
  
  //Infinite loop waiting for edition to finish
  realtimeinit(0.2);count=0
  while or(win==winsid())&fig.user_data==[] 
    count=count+1
    realtime(count)
  end
  ok=or(win==winsid())&fig.user_data=='ok';
  //Edition finish, disable handler and menus
  delmenu(win,'Edit');
  seteventhandler('') //remove the ged event handler
  
  //Restore current window
  scf(gh_curwin)
  //Update Icon if requested
  if fig.user_data=='ok' then //user finished edition by "Ok"
    gr_i(1)=gen_code(win) //create the Scilab code of the graphics
    //check it
    mac=null();deff('[]=mac()',gr_i(1),'n') 
    if check_mac(mac) then
      //update scicos object
      o=scs_m.objs(K)
      o.graphics.gr_i=gr_i
      scs_m.objs(K)=o
      
      o_size = size(gh_curwin.children.children) ;
      gr_k=get_gri(K,o_size(1))
      drawlater() ;
      update_gr(gr_k,o)
      draw(gh_curwin.children);
      show_pixmap() ; 
      
    end
  end
  delete(fig)
endfunction
