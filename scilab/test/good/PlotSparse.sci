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
     error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"PlotSparse"));
   end
   
   if (typeof(A) == "sparse") then
      [m, n] = size(A)
      nel = nnz(A)
   else   
      error(msprintf(gettext("%s: Wrong type for input argument #%d: sparse matrix expected.\n"),"PlotSparse",1));
   end
   
   if rhs == 1 then
      markColor = default_markColor
      markId = default_markId
   elseif typeof(style) ~= "string" then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"PlotSparse",2));
   else
      [ markColor , markId ] = ana_style(style)
   end
    
   // display
   drawlater()
   ij = spget(A)
   plot2d(ij(:,2), ij(:,1),-markId)
   e = gce();
   e1 = e.children(1);
   e1.mark_foreground = markColor;    
   
   ca=gca();
   ca.box="on";
   ca.axes_reverse(2)="on";
   mat_size = size(A);
   ca.data_bounds = [-.5, -.5 ; mat_size(2)+.5 , mat_size(1)+.5];
   ca.tight_limits="on";
   
   // information about nnz
   captions(e1, "nnz = " + string(nnz(A)), "lower_caption");
   
   drawnow()

endfunction
