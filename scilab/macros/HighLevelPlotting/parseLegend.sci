function [mat,nbProcessed,typeOfLegend]=parseLegend(fun,parseList,nb)

mat=[];
typeOfLegend=1;
nbProcessed=0;
   
while length(parseList)
   if type(parseList(1))==10
      if size(mat,1)<nb
         mat=[mat;parseList(1)];
         parseList(1)=null();
	 nbProcessed=nbProcessed+1;
      else
         break;
      end
   elseif type(parseList(1))==1
      if length(parseList(1))==1
         if  parseList(1)>=-1 & parseList(1)<=5
            typeOfLegend=parseList(1);
	    parseList(1)=null();
	    nbProcessed=nbProcessed+1;
	    break;
         else
             error('legend : legend style must be a scalar between -1 and 5');
         end
       else
          break;
       end
   end
end
