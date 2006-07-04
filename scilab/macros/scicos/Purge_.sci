function Purge_()
    Cmenu=[]
    %pt=[]
    disablemenus()
    scs_m=do_purge(scs_m);
    enablemenus()
    needcompile=4;
    edited=%t
endfunction
