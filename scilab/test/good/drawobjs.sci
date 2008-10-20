// Copyright INRIA

function drawobjs(scs_m)

  drawlater() ;

    for i=1 : lstsize(scs_m.objs) //** draw object by object 
    
      gh_blk = drawobj(scs_m.objs(i)) ; 
  
    end
  
  drawnow()  ;
    
  show_pixmap() ;
  
  drawtitle(scs_m.props)
  
  show_info(scs_m.props.doc)

endfunction
