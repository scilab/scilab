//  
//  builder.sce file to build the scispt toolbox
//  Bruno Pincon (some modifs from Antonio Frasson)
//

//---variable part to customize---------------------//

UMF_INC_DIR = "/usr/local/include/ufsparse/";  // the path until umfpack.h
UMF_LIB_DIR = "/usr/local/lib/";  // the path until libumfpack.a
AMD_LIB_DIR = "/usr/local/lib/"  // the path until libamd.a 

COMPILER_PLUS_FLAG_1 = "gcc -c -O";  // interface routines
COMPILER_PLUS_FLAG_2 = "gcc -c -O3"; // for the taucs snmf stuff   

SCILAB_ATLAS = %t ; // %t if scilab compiled with atlas
                    // %f if scilab classic
UMF_WITH_CBLAS = %f; // %t if umfpack compiled with the C blas interface            
                     // %f if not

// set the following only if UMF_WITH_CBLAS = %t;
ATLAS_BLAS_LIB_DIR = "/usr/local/lib/"; 


//---don't edit below (normally !)------------------//

// blas lib files involved (used only when CASE22 == %t)
BLAS_LIB = [ "libcblas.a"  ;...  // C blas interface
             "libatlas.a" ];     // atlas core

// build makefile
fd=mopen("Makefile","w");
  mfprintf(fd,"SCIDIRINC = %s\n",SCI+"/routines");
  mfprintf(fd,"UMFDIRINC = %s\n",UMF_INC_DIR);
  mfprintf(fd,"CC1 = %s\n",COMPILER_PLUS_FLAG_1);
  mfprintf(fd,"CC2 = %s\n",COMPILER_PLUS_FLAG_2);
  mfprintf(fd,"all: intscispt.o taucs_scilab.o\n");
  mfprintf(fd,"intscispt.o: intscispt.c \n");
  mfprintf(fd,"\t$(CC1) intscispt.c  -I${UMFDIRINC} -I${SCIDIRINC}\n");
  mfprintf(fd,"taucs_scilab.o: taucs_scilab.c \n");
  mfprintf(fd,"\t$(CC2) taucs_scilab.c -I${SCIDIRINC}\n");
  mfprintf(fd,"clean:\n");
  mfprintf(fd,"\trm *.o loader_inc.sce\n");
mclose(fd);

// compile the interface
unix("make");

// build loader_inc.sce
fd=mopen("loader_inc.sce","w");
if UMF_WITH_CBLAS then
   if SCILAB_ATLAS then
      mfprintf(fd,"objects = [ path+\""intscispt.o\"" ; \n")
      mfprintf(fd,"            path+\""taucs_scilab.o\"" ; \n")
      mfprintf(fd,"            \""%s\"" ;\n",UMF_LIB_DIR+"libumfpack.a")
      mfprintf(fd,"            \""%s\"" ;\n",AMD_LIB_DIR+"libamd.a")      
      mfprintf(fd,"            \""%s\"" ];\n", ATLAS_BLAS_LIB_DIR+BLAS_LIB(1))
   else
      mfprintf(fd,"objects = [ path+\""intscispt.o\"" ; \n")
      mfprintf(fd,"            path+\""taucs_scilab.o\"" ; \n")
      mfprintf(fd,"            \""%s\"" ;\n",UMF_LIB_DIR+"libumfpack.a")
      mfprintf(fd,"            \""%s\"" ;\n",AMD_LIB_DIR+"libamd.a")            
      mfprintf(fd,"            \""%s\"" ;\n", ATLAS_BLAS_LIB_DIR+BLAS_LIB(1))
      mfprintf(fd,"            \""%s\"" ];\n",ATLAS_BLAS_LIB_DIR+BLAS_LIB(2))
   end
else
   mfprintf(fd,"objects = [ path+\""intscispt.o\"" ; \n")
   mfprintf(fd,"            path+\""taucs_scilab.o\"" ; \n")
   mfprintf(fd,"            \""%s\"" ;\n",UMF_LIB_DIR+"libumfpack.a")
   mfprintf(fd,"            \""%s\"" ];\n",AMD_LIB_DIR+"libamd.a")
end
mclose(fd);
