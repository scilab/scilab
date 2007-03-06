function s=date()
	w=getdate();
	month=[gettext('messages','time_message_17'), ..
	gettext('messages','time_message_18'), ..
	gettext('messages','time_message_19'), ..
	gettext('messages','time_message_20'), ..
	gettext('messages','time_message_21'), ..
	gettext('messages','time_message_22'), ..
	gettext('messages','time_message_23'), ..
	gettext('messages','time_message_24'), ..
	gettext('messages','time_message_25'), ..
	gettext('messages','time_message_26'), ..
	gettext('messages','time_message_27'), ..
	gettext('messages','time_message_28')];
	s = msprintf("%02d-%s-%04d",w(6),month(w(2)),w(1));
endfunction
