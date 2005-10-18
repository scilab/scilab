function Addnewblock_()
    Cmenu='Open/Set'
    [scs_m,%fct]=do_addnew(scs_m)
    if %fct<>[] then 
      edited=%t  // a block has been added
      if %fct<>emptystr() then
	getf(%fct),  // requires loading function
      end
    end
endfunction
