function [perm,rec,tr,indsRec,indsT]=classmarkov(M)
//returns a permutation vector perm such that
//M(perm,perm) = [M11 0 0 0 0   0]
//               [0 M22 0 0     0]
//               [0 0 M33       0]
//               [      ...      ]
//               [0 0       Mrr 0]
//               [* *        *  Q]
//Each Mii is a Markov matrix of dimension rec(i)  i=1,..,r
//Q is sub-Markov matrix of dimension tr
//States 1 to sum(rec) are recurrent and states from r+1 to n
//are transient. 
//perm=[indsRec,indsT] where indsRec is a  vector of size sum(rec) 
//and indsT is a vector of size indsT.
  if type(M)==1 
    Mb=sparse(M<>0);
  else Mb=M<>0;
  end 
  g=mat_2_graph(bool2s(Mb),1,'node-node');
  [nc,ncomp]=strong_connex(g);
  indsRec=[];indsT=[];rec=[];tr=0;
  for i=1:nc
    inds=find(ncomp==i);   
    nb=size(inds,'*');
    M1=M(inds,:); M1(:,inds)=[];
    if sum(M1)==0 then
      indsRec=[indsRec,inds];
      rec=[rec,nb];
    else
      indsT=[indsT,inds];
      tr=tr+nb;
    end
  end
  perm=[indsRec,indsT];
endfunction

