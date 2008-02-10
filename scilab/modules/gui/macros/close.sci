function close(h)
// Copyright INRIA 2008
// Vincent COUVERT

// Close a graphic figure or delete an uimenu or an uicontrol
// This function has been written for compatibility with old TK uicontrols/uimenus

// handle has been given ?
rhs=argn(2);

if rhs < 1 then
  // No handle given so destroy current figure
  delete(gcf());
else 
  if type(h) == 9 then // Graphics handle
    delete(h);
  else
    // Do not know what to do
    error(msprintf(gettext("Do not know how to delete an object of type: %d.\n"), type(h)));
  end
end
endfunction

