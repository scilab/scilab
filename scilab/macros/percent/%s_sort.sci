function [p,q]=%s_sort(A,flag)
//Utility function for column sorting
// Copyright INRIA
[p,q]=sort(A.',"r");
p=p.';q=q.';
endfunction
