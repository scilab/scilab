function [bool_val]=rescomp(sci_res,mtlb_res)
// Copyright INRIA
// Compare Matlab and Scilab results to validate Scilab reference file

// Define tolerance on error
//prec=1000000*%eps;
prec=1000*%eps
my_inf=1D300

res_type=type(sci_res);

if type(sci_res)<>10 then
  if size(sci_res)<>size(mtlb_res) then
    bool_val=%F
    return
  end
end

bool_val=%T

if mtlb_res==[]|sci_res==[] then
  bool_val=and(sci_res==mtlb_res);
elseif res_type==10 then
  //bool_val=and(ascii(sci_res)==ascii(mtlb_res));
  bool_val=sci_res==mtlb_res
elseif or(res_type==[4,6]) | type(mtlb_res)==4 then
  mtlb_res(find(bool2s(mtlb_res)<>0))=1;
  bool_val=and(bool2s(sci_res)==mtlb_res)
else
  // Looking for Nan values in mtlb_res
  ind=find(isnan(mtlb_res));
  ind1=find(isnan(real(sci_res)));// Real part because Scilab can return %Nan+%Nan*%i
  if ind<>ind1 then
      bool_val=%F;
  end
 // Looking for Inf values in mtlb_res
  ind=find(isinf(mtlb_res));
  if isreal(sci_res) then
    sci_res(find(sci_res>my_inf))=%inf
    sci_res(find(sci_res<-my_inf))=-%inf
  else
    sci_res(find(abs(sci_res)>my_inf))=%inf
  end
  ind1=find(isinf(sci_res));
  if ind<>ind1 then
    bool_val=%F;
  end
  //Indexes corresponding to other values than Nan and Inf
  ind=find((~isinf(mtlb_res))&(~isnan(mtlb_res)));
  diff_mat=abs(sci_res-mtlb_res);
  if (max(abs(mtlb_res(ind)))>1)&(find(mtlb_res(ind)==0)==[]) then
    bool_val=and(max(abs(diff_mat(ind)./mtlb_res(ind)))<prec)
  else
    bool_val=and(abs(diff_mat(ind))<prec);
  end
end

endfunction
