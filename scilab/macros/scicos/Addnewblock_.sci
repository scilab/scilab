function Addnewblock_()
    Cmenu='Open/Set'
    [scs_m,%fct]=do_addnew(scs_m)
    if %fct<>[] then 
      getf(%fct),
      newblocks=[newblocks;%fct]
    end
endfunction
