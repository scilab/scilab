function matin = squeeze(matin)

// PURPOSE: Remove singleton dimensions, that is any dimension 
// for which the size of the input hypermatrix is 1; if the 
// input is a matrix, it is unaffected
// ------------------------------------------------------------
// INPUT:
// * matin = a hypermatrix or a matrix of constant type
// ------------------------------------------------------------
// OUTPUT: 
// * matout = a hypermatrix or a matrix of constant type
// ------------------------------------------------------------
// Written by  Eric Dubois 2002-2007
// Modified Jean-Baptiste Silvy 2007


typein=typeof(matin)

select typein
case 'hypermat' then
  newDims = matin.dims( find( matin.dims <> 1 ) ) ;
  if newDims == [] then
    matin.dims = 1 ;
  else
    matin.dims = newDims ;
  end
case 'constant'
  // it is a standard matrix nothing to do
else 
  error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),'squeeze',1))
end

endfunction
