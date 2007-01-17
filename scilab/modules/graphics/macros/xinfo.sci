function xinfo( newMessage )
// Copyright INRIA
// xinfo function
// Set the text of the message bar of the current figure
// Jean-Baptiste Silvy

curFig = gcf() ;
curFig.info_message = newMessage ;

endfunction