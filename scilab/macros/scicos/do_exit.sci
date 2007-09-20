function do_exit()
// Copyright INRIA

//** This function is executed at the end of the SCICOS main loop
//
//** N.B. In case of "super_block" the 'scicos()' is resursively reactivate 
//**      inside the 'set' section of 'SUPER_f.sci' interfacing function of
//**      Super Block 

//** xinfo(" "); 

r = 0 ; 

//**---------------------------------------------------------------
//** Exit from scicos top diagram edited: ask for save the diagram 
if edited then

  if ~super_block then
    r = x_choose(['Save';'Save As'],..
	         ['Diagram has not been saved';
	          'What to do before leaving?'],'Don''t save')
  end

end

//** if the answer is positive 
if r==1 then
  ok = do_save(); //** save the diagram 
  if ~ok then do_SaveAs();end; //** or ask to "Save As"
elseif r==2 then //** ask to Save As
  do_SaveAs()
end

//**---------------------------------------------------------------

 
//** If is not a superblock and is NOT a palette  
//** if ~super_block & ~pal_mode  then

//**----------------------------------------------------------------
if ~super_block then //** if I'm exit from the main Scicos diagram
  //** if the simulation is not finished call 'do_terminate()' housekeeping routine 
  if alreadyran then do_terminate(),end ; //** terminate SCICOS simulation 

end
//**----------------------------------------------------------------

winrem=[size(windows,1):-1:noldwin+1]

global %scicos_navig
global inactive_windows

if %scicos_navig<>[] then
  ii=winrem(find(windows(winrem,1)>0)) //find super block (not palette)
  if size(ii,'*')<>1 then disp('non e possibile'),pause,end
  winkeep=windows(ii(1),2)
  inactive_windows(1)($+1)=super_path
  inactive_windows(2)($+1)=winkeep  // (1) is for security
  if or(winkeep==winsid()) then  // in case the current window is open and
                            // remains open by becoming inactive
    	ww=gcf()
	scf(winkeep)
	ha=gcf()
        if enable_undo then
	    ha.user_data=list(scs_m,Select,enable_undo,scs_m_save,nc_save);
        else
	    ha.user_data=list(scs_m,Select,enable_undo,[],[]);  // no undo information
        end
	scf(ww)
  end	   
else
  ii=-1
end

for i=winrem
  if i<>ii then
    win=windows(i,2)
    if or(win==winsid()) then
      gh_del = scf(win) ; //** select the 'win'window and get the handle
      delete (gh_del)   ; //** delete the window   
    end
  end
end
endfunction
