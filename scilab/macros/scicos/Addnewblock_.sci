function Addnewblock_()
    Cmenu=[]// here because Cmenu Quit can come out
    [scs_m,%fct]=do_addnew(scs_m)
    if %fct<>[] then 
      edited=%t  // a block has been added
      neecompile=4
      if %fct<>emptystr() then
	getf(%fct),  // requires loading function
      end
    end
endfunction
