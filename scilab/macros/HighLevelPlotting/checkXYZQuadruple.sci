function [xout,yout,zout,paramFlag]=checkXYZQuadruple(typeOfPlot,xin,yin,zin,cin,extra)

global globalSurf currentSurfColor

paramFlag=%F;

[lhs,rhs]=argn(0);

if typeOfPlot == 'plot3'

   if or(size(xin) ~= size(yin)) | or(size(xin)~= size(zin))
      error('plot3 : matrices must be the same size');
   end 
   if or(size(xin)==1)
      xout=xin(:);
      yout=yin(:);
      zout=zin(:);
   else
      xout=xin;
      yout=yin;
      zout=zin;   
   end
   return
end



if type(zin)==13 // interesting case, zin is a function
      
   if and(size(xin)~=1) |  and(size(yin)~=1) // else X and Y must be *vectors*
      error(sprintf('%s : X and Y must be vectors',typeOfPlot));
   end

   xin=xin(:); // to ensure that xin and yin are column vectors
   yin=yin(:);
   
   [nArgOut1,vectInput]=check3dFun(typeOfPlot,zin,xin,yin);
   if ~vectInput | (nArgOut1~=1 & nArgOut1~=3)
      error(sprintf('%s : function must return 1 or 3 output arguments and accept vector inputs',typeOfPlot));
   end
   
   paramHeader='[x,y,z]=zout(u,v)';
   paramCode='[x,y,z]=globalSurf(u,v);';
   paramFlag=%T;
   
   if nArgOut1==1
      paramHeader='z=zout(u,v)';
      paramCode='z=globalSurf(u,v);';
      xin=xin($:-1:1);
      paramFlag=%F;
   end
      
   if rhs==5 // There is a fourth argument for the color
    if type(cin)==13 // It has to be a function
      [nArgOut2,vectInput]=check3dFun(typeOfPlot,cin,xin,yin);
      if ~vectInput | (nArgOut2~=1)
         error(sprintf('%s : color fun. must return 1 out. arguments and accept vect. inputs',typeOfPlot));
      end
      currentSurfColor=cin;
      paramCode=paramCode+'z=z+%i*currentSurfColor(u,v)';
     else
         error(sprintf('%s : color spec must be also a function',typeOfPlot));
     end       
   end

   // now we define the function that will be returned.
   
   globalSurf=zin;
   deff(paramHeader,['global globalSurf currentSurfColor';paramCode]);
   xout=xin; yout=yin;
      
elseif type(zin)==1 // 'classical' (Matlab style) case, zin is a matrix

   if typeOfPlot=='trisurf' |...
      typeOfPlot=='trimesh' |...
	  typeOfPlot=='trisurfl' |...
	  typeOfPlot=='tripcolor' |...
	  typeOfPlot=='triplot'
	  
      paramFlag = xin;
      xout=yin;
      yout=zin;
   
      if typeOfPlot=='triplot'
        zout=[];
	  	return
	  end

      zout=cin;

      if rhs==6 // there is a color spec
         if typeOfPlot=='trisurf'
            if type(extra)==1 // the color spec is a matrix
              if and(size(extra)==size(zout))  
                zout=zout+%i*extra;
	      else
	        error(sprintf('%s : color spec must be %d x %d',typeOfPlot,size(zout,1),size(zout,2)));
	      end
            elseif type(extra)==13 // the color spec is a function
	        zout=zout+%i*extra(xout,yout,zout);
            else  
              error(sprintf('%s : color spec must be either a matrix or a function',typeOfPlot));
            end
         end
      end
      return;
   end

   paramFlag=%T;
   if or(size(xin)==1) &  or(size(xin)==1) // X and Y are vectors (non-parametric case)
      xout=xin(:);
      yout=yin(:);
      if length(xout)~=size(zin,2) | length(yout)~=size(zin,1)
         error(sprintf('%s : Z matrix must be %d x %d',typeOfPlot,length(yout),length(xout)));
      end
      paramFlag=%F;
   elseif or(size(xin)~=size(yin)) & or(size(xin)~=size(zin)) //if X,Y,Z do not have the same size either
      error(sprintf('%s : X, Y and Z have incompatible dimensions',typeOfPlot));
   end
   
   zout=zin;
   
   if rhs==5 
      if type(cin)==1 // the color spec is a matrix
         if and(size(cin)==size(zin))  
            zout=zout+%i*cin;
	 else
	    error(sprintf('%s : color spec must be %d x %d',typeOfPlot,size(zout,1),size(zout,2)));
	 end
      else
         error(sprintf('%s : color spec must be a matrix',typeOfPlot));
      end
   end
if paramFlag
   xout=xin;
   yout=yin;
else
   zout=zout.'; // remember zin was passed in the 'matlab style' (rows=Y, cols=X)
end
end
