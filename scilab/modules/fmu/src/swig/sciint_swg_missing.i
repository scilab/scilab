/* --------------------------------------------------------------------------
 *
 *  Missing typemaps on SWIG
 *
 * --------------------------------------------------------------------------*/

%fragment("SWIG_SciInt32_FromIntArrayAndSize", "header") {
SWIGINTERN int
SWIG_SciInt32_FromIntArrayAndSize(void *pvApiCtx, int iVarOut, int iRows, int iCols, const int *piData) {
  SciErr sciErr;

  sciErr = createMatrixOfInteger32(pvApiCtx, SWIG_NbInputArgument(pvApiCtx) + iVarOut, iRows, iCols, piData);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return SWIG_ERROR;
  }

  return SWIG_OK;
}
}
