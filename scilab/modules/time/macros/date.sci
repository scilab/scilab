function s=date()
	w=getdate();
	month=[gettext('time_message_17'), ..
	gettext('time_message_18'), ..
	gettext('time_message_19'), ..
	gettext('time_message_20'), ..
	gettext('time_message_21'), ..
	gettext('time_message_22'), ..
	gettext('time_message_23'), ..
	gettext('time_message_24'), ..
	gettext('time_message_25'), ..
	gettext('time_message_26'), ..
	gettext('time_message_27'), ..
	gettext('time_message_28')];
	s = msprintf("%02d-%s-%04d",w(6),month(w(2)),w(1));
endfunction
