function [%Xlist,%OPT]=lmisolver(%Xinit,%evalfunc,%options) 
// Copyright INRIA
%OPT=[];%Xlist=list();
[LHS,RHS]=argn(0);

if RHS==2 then 
  %Mb = 1e3;%ato = 1e-10;%nu = 10;%mite = 100;%rto = 1e-10;
else
  %Mb=%options(1);%ato=%options(2);%nu=%options(3);%mite=%options(4);%rto=%options(5);
end

%to=1e-5
%tol=1e-10

[%Xinit,%ind_X]=aplat(%Xinit);

%dim_X=list();
for %ia=1:size(%Xinit)
    %dim_X(%ia)=size(%Xinit(%ia))
end
%dim_X=matrix(list2vec(%dim_X),2,size(%dim_X))';


%x0=list2vec(%Xinit);
%nvars=size(%x0,'*')

//Testing feasibility of initial guess
[%b,%F_0,%linobj0]=%evalfunc(vec2list(%x0,%dim_X,%ind_X));
if size(%linobj0,'*')==0 then
  %Val=aplat(%F_0)
  %sm=100;
  for %w=%Val
    if %w~=[] then 
      %sm=min(%sm,mini(real(spec(%w))))
    end
  end     
  %nor=0
  %Val=aplat(%b)
  for %w=%Val
    if %w~=[] then 
      %nor=max(%nor,norm(%w,1))
    end
  end     
  if %sm>=-%tol & %nor<%tol then
    %Xlist=vec2list(%x0,%dim_X,%ind_X);
    disp('initial guess is feasible');return; 
  end
end


%xz=zeros(%nvars,1);
[%b,%F_0,%linobj0]=%evalfunc(vec2list(%xz,%dim_X,%ind_X));
%b=list2vec(aplat(%b));
%F_0=aplat(%F_0);

%blck_szs=[];
  for %lmii=%F_0
    [%mk,%mk]=size(%lmii);%blck_szs=[%blck_szs,%mk]
  end
%blck_szs=%blck_szs(find(%blck_szs~=0));


[%F_0,%dim_F]=list2vec(%F_0);
%linobj0=list2vec(aplat(%linobj0));

%Ncstr=[];%F_is=[];%linobj=[];


//Construction of canonical representation
disp('Construction of canonical representation')
%spF_0=sparse(%F_0);
%spb=sparse(%b);
%lX=size(%Xinit)
%XZER=%Xinit
for %ka=1:%lX
  %XZER(%ka)=sparse(0*%Xinit(%ka))
end

for  %ja=1:%lX
  %Dum=%XZER(%ja)
  %row=%dim_X(%ja,1)
  %coll=%dim_X(%ja,2)
  for %ca=1:%coll
    for %ra=1:%row
      %Dum(%ra,%ca)=1
      %XZER(%ja)=%Dum;
      [%bi,%F_i,%linobji]=%evalfunc(recons(%XZER,%ind_X));
      %Ncstr=[%Ncstr,splist2vec(%bi)-%spb];
      %F_is=[%F_is,splist2vec(%F_i)-%spF_0];
      %linobj=[%linobj,%linobji-%linobj0];
      %Dum(%ra,%ca)=0;
      %XZER(%ja)=%Dum
    end
  end
end

if size(%Ncstr,'*')==0 then
   %Ncstr=speye(%nvars,%nvars);
 else
disp('Basis Construction')
[%x0,%Ncstr]=linsolve(%Ncstr,%b,%x0);
end


%F_0=%F_0+%F_is*%x0;%F_is=%F_is*%Ncstr;
%linobj0=%linobj0+%linobj*%x0;%linobj=%linobj*%Ncstr;



if %blck_szs == [] then
// is objective constant on LME constraint set, Xinit is feasible 
  if maxi(abs(%linobj+0)) < %to then
    disp(' ');
    disp(' Objective constant.');
    %Xlist=vec2list(%x0,%dim_X,%ind_X);
    %Xopt=%linobj0;
    return
  else
    error('solution unbounded.');
  end
end

[%fm,%m]=size(%F_is);

//Testing well-posedness
if %fm<%m then 
   error('Ill-posed problem: # of unknowns ('+string(%m)+...
                            ') > # of constraints ('+string(%fm)+')');
end

//Testing rank deficiency
if size(%F_is,'*')<>0 then
  %P=speye(%m,%m);
  [%ptr,%rk]=lufact([%F_is spzeros(%fm,%fm-%m)]',[%tol,0.001]);
  if %rk<%m then
    [%P,%L,%U,%Q]=luget(%ptr);%L=[];%U=[];%Q=[];
    %P=%P';%P=%P(1:%rk,1:%m)';
    warning(' rank deficient problem');
    ludel(%ptr);
    //Testing to see if linobj in the range of F_is
    if size(%linobj,'*') <> 0 then
      [%ptr,%rk2]=lufact([[%F_is;%linobj] spzeros(%fm+1,%fm+1-%m)]',[%tol,0.001]);
      if %rk<%rk2 then
        error(' solution unbounded');
      end
    end
  end
  ludel(%ptr);
  %linobj=%linobj*%P
  %F_is=%F_is*%P;
  %Ncstr=%Ncstr*%P;
  %m=%rk;
  %P=[];
end



//Testing to see if solution or the LMI value is unique
if size(%F_is,'*')==0 then
  %Val=vec2list(%F_0,%dim_F);
  %flag=%t
  for %w=%Val
    if %w~=[] then 
      if mini(real(spec(%w)))<-%tol then %flag=%f; end
    end
  end     
  if %flag then 
    %Xlist=vec2list(%x0,%dim_X,%ind_X);return; 
  else 
    error('not feasible or badly defined problem');
  end   
end

//Testing feasibility of initial guess
%Val=vec2list(%F_0,%dim_F);
%sm=100;
for %w=%Val
   if %w~=[] then 
     %sm=min(%sm,mini(real(spec(%w))))
   end
end     

if %sm>=-%tol & size(%linobj,'*')==0 then
          %Xlist=vec2list(%x0,%dim_X,%ind_X);return; 
end

%M=%Mb*maxi(full((ones(1,%fm)*(abs([%F_0,%F_is])))));


if ~(%sm>%to) then
	disp('     FEASIBILITY PHASE.');

	// mineigF is the smallest eigenvalue of F_0
	%mineigF = 0.0;
	%blck_szs=matrix(%blck_szs,1,size(%blck_szs,'*'));
	%ka=0; 
        for %n=%blck_szs,
	   %mineigF = mini(%mineigF, mini(real(spec(matrix(%F_0(%ka+[1:%n^2]),%n,%n)))));
	   %ka=%ka+%n^2;   
	end;

	// I is the identity
	%I = zeros(%fm,1);  
	%ka=0; 
	for %n=%blck_szs,
	   %I(%ka+[1:%n^2]) = matrix(eye(%n,%n),%n^2,1);   // identity
	   %ka=%ka+%n^2; 
	end;

	if (%M < %I'*%F_0+1e-5), 
	   error('Mbound too small.'); 
	end;

	// initial x0 
	%x00 = [zeros(%m,1); max(-1.1*%mineigF, 1e-5)];

	// Z0 is the projection of I on the space Tr F_i*Z = 0

	     [%ptr,%rkA]=lufact(%F_is'*%F_is,[%tol,0.001]);
	     %Z0=lusolve(%ptr,full(%F_is'*%I));
	     %Z0=%I-%F_is*%Z0;
             ludel(%ptr)
        //check: trace(F_is*Z0) = 0 <=> %F_is(:,k)'*%Z0= 0 (k = 1:m)
// mineigZ is the smallest eigenvalue of Z0
	%mineigZ = 0.0;
	%ka=0; 
	for %n=%blck_szs,
	   %mineigZ = mini(%mineigZ, mini(real(spec(matrix(%Z0(%ka+[1:%n^2]),%n,%n)))) );
	   %ka=%ka+%n^2;   
	end

	%Z0(%ka+1) = max( -1.1 *%mineigZ, 1e-5 );  // z  
	%Z0(1:%ka) = %Z0(1:%ka) + %Z0(%ka+1)*%I; 
	%Z0 = %Z0 / (%I'*%Z0(1:%ka));    // make Tr Z0 = 1
	//Pack Z0 and F_is
	%Z0=pack(%Z0,[%blck_szs,1]);

	[%xi,%Z0,%ul,%info]=...
       semidef(%x00,%Z0,full(pack([%F_0,%F_is,%I;%M-%I'*%F_0,-%I'*%F_is,0],...
         [%blck_szs,1])),[%blck_szs,1],[zeros(%m,1); 1],[%nu,%ato,-1,0,%mite]);

        %xi=%xi(1:%m);

	select %info(1)
	case 1
	     error('Max. iters. exeeded')
	case 2 then
	     disp('Absolute accuracy reached')
	case 3 then
	     disp('Relative accuracy reached')
	case 4 then
	     disp('Target value reached')
	case 5 then
	     error('Target value not achievable')
	else
	     warning('No feasible solution found')
	end


	if %info(2) == %mite then error('max # of iterations exceeded');end
	if (%ul(1) > %ato) then error('No feasible solution exists');end
 //       if (%ul(1) > 0) then %F_0=%F_0+%ato*%I;end

	disp('feasible solution found')

else

	disp('Initial guess feasible')
	%xi=zeros(%m,1);
end


if size(%linobj,'*')<>0 then

disp('      OPTIMIZATION PHASE.') 

%M = max(%M, %Mb*sum(abs([%F_0,%F_is]*[1; %xi])));  

// I is the identity
	%I = zeros(%fm,1);  
	%ka=0; 
	for %n=%blck_szs,
	   %I(%ka+[1:%n^2]) = matrix(eye(%n,%n),%n^2,1);   // identity
	   %ka=%ka+%n^2; 
	end;
  
// M must be greater than trace(F(x0))   for bigM.sci
 [%ptr,%rkA]=lufact(%F_is'*%F_is,[%tol,0.001]);
	     %Z0=lusolve(%ptr,full(%F_is'*%I-%linobj'));
	     %Z0=%I-%F_is*%Z0;
             ludel(%ptr)

//check: trace(F_is*Z0) = c <=> %F_is(:,k)'*%Z0= %linobj(k) (k = 1:m)
// mineigZ is the smallest eigenvalue of Z0

%mineigZ = 0.0;

%ka=0; for %n=%blck_szs,
  %mineigZ = mini(%mineigZ, mini(real(spec(matrix(%Z0(%ka+[1:%n^2]),%n,%n)))));
  %ka=%ka+%n^2;
end;

%Z0(%ka+1) = max(1e-5, -1.1*%mineigZ);  
%Z0(1:%ka) = %Z0(1:%ka) + %Z0(%ka+1)*%I; 

if (%M < %I'*[%F_0,%F_is]*[1;%xi] + 1e-5), 
   error('M must be strictly greater than trace of F(x0).'); 
end;

// add scalar block Tr F(x) <= M

%blck_szs = [%blck_szs,1];


[%xopt,%z,%ul,%info]=semidef(%xi,pack(%Z0,%blck_szs),...
   full(pack([%F_0,%F_is; %M-%I'*%F_0,-%I'*%F_is],%blck_szs)),...
                 %blck_szs,full(%linobj),[%nu,%ato,%rto,0.0,%mite]);

if %info(2) == %mite then 
  warning('max # of iterations exceeded, solution may not be optimal');
end;
if sum(abs([%F_0,%F_is]*[1; %xopt])) > 0.9*%M then 
                                disp( 'may be unbounded below');end;
if %xopt<>[]&~(%info(2) == %mite) then disp('optimal solution found');else %xopt=%xi;end

else

%xopt=%xi;
end

%Xlist=vec2list(%x0+%Ncstr*%xopt,%dim_X,%ind_X);
%OPT=%linobj0+%linobj*%xopt;


endfunction
function [r,ind]=recons(r,ind)
//reconstruct a list from a flat list (see aplat)
if ind==-1 then r=r(:);return;end
nr=size(r)
ma=0
for k=nr:-1:1
   mm=size(ind(k),'*');
   if ma<=mm then ma=mm;ki=k; end
end

if ma<=1 then return; end
vi=ind(ki);vi=vi(1:ma-1);
k=ki
vj=vi

while vj==vi
  k=k+1
  if k>nr then break; end
  vv=ind(k);
  if size(vv,'*')==ma then vj=vv(1:ma-1); else vj=[]; end
end
kj=k-1
rt=list(r(ki))
for k=ki+1:kj
  rt(k-ki+1)=r(ki+1)
  r(ki+1)=null()
  ind(ki+1)=null()
end
ind(ki)=vi
r(ki)=rt
[r,ind]=recons(r,ind)

endfunction
function [bigVector]=splist2vec(li)
//li=list(X1,...Xk) is a list of matrices
//bigVector: sparse vector [X1(:);...;Xk(:)] (stacking of matrices in li)
bigVector=[];
li=aplat(li)
for mati=li
  sm=size(mati);
  bigVector=[bigVector;sparse(matrix(mati,prod(sm),1))];
end

endfunction
function [A,b]=spaff2Ab(lme,dimX,D,ind)
//Y,X,D are lists of matrices. 
//Y=lme(X,D)= affine fct of Xi's; 
//[A,b]=matrix representation of lme in canonical basis.
[LHS,RHS]=argn(0)
select RHS
case 3 then
nvars=0;
for k=dimX'
   nvars=nvars+prod(k);
end
x0=zeros(nvars,1);
b=list2vec(lme(vec2list(x0,dimX),D));
A=[];
for k=1:nvars
xi=x0;xi(k)=1;
   A=[A,sparse(list2vec(lme(vec2list(xi,dimX),D))-b)];
end

case 4 then
nvars=0;
for k=dimX'
   nvars=nvars+prod(k);
end
x0=zeros(nvars,1);
b=list2vec(lme(vec2list(x0,dimX,ind),D));
A=[];
for k=1:nvars
xi=x0;xi(k)=1;
  A=[A,sparse(list2vec(lme(vec2list(xi,dimX,ind),D))-b)];
end
end
endfunction
