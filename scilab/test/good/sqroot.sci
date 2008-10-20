function [S]=sqroot(Q)
// Copyright INRIA
  Q1=(Q+Q')/2;
  if norm(Q1-Q,1) > 100*%eps then warning('sqroot: input not symmetric!');end
  tt=mini(spec(Q1));
  if tt <-10*%eps then warning('sqroot: input not semi-definite positive!');end
  if norm(Q,1) < sqrt(%eps) then S=[];return;end
  [u,S,v,rk]=svd(Q);
  S=v(:,1:rk)*sqrt(S(1:rk,1:rk));
  if norm(imag(Q1),1) <1.d-8 then S=real(S);;end
endfunction
