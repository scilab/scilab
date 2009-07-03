/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
//=============================================================================
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
//=============================================================================
namespace DotNetScilab
{
    public class Scilab
    {
        //=============================================================================
        private const string CALL_SCILAB_DLL = "call_scilab.dll";
        private const string LIBSCILAB_DLL = "libscilab.dll";
        //private bool initialized = false;
        //=============================================================================
        /// <summary>
        /// Constructor, initialize scilab engine.
        /// </summary>
        public Scilab()
        {
            // start Scilab engine
            StartScilab(null, null, null);
            // Disable TCL/TK and Java graphic interfaces
            DisableInteractiveMode();
        }
        //=============================================================================
        /// <summary>
        /// Terminate Scilab engine.
        /// </summary>
        /// <returns>1 if it is correctly finished</returns>
        public int finish()
        {
            int ierr = TerminateScilab(null);
            return ierr;
        }
        //=============================================================================
        /// <summary>
        /// Send a job to scilab
        /// </summary>
        /// <param name="command">command to send to scilab</param>
        /// <returns>error code operation, 0 : OK</returns>
        public int sendScilabJob(string command)
        {
            return SendScilabJob(command);
        }
        //=============================================================================
        /// <summary>
        /// Write a named matrix of double in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <param name="iRows"> Number of row</param>
        /// <param name="iCols"> Number of column</param>
        /// <param name="matrixDouble"> pointer on data</param>
        /// <returns> if the operation successes (0) or not ( !0 )</returns>
        public int setNamedMatrixOfDouble(string matrixName, int iRows, int iCols, double[] matrixDouble)
        {
            return createNamedMatrixOfDouble(matrixName, iRows, iCols, matrixDouble);
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of double from Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of double from scilab. If Variable name does not exist returns null</returns>
        public unsafe double[] getNamedMatrixOfDouble(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;

            double[] matrixDouble = new double[0];
            // first , we want to known dimensions iRows and iCols
            readNamedMatrixOfDouble(matrixName, &iRows , &iCols, matrixDouble);
            // we allocate matrixDouble array
            if (iRows * iCols > 0)
            {
                matrixDouble = new double[iRows * iCols];

                // get values in matrixDouble
                readNamedMatrixOfDouble(matrixName, &iRows, &iCols, matrixDouble);

                return matrixDouble;
            }
            return null;
        }
        //=============================================================================
        /// <summary>
        /// Get dimensions of a named matrix of double in scilab
        /// </summary>
        /// <returns>a int array. if variable name does not exist dimensions are 0 0 </returns>
        public unsafe int[] getNamedMatrixOfDoubleDimensions(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;
            int[] iDim = new int[2];

            readNamedMatrixOfDouble(matrixName, &iRows, &iCols, null);

            iDim[0] = iRows;
            iDim[1] = iCols;

            return iDim;
        }
        //=============================================================================
        /// <summary>
        /// Write a named matrix of string in scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <param name="iRows"> Number of row</param>
        /// <param name="iCols"> Number of column</param>
        /// <param name="matrixDouble"> pointer on data</param>
        /// <returns> if the operation successes (0) or not ( !0 )</returns>
        public int setNamedMatrixOfString(string matrixName, int iRows, int iCols, string[] matrixString)
        {
            return createNamedMatrixOfString(matrixName, iRows, iCols, matrixString);
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of string from scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of string from scilab. If Variable name does not exist returns null</returns>
        public unsafe string[] getNamedMatrixOfString(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;

            // first , we want to known dimensions iRows and iCols
            readNamedMatrixOfString(matrixName, &iRows, &iCols, null, null);

            // we allocate lengthmatrixString
            int[] lengthmatrixString = new int[iRows * iCols];
            // we get length of strings
            readNamedMatrixOfString(matrixName, &iRows, &iCols, lengthmatrixString, null);

            // we allocate each string
            string[] matrixString = new string[iRows * iCols];
            for (int i = 0; i < iRows * iCols; i++)
            {
                matrixString[i] = new string(' ',lengthmatrixString[i]);
            }
            // we get strings from scilab
            readNamedMatrixOfString(matrixName, &iRows, &iCols, lengthmatrixString, matrixString);

            return matrixString;
        }
        //=============================================================================
        /// <summary>
        /// Get dimensions of a named matrix of string in scilab
        /// </summary>
        /// <returns>a int array. if variable name does not exist dimensions are 0 0 </returns>
        public unsafe int[] getNamedMatrixOfStringDimensions(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;
            int[] iDim = new int[2];

            readNamedMatrixOfString(matrixName, &iRows, &iCols, null, null);

            iDim[0] = iRows;
            iDim[1] = iCols;

            return iDim;
        }
        //=============================================================================
        /// <summary>
        /// import SendScilabJob from C (see CallScilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int SendScilabJob([In]String job);
        //=============================================================================
        /// <summary>
        /// import StartScilab from C (see CallScilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int StartScilab([In] String SCIpath,
                                              [In] String ScilabStartup,
                                              [In] Int32[] Stacksize);
        //=============================================================================
        /// <summary>
        /// import TerminateScilab from C (see CallScilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int TerminateScilab([In] String ScilabQuit);
        //=============================================================================
        /// <summary>
        /// import DisableInteractiveMode from C (see CallScilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern void DisableInteractiveMode();
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfString from C (see api_string.h)
        /// </summary>
        [DllImport(LIBSCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int createNamedMatrixOfString([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(LIBSCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int createNamedMatrixOfDouble([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfString from C (see api_string.h)
        /// </summary>
        [DllImport(LIBSCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedMatrixOfString([In] String _pstName,
                                                          [Out]  Int32* _piRows, [Out]  Int32* _piCols,
                                                          [In, Out] int[] _piLength,
                                                          [In, Out] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(LIBSCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedMatrixOfDouble([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] Double[] _pdblReal);
        //=============================================================================
    }
}
//=============================================================================
