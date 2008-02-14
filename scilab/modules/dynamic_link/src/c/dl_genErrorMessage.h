/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
/**
 * Generates an scierror with the appropriate message
 *
 * @param fname name of the scilab function
 * @param errorCode the code of error
 * @param SharedLibraryName name of the library
 */
void dl_genErrorMessage(char* fname, int errorCode, char* SharedLibraryName);
/*--------------------------------------------------------------------------*/
