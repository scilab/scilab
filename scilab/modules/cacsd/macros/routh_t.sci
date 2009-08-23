// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) 1999 - Lucien.Povy@eudil.fr (to get a good table)
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=routh_t(h,k)
//r=routh_t(h,k) computes routh table of denominator of the
//system described by transfer matrix SISO continue h with the
//feedback by the gain k
//If  k=poly(0,'k') we will have a polynomial matrix with dummy variable
//k, formal expression of the Routh table.
//r=routh_t(d) computes Routh table of h :attention ! d=denom of system

//If a zero row appears, it means that there exist a pair of pure imaginary
//roots (oscillating system) or symmetric real roots. In this case, the pure imaginary roots are the
//imaginary roots of the bisquare polynomial given by the previous row. The
//routh table can be continued replacing this row by the derivative of this
//polynomial.
//see http://www.jdotec.net/s3i/TD_Info/Routh/Routh.pdf for degenerated
//cases
  
  //see also 
//Comments on the Routh-Hurwitz criterion, Shamash, Y.,Automatic Control, IEEE T.A.C
//Volume 25, Issue 1, Feb 1980 Page(s): 132 - 133
  
  //http://controls.engin.umich.edu/wiki/index.php/RouthStability
  [lhs,rhs]=argn(0);
  h1=h(1);
  if rhs==2 then
    if typeof(h)<>'rational' then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: rational fraction array expected.\n"),"routh_t",1));
    end
    [n,d]=h(2:3)
    if size(n,'*')<>1 then
      error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"routh_t",1))
    end
    nd=maxi([degree(d) degree(n)])+1;
    cod=coeff(d,0:nd-1);//coeff du denominateur
    con=coeff(n,0:nd-1);//coeff du numerateur
    cobf=cod+k*con //coeff de la boucle fermee
  else
    if type(h)>2 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial array expected.\n"),"routh_t",1));
    end
    if size(h,'*')<>1 then
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"routh_t",1))
    end

    nd=degree(h)+1;
    cobf=coeff(h,0:nd-1)
  end;
  //
  r1=cobf(nd:-2:1);
  r2=cobf(nd-1:-2:1);
  ncol=size(r1,'*');
  if size(r2,'*')<>ncol then r2=[r2,0],end
  r=[r1;r2]
  if ncol<2 then r=[],return,end;
  if rhs==2 then

    for i=3:nd,
      r(i,1:ncol-1)=[r(i-1,1),-r(i-2,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]
    end;
  else
    for i=3:nd,
     if and(r(i-1,:)==0) then
	naux=nd-i+2 //order of previous polynomial
	exponents=naux:-2:0
	ncoeff=size(exponents,'*')
	r(i-1,1:ncoeff)=r(i-2,1:ncoeff).*exponents //derivative of previous polynomial
      end
      if r(i-1,1)==0 then 
	if rhs==1 then
	  if typeof(r)=='rational' then 
	    //scilab is not able to handle multivariable polynomials
	    r=horner(r,%eps^2); 
	  end
	  r(i-1,1)=poly(0,'eps')
	else
	  r(i-1,1)=%eps^2,
	end
      end
      r(i,1:ncol-1)=[1.,-r(i-2,1)/r(i-1,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]
      
    end;
  end;
endfunction
