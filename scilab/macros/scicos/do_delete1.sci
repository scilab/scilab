function [scs_m,DEL,DELL]=do_delete1(scs_m,K,gr)
//perform deletion of scs_m object whose index are given in the vector 
//K and all other relevant objects (link, splits,..)
// if gr==%t objects are also graphicaly erased.
//
// deleted objects are replaced by the value : list('Deleted') not to 
// change the indexing use do_purge to suppress them and to renumber objects 
//!
// Copyright INRIA
DEL=[] //table of deleted objects
DELL=[] //table of redefined links
while K<>[] do
  k=K(1);K(1)=[]
  o=scs_m(k);
  if find(DEL==k)==[] then typ=o(1);else typ='Deleted',end
  //typ=o(1)
  DEL=[DEL k]
  if typ=='Link' then
    [ct,from,to]=o(7:9)
    //  free connected ports
    scs_m(from(1))=mark_prt(scs_m(from(1)),from(2),'out',ct(2),0)
    scs_m(to(1))=mark_prt(scs_m(to(1)),to(2),'in',ct(2),0)
    
    // erase and delete link
    if gr==%t then drawobj(o),end
    fromblock=scs_m(from(1));
    toblock=scs_m(to(1));
    
    if fromblock(5)=='SPLIT_f'|fromblock(5)=='CLKSPLIT_f' then
      //user kills a split  output link, 
      connected=get_connected(scs_m,from(1))//get links connected to the split block
      if size(connected,'*')==2 then
	//we create a unique link with  the split input and remaining output link
	//and suppress the split block
	if find(connected(2)==DEL)<>[] then // delete split
	  K=[from(1) K]
	else
	  if gr==%t then drawobj(scs_m(from(1))),end // clear  split block
	  DEL=[DEL  from(1)]       //suppress split block
	  o1=scs_m(connected(1));from1=o1(9);
	  o2=scs_m(connected(2));

	  //create a unique link
	  if from1(1)<>from(1) then [o1,o2]=(o2,o1),connected=connected([2 1]);end
	  from1=o1(9);to2=o2(9);ct2=o2(7)
	  //the links comes from connected(1) block and goes to connected(2) block
	  x1=o1(2);y1=o1(3)
	  if x1($-1)==x1($)&o2(2)(1)==o2(2)(2)|.. // both segs are vertical
	      y1($-1)==y1($)&o2(3)(1)==o2(3)(2) then // both segs are horizontal
	    o1(2)=[x1(1:$-1);o2(2)(2:$)];
	    o1(3)=[y1(1:$-1);o2(3)(2:$)];
	  else
	    o1(2)=[x1(1:$-1);o2(2)];
	    o1(3)=[y1(1:$-1);o2(3)];
	  end
	  o1(9)=o2(9);
	  DEL=[DEL connected(1)] // supress one link
	  DELL=[DELL  connected(1)]
	  scs_m(connected(2))=o1 //change link
	  scs_m(to2(1))=mark_prt(scs_m(to2(1)),to2(2),'in',ct2(2),connected(2))
	  scs_m(o1(8)(1))=mark_prt(scs_m(o1(8)(1)),o1(8)(2),'out',o1(7)(2),connected(2))
	end
      end
    end
    if toblock(5)=='SPLIT_f'|toblock(5)=='CLKSPLIT_f' then
      //user kills a split input link
      //ask for split deletion
      K=[K to(1)]
    end
  elseif typ=='Block' then
    // get connected links
    connected=get_connected(scs_m,k)
    //ask for connected links deletion
    K=[K connected]
    // erase and delete block
    if gr==%t then drawobj(scs_m(k)),end
  elseif typ=='Text' then
    if gr==%t then drawobj(o),end
  elseif typ=='Deleted' then
  else
    message('This object can''t be deleted')
  end
end


if gr==%t then 
  if pixmap then xset('wshow'),end,
end
for k=DEL,scs_m(k)=list('Deleted'),end


