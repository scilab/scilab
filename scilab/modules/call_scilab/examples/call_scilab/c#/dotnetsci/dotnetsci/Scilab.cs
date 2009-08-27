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
using System.Threading;
//=============================================================================
namespace DotNetScilab
{
    public sealed class Scilab
    {
        //=============================================================================
        public enum ScilabType { 
        sci_matrix = 1 ,
        sci_poly = 2 ,
        sci_boolean = 4 ,
        sci_sparse = 5  ,
        sci_boolean_sparse = 6,
        sci_matlab_sparse = 7, /* matlab sparse matrix  */
        sci_ints = 8,
        sci_handles = 9,
        sci_strings = 10,
        sci_u_function = 11,
        sci_c_function = 13 ,
        sci_lib =  14,
        sci_list = 15,
        sci_tlist = 16,
        sci_mlist = 17,
        sci_lufact_pointer = 128, /* lufact pointer */
        sci_implicit_poly = 129,
        sci_intrinsic_function = 130};
        //=============================================================================
        static Scilab instance = null;
        static readonly object padlock = new object();
        private Boolean withGraphics = false;
        //=============================================================================
        private const string CALL_SCILAB_DLL = "call_scilab.dll";
        private const string API_SCILAB_DLL = "api_scilab.dll";
        private const string GRAPHICS_DLL = "graphics.dll";
        private const string OUTPUT_STREAM_DLL = "scioutput_stream.dll";
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
            withGraphics = false;
        }
        //=============================================================================
        public Scilab(Boolean _bWithGraphics)
        {
            // start Scilab engine
            StartScilab(null, null, null);
            // Disable TCL/TK and Java graphic interfaces
            if (_bWithGraphics == false)
            {
                DisableInteractiveMode();
                withGraphics = false;
            }
            else
            {
                withGraphics = true;
            }
        }
        //=============================================================================
        /// <summary>
        /// Singleton 
        /// Only one instance of Scilab can be launch
        /// thread safe
        /// </summary>
        public static Scilab Instance
        {
            get
            {
                lock (padlock)
                {
                    if (instance == null)
                    {
                        instance = new Scilab();
                    }
                    return instance;
                }
            }
        }
        //=============================================================================
        /// <summary>
        /// Destructor
        /// </summary>
        ~Scilab()
        {
            finish();
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
            // first , we want to known dimensions iRows and iCols
            int[] iDims = getNamedMatrixOfDoubleDimensions(matrixName);
            int iRows = iDims[0];
            int iCols = iDims[1];

            // we allocate matrixDouble array
            if (iRows * iCols > 0)
            {
                double[] matrixDouble = new double[iRows * iCols];

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
            // first , we want to known dimensions iRows and iCols
            int[] iDim = getNamedMatrixOfStringDimensions(matrixName);
            int iRows = iDim[0];
            int iCols = iDim[1];
            
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
        /// Write a named matrix of boolean in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <param name="iRows"> Number of row</param>
        /// <param name="iCols"> Number of column</param>
        /// <param name="matrixBoolean"> pointer on data</param>
        /// <returns> if the operation successes (0) or not ( !0 )</returns>
        public int setNamedMatrixOfBoolean(string matrixName, int iRows, int iCols, Boolean[] matrixBoolean)
        {
            int[] matrixInt = new int[matrixBoolean.Length];
            for (int i = 0; i< matrixBoolean.Length; i++)
            {
                if (matrixBoolean[i] == true)
                {
                    matrixInt[i] = 1;
                }
                else
                {
                    matrixInt[i] = 0;
                }
            }
            return createNamedMatrixOfBoolean(matrixName, iRows, iCols, matrixInt);
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of boolean from Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of boolean from scilab. If Variable name does not exist returns null</returns>
        public unsafe Boolean[] getNamedMatrixOfBoolean(string matrixName)
        {
            // first , we want to known dimensions iRows and iCols
            int[] iDim = getNamedMatrixOfBooleanDimensions(matrixName);
            int iRows = iDim[0];
            int iCols = iDim[1];

            // we allocate matrixInt array
            if (iRows * iCols > 0)
            {
                int[] matrixInt = new int[iRows * iCols];

                // get values in matrixDouble
                readNamedMatrixOfBoolean(matrixName, &iRows, &iCols, matrixInt);

                Boolean[] matrixBoolean = new Boolean[iRows * iCols];
                for (int i = 0; i < iRows * iCols; i++ )
                {
                    if (matrixInt[i] == 1)
                    {
                        matrixBoolean[i] = true;
                    }
                    else
                    {
                        matrixBoolean[i] = false;
                    }
                }
                return matrixBoolean;
            }
            return null;
        }
        //=============================================================================
        /// <summary>
        /// Get dimensions of a named matrix of boolean in scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a int array. if variable name does not exist dimensions are 0 0 </returns>
        public unsafe int[] getNamedMatrixOfBooleanDimensions(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;
            int[] iDim = new int[2];

            readNamedMatrixOfBoolean(matrixName, &iRows, &iCols, null);

            iDim[0] = iRows;
            iDim[1] = iCols;

            return iDim;
        }
        //=============================================================================
        /// <summary>
        /// Write a named matrix of int(32) in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <param name="iRows"> Number of row</param>
        /// <param name="iCols"> Number of column</param>
        /// <param name="matrixInt"> pointer on data</param>
        public int setNamedMatrixOfInt(string matrixName, int iRows, int iCols, int[] matrixInt)
        {
            return createNamedMatrixOfInteger32(matrixName, iRows, iCols, matrixInt);
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of int(32) in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of int(32) from scilab. If Variable name does not exist returns null</returns>
        public unsafe int[] getNamedMatrixOfInt(string matrixName)
        {
            // first , we want to known dimensions iRows and iCols
            int[] iDim = getNamedMatrixOfIntDimensions(matrixName);
            int iRows = iDim[0];
            int iCols = iDim[1];

            // we allocate matrixInt array
            if (iRows * iCols > 0)
            {
                int[] matrixInt = new int[iRows * iCols];

                // get values in matrixInt
                readNamedMatrixOfInteger32(matrixName, &iRows, &iCols, matrixInt);
                return matrixInt;
            }
            return null;
        }
        //=============================================================================
        /// <summary>
        /// Get dimensions of a named matrix of int(32) in scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a int array. if variable name does not exist dimensions are 0 0 </returns>
        public unsafe int[] getNamedMatrixOfIntDimensions(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;
            int[] iDim = new int[2];

            readNamedMatrixOfInteger32(matrixName, &iRows, &iCols, null);

            iDim[0] = iRows;
            iDim[1] = iCols;

            return iDim;
        }
        //=============================================================================
        /// <summary>
        /// Write a named matrix of complex double in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <param name="iRows">Number of row</param>
        /// <param name="iCols">Number of column</param>
        /// <param name="matrixRealPart">real part</param>
        /// <param name="matrixImagPart">imag part</param>
        /// <returns></returns>
        public int setNamedMatrixOfComplexDouble(string matrixName,
                                                int iRows, int iCols,
                                                double[] matrixRealPart,
                                                double[] matrixImagPart)
        {
            return createNamedComplexMatrixOfDouble(matrixName,
                                                    iRows, iCols,
                                                    matrixRealPart,
                                                    matrixImagPart);
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of complex double in Scilab (Real part)
        /// </summary>
        /// <param name="matrixName">variable name</param>
        /// <returns> real part</returns>
        public unsafe double[] getNamedMatrixOfComplexDoubleRealPart(string matrixName)
        {
            int[] iDim = getNamedMatrixOfComplexDoubleDimensions(matrixName);
            int iRows = iDim[0];
            int iCols = iDim[1];
            double[] dRealPart = new double[iRows * iCols];
            double[] dImagPart = new double[iRows * iCols];

            readNamedComplexMatrixOfDouble(matrixName,
                                           &iRows, &iCols,
                                           dRealPart,
                                           dImagPart);

            return dRealPart;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of complex double in Scilab (Imag part)
        /// </summary>
        /// <param name="matrixName">variable name</param>
        /// <returns>imag part</returns>
        public unsafe double[] getNamedMatrixOfComplexDoubleImagPart(string matrixName)
        {
            int[] iDim = getNamedMatrixOfComplexDoubleDimensions(matrixName);
            int iRows = iDim[0];
            int iCols = iDim[1];
            double[] dRealPart = new double[iRows * iCols];
            double[] dImagPart = new double[iRows * iCols];

            readNamedComplexMatrixOfDouble(matrixName,
                               &iRows, &iCols,
                               dRealPart,
                               dImagPart);
            return dImagPart;
        }
        //=============================================================================
        /// <summary>
        /// Get dimensions of a matrix of complex double in scilab
        /// </summary>
        /// <param name="matrixName">variable name</param>
        /// <returns>dimension</returns>
        public unsafe int[] getNamedMatrixOfComplexDoubleDimensions(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;
            int[] iDim = new int[2];

            readNamedComplexMatrixOfDouble(matrixName, &iRows, &iCols, null,null);

            iDim[0] = iRows;
            iDim[1] = iCols;

            return iDim;
        }
        //=============================================================================
        /// <summary>
        /// get scilab type of named matrix
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>scilab type (see enum ScilabType)</returns>
        public int getNamedMatrixType(string matrixName)
        {
            return getNamedVarType(matrixName);
        }
        //=============================================================================
        /// <summary>
        /// Detect if a variable name exists in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns> true if exists</returns>
        public unsafe Boolean existNamedVariable(string matrixName)
        {
            int* piAdress = null;
            int ierr = getVarAddressFromName(matrixName, &piAdress);
            if ( (ierr == 1) && (piAdress != null)) return true;
            return false;
        }
        //=============================================================================
        /// <summary>
        /// Execute a scilab script .sce
        /// </summary>
        /// <param name="scriptFilename">the path to the .sce file</param>
        /// <returns>error code operation, 0 : OK</returns>
        public int execScilabScript(String scriptFilename)
        {
            return sendScilabJob( "exec('" + scriptFilename + "');" );
        }
        //=============================================================================
        /// <summary>
        /// Detect if a Scilab graphic window is opened
        /// </summary>
        /// <returns>true or false</returns>
        public Boolean HaveAGraph()
        {
            if (withGraphics)
            {
                int ierr = sciHasFigures();
                if (ierr == 1) return true;
            }
            return false;
        }
        //=============================================================================
        /// <summary>
        /// do a scilab event
        /// parser need to run to do a event
        /// </summary>
        /// <returns>error code operation, 0 : OK</returns>
        public int doEvent()
        {
            // do a pause (we do not want 100% CPU used)
            // ugly but it works ...
            Thread.Sleep(1);
            // do a loop of parser
            return sendScilabJob("");
        }
        //=============================================================================
        /// <summary>
        /// Terminate Scilab engine.
        /// </summary>
        /// <returns>1 if it is correctly finished</returns>
        private int finish()
        {
            int ierr = TerminateScilab(null);
            return ierr;
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
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int createNamedMatrixOfString([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int createNamedMatrixOfDouble([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfBoolean from C (see api_boolean.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private static extern int createNamedMatrixOfBoolean([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] int[] _piBool);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfInteger32 from C (see api_int.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int createNamedMatrixOfInteger32([In] String _pstName,
                                                           [In] int _iRows, [In] int _iCols,
                                                           [In] int[] _piData);
        //=============================================================================
        /// <summary>
        /// import createNamedComplexMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int createNamedComplexMatrixOfDouble([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal,
                                                            [In] double[] _pdblImg);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedMatrixOfString([In] String _pstName,
                                                          [Out]  Int32* _piRows, [Out]  Int32* _piCols,
                                                          [In, Out] int[] _piLength,
                                                          [In, Out] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedMatrixOfDouble([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] Double[] _pdblReal);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfBoolean from C (see api_boolean.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedMatrixOfBoolean([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] int[] _piBool);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfInteger32 from C (see api_int.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedMatrixOfInteger32([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] int[] _piData);

        //=============================================================================
        /// <summary>
        /// import readNamedComplexMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int readNamedComplexMatrixOfDouble([In] String _pstName,
                                                        [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                        [In, Out] double[] _pdblReal,
                                                        [In, Out] double[] _pdblImg);
        //=============================================================================
        /// <summary>
        /// get Variable Adress in scilab stack from name
        /// used for getNamedMatrixType (internal)
        /// </summary>
        /// <param name="_pstName">variable name</param>
        /// <param name="_piAddress"> stack address</param>
        /// <returns>1 if ok</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int getVarAddressFromName([In] String _pstName,
                                                               [Out] Int32** _piAddress);
        //=============================================================================
        /// <summary>
        /// get Variable type in scilab stack from name
        /// </summary>
        /// <param name="_pstName">variable name</param>
        /// <returns>type or -1</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int getNamedVarType([In] String _pstName);
        //=============================================================================
        /// <summary>
        /// get variable type with adress in scilab stack
        /// used for getNamedMatrixType (internal)
        /// </summary>
        /// <param name="_piAddress"> stack address</param>
        /// <returns>scilab type, 0 fails</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int getVarType([In] Int32* _piAddress);
        //=============================================================================
        /// <summary>
        ///  Detect if a Scilab graphic window is opened
        /// </summary>
        /// <returns>0 (FALSE) or 1 (TRUE)</returns>
        [DllImport(GRAPHICS_DLL, CharSet = CharSet.Ansi)]
        private unsafe static extern int sciHasFigures();
        //=============================================================================
        /// <summary>
        ///  get last error code
        /// </summary>
        /// <returns>last error code</returns>
        [DllImport(OUTPUT_STREAM_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int GetLastErrorCode();
        //=============================================================================
    }
}
//=============================================================================
