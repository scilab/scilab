function Addnewblock_()
// Copyright INRIA
    Cmenu=[]// here because Cmenu Quit can come out
    [scs_m,%fct]=do_addnew(scs_m)
    if %fct<>[] then 
      if %fct<>emptystr() then
	getf(%fct),  // requires loading function
      end
    end
endfunction
