function RotateRight_()
// Copyright INRIA
//** Al@n

    scs_m_save = scs_m ;

    [scs_m] = do_turn(%pt,scs_m,-45)

    Cmenu=[];
    %pt = [];
endfunction
