function s=date()
	w=getdate();
	month=[gettext("Jan"), ..
	gettext("Feb"), ..
	gettext("Mar"), ..
	gettext("Apr"), ..
	gettext("May"), ..
	gettext("Jun"), ..
	gettext("Jul"), ..
	gettext("Aug"), ..
	gettext("Sep"), ..
	gettext("Oct"), ..
	gettext("Nov"), ..
	gettext("Dec")];
	s = msprintf("%02d-%s-%04d",w(6),month(w(2)),w(1));
endfunction
