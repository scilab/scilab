function cors=shiftcors(cors,ns)
// Copyright INRIA
n=size(cors)
for k=1:n
  if type(cors(k))==15 then
    cors(k)=shiftcors(cors(k),ns)
  else
    if cors(k)<0 then cors(k)=cors(k);
    elseif cors(k)<>0 then cors(k)=cors(k)+ns,end
  end
end
endfunction
