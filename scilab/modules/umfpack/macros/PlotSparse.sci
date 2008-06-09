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
