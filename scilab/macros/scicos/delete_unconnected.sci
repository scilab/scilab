function scs_m=delete_unconnected(scs_m);
// Copyright INRIA

 n = lstsize(scs_m.objs);

 if n==0 then return, end ; //** exit point
 DEL=[];
 DELL=[]
 finish=%f

 while ~finish
  finish = %t
  for k=1:n  //loop on scs_m objects
    x = getfield(1,scs_m.objs(k))
    if x(1)=='Block' then
      if scs_m.objs(k).gui<>'SUM_f'&scs_m.objs(k).gui<>'SOM_f' then

	if find(scs_m.objs(k).gui==['IFTHEL_f','ESELECT_f']) then
	  kk=[find(scs_m.objs(k).graphics.pein==0),find(scs_m.objs(k).graphics.pin==0)]
	  if kk<> [] 	// a synchro block is not active, remove it
	    [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
	    DEL=[DEL DEL1]
	    DELL=[DELL DELL1]
	    finish=%f
	  end

	else
	  kk=[find(scs_m.objs(k).graphics.pin==0)]
	  if kk<>[] then // at least one  input port is not connected delete the block

	    if or(getfield(1,scs_m.objs(k).graphics)=="in_implicit") then
	      if or(scs_m.objs(k).graphics.in_implicit(kk)<>"I") then 
		[scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
		DEL=[DEL DEL1]
		DELL=[DELL DELL1]
		finish=%f
	      end

	    else
	      [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
	      DEL=[DEL DEL1]
	      DELL=[DELL DELL1]
	      finish=%f
	    end

	  end
	end
      end
    end
  end
 end

 //suppress rigth-most deleted elements
 while getfield(1,scs_m.objs($))=='Deleted' then
   scs_m.objs($)=null();
   if lstsize(scs_m.objs)==0 then break,end
 end

 // Notify by hiliting and message edition
 if DEL<>[] then
   //** save the current figure handle
   gh_wins = gcf();
   if flgcdgen<>-1 then path=[numk path]; scs_m_s=all_scs_m; end
   if path<>[] then //** super block case
       //** get the maximun number of figure id
       mxwin = maxi(winsid());
       for k=1:size(path,'*')
         //** hilite entity path(k)
         hilite_obj(path(k))
         //** recurcively store contents of super block in
         //** scs_m_s structure
         scs_m_s = scs_m_s.objs(path(k)).model.rpar;
         //** open figure of super block(s)
         scs_show(scs_m_s, mxwin+k)
     end
   end

   //** hilite/unhilite
   //** entity in the current figure
   k=DEL
   //** find any indices of k which are not in DELL
   ind_k=find(k<>DELL)
   if ind_k<>[] then
     k=k(ind_k)
    //** hilite entities k
     hilite_obj(k)
    //** display an error message
    message(['Hilited block(s) or link(s) are ignored because of'
            'undefined input(s)'])
    //** unhilite entities k
    unhilite_obj(k)
   end

   for k=size(path,'*'):-1:1 //** close figure of super_block(s)
     //** select the mxwin+k window and get the handle
     gh_del = scf(mxwin+k);
     //** delete the window
     delete(gh_del)
   end

   //** restore the active window
   scf(gh_wins);

   //** unhilite super block entity in active window
   if path<>[] then
     unhilite_obj(path(1))
   end
 end

endfunction
