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
