function [links_table,sco_mat,ok]=global_case(links_table,sco_mat)
// Copyright INRIA
  [s,k]=gsort(sco_mat(:,2))
  sco_mat=sco_mat(k,:);
  index=find((sco_mat(:,2)=='1')&(sco_mat(:,4)=='3'))
  if index<> [] then
    for i=1:size(index,'*')
	 indexx=find((sco_mat(index(:),3)==sco_mat(index(i),3))&(sco_mat(index(:),5)==sco_mat(index(i),5)))
         if size(indexx,'*')>1 then
	    message(["Error In Compilation";"You cannot have multiple global GOTO with the same tag"])
	    ok=%f
	    return
	 end
    end
    for i=index
         index1=find((sco_mat(:,2)=='-1')&(sco_mat(:,3)==sco_mat(i,3))&(sco_mat(:,5)==sco_mat(i,5)))
         if index1<>[] then
            for j=index1
	         index2=find(links_table(:,1)==-evstr(sco_mat(j,1)))
	         if index2<>[] then
	             links_table(index2',1)=-evstr(sco_mat(i,1))
	         end
            end
          sco_mat(index1,:)=[]
          end
     end
  end
endfunction
