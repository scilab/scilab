function [vtps_n,vnms_n]=updatevtps(nv_old,vtps_n,vtps_1,vnms_n,vnms_1)
//add new variables
// Copyright INRIA
nv1=min(nv_old,size(vnms_1,1),size(vnms_n,1))
k=find(vnms_1(1:nv1,1)<>vnms_n(1:nv1,1))
if k<>[] then vnms_n(k,1)=vnms_1(k,1),end

for k=nv_old+1:size(vtps_1)
  if find(vnms_1(k,1)==vnms_n(:,1))==[] then
    vnms_n=[vnms_n;vnms_1(k,:)]
    vtps_n($+1)=vtps_1(k)
  end
end
for k=1:min(size(vtps_n) , size(vtps_1))
  for l=1:3 
    if vtps_n(k)(l)<>vtps_1(k)(l) then
      vtps_n(k)(l)='?'
    end
  end
end
