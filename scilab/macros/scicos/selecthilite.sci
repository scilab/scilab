function selecthilite(Select, flag) 
// Copyright INRIA
if Select==[] then return,end
  
gh_winback = gcf() ; //** save the active window
  
//  gh_curwin = [] ; //** acquire the current Scicos window 
  
// Important assumption: all Selected are in the same window

[junk, win, o] = get_selection(Select(1,:))
gh_curwin = scf(win); //** select current window 
drawlater();  // for palettes, diagrams are already in this mode

  for i=1:size(Select,1)

    [junk, win, o] = get_selection(Select(i,:))

//    gh_curwin = scf(win); //** select current window

    o_size = size ( gh_curwin.children.children ) ;
    //** initial size
    //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
    //** I need an equivalent index for the graphics

    //** "k" is the object index in the data structure "scs_m"
    //** compute the equivalent "gh_k" for the graphics datastructure
    k = Select (i,1)
    //gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
    gh_k = get_gri(k,o_size(1)); //** Alan -

    if gh_k>0 then //** Alan - to disable some crash when we have delete obj
        //** get the handle of the graphics entity to modify
        gh_obj = gh_curwin.children.children(gh_k);
        //** update mark_mode property
        gh_obj.children(1).mark_mode = flag  ;
    end

  end  


draw(gh_curwin.children);
if gh_curwin.pixmap=='on' then 
   show_pixmap();
else
   drawnow();  // in case of palettes
end

scf(gh_winback); //** restore the 

endfunction
