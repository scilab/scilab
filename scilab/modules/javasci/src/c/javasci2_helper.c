/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include <string.h>

#define ENABLE_HELPERS
#include "javasci2_helper.h"
#include "api_scilab.h"
#include "lasterror.h"

BOOL isComplexVar(char *variableName) {
    int iComplex = isNamedVarComplex(pvApiCtx, variableName);

    return iComplex != 0; /* 0 = not complex */
}

sci_int_types getIntegerPrecision(char* variableName) {
    SciErr sciErr;

    int iPrec;

    sciErr = getNamedMatrixOfIntegerPrecision(pvApiCtx, variableName, &iPrec);
    if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
    return iPrec;
}

double * getDouble(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    double * matrixOfDouble = NULL;
    sciErr = readNamedMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfDouble=(double*)malloc(((*nbRow)*(*nbCol))*sizeof(double));

    /* Load the matrix */
    sciErr = readNamedMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol, matrixOfDouble);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfDouble;

}


int putDouble(char* variableName, double *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfDouble(pvApiCtx,variableName,nbRow,nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


double * getDoubleComplexReal(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    double * matrixOfDoubleComplexReal = NULL;
    double * matrixOfDoubleComplexImg = NULL;
    sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol , NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfDoubleComplexReal=(double*)malloc(((*nbRow)*(*nbCol))*sizeof(double));
    matrixOfDoubleComplexImg=(double*)malloc(((*nbRow)*(*nbCol))*sizeof(double));

    /* Load the matrix */
    sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol, matrixOfDoubleComplexReal, matrixOfDoubleComplexImg);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfDoubleComplexReal;
}

double * getDoubleComplexImg(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    double * matrixOfDoubleComplexReal = NULL;
    double * matrixOfDoubleComplexImg = NULL;
    sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol , NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfDoubleComplexReal=(double*)malloc(((*nbRow)*(*nbCol))*sizeof(double));
    matrixOfDoubleComplexImg=(double*)malloc(((*nbRow)*(*nbCol))*sizeof(double));

    /* Load the matrix */
    sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol, matrixOfDoubleComplexReal, matrixOfDoubleComplexImg);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfDoubleComplexImg;

}

int putDoubleComplex(char* variableName, double *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    double *variableImg=variable+((nbRow)*(nbCol));
    sciErr = createNamedComplexMatrixOfDouble(pvApiCtx,variableName,nbRow,nbCol, variable, variableImg);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


BOOL * getBoolean(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    BOOL * matrixOfBoolean = NULL;
    sciErr = readNamedMatrixOfBoolean(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfBoolean=(BOOL*)malloc(((*nbRow)*(*nbCol))*sizeof(BOOL));

    /* Load the matrix */
    sciErr = readNamedMatrixOfBoolean(pvApiCtx, variableName, nbRow, nbCol, matrixOfBoolean);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfBoolean;

}


int putBoolean(char* variableName, BOOL *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfBoolean(pvApiCtx,variableName,nbRow,nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;


}



///////////////////// byte / int8
byte * getByte(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    byte * matrixOfByte = NULL;
    sciErr = readNamedMatrixOfInteger8(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfByte=(byte*)malloc(((*nbRow)*(*nbCol))*sizeof(byte));

    /* Load the matrix */
    sciErr = readNamedMatrixOfInteger8(pvApiCtx, variableName, nbRow, nbCol, matrixOfByte);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfByte;

}


int putByte(char* variableName, byte *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfInteger8(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


byte * getUnsignedByte(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    byte * matrixOfByte = NULL;
    sciErr = readNamedMatrixOfUnsignedInteger8(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfByte=(byte*)malloc(((*nbRow)*(*nbCol))*sizeof(byte));

    /* Load the matrix */
    sciErr = readNamedMatrixOfUnsignedInteger8(pvApiCtx, variableName, nbRow, nbCol, matrixOfByte);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfByte;

}


int putUnsignedByte(char* variableName, byte *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfUnsignedInteger8(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


//////////////////////////// short / int16

short * getShort(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    short * matrixOfShort = NULL;
    sciErr = readNamedMatrixOfInteger16(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfShort=(short*)malloc(((*nbRow)*(*nbCol))*sizeof(short));

    /* Load the matrix */
    sciErr = readNamedMatrixOfInteger16(pvApiCtx, variableName, nbRow, nbCol, matrixOfShort);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfShort;

}


int putShort(char* variableName, short *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfInteger16(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


unsigned short * getUnsignedShort(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    unsigned short * matrixOfShort = NULL;

    sciErr = readNamedMatrixOfUnsignedInteger16(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfShort=(unsigned short*)malloc(((*nbRow)*(*nbCol))*sizeof(unsigned short));

    /* Load the matrix */
    sciErr = readNamedMatrixOfUnsignedInteger16(pvApiCtx, variableName, nbRow, nbCol, matrixOfShort);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfShort;

}


int putUnsignedShort(char* variableName, unsigned short *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfUnsignedInteger16(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


////////////////////// int / int32

int * getInt(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    int * matrixOfInt = NULL;
    sciErr = readNamedMatrixOfInteger32(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfInt=(int*)malloc(((*nbRow)*(*nbCol))*sizeof(int));

    /* Load the matrix */
    sciErr = readNamedMatrixOfInteger32(pvApiCtx, variableName, nbRow, nbCol, matrixOfInt);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfInt;

}


int putInt(char* variableName, int *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfInteger32(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


unsigned int * getUnsignedInt(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    int * matrixOfInt = NULL;
    sciErr = readNamedMatrixOfUnsignedInteger32(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfInt=(int*)malloc(((*nbRow)*(*nbCol))*sizeof(int));

    /* Load the matrix */
    sciErr = readNamedMatrixOfUnsignedInteger32(pvApiCtx, variableName, nbRow, nbCol, matrixOfInt);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfInt;

}


int putUnsignedInt(char* variableName, unsigned int *variable, int nbRow, int nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfUnsignedInteger32(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


////////////////////// long / int64
#ifdef __SCILAB_INT64__

long * getLong(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    long * matrixOfLong = NULL;
    sciErr = readNamedMatrixOfInteger64(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfLong=(long*)malloc(((*nbRow)*(*nbCol))*sizeof(long));

    /* Load the matrix */
    sciErr = readNamedMatrixOfInteger64(pvApiCtx, variableName, nbRow, nbCol, matrixOfLong);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfLong;

}


int putLong(char* variableName, long *variable, int *nbRow, int *nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfInteger64(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


unsigned long * getUnsignedLong(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    long * matrixOfLong = NULL;
    sciErr = readNamedMatrixOfUnsignedInteger64(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfLong=(long*)malloc(((*nbRow)*(*nbCol))*sizeof(long));

    /* Load the matrix */
    sciErr = readNamedMatrixOfUnsignedInteger64(pvApiCtx, variableName, nbRow, nbCol, matrixOfLong);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfLong;

}


int putUnsignedLong(char* variableName, unsigned long *variable, int *nbRow, int *nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfUnsignedInteger64(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}


#endif


/**
 * Call the Scilab function getLastErrorMessage
 * Take the result (a matrix of string) and concatenate into a single string
 * This is way easier to manage in swig.
*/
char* getLastErrorMessageSingle(void) {
    int iNbLines, i, nbChar=0;
    const char **msgs = getLastErrorMessage(&iNbLines);
    char *concat;

    for (i=0; i<iNbLines; i++)
    {
        nbChar += (int)strlen(msgs[i]);
    }
    concat = (char*)malloc((nbChar+1)*sizeof(char));
    strcpy(concat,"");
    for (i=0; i<iNbLines; i++)
    {
        strcat(concat, msgs[i]);
    }
    return concat;
}



char ** getString(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    int i = 0;

    int* piLen      = NULL;
    char** pstData  = NULL;

    //fisrt call to retrieve dimensions
    sciErr = readNamedMatrixOfString(pvApiCtx,variableName,nbRow, nbCol, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    piLen = (int*)malloc(sizeof(int) * (*nbRow)*(*nbCol));

    //second call to retrieve length of each string
    sciErr = readNamedMatrixOfString(pvApiCtx,variableName, nbRow, nbCol, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    pstData = (char**)malloc(sizeof(char*) * (*nbRow) * (*nbCol));

    for(i = 0 ; i < (*nbRow) * (*nbCol) ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = readNamedMatrixOfString(pvApiCtx, variableName, nbRow, nbCol, piLen, pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return pstData;

}


int putString(char* variableName, char **variable, int nbRow, int nbCol) {
    SciErr sciErr;

    sciErr = createNamedMatrixOfString(pvApiCtx, variableName, nbRow, nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;
}

BOOL isExistingVariable(char* variableName) {
    int iExisting = isNamedVarExist(pvApiCtx, variableName);

    return iExisting != 0; /* 0 = not existing variable */
}
