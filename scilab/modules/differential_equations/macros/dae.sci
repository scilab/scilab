// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sabine GAUZERE
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [varargout]=dae(varargin)
  [lhs,rhs]=argn();
  if (isdef('%DAEOPTIONS')==%F),
    %DAEOPTIONS=list ([],0,[],[],[],0,0);
  end
  if (size(varargin) == 0),
	error(sprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "dae",3,8));
  end
  if (type(varargin(1))==1), //standard case (dassl)
    
        if (rhs==4), //call without optional arguments
           [x0,t0,t,res]=varargin(:)
           if (lhs==2),
            [y,hd]=dassl(x0,t0,t,res,%DAEOPTIONS);
           elseif (lhs==1),
            [y]=dassl(x0,t0,t,res,%DAEOPTIONS);
           else,
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
           end

        elseif (rhs==8), // call with all the optional arguments
           [x0,t0,t,rtol,atol,res,jac,hd]=varargin(:)
           if (lhs==2),
            [y,hd]=dassl(x0,t0,t,atol,rtol,res,jac,%DAEOPTIONS,hd);
           elseif (lhs==1),
            [y]=dassl(x0,t0,t,atol,rtol,res,jac,%DAEOPTIONS,hd);
           else,
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
           end

        elseif (rhs==5),

            if (type(varargin(4))==1),
              [x0,t0,t,atol,res]=varargin(:)
              if (lhs==2),
               [y,hd]=dassl(x0,t0,t,atol,res,%DAEOPTIONS);
              elseif (lhs==1),
               [y]=dassl(x0,t0,t,atol,res,%DAEOPTIONS);
              else,
               error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
              end

            else,

              if (type(varargin(5))==1),
                [x0,t0,t,res,hd]=varargin(:)
                if (lhs==2),
                 [y,hd]=dassl(x0,t0,t,res,%DAEOPTIONS,hd);
                elseif (lhs==1),
                 [y]=dassl(x0,t0,t,res,%DAEOPTIONS,hd);
                else,
                 error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                end
             
              else,
                [x0,t0,t,res,jac]=varargin(:)
                if (lhs==2),
                 [y,hd]=dassl(x0,t0,t,res,jac,%DAEOPTIONS);
                elseif (lhs==1),
                 [y]=dassl(x0,t0,t,res,jac,%DAEOPTIONS); 
                else,
                 error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                end

              end

            end

          elseif(rhs==6),

            if (type(varargin(4))==1),

              if(type(varargin(5))==1),
                [x0,t0,t,rtol,atol,res]=varargin(:)
                if (lhs==2),
                 [y,hd]=dassl(x0,t0,t,atol,rtol,res,%DAEOPTIONS);
                elseif (lhs==1),
                 [y]=dassl(x0,t0,t,atol,rtol,res,%DAEOPTIONS); 
                else,
                 error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                end

              else,

                if (type(varargin(6))==1),
                 [x0,t0,t,atol,res,hd]=varargin(:)
                 if (lhs==2),
                  [y,hd]=dassl(x0,t0,t,atol,res,%DAEOPTIONS,hd);
                 elseif (lhs==1),
                  [y]=dassl(x0,t0,t,atol,res,%DAEOPTIONS,hd); 
                 else,
                   error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                 end

                else,
                 [x0,t0,t,atol,res,jac]=varargin(:)
                 if (lhs==2),
                  [y,hd]=dassl(x0,t0,t,atol,res,jac,%DAEOPTIONS);
                 elseif (lhs==1),
                  [y]=dassl(x0,t0,t,atol,res,jac,%DAEOPTIONS); 
                 else,
                  error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                 end

                end
              end

            else,
              [x0,t0,t,res,jac,hd]=varargin(:)
              if (lhs==2),
               [y,hd]=dassl(x0,t0,t,res,jac,%DAEOPTIONS,hd);
              elseif (lhs==1),
               [y]=dassl(x0,t0,t,res,jac,%DAEOPTIONS,hd); 
              else,
               error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
             end

            end

        elseif (rhs==7),

            if(type(varargin(5))==1),

              if (type(varargin(7))==1),
                [x0,t0,t,rtol,atol,res,hd]=varargin(:)
                if (lhs==2),
                 [y,hd]=dassl(x0,t0,t,atol,rtol,res,%DAEOPTIONS,hd);
                elseif (lhs==1),
                 [y]=dassl(x0,t0,t,atol,rtol,res,%DAEOPTIONS,hd); 
                else,
                 error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                end
              
              else,
                [x0,t0,t,rtol,atol,res,jac]=varargin(:)
                if (lhs==2),
                 [y,hd]=dassl(x0,t0,t,atol,rtol,res,jac,%DAEOPTIONS);
                elseif (lhs==1),
                 [y]=dassl(x0,t0,t,atol,rtol,res,jac,%DAEOPTIONS); 
                else,
                 error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
                end

              end

            else,
              [x0,t0,t,atol,res,jac,hd]=varargin(:)
              if (lhs==2),
               [y,hd]=dassl(x0,t0,t,atol,res,jac,%DAEOPTIONS,hd);
              elseif (lhs==1),
               [y]=dassl(x0,t0,t,atol,res,jac,%DAEOPTIONS,hd); 
              else,
               error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
              end
            end

        else,
          error(sprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dae",4,8));
        end
        
     if (%DAEOPTIONS(2)==0),   
        [r,c]=size(y);
        y=y([2:1:r],:);
     end
     if (lhs==2),
        varargout=list(y,hd);
     elseif (lhs==1),
        varargout=list(y);
     else,
        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",1,2));
     end       
   
      elseif (varargin(1)=="root"), // Case root (dasrt)
        [lhs,rhs]=argn();

        if (rhs==7),// Call without optional arguments
          [typ,x0,t0,t,res,ng,surface]=varargin(:)
          if (lhs==2),
           [y,nn]=dasrt(x0,t0,t,res,ng,surface,%DAEOPTIONS);
          elseif (lhs==3),
           [y,nn,hd]=dasrt(x0,t0,t,res,ng,surface,%DAEOPTIONS); 
          else,
           error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
          end

        elseif (rhs==11),// Call with all the optional arguments
           [typ,x0,t0,t,rtol,atol,res,jac,ng,surface,hd]=varargin(:)
           if (lhs==2),
            [y,nn]=dasrt(x0,t0,t,atol,rtol,res,jac,ng,surface,%DAEOPTIONS,hd);
           elseif (lhs==3),
            [y,nn,hd]=dasrt(x0,t0,t,atol,rtol,res,jac,ng,surface,%DAEOPTIONS,hd); 
           else,
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
           end

        elseif (rhs==8),

          if (type(varargin(5))==1),
           [typ,x0,t0,t,atol,res,ng,surface]=varargin(:)
           if (lhs==2),
            [y,nn]=dasrt(x0,t0,t,atol,res,ng,surface,%DAEOPTIONS);
           elseif (lhs==3),
            [y,nn,hd]=dasrt(x0,t0,t,atol,res,ng,surface,%DAEOPTIONS);
           else,
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
           end

          else,

            if (type(varargin(8))==1),
             [typ,x0,t0,t,res,ng,surface,hd]=varargin(:)
             if (lhs==2),
              [y,nn]=dasrt(x0,t0,t,res,ng,surface,%DAEOPTIONS,hd);
             elseif (lhs==3),
              [y,nn,hd]=dasrt(x0,t0,t,res,ng,surface,%DAEOPTIONS,hd); 
             else,
              error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
             end
           
            else,
             [typ,x0,t0,t,res,jac,ng,surface]=varargin(:)
             if (lhs==2),
              [y,nn]=dasrt(x0,t0,t,res,jac,ng,surface,%DAEOPTIONS);
             elseif (lhs==3),
              [y,nn,hd]=dasrt(x0,t0,t,res,jac,ng,surface,%DAEOPTIONS);
             else,
              error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
             end

            end

          end
     
        elseif (rhs==9),

          if (type(varargin(5))==1),

            if (type(varargin(6))==1),
             [typ,x0,t0,t,rtol,atol,res,ng,surface]=varargin(:)
             if (lhs==2),
              [y,nn]=dasrt(x0,t0,t,atol,rtol,res,ng,surface,%DAEOPTIONS);
             elseif (lhs==3),
              [y,nn,hd]=dasrt(x0,t0,t,atol,rtol,res,ng,surface,%DAEOPTIONS); 
             else
              error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
             end

            else,

                if (type(varargin(9))==1),
                 [typ,x0,t0,t,atol,res,ng,surface,hd]=varargin(:)
                 if (lhs==2),
                  [y,nn]=dasrt(x0,t0,t,atol,res,ng,surface,%DAEOPTIONS,hd);
                 elseif (lhs==3),
                  [y,nn,hd]=dasrt(x0,t0,t,atol,res,ng,surface,%DAEOPTIONS,hd); 
                 else
                  error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
                 end

                else,
                 [typ,x0,t0,t,atol,res,jac,ng,surface]=varargin(:)
                 if (lhs==2),
                  [y,nn]=dasrt(x0,t0,t,atol,res,jac,ng,surface,%DAEOPTIONS);
                 elseif (lhs==3),
                  [y,nn,hd]=dasrt(x0,t0,t,atol,res,jac,ng,surface,%DAEOPTIONS); 
                 else
                  error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
                 end

                end
              end

          else,
               [typ,x0,t0,t,res,jac,ng,surface,hd]=varargin(:)
               if (lhs==2),
                [y,nn]=dasrt(x0,t0,t,res,jac,ng,surface,%DAEOPTIONS,hd);
               elseif (lhs==3),
                [y,nn,hd]=dasrt(x0,t0,t,res,jac,ng,surface,%DAEOPTIONS,hd);
               else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
               end
         end

        elseif (rhs==10),

          if (type(varargin(5))==1),

            if (type(varargin(6))==1),

                if (type(varargin(10))==1),
                 [typ,x0,t0,t,rtol,atol,res,ng,surface,hd]=varargin(:)
                 if (lhs==2),
                  [y,nn]=dasrt(x0,t0,t,atol,rtol,res,ng,surface,%DAEOPTIONS,hd);
                 elseif (lhs==3),
                  [y,nn,hd]=dasrt(x0,t0,t,atol,rtol,res,ng,surface,%DAEOPTIONS,hd); 
                 else
                  error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
                 end

                else,
                  [typ,x0,t0,t,rtol,atol,res,jac,ng,surface]=varargin(:)
                  if (lhs==2),
                    [y,nn]=dasrt(x0,t0,t,atol,rtol,res,jac,ng,surface,%DAEOPTIONS);
                  elseif (lhs==3),
                    [y,nn,hd]=dasrt(x0,t0,t,atol,rtol,res,jac,ng,surface,%DAEOPTIONS);
                  else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
                  end
               end

            else,
                 [typ,x0,t0,t,atol,res,jac,ng,surface,hd]=varargin(:)
                 if (lhs==2),
                  [y,nn]=dasrt(x0,t0,t,atol,res,jac,ng,surface,%DAEOPTIONS,hd);
                 elseif (lhs==3),
                  [y,nn,hd]=dasrt(x0,t0,t,atol,res,jac,ng,surface,%DAEOPTIONS,hd); 
                 else
                  error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
                 end
 
              end
   
         else,
           [typ,x0,t0,t,res,jac,ng,surface,hd]=varargin(:)
           if (lhs==2),
            [y,nn]=dasrt(x0,t0,t,res,jac,ng,surface,%DAEOPTIONS,hd); 
           elseif (lhs==3),
            [y,nn,hd]=dasrt(x0,t0,t,res,jac,ng,surface,%DAEOPTIONS,hd); 
           else
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
           end
         end

       else,
         error(sprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dae",7,11));
     end
     
     if (%DAEOPTIONS(2)==0),
        [r,c]=size(y);
        y=y([2:1:r],:);
     end
     if (lhs==2),
        varargout=list(y,nn);
     elseif (lhs==3),
        varargout=list(y,nn,hd);
     else,
        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae",2,3));
     end       
     else
        error(sprintf(gettext("%s: Invalid option %s: real matrix expected.\n"),"dae","root"));
     end
     
endfunction
