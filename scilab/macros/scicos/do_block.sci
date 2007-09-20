function [scs_m] = do_block(%pt,scs_m)
//
// Copyright INRIA
//
//** 18 Sept 2006
//**
//
// do_block - edit a block icon
//
//** 02/12/06 : use of objects permutation in gh_curwin.children.children()

  //** win = %win; //** just for memo: '%win' is the clicked window

  if Select==[] then //** No object is selected

    xc = %pt(1); yc = %pt(2); %pt=[]; //** acquire mouse coordinate
    K = getblock(scs_m,[xc;yc]) ; //** look from a clicked object
    if K==[] then return, end //** if no object --> EXIT

  else //** if the object is selected

    K = Select(:,1)'; %pt=[]
    //** Filter out the multiple object selected cases
    if size(K,'*')>1|%win<>Select(1,2) then
      message("Only one block can be selected in current window for this operation.") ;
      return ; //** EXIT
    end
  end


  gr_i = scs_m.objs(K).graphics.gr_i ; //** isolate the graphics command string 'gr_i'

  if type(gr_i)==15 then //** ? boh
    [gr_i,coli] = gr_i(1:2)
  else
    coli=[] ;
  end

  if gr_i==[] then gr_i=' ',end

  // Acquire the current clicked window and put to "on" the pixmap mode
  gh_curwin = scf(%win) ;
  while %t do
    //** use a dialog box to get input
    gr_i = dialog(['Give scilab instructions to draw block';
	 	 'shape.';
		 'orig(1) : block down left corner x coordinate';
		 'orig(2) : block down left corner y coordinate';
		 'sz(1)   : block width';
		 'sz(2)   : block height'],gr_i)

    if gr_i==[] then return; end ; //** no update : EXIT

    mac = null(); deff('[]=mac()', gr_i, 'n')

    if check_mac(mac) then
      o = scs_m.objs(K)
      o.graphics.gr_i = list(gr_i,coli) ; //** update the graphic command string
      scs_m.objs(K) = o ; //** update the data structure

      //** Alan/Simone 13/12/06 : Use of update_gr
      o_size = size(gh_curwin.children.children) ;
      //gr_k = o_size(1) - K + 1 ; //** semi empirical equation :)
      gr_k=get_gri(K,o_size(1))
      drawlater() ;
      update_gr(gr_k,o)
      draw(gh_curwin.children);
      show_pixmap() ; //** re-draw the graphic object and show on screen
      break; //** exit from the while loop
    end

  end //** of the while(1)

endfunction
