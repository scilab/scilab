function [stk,txt,top]=func2sci(op,stk)
// translate all functions calls
//!parameters
// op : list describing the function call
//      op(1) : '25' function call code
//      op(2) : name of the called function
//      op(3) :number of actual right hand side argument of the function call
//      op(4) :number of actual left hand side argument of the function call

// Copyright INRIA
// txt : column vector of string
//       complementary scilab instructions needed to translate function call
// top : on input (from global context) the position of the la
//
//!
lhs=evstr(op(4)) 
rhs=max(evstr(op(3)) ,0)
txt=[]

if exists('sci_'+op(2))==1 then // a translation function exists
  execstr('[stkr,txt,top1]=sci_'+op(2)+'()')
  
elseif or(op(2)==same) then // identical syntax
  [stkr,txt,top1]=sci_gener(op(2))
elseif res_path==[] then
  [stkr,txt,top1]=sci_gener(op(2))
else
  sci_file=res_path+'sci_'+op(2)+'.sci'
  ierr=execstr('getf(sci_file)','errcatch')
  if ierr==0 then // a translation function exists
    execstr('[stkr,txt,top1]=sci_'+op(2)+'()')
  // no translation indication given
  elseif Recmode then
    // check if the m-file exists in the given paths
    path=mfile_path(op(2))
    if path==[] then 
      [stkr,txt,top1]=sci_gener(op(2))
    elseif or(op(2)==nametbl)
      [stkr,txt,top1]=sci_gener(op(2))
    else
      fnam=op(2)
      scipath=res_path+fnam+'.sci'
      scepath=res_path+fnam+'.sce'
      if newest(path,scipath,scepath)==1 then
	mfile2sci(path,res_path,%f,%t)
      end
      getf(sci_file)
      execstr('[stkr,txt,top1]=sci_'+op(2)+'()')
    end
  else  //default translation
    [stkr,txt,top1]=sci_gener(op(2))
  end
end
// add lhs expression to the stack
top=top-rhs
if lhs>1 then
  for k=1:lhs
    top=top+1
    stk(top)=stkr(k)
  end
else
  top=top+1
  stk(top)=stkr
end
if op(2)=='global' then
  [vnms,vtps]=resume(vnms,vtps)
end

