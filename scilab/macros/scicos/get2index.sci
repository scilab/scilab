function [scs_m_i, cpr_i, sflag] = get2index(object, k)
// Copyright INRIA
// 
//**               scs_m_i : Object numeber (index) in the diagram (scs_m)
//**
//**               cpr_i   : Compiled structure index 
//**
//** the code is derived from 
//**
//** "Info" -> GetInfo -> do_block_info -> get_block_info ->  standard_document 
//
// 
//pause

  sflag=[]
  object_type = typeof(object)
  //
  if object_type=="Block" then
    
    scs_m_i = k ; 
    //disp('ici');pause
    if %cpr<>list()&needcompile<>4&k<>-1 then
	cor = %cpr.cor
	corinv = %cpr.corinv
	path = list()
	for kp=1:size(super_path,'*')
	     path(kp) = super_path(kp)
        end
	path($+1) = k
	ind = cor(path)

	//** if ind>0 & ind<=size(corinv) then
	cpr_i = cor(path) ; //** the good one
	//disp('ici');pause
	if type(cpr_i)==15 then
	  tmp_cpr=[];
	  sflag='*'
	  ssflag=''
	  for i=1:size(cpr_i)
	    if type(cpr_i(i))<>15 then
	      if cpr_i(i)<>0&cpr_i(i)<=size(corinv) then
	        tmp_cpr=[tmp_cpr,cpr_i(i)]
	      end
	    else //another sblock
	      ssflag=' *'
	    end
	  end
	  cpr_i='['+strcat(string(tmp_cpr),' ')+ssflag+']';
	end 
	//** else
	//**  cpr_i= -7; //** ['Compiled structure Index   : suppressed'; ' ']
	//**end
     else
	cpr_i= -9; //**   ['Compiled structure Index   : Not available';' ']
     end
   else
     scs_m_i = [] ;
     cpr_i   = [] ;
   end
     
endfunction
