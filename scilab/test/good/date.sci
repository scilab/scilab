function s=date()
	w=getdate();
	month=['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
	s = msprintf("%02d-%s-%04d",w(6),month(w(2)),w(1));
endfunction
