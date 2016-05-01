/*
 *  MATRIX SPALLOCATION MODULE
 *
 *  Author:                     Advising professor:
 *      Kenneth S. Kundert          Alberto Sangiovanni-Vincentelli
 *      UC Berkeley
 *
 *  This file contains the allocation and deallocation routines for the
 *  sparse matrix routines.
 *
 *  >>> User accessible functions contained in this file:
 *  spCreate
 *  spDestroy
 *  spError
 *  spWhereSingular
 *  spGetSize
 *  spSetReal
 *  spSetComplex
 *  spFillinCount
 *  spElementCount
 *
 *  >>> Other functions contained in this file:
 *  spcGetElement
 *  InitializeElementBlocks
 *  spcGetFillin
 *  RecordAllocation
 *  AllocateBlockOfAllocationList
 *  EnlargeMatrix
 *  ExpandTranslationArrays
 */


/*
 *  Revision and copyright information.
 *
 *  Copyright (c) 1985,86,87,88
 *  by Kenneth S. Kundert and the University of California.
 *
 *  Permission to use, copy, modify, and distribute this software and
 *  its documentation for any purpose and without fee is hereby granted,
 *  provided that the copyright notices appear in all copies and
 *  supporting documentation and that the authors and the University of
 *  California are properly credited.  The authors and the University of
 *  California make no representations as to the suitability of this
 *  software for any purpose.  It is provided `as is', without express
 *  or implied warranty.
 */



/*
 *  IMPORTS
 *
 *  >>> Import descriptions:
 *  spConfig.h
 *      Macros that customize the sparse matrix routines.
 *  spmatrix.h
 *      Macros and declarations to be imported by the user.
 *  spDefs.h
 *      Matrix type and macro definitions for the sparse matrix routines.
 */

#define spINSIDE_SPARSE
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"
#include "spmalloc.h"
#include "spAllocate.h"
#include "sci_malloc.h"

static int InitializeElementBlocks( MatrixPtr  Matrix, int InitialNumberOfElements, int NumberOfFillinsExpected );

static int RecordAllocation( MatrixPtr Matrix, char *AllocatedPtr );

static int AllocateBlockOfAllocationList( MatrixPtr Matrix );
/*
 *  MATRIX SPALLOCATION
 *
 *  Allocates and initializes the data structures associated with a matrix.
 *
 *  >>> Returned:
 *  A pointer to the matrix is returned cast into the form of a pointer to
 *  a character.  This pointer is then passed and used by the other matrix
 *  routines to refer to a particular matrix.  If an error occurs, the NULL
 *  pointer is returned.
 *
 *  >>> Arguments:
 *  Size  <input>  (int)
 *      Size of matrix or estimate of size of matrix if matrix is EXPANDABLE.
 *  Complex  <input>  (int)
 *      Type of matrix.  If Complex is 0 then the matrix is real, otherwise
 *      the matrix will be complex.  Note that if the routines are not set up
 *      to handle the type of matrix requested, then a spPANIC error will occur.
 *      Further note that if a matrix will be both real and complex, it must
 *      be specified here as being complex.
 *  pError  <output>  (int *)
 *      Returns error flag, needed because function spError() will not work
 *      correctly if spCreate() returns NULL.
 *
 *  >>> Local variables:
 *  AllocatedSize  (int)
 *      The size of the matrix being allocated.
 *  Matrix  (MatrixPtr)
 *      A pointer to the matrix frame being created.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 *  spPANIC
 *  Error is cleared in this routine.
 */

char *spCreate(int Size, SPBOOLEAN  Complex, int *pError )
{
    register  unsigned  SizePlusOne;
    register  MatrixPtr  Matrix;
    register  int  I;
    int  AllocatedSize;

    /* Begin `spCreate'. */
    /* Clear error flag. */
    *pError = spOKAY;

    /* Test for valid size. */
    if ((Size < 0) OR (Size == 0 AND NOT EXPANDABLE))
    {
        *pError = spPANIC;
        return NULL;
    }

    /* Test for valid type. */
#if NOT spCOMPLEX
    if (Complex)
    {
        *pError = spPANIC;
        return NULL;
    }
#endif
#if NOT REAL
    if (NOT Complex)
    {
        *pError = spPANIC;
        return NULL;
    }
#endif

    /* Create Matrix. */
    AllocatedSize = Max( Size, MINIMUM_ALLOCATED_SIZE );
    SizePlusOne = (unsigned)(AllocatedSize + 1);

    if ((Matrix = SPALLOC(struct MatrixFrame, 1)) == NULL)
    {
        *pError = spNO_MEMORY;
        return NULL;
    }

    /* Initialize matrix */
    Matrix->ID = SPARSE_ID;
    Matrix->Complex = Complex;
    Matrix->PreviousMatrixWasComplex = Complex;
    Matrix->Factored = NO;
    Matrix->Elements = 0;
    Matrix->Error = *pError;
    Matrix->Fillins = 0;
    Matrix->Reordered = NO;
    Matrix->NeedsOrdering = YES;
    Matrix->NumberOfInterchangesIsOdd = NO;
    Matrix->Partitioned = NO;
    Matrix->RowsLinked = NO;
    Matrix->InternalVectorsAllocated = NO;
    Matrix->SingularCol = 0;
    Matrix->SingularRow = 0;
    Matrix->Size = Size;
    Matrix->AllocatedSize = AllocatedSize;
    Matrix->ExtSize = Size;
    Matrix->AllocatedExtSize = AllocatedSize;
    Matrix->CurrentSize = 0;
    Matrix->ExtToIntColMap = NULL;
    Matrix->ExtToIntRowMap = NULL;
    Matrix->IntToExtColMap = NULL;
    Matrix->IntToExtRowMap = NULL;
    Matrix->MarkowitzRow = NULL;
    Matrix->MarkowitzCol = NULL;
    Matrix->MarkowitzProd = NULL;
    Matrix->DoCmplxDirect = NULL;
    Matrix->DoRealDirect = NULL;
    Matrix->Intermediate = NULL;
    Matrix->RelThreshold = DEFAULT_THRESHOLD;
    Matrix->AbsThreshold = 0.0;

    Matrix->TopOfAllocationList = NULL;
    Matrix->RecordsRemaining = 0;
    Matrix->ElementsRemaining = 0;
    Matrix->FillinsRemaining = 0;

    RecordAllocation( Matrix, (char *)Matrix );
    if (Matrix->Error == spNO_MEMORY)
    {
        goto MemoryError;
    }

    /* Take out the trash. */
    Matrix->TrashCan.Real = 0.0;
#if spCOMPLEX
    Matrix->TrashCan.Imag = 0.0;
#endif
    Matrix->TrashCan.Row = 0;
    Matrix->TrashCan.Col = 0;
    Matrix->TrashCan.NextInRow = NULL;
    Matrix->TrashCan.NextInCol = NULL;
#if INITIALIZE
    Matrix->TrashCan.pInitInfo = NULL;
#endif

    /* Allocate space in memory for Diag pointer vector. */
    SPCALLOC( Matrix->Diag, ElementPtr, SizePlusOne);
    if (Matrix->Diag == NULL)
    {
        goto MemoryError;
    }

    /* Allocate space in memory for FirstInCol pointer vector. */
    SPCALLOC( Matrix->FirstInCol, ElementPtr, SizePlusOne);
    if (Matrix->FirstInCol == NULL)
    {
        goto MemoryError;
    }

    /* Allocate space in memory for FirstInRow pointer vector. */
    SPCALLOC( Matrix->FirstInRow, ElementPtr, SizePlusOne);
    if (Matrix->FirstInRow == NULL)
    {
        goto MemoryError;
    }

    /* Allocate space in memory for IntToExtColMap vector. */
    if (( Matrix->IntToExtColMap = SPALLOC(int, SizePlusOne)) == NULL)
    {
        goto MemoryError;
    }

    /* Allocate space in memory for IntToExtRowMap vector. */
    if (( Matrix->IntToExtRowMap = SPALLOC(int, SizePlusOne)) == NULL)
    {
        goto MemoryError;
    }

    /* Initialize MapIntToExt vectors. */
    for (I = 1; I <= AllocatedSize; I++)
    {
        Matrix->IntToExtRowMap[I] = I;
        Matrix->IntToExtColMap[I] = I;
    }

#if TRANSLATE
    /* Allocate space in memory for ExtToIntColMap vector. */
    if (( Matrix->ExtToIntColMap = SPALLOC(int, SizePlusOne)) == NULL)
    {
        goto MemoryError;
    }

    /* Allocate space in memory for ExtToIntRowMap vector. */
    if (( Matrix->ExtToIntRowMap = SPALLOC(int, SizePlusOne)) == NULL)
    {
        goto MemoryError;
    }

    /* Initialize MapExtToInt vectors. */
    for (I = 1; I <= AllocatedSize; I++)
    {
        Matrix->ExtToIntColMap[I] = -1;
        Matrix->ExtToIntRowMap[I] = -1;
    }
    Matrix->ExtToIntColMap[0] = 0;
    Matrix->ExtToIntRowMap[0] = 0;
#endif

    /* Allocate space for fill-ins and initial set of elements. */
    InitializeElementBlocks( Matrix, SPACE_FOR_ELEMENTS * AllocatedSize,
                             SPACE_FOR_FILL_INS * AllocatedSize );
    if (Matrix->Error == spNO_MEMORY)
    {
        goto MemoryError;
    }

    return (char *)Matrix;

MemoryError:

    /* Deallocate matrix and return no pointer to matrix if there is not enough
       memory. */
    *pError = spNO_MEMORY;
    spDestroy( (char *)Matrix);
    return NULL;
}









/*
 *  ELEMENT SPALLOCATION
 *
 *  This routine allocates space for matrix elements. It requests large blocks
 *  of storage from the system and doles out individual elements as required.
 *  This technique, as opposed to allocating elements individually, tends to
 *  speed the allocation process.
 *
 *  >>> Returned:
 *  A pointer to an element.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (MatrixPtr)
 *      Pointer to matrix.
 *
 *  >>> Local variables:
 *  pElement  (ElementPtr)
 *      A pointer to the first element in the group of elements being allocated.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 */

ElementPtr spcGetElement( MatrixPtr Matrix )
{
    ElementPtr  pElement;

    /* Begin `spcGetElement'. */

    /* Allocate block of MatrixElements if necessary. */
    if (Matrix->ElementsRemaining == 0)
    {
        pElement = SPALLOC(struct MatrixElement, ELEMENTS_PER_ALLOCATION);
        RecordAllocation( Matrix, (char *)pElement );
        if (Matrix->Error == spNO_MEMORY)
        {
            return NULL;
        }
        Matrix->ElementsRemaining = ELEMENTS_PER_ALLOCATION;
        Matrix->NextAvailElement = pElement;
    }

    /* Update Element counter and return pointer to Element. */
    Matrix->ElementsRemaining--;
    return Matrix->NextAvailElement++;

}








/*
 *  ELEMENT SPALLOCATION INITIALIZATION
 *
 *  This routine allocates space for matrix fill-ins and an initial set of
 *  elements.  Besides being faster than allocating space for elements one
 *  at a time, it tends to keep the fill-ins physically close to the other
 *  matrix elements in the computer memory.  This keeps virtual memory paging
 *  to a minimum.
 *
 *  >>> Arguments:
 *  Matrix  <input>    (MatrixPtr)
 *      Pointer to the matrix.
 *  InitialNumberOfElements  <input> (int)
 *      This number is used as the size of the block of memory, in
 *      MatrixElements, reserved for elements. If more than this number of
 *      elements are generated, then more space is allocated later.
 *  NumberOfFillinsExpected  <input> (int)
 *      This number is used as the size of the block of memory, in
 *      MatrixElements, reserved for fill-ins. If more than this number of
 *      fill-ins are generated, then more space is allocated, but they may
 *      not be physically close in computer's memory.
 *
 *  >>> Local variables:
 *  pElement  (ElementPtr)
 *      A pointer to the first element in the group of elements being allocated.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 */

static int
InitializeElementBlocks( MatrixPtr  Matrix, int InitialNumberOfElements,
                         int NumberOfFillinsExpected )
{
    ElementPtr  pElement;

    /* Begin `InitializeElementBlocks'. */

    /* Allocate block of MatrixElements for elements. */
    pElement = SPALLOC(struct MatrixElement, InitialNumberOfElements);
    RecordAllocation( Matrix, (char *)pElement );
    if (Matrix->Error == spNO_MEMORY)
    {
        return 0;
    }
    Matrix->ElementsRemaining = InitialNumberOfElements;
    Matrix->NextAvailElement = pElement;

    /* Allocate block of MatrixElements for fill-ins. */
    pElement = SPALLOC(struct MatrixElement, NumberOfFillinsExpected);
    RecordAllocation( Matrix, (char *)pElement );
    if (Matrix->Error == spNO_MEMORY)
    {
        return 0;
    }
    Matrix->FillinsRemaining = NumberOfFillinsExpected;
    Matrix->NextAvailFillin = pElement;

    /* Allocate a fill-in list structure. */
    Matrix->FirstFillinListNode = SPALLOC(struct FillinListNodeStruct, 1);
    RecordAllocation( Matrix, (char *)Matrix->FirstFillinListNode );
    if (Matrix->Error == spNO_MEMORY)
    {
        return 0;
    }
    Matrix->LastFillinListNode = Matrix->FirstFillinListNode;

    Matrix->FirstFillinListNode->pFillinList = pElement;
    Matrix->FirstFillinListNode->NumberOfFillinsInList = NumberOfFillinsExpected;
    Matrix->FirstFillinListNode->Next = NULL;

    return 0;
}










/*
 *  FILL-IN SPALLOCATION
 *
 *  This routine allocates space for matrix fill-ins. It requests large blocks
 *  of storage from the system and doles out individual elements as required.
 *  This technique, as opposed to allocating elements individually, tends to
 *  speed the allocation process.
 *
 *  >>> Returned:
 *  A pointer to the fill-in.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (MatrixPtr)
 *      Pointer to matrix.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 */

ElementPtr spcGetFillin( MatrixPtr  Matrix )
{
    struct FillinListNodeStruct *pListNode;
    ElementPtr  pFillins;

    /* Begin `spcGetFillin'. */

#if NOT STRIP OR LINT
    if (Matrix->FillinsRemaining == 0)
    {
        return spcGetElement( Matrix );
    }
#endif
#if STRIP OR LINT

    if (Matrix->FillinsRemaining == 0)
    {
        pListNode = Matrix->LastFillinListNode;

        /* First see if there are any stripped fill-ins left. */
        if (pListNode->Next != NULL)
        {
            Matrix->LastFillinListNode = pListNode = pListNode->Next;
            Matrix->FillinsRemaining = pListNode->NumberOfFillinsInList;
            Matrix->NextAvailFillin = pListNode->pFillinList;
        }
        else
        {
            /* Allocate block of fill-ins. */
            pFillins = SPALLOC(struct MatrixElement, ELEMENTS_PER_ALLOCATION);
            RecordAllocation( Matrix, (char *)pFillins );
            if (Matrix->Error == spNO_MEMORY)
            {
                return NULL;
            }
            Matrix->FillinsRemaining = ELEMENTS_PER_ALLOCATION;
            Matrix->NextAvailFillin = pFillins;

            /* Allocate a fill-in list structure. */
            pListNode->Next = SPALLOC(struct FillinListNodeStruct, 1);
            RecordAllocation( Matrix, (char *)pListNode->Next );
            if (Matrix->Error == spNO_MEMORY)
            {
                return NULL;
            }
            Matrix->LastFillinListNode = pListNode = pListNode->Next;

            pListNode->pFillinList = pFillins;
            pListNode->NumberOfFillinsInList = ELEMENTS_PER_ALLOCATION;
            pListNode->Next = NULL;
        }
    }
#endif

    /* Update Fill-in counter and return pointer to Fill-in. */
    Matrix->FillinsRemaining--;
    return Matrix->NextAvailFillin++;
}









/*
 *  RECORD A MEMORY SPALLOCATION
 *
 *  This routine is used to record all memory allocations so that the memory
 *  can be freed later.
 *
 *  >>> Arguments:
 *  Matrix  <input>    (MatrixPtr)
 *      Pointer to the matrix.
 *  AllocatedPtr  <input>  (char *)
 *      The pointer returned by malloc or calloc.  These pointers are saved in
 *      a list so that they can be easily freed.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 */

static int
RecordAllocation( MatrixPtr Matrix, char *AllocatedPtr )
{
    /* Begin `RecordAllocation'. */
    /*
     * If Allocated pointer is NULL, assume that malloc returned a NULL pointer,
     * which indicates a spNO_MEMORY error.
     */
    if (AllocatedPtr == NULL)
    {
        Matrix->Error = spNO_MEMORY;
        return 0;
    }

    /* Allocate block of MatrixElements if necessary. */
    if (Matrix->RecordsRemaining == 0)
    {
        AllocateBlockOfAllocationList( Matrix );
        if (Matrix->Error == spNO_MEMORY)
        {
            SPFREE(AllocatedPtr);
            return 0;
        }
    }

    /* Add Allocated pointer to Allocation List. */
    (++Matrix->TopOfAllocationList)->AllocatedPtr = AllocatedPtr;
    Matrix->RecordsRemaining--;
    return 0;

}








/*
 *  ADD A BLOCK OF SLOTS TO SPALLOCATION LIST
 *
 *  This routine increases the size of the allocation list.
 *
 *  >>> Arguments:
 *  Matrix  <input>    (MatrixPtr)
 *      Pointer to the matrix.
 *
 *  >>> Local variables:
 *  ListPtr  (AllocationListPtr)
 *      Pointer to the list that contains the pointers to segments of memory
 *      that were allocated by the operating system for the current matrix.
 *
 *  >>> Possible errors:
 *  spNO_MEMORY
 */

static int
AllocateBlockOfAllocationList( MatrixPtr Matrix )
{
    register  int  I;
    register  AllocationListPtr  ListPtr;

    /* Begin `AllocateBlockOfAllocationList'. */
    /* Allocate block of records for allocation list. */
    ListPtr = SPALLOC(struct AllocationRecord, (ELEMENTS_PER_ALLOCATION + 1));
    if (ListPtr == NULL)
    {
        Matrix->Error = spNO_MEMORY;
        return 0;
    }

    /* String entries of allocation list into singly linked list.  List is linked
       such that any record points to the one before it. */

    ListPtr->NextRecord = Matrix->TopOfAllocationList;
    Matrix->TopOfAllocationList = ListPtr;
    ListPtr += ELEMENTS_PER_ALLOCATION;
    for (I = ELEMENTS_PER_ALLOCATION; I > 0; I--)
    {
        ListPtr->NextRecord = ListPtr - 1;
        ListPtr--;
    }

    /* Record allocation of space for allocation list on allocation list. */
    Matrix->TopOfAllocationList->AllocatedPtr = (char *)ListPtr;
    Matrix->RecordsRemaining = ELEMENTS_PER_ALLOCATION;

    return 0;
}








/*
 *  MATRIX DEALLOCATION
 *
 *  Deallocates pointers and elements of Matrix.
 *
 *  >>> Arguments:
 *  Matrix  <input>  (char *)
 *      Pointer to the matrix frame which is to be removed from memory.
 *
 *  >>> Local variables:
 *  ListPtr  (AllocationListPtr)
 *      Pointer into the linked list of pointers to allocated data structures.
 *      Points to pointer to structure to be freed.
 *  NextListPtr  (AllocationListPtr)
 *      Pointer into the linked list of pointers to allocated data structures.
 *      Points to the next pointer to structure to be freed.  This is needed
 *      because the data structure to be freed could include the current node
 *      in the allocation list.
 */

void spDestroy(register char *eMatrix)
{
    MatrixPtr Matrix = (MatrixPtr)eMatrix;
    register  AllocationListPtr  ListPtr, NextListPtr;


    /* Begin `spDestroy'. */
    ASSERT( IS_SPARSE( Matrix ) );

    /* Deallocate the vectors that are located in the matrix frame. */
    SPFREE( Matrix->IntToExtColMap );
    SPFREE( Matrix->IntToExtRowMap );
    SPFREE( Matrix->ExtToIntColMap );
    SPFREE( Matrix->ExtToIntRowMap );
    SPFREE( Matrix->Diag );
    SPFREE( Matrix->FirstInRow );
    SPFREE( Matrix->FirstInCol );
    SPFREE( Matrix->MarkowitzRow );
    SPFREE( Matrix->MarkowitzCol );
    SPFREE( Matrix->MarkowitzProd );
    SPFREE( Matrix->DoCmplxDirect );
    SPFREE( Matrix->DoRealDirect );
    SPFREE( Matrix->Intermediate );


    /* Sequentially step through the list of allocated pointers freeing pointers
     * along the way. */

    ListPtr = Matrix->TopOfAllocationList;
    while (ListPtr != NULL )
    {
        char *LocPtr;
        /* dans certain cas le pointeur ds la zone a desalouer
        se trouve lui meme ds la dite zone en fait quand
         ( ListPtr ==  ListPtr->AllocatedPtr )
         donc un free(x) suivit de x=0
         fait que l'on essaye d'ecrire ds une zone que l'on vient de desalouer
         ce qui plante sur linux
         fprintf(stderr,"Warning bad SPFREE\n");
         je regle le probleme en mettant a zero avant le free !
         */
        NextListPtr = ListPtr->NextRecord;
        /* BUGUED : SPFREE( ListPtr->AllocatedPtr) */
        LocPtr = ListPtr->AllocatedPtr;
        ListPtr->AllocatedPtr = NULL;
        if ( LocPtr != NULL)
        {
            FREE(LocPtr);
        }
        ListPtr = NextListPtr;
    }
    return;
}







/*
 *  RETURN MATRIX ERROR STATUS
 *
 *  This function is used to determine the error status of the given matrix.
 *
 *  >>> Returned:
 *      The error status of the given matrix.
 *
 *  >>> Arguments:
 *  eMatrix  <input>  (char *)
 *      The matrix for which the error status is desired.
 */

int spError( char  *eMatrix )
{
    /* Begin `spError'. */

    if (eMatrix != NULL)
    {
        ASSERT(((MatrixPtr)eMatrix)->ID == SPARSE_ID);
        return ((MatrixPtr)eMatrix)->Error;
    }
    else
    {
        return spNO_MEMORY;
    }   /* This error may actually be spPANIC,
                                * no way to tell. */
}









/*
 *  WHERE IS MATRIX SINGULAR
 *
 *  This function returns the row and column number where the matrix was
 *  detected as singular or where a zero was detected on the diagonal.
 *
 *  >>> Arguments:
 *  eMatrix  <input>  (char *)
 *      The matrix for which the error status is desired.
 *  pRow  <output>  (int *)
 *      The row number.
 *  pCol  <output>  (int *)
 *      The column number.
 */

void spWhereSingular( char *eMatrix, int *pRow, int *pCol )
{
    MatrixPtr Matrix = (MatrixPtr)eMatrix;

    /* Begin `spWhereSingular'. */
    ASSERT( IS_SPARSE( Matrix ) );

    if (Matrix->Error == spSINGULAR OR Matrix->Error == spZERO_DIAG)
    {
        *pRow = Matrix->SingularRow;
        *pCol = Matrix->SingularCol;
    }
    else
    {
        *pRow = *pCol = 0;
    }
    return;
}






/*
 *  MATRIX SIZE
 *
 *  Returns the size of the matrix.  Either the internal or external size of
 *  the matrix is returned.
 *
 *  >>> Arguments:
 *  eMatrix  <input>  (char *)
 *      Pointer to matrix.
 *  External  <input>  (SPBOOLEAN)
 *      If External is set true, the external size , i.e., the value of the
 *      largest external row or column number encountered is returned.
 *      Otherwise the true size of the matrix is returned.  These two sizes
 *      may differ if the TRANSLATE option is set true.
 */

int spGetSize(char  *eMatrix, SPBOOLEAN  External )
{
    MatrixPtr Matrix = (MatrixPtr)eMatrix;

    /* Begin `spGetSize'. */
    ASSERT( IS_SPARSE( Matrix ) );

#if TRANSLATE
    if (External)
    {
        return Matrix->ExtSize;
    }
    else
    {
        return Matrix->Size;
    }
#else
    return Matrix->Size;
#endif
}








/*
 *  SET MATRIX COMPLEX OR REAL
 *
 *  Forces matrix to be either real or complex.
 *
 *  >>> Arguments:
 *  eMatrix  <input>  (char *)
 *      Pointer to matrix.
 */

void spSetReal( char *eMatrix )
{
    /* Begin `spSetReal'. */

    ASSERT( IS_SPARSE( (MatrixPtr)eMatrix ) AND REAL);
    ((MatrixPtr)eMatrix)->Complex = NO;
    return;
}


void spSetComplex( char  *eMatrix )
{
    /* Begin `spSetComplex'. */

    ASSERT( IS_SPARSE( (MatrixPtr)eMatrix ) AND spCOMPLEX);
    ((MatrixPtr)eMatrix)->Complex = YES;
    return;
}









/*
 *  ELEMENT OR FILL-IN COUNT
 *
 *  Two functions used to return simple statistics.  Either the number
 *  of total elements, or the number of fill-ins can be returned.
 *
 *  >>> Arguments:
 *  eMatrix  <input>  (char *)
 *      Pointer to matrix.
 */

int spFillinCount( char *eMatrix )
{
    /* Begin `spFillinCount'. */

    ASSERT( IS_SPARSE( (MatrixPtr)eMatrix ) );
    return ((MatrixPtr)eMatrix)->Fillins;
}


int spElementCount( char  *eMatrix )
{
    /* Begin `spElementCount'. */

    ASSERT( IS_SPARSE( (MatrixPtr)eMatrix ) );
    return ((MatrixPtr)eMatrix)->Elements;
}
