function F=randpencil(eps,infi,fin,eta)
//returns a random pencil with given kronecker structure 
//eps=[eps1,...,epsk]; epsilon blocks (size eps1x(eps1+1),....)
//fin=[l1,...,ln]  finite eigenvalues (assumed real)  (possibly [])
//infi=[k1,...,kp] size of J blocks, ki>=1  (infi=[] if no J blocks)
//eta=[eta1,...,etap];   eta blocks size (eta1+1)xeta1,...)
// epsi's should be >=0 
// etai's should be >=0
// infi's should be >=1
// If called with eps=[0,...,0], infi=[], fin=[], eta=[]
// randpencil returns F=[];
// this should be an empty matrix with zero rows and coldim(eps) columns
// If called with eps=[], infi=[], fin=[], eta=[0,..,0]
// randpencil returns F=[];
// this should be a empty matrix with coldim(eta) rows and 0 columns.
// (bad behavior of the empty matrix!!!!!)

// Copyright INRIA
  [LHS,RHS]=argn(0);
  if RHS<>4 then error('randpencil requires 4 (possibly []) input parameters!');
  end
  select type(fin)
  case 1
    if diag(fin)<>[] then Fin=%s*eye()-diag(fin);else Fin=[];end
  case 2
    Fin=%s*eye()-companion(fin);
  end
  deff('j=%jdrn(n)','j=zeros(n,n);for k=1:n-1;j(k,k+1)=1;end')
  deff('Lk=%eta(k)','Lk=zeros(k+1,k);if k==0 then return;end;for j=1:k;Lk(j,j)=%s;Lk(j+1,j)=-1;end');
  deff('Lk=%epsilon(k)','Lk=zeros(k,k+1);if k==0 then return;end;for j=1:k;Lk(j,j)=%s;Lk(j,j+1)=-1;end');

  J=[];
  for kk=infi;
    J=sysdiag(J,%jdrn(kk));
  end
  if J==[] then Infin=[],else Infin=%s*J-eye();end

  flageps=%f;
  Eps=[];
  seps=sort(eps);
  if seps(1)==0 then flageps=%t;end
  if ~flageps then
    for k=seps;
      if k==0 then [p,q]=size(Eps); Eps=[Eps,zeros(p,1)];end
      if k<>0 then Eps=sysdiag(Eps,%epsilon(k));end
    end
  end

  flageta=%f;
  Eta=[];
  seta=sort(eta);
  if seta(1)==0 then flageta=%t;end
  if ~flageta then
    for k=seta;
      if k==0 then [p,q]=size(Eta); Eta=[Eta;zeros(1,q)];end
      if k<>0 then Eta=sysdiag(Eta,%eta(k));end
    end
  end

  F=sysdiag(Eps,Infin,Fin,Eta);

  [p,q]=size(F);ncols=q;

  if flageps then
    F=[zeros(p,prod(size(eps))),F];
    if F==[] then ncols=prod(size(eps));end
  end

  if flageta then
    [p,q]=size(F);
    if F~=[] then
      F=[F;zeros(prod(size(eta)),q)];
    else
      F=[F;zeros(prod(size(eta)),ncols)];
    end
  end
  // This can be uncommented for a seemingly more random pencil!
  //[p,q]=size(F);
  //rand('seed',0);
  //rand('normal')
  //Q=rand(p,p);
  //Z=rand(q,q);
  //F=Q*F*Z;
endfunction

