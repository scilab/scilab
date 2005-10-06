function SaveAs_()
    disablemenus()
    Cmenu=[]
    [scs_m,editedx]=do_SaveAs()
    if ~super_block then edited=editedx;end
    enablemenus()
endfunction
