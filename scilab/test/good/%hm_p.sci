function %hm_p(M)
// Copyright INRIA
// hypermatrix display
dims=double(M('dims'))
nd=size(dims,'*')
if nd<3 then
  if nd==1 then
    disp(M('entries'))
  else
    disp(matrix(M('entries'),dims(1),dims(2)))
  end
  return
end
I=(1:dims(3));
for k=4:nd
  I=[ones(1,dims(k)).*.I;
     (1:dims(k)).*.ones(1,size(I,2))];
end
k=1;sz=dims(1)*dims(2)
for II=I
  tit='(:,:,'+strcat(string(II'),',')+')'
  write(%io(2),tit)
  disp(matrix(M('entries')(k:k-1+sz),dims(1),dims(2)))
  k=k+sz
end
endfunction
