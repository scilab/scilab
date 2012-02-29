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

function [col, mark] = ana_style(style)
  //
  //  an utility for PlotSparse
  //
  tab_col  = ["k" "b" "r" "g" "c" "m" "y" "t" "G"]
  num_col  = [ 1   2   5   3   4   6  32  16  13 ]
  tab_mark = ["." "+" "x" "*" "D" "d" "^" "v" "o"]
  num_mark = [ 0   1   2   3   4   5   6   7   9 ]

  deff("[b] = is_digit(c)",[ "code = _str2code(c)";...
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
