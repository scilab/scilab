//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project 
//   <bruno.pincon@iecn.u-nancy.fr>
// 
// This set of scilab 's macros provide a few sparse utilities.
// 
// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// 
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability. 
// 
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or 
// data to be ensured and,  more generally, to use and operate it in the 
// same conditions as regards security. 
// 
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.

function [K1] = condestsp(A, arg2, arg3)
  //                     (A, LUp  , t)
  //  PURPOSE
  //     Give an estimate of the 1-norm condition number of 
  //     the sparse matrix A by Algorithm 2.4 appearing in :
  //
  //      "A block algorithm for matrix 1-norm estimation
  //       with an application to 1-norm pseudospectra"
  //       Nicholas J. Higham and Francoise Tisseur
  //       Siam J. Matrix Anal. Appl., vol 21, No 4, pp 1185-1201
  //
  //  PARAMETERS
  //     A   : a square sparse matrix
  //  
  //     LUp : (optional) a pointer to (umf) LU factors of A
  //           gotten by a call to umf_lufact ; if you
  //           have already computed the LU (= PAQ) factors
  //           it is recommanded to give this optional
  //           parameter (as the factorization may be time
  //           consuming)
  //
  //     t   : (optional) a positive integer
  //
  //     K1  : estimated 1-norm condition number of A
  //
  //  POSSIBLE CALLING SEQUENCES
  //     [K1, [x]] = condestsp(A, LUp, t)
  //     [K1, [x]] = condestsp(A, LUp)
  //     [K1, [x]] = condestsp(A, t)
  //     [K1, [x]] = condestsp(A)
  //
  //  AUTHOR
  //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> but nearly
  //     close to the given algorithm as this one is written in
  //     a "matlab-like" language
  //
  [lhs, rhs] = argn()

  if rhs<1 | rhs>3 | lhs > 2 then
     error(msprintf(gettext("%s: Wrong number of input arguments.\n"),"condestsp"))
  end
  
  if typeof(A) ~= "sparse"  then
     error(msprintf(gettext("%s: Wrong type for first input argument: Square sparse matrix expected.\n"),"condestsp"))
  else
     [n,m] = size(A)
     if n~=m  then
        error(msprintf(gettext("%s: Wrong size for first input argument: Square sparse matrix expected.\n"),"condestsp"))
     end
  end
    
  factor_inside = %f  // when LUp is given (after the following tests
                      // this var is set to %t if the factorisation 
		      // is computed inside this function)
  if rhs == 1 then
     LUp = umf_lufact(A) ; factor_inside = %t ; t = 2
  elseif rhs == 2 then
     if typeof(arg2) == "pointer" then
	LUp = arg2 ; t = 2
     else
	t = arg2 ; LUp = umf_lufact(A) ; factor_inside = %t
     end
  elseif rhs == 3 then
     LUp = arg2 ; t = arg3
  end

  // verify if LUp and T are valid !
  [OK, nrow, ncol] = umf_luinfo(LUp)
  if ~OK then
     error(" the LU pointer is not valid")
  elseif n ~= nrow | nrow ~= ncol
     error(" the matrix and the LU factors have not the same dimension !")
  end
     
  if int(t)~=t | length(t)~=1 | or(t < 1) then
     error(" invalid type and/or size and/or value for the second arg")
  end   

  // go on
  
  // the algo need a fortran-like sign function (with sign(0) = 1
  // and not with sign(0)=0 as the scilab native 's one)
  deff("s = fsign(x)", "s = sign(x) ; s(find(s == 0)) = 1")
  
  
  // Part 1 : computes ||A||_1
  norm1_A = norm(A,1)
  
  // Part 2 : computes (estimates) || A^(-1) ||_1
  
  // 1/ choose starting matrix X (n,t)
  X = ones(n,t)
  X(:,2:t) = fsign(rand(n,t-1)-0.5)
  X = test_on_columns(X) / n
  Y = zeros(X) ; Z = zeros(X)
  ind_hist = []
  est_old = 0
  ind = zeros(n,1)
  S = zeros(n,t)
  k = 1 ; itmax = 5
  
  while %t
     // solve Y = A^(-1) X <=> A Y = X
     for j=1:t
	Y(:,j) = umf_lusolve(LUp, X(:,j))
     end
     [est, ind_est] = max( sum(abs(Y),"r") )
     if est > est_old  |  k==2 then
	ind_best = ind_est
	w = Y(:,ind_best)
     end
     
     if k >= 2  &  est <= est_old then, est = est_old, break, end
     est_old = est ; S_old = S
     
     if k > itmax then , break , end
     
     S = fsign(Y)
     
     // if every column of S is // to a column of S_old then it is finish
     if and( abs(S_old'*S) == n ) then , break , end

     if t > 1 then
       // s'assurer qu'aucune colonne de S n'est // a une autre
       // ou a une colonne de S_old en remplacant des colonnes par rand{-1,1}
       S = test_on_columns(S,S_old)
     end
     
     // calcul de Z = A' S
     for j=1:t
	Z(:,j) = umf_lusolve(LUp, S(:,j),"A''x=b")
     end

     [h,ind] = sort(max(abs(Z),"c"))
     if k >= 2  then
       if h(1) == h(ind_best) then , break , end
     end

     if (t > 1) & (k > 1) then
	j = 1
	for l=1:t
	   while %t
	      if find(ind_hist == ind(j)) == [] then  
	         ind(l) = ind(j)
		 j = j + 1
	         break
	      else
	         j = j + 1
	      end
	   end
        end
     end
     X = zeros(n,t)
     for l = 1:t
        X(ind(l),l) = 1
     end
     ind_hist = [ind_hist ; ind(1:t)]
     k = k + 1
  end

  K1 = est * norm1_A
  if factor_inside then
     umf_ludel(LUp)
  end
  
endfunction


function [X] = test_on_columns(X,Xold)
   // 
   //  X and Xold are (n,t) matrix , Xold being an optional argument
   //  verify and force all columns of the new X to be non paralleles
   //  and also to be non paralleles to the columns of Xold if Xold
   //  is given
   //  2 columns j1 and j2 are // if  | Cj1'*Cj2 | = n because
   //  all elements are in {+1,-1} 
   //
   [lhs,rhs] = argn()
   [n,t] = size(X)
   if rhs == 1 then
      for j=2:t
	 while %t
	    res_test = abs(X(:,j)'*X(:,1:j-1))
	    if or(res_test == n) then
	       X(:,j) = fsign(rand(n,1)-0.5)
	    else
	       break
	    end
	 end
      end
   else   // rhs = 2
      for j=1:t
	 while %t
	    res_test = abs([X(:,j)'*Xold  X(:,j)'*X(:,1:j-1)])
	    if or(res_test == n) then
	       X(:,j) = fsign(rand(n,1)-0.5)
	    else
	       break
	    end
	 end
      end
   end  
endfunction

function [A,description,ref,mtype] = ReadHBSparse(filename)
   //
   //  PURPOSE
   //     An utility to read the Harwell-Boeing sparse matrix 
   //     format. Currently don't work for unassembled matrix.
   //     Also possible rhs presents in the file are not red.
   //
   //  ARGUMENTS
   //     filename   : (optional) a string given the filename 
   //                  (eventually preceeding by the path), if 
   //                  filename is not given then the function 
   //                  use xgetfile to get filename
   //     A          : the sparse matrix
   //     description: a string given some information about the
   //                  matrix
   //     ref        : a string given the reference of the matrix
   //     mtype      : a string given the "type" of the matrix
   //                  
   //  COMMENTS 
   //     Generally the file name is of the form ref.matrixtype  
   //     where mtype is a 3 letters word given some 
   //     informations (already inside the file) on the matrix : 
   //        1st letter : R|C|P   for real|complex|pattern (no values given)
   //        2d  letter : S|H|Z|U for symetric|hermitian|skew symetric|unsymetric
   //        3d  letter : A|E     for assembled|unassembled matrix
   //                             (case E is not treated by this func)
   //  REFERENCES
   //     Users' Guide for the Harwell-Boeing Sparse Matrix Collection
   //     Iain S. Duff, Roger G. Grimes, John G. Lewis
   //
   //     You may found this guide and numerous sparse
   //     matrices (in the Harwell-Boeing format) at the
   //     University of Florida Sparse Matrix Collection
   //     web site :
   //
   //        http://www.cise.ufl.edu/research/sparse/matrices/
   //
   //     maintained by Tim Davis <http://www.cise.ufl.edu/~davis/>
   //
   //  AUTHOR
   //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
   //
   [lhs, rhs] = argn()

   if rhs == 0 then
      filename = xgetfile("*.[rc][shzu]a", title=["Choose a sparse matrix"; ...
		                                  "   then click on OK   "])
   elseif rhs == 1 then
      if typeof(filename) ~= "string" then
	 error(" the argument must be a string")
      end
   else
      error(" bad number of arguments")
   end
   
   unit = file("open", filename, "old")

   // 1)  read the 4 or 5 header lines
   
   line1 = read(unit,1,1,"(A)")
   ref = stripblanks(part(line1,73:80))
   ref = convstr(ref)
   description = stripblanks(part(line1,1:72))
   description = convstr(description)

   // normaly the line2 contains 5 numbers and if the last is zero
   // the file contains no rhs but in this case in some HB file
   // we have only 4 numbers. So the following is a trick to take
   // into account this pb
   line2 = read(unit,1,1,"(A)") // read the line 2 as a string
   line2 = evstr(line2)         // this string is then transform is a row vector
   if length(line2) < 5 then
      Rhs_in_file = %f
   else
      if line2(5) == 0 then
	 Rhs_in_file = %f
      else
	 Rhs_in_file = %t
	 warning(" The file contains a rhs but it will not be read !")
      end
   end
   
   line3 = read(unit,1,1,"(A)")
   mtype = convstr(part(line3,1:3))
   
   Dimensions = evstr(part(line3,4:80))
   if part(mtype,3)=="e" then
      error("currently don''t read unassembled (elemental) sparse matrix")
   end
   TypeValues = part(mtype,1) // r for real, c for complex, p for pattern
				   
   m     = Dimensions(1)   // number of rows
   n     = Dimensions(2)   // number of columns 
   nb_nz = Dimensions(3)   // number of non zeros
   
   
   line4 = read(unit,1,1,"(A)")  // these are the formats
   form1 = stripblanks(part(line4,1:16))
   form2 = stripblanks(part(line4,17:32))
   form3 = stripblanks(part(line4,33:52))
   // for the 2 first replace Ix by Fx.0 : the read func uses only float formats
   form1 = replace_Ix_by_Fx(form1)
   form2 = replace_Ix_by_Fx(form2)
   
   if Rhs_in_file then  // a 5 header line to read (but ignored)
      line5 = read(unit,1,1,"(A)")  
   end
   
   
   // 2) read the datas
   
   col_ptr = read(unit,1,n+1,form1)
   ind_row = read(unit,1,nb_nz,form2)
   select TypeValues
   case "p" // values given 
      warning(" No values for this matrix (only non zero pattern) : put some 1")
      val = ones(1,nb_nz)
   case "r" // values are real
      val = read(unit,1,nb_nz,form3)
   case "c" // values are complex
      valc = matrix( read(unit,1,2*nb_nz,form3) , 2, nb_nz )
      val = valc(1,:) + %i*(valc(2,:))
      clear valc
   end

   file("close", unit)
   
   // 3) form the sparse scilab matrix

   // 3-1/ form the basic matrix
   ind_col = ones(1,nb_nz)
   for i = 2:n
      ind_col(col_ptr(i):col_ptr(i+1)-1) = i
   end
   
   A = sparse([ind_row' ind_col'], val, [m n])
   clear ind_row ind_col col_ptr val  // to regain some memory
   
   // 3-2/ complete the matrix depending the symetry property
   MatrixSymetry = part(mtype,2)
   select MatrixSymetry
   case "s"  // (real or complex) symmetric matrix 
      A = A - diag(diag(A)) + A.'
   case "h"  // complex hermitian matrix
      A = A - diag(diag(A)) + A'
   case "z"  // skew symmetric matrix
      A = A - A'
   end
   
endfunction

function [form_out] = replace_Ix_by_Fx(form_in)
   //
   //  replace Ix by Fx.0 (assuming that x is 1 char max)
   //  (utility function used by ReadHBSparse)
   //
   n = length(form_in)
   indI = strindex(form_in,"I")
   if indI== [] then
      indI = strindex(form_in,"i")
   end 
   form_out = part(form_in,1:indI-1)+"F"+part(form_in,indI+1) ...
	      +".0"+part(form_in,indI+2:n)
endfunction

function [] = scisptdemo()
   // demo pour l'interface umf
   mode(-1)
   st = stacksize();
   if st(1) < 3000000 then
      x_message([" For this demo the current stack size is not enought ";
		 " enter the following at the scilab prompt :          ";
		 "                                                     ";
		 "              stacksize(3000000);                    ";
		 "                                                     ";
                 "           then re enter scisptdemo()                ";
		 "                                                     ";
		 "               CLICK TO  CONTINUE                    "]);
      return
   end

   oldln = lines();
   lines(0)

   deff('[]=demoex(num)','exec(scisptdemolist(num,2),-1)')
   a = gda();
   a.title.font_size = 3;
   a;title.font_style = 6;
   

   scisptdemolist = ["how to use this stuff"           , DIR_SCISPT_DEM+"scisptdem1.dem";
                     "display a speed comparison test" , DIR_SCISPT_DEM+"scisptdem2.dem";
	             "small tests for condestsp"       , DIR_SCISPT_DEM+"scisptdem3.dem"];
   while %t
      num=x_choose(scisptdemolist(:,1), "Click to choose a demo");
      if num==0 then 
	 lines(oldln(1)) , break
      else
	 demoex(num)
      end
   end
   
   sda()
endfunction

function [] = PlotSparse(A, style)
   //
   //  PURPOSE
   //     plot the pattern of non nul elements of a sparse matrix
   //
   //  ARGUMENTS
   //     A     : a sparse matrix
   //     style : (optional) a string given the color and/or the
   //             marker type of the form "[color][mark]" where
   //             color may be a number referring the color you
   //             want to use (in the current colormap). If you
   //             use the std colormap then color may be one of 
   //             the following letters :
   //               k  for black       b  for blue
   //               r  for red         g  for green
   //               c  for cyan        m  for magenta
   //               y  for yellow      t  for turquoise
   //               G  a dark green
   //
   //             mark must be one of the following :
   //               .  point             +  plus 
   //               x  cross             *  circled plus 
   //               D  filled diamond    d  diamond
   //               ^  upper triangle    v  down triangle
   //               o  circle
   //
   //             by default you have "b." (in fact the 2d color) and 
   //             this is also forced in case of error. 
   //
   //  COMMENTS
   //     for "big" matrix use essentially point (.) as marker
   //     
   //
   //  AUTHOR
   //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
   //
   default_markColor = 2  // blue in std colormap
   default_markId    = 0  // a point .
   
   [lhs, rhs] = argn()
   
   if ( rhs<1 | rhs>2 ) then
      error(" Bad number of arguments")
   end
   
   if (typeof(A) == "sparse") then
      [m, n] = size(A)
      nel = nnz(A)
   else   
      error(" Arg #1 must be a sparse matrix")
   end
   
   if rhs == 1 then
      markColor = default_markColor
      markId = default_markId
   elseif typeof(style) ~= "string" then
      error(" Arg #2 must be a string")
   else
      [ markColor , markId ] = ana_style(style)
   end
   
   drawlater()
   plot2d(%inf, %inf, strf="030", rect=[-1,-1,n+1,m+1])
   
   // the tics
   x = [0 n/2 n]
   dx = -0.02*n ; dy = -0.05*m 
   xstring(x(1)+dx, dy, "1")
   h1 = gce();
   xstring(x(2)+dx, dy, string(floor(x(2))))
   h2 = gce();
   xstring(x(3)+dx, dy, string(x(3)))
   h3 = gce();
   
   y = [0 m/2 m]
   dx = 0.02*m ; dy = 0
   xstring(m+dx, y(1), string(y(3)))
   h4 = gce();
   xstring(m+dx, y(2), string(floor(y(2))))
   h5 = gce();
   xstring(m+dx, y(3), "1")
   h6 = gce();

   // information about nnz
   xstring(0, -0.1*m, "nnz = "+string(nnz(A)))
   h7 = gce();
     
   glue([h1,h2,h3,h4,h5,h6,h7])
   
   // display
   ij = spget(A)
   xp = ij(:,2) - 0.5
   yp = m+0.5 - ij(:,1)
   plot2d(xp,yp,-markId,strf="000")
   e = gce();
   e.children(1).mark_foreground = markColor;

   // the rectangle
   xrect(0,m,n,m)
   drawnow()
   
endfunction

function [col, mark] = ana_style(style)
  //
  //  an utility for PlotSparse
  //
  tab_col  = ["k" "b" "r" "g" "c" "m" "y" "t" "G"]
  num_col  = [ 1   2   5   3   4   6  32  16  13 ]
  tab_mark = ["." "+" "x" "*" "D" "d" "^" "v" "o"]
  num_mark = [ 0   1   2   3   4   5   6   7   9 ]

  deff("[b] = is_digit(c)",[ "code = str2code(c)";...
		             "b = 0 <= code & code <= 9" ])
  n = length(style)
  if n >= 1 then
     c = part(style,1) ; ic = 1
     ind = grep(tab_col, c)
     if ind == [] then
	if is_digit(c) then
	   while %t
	      ic = ic+1
	      if ic <= n then
		 c = part(style,ic)
		 if ~is_digit(c) then , break, end
	      else
		 break
	      end
	   end
	   col = evstr(part(style,1:ic-1))
	   nb_col = xget("lastpattern")
	   if col < 1  |  col > nb_col then
	      col = default_markColor
	   end
	else
	   col = default_markColor
	end
     else
	ic = 2
	col = num_col(ind)
     end
     reste = part(style,ic:n)
     if reste == "" then
	mark = default_markId
     else
	ind = grep(tab_mark, part(style,ic:n))
	if ind == [] then
	   mark = default_markId
	else
	   mark = num_mark(ind)
	end
     end
  else
     col = default_markColor ; mark = default_markId
  end

endfunction

function [K2, lm, vm, lM, vM] = cond2sp(A, C, rtol, itermax, verb)
   //
   //  PURPOSE
   //     for a s.p.d. matrix computes the maximum and minimum
   //     eigen element (value and vector) with the power and
   //     inverse power method then the 2-norm condition number
   //     K2 = lM / lm
   //
   //  PARAMETERS
   //    inputs
   //    ------
   //     A       : a sparse s.p.d. matrix
   //     C       : pointer onto a Cholesky factorization (gotten with 
   //               taucs_chfact)
   //     rtol     : (optional) relative precision for the output test 
   //                   (l_new - l_old)/l_new < rtol 
   //     itermax : (optional) maximum number of iteration in each step
   //     verb    : (optional) a boolean must be %t for display result 
   //               for each iteration
   //
   //   outputs
   //   -------
   //     K2      : 2-norm condition number
   //     lm      : min eigenvalue
   //     vm      : associated eigenvector
   //     lM      : max eigenvalue
   //     vM      : associated eigenvector
      
   //
   [lhs, rhs] = argn()
   // no verif
   if ~exists("verb", "local") then , verb = %f , end
   if ~exists("rtol", "local") then , rtol = 1.e-3, end
   if ~exists("itermax","local") then , itermax = 30 , end
   itermax = max(4,itermax)  // 4 iterations are forced 
   
   // 1) computes (with "direct Rayleigh power method") lM, vM 
   n = size(A,1)
   x = rand(n,1) ; x = x / norm(x)
   y = A*x
   lM_old = x'*y
   iter = 0
   if verb then
      mprintf("\n\r approximate (lM,vM) with the iterative power method \n")
      mprintf(" ----------------------------------------------------\n")
   end   
   while %t
      iter = iter + 1
      x = y / norm(y)
      y = A*x
      lM = x'*y
      if verb then , mprintf(" iteration %3d : lM = %e  \n", iter, lM) , end
      crit = abs((lM - lM_old)/lM) 
      if crit < rtol  &  iter > 3 then 
	 break
      else
	 lM_old = lM
      end
      if iter >= itermax then
	 mprintf(" Warning : for lM ""convergence"" at rtol = %e \n", rtol)
	 mprintf("           don''t reached after %d iterations (got only %e) \n", ...
		 itermax, crit)
	 break
      end
   end
   vM = x

   // 2) computes (with "inverse Rayleigh power method") lm, vm 
   x = rand(n,1) ; x = x / norm(x)
   y = taucs_chsolve(C,x)
   lm_old = x'*y
   iter = 0
   if verb then
      mprintf("\n\r approximate (lm,vm) with the inverse iterative power method \n")
      mprintf(" ------------------------------------------------------------\n")
   end   
   while %t
      iter = iter + 1
      x = y / norm(y)
      y = taucs_chsolve(C,x)
      lm = x'*y
      if verb then , mprintf(" iteration %3d : lm = %e  \n", iter, 1/lm) , end
      crit = abs((lm - lm_old)/lm)
      if crit < rtol  &  iter > 3 then 
	 break
      else
	 lm_old = lm
      end
      if iter >= itermax then
	 mprintf(" Warning : for lm ""convergence"" at rtol = %e \n", rtol)
	 mprintf("           don''t reached after %d iterations (got only %e) \n", ...
		 itermax, crit)
	 break
      end
    end
   vm = x
   lm = 1/lm;
   K2 = lM/lm;
   
endfunction

function [xn, rn] = rafiter(A, C, b, x0, nb_iter, verb)
   // raffinement iteratif
   //
   if ~exists("verb", "local") then , verb = %f , end
   if ~exists("nb_iter", "local") then , nb_iter = 2, end

   xn = x0
   for i=1:nb_iter
      rn = res_with_prec(A, xn, b)
      dx = taucs_chsolve(C, rn)
      if verb then
	 crit1 = norm(rn) ; crit2 = norm(dx)
	 mprintf(" it %2d : ||r|| = %e , ||dx|| = %e \n", i, crit1, crit2)
      end
      xn = xn - dx
   end
endfunction
