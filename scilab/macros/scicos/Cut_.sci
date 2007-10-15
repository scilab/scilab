function Cut_()
// Copyright INRIA
  if Select==[] then //** if no object has been selected 
      Cmenu = []; //** clear all the state variable 
      %pt   = [];
      %ppt  = [];
      return
  end
  
  gh_curwin = gh_current_window ; //** acquire the current window handler
  
  if Select(1,2)==curwin then
    scs_m_save = scs_m     ; //** save the diagram
    nc_save = needcompile  ; 
    del = setdiff(1:size(scs_m.objs), Select(:,1))  ; //** isolate all the object to be deleted 
    scs_m_sel = do_purge( do_delete1(scs_m,del,%f) ); //** extract the structure BUT do not update the graphics 
    
    if size(scs_m_sel.objs)==1 then    //** if the structure contains ONLU a single object   
       	Clipboard = scs_m_sel.objs(1)  //** isolate the object  
    else
    	Clipboard = scs_m_sel          //** save all the diagram 
    end
 
    [scs_m,DEL] = do_delete1(scs_m,Select(:,1)',%t) ; //** eliminate physically the block and 
 
    Select(find(Select(:,2)==curwin),:)=[];
    
    if DEL<>[] then 
      needcompile = 4,
      edited = %t
      enable_undo = %t
      //** suppress the last "Deleted" elements and the associated graphics compound 
      //** The last object in scs_m is associated to the first object in the graphics datastructure
      while getfield(1,scs_m.objs($))=='Deleted' then
	  scs_m.objs($) = null(); //** erase the 'Deleted' elements from scs_m.objs
          gh_object_to_delete = gh_curwin.children.children(1); //** the top element
          delete(gh_object_to_delete) ; //** delete the elements from the graphics datastructure 
                                        //** in order to mantain the coherency 

	  if lstsize(scs_m.objs)==0 then break,end ; //** EXIT in case of empty diagram 
      end
    
    end
    Cmenu=[];
  else
    message(['Only current diagram can be edited'])
    Cmenu=[]; %pt=[]; %ppt=[]
  end
  
endfunction


