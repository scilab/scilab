function SetDiagramInfo_()
// Copyright INRIA
    
    Cmenu = [] ;
    
    [ok,info] = do_set_info(scs_m.props.doc);
    
    if ok then
      scs_m.props.doc = info
    end

endfunction
