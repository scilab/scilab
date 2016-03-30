/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
//=============================================================================
/* Please notice that it is only a example and not a full C# wrapper */
//=============================================================================
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
//=============================================================================
namespace DotNetScilab
{
    /* Scilab Types */
    public enum ScilabType
    {
        sci_matrix = 1,
        sci_poly = 2,
        sci_boolean = 4,
        sci_sparse = 5,
        sci_boolean_sparse = 6,
        sci_matlab_sparse = 7,
        sci_ints = 8,
        sci_handles = 9,
        sci_strings = 10,
        sci_u_function = 11,
        sci_c_function = 13,
        sci_lib = 14,
        sci_list = 15,
        sci_tlist = 16,
        sci_mlist = 17,
        sci_pointer = 128,
        sci_implicit_poly = 129,
        sci_intrinsic_function = 130
    };

    public sealed class Scilab
    {
        //=============================================================================
        static Scilab instance = null;
        static readonly object padlock = new object();
        private Boolean withGraphics = false;
        //=============================================================================
        /// <summary>
        /// Constructor, initialize scilab engine.
        /// </summary>
        public Scilab()
        {
            // Disable TCL/TK and Java graphic interfaces
            Scilab_cs_wrapper.DisableInteractiveMode();
            withGraphics = false;

            // start Scilab engine configurated without java
            Scilab_cs_wrapper.StartScilab(null, null, null);
        }
        //=============================================================================
        public Scilab(Boolean _bWithGraphics)
        {
            // Disable TCL/TK and Java graphic interfaces
            if (_bWithGraphics == false)
            {
                Scilab_cs_wrapper.DisableInteractiveMode();
                withGraphics = false;
            }
            else
            {
                withGraphics = true;
            }

            // start Scilab engine
            Scilab_cs_wrapper.StartScilab(null, null, null);
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
            // freed by O.S
            //Scilab_cs_wrapper.TerminateScilab(null);
        }
        //=============================================================================
        /// <summary>
        /// Send a job to scilab
        /// </summary>
        /// <param name="command">command to send to scilab</param>
        /// <returns>error code operation, 0 : OK</returns>
        public int SendScilabJob(string command)
        {
            return Scilab_cs_wrapper.SendScilabJob(command);
        }
        //=============================================================================
        /// <summary>
        ///  get last error code
        /// </summary>
        /// <returns>last error code</returns>
        public int GetLastErrorCode()
        {
            return Scilab_cs_wrapper.GetLastErrorCode();
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
        public int createNamedMatrixOfDouble(string matrixName, int iRows, int iCols, double[] matrixDouble)
        {
            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.createNamedMatrixOfDouble(ptrEmpty, matrixName, iRows, iCols, matrixDouble);
            return SciErr.iErr;
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
        public int createNamedMatrixOfString(string matrixName, int iRows, int iCols, string[] matrixString)
        {
            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.createNamedMatrixOfString(ptrEmpty, matrixName, iRows, iCols, matrixString);
            return SciErr.iErr;
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
        public int createNamedMatrixOfBoolean(string matrixName, int iRows, int iCols, Boolean[] matrixBoolean)
        {
            int[] matrixInt = new int[matrixBoolean.Length];
            for (int i = 0; i < matrixBoolean.Length; i++)
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
            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.createNamedMatrixOfBoolean(ptrEmpty, matrixName, iRows, iCols, matrixInt);
            return SciErr.iErr;
        }
        //=============================================================================
        /// <summary>
        /// Write a named matrix of int(32) in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <param name="iRows"> Number of row</param>
        /// <param name="iCols"> Number of column</param>
        /// <param name="matrixInt"> pointer on data</param>
        public int createNamedMatrixOfInt32(string matrixName, int iRows, int iCols, int[] matrixInt)
        {
            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.createNamedMatrixOfInteger32(ptrEmpty, matrixName, iRows, iCols, matrixInt);
            return SciErr.iErr;
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
        public int createNamedComplexMatrixOfDouble(string matrixName,
                                                int iRows, int iCols,
                                                double[] matrixRealPart,
                                                double[] matrixImagPart)
        {
            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.createNamedComplexMatrixOfDouble(ptrEmpty, matrixName,
                                                    iRows, iCols,
                                                    matrixRealPart,
                                                    matrixImagPart);
            return SciErr.iErr;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of double from Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of double from scilab. If Variable name does not exist returns null</returns>
        public unsafe double[] readNamedMatrixOfDouble(string matrixName)
        {
            int iRows = 0;
            int iCols = 0;

            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.readNamedMatrixOfDouble(ptrEmpty, matrixName, &iRows, &iCols, null);

            if (iRows * iCols > 0)
            {
                double[] matrixDouble = new double[iRows * iCols];

                // get values in matrixDouble
                SciErr = Scilab_cs_wrapper.readNamedMatrixOfDouble(ptrEmpty, matrixName, &iRows, &iCols, matrixDouble);
                if (SciErr.iErr != 0) return null;
                return matrixDouble;
            }
            return null;
        }
        //=============================================================================
        /// <summary>
        /// Get dimensions of a named matrix in scilab
        /// </summary>
        /// <returns>a int array. 
        /// if variable name does not exist dimensions are null </returns>
        public unsafe int[] getNamedVarDimension(string matrixName)
        {
            int[] iDim = null;
            int iRows = 0;
            int iCols = 0;

            System.IntPtr ptrEmpty = new System.IntPtr();
            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.getNamedVarDimension(ptrEmpty, matrixName, &iRows, &iCols);
            if (SciErr.iErr == 0)
            {
                iDim = new int[2];
                iDim[0] = iRows;
                iDim[1] = iCols;
            }
            return iDim;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of string from scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of string from scilab. If Variable name does not exist returns null</returns>
        public unsafe string[] readNamedMatrixOfString(string matrixName)
        {
            string[] matrixString = null;

            int[] iDim = getNamedVarDimension(matrixName);

            if (iDim != null)
            {
                int iRows = iDim[0];
                int iCols = iDim[1];

                // we allocate lengthmatrixString
                int[] lengthmatrixString = new int[iRows * iCols];

                System.IntPtr ptrEmpty = new System.IntPtr();

                // we get length of strings
                Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.readNamedMatrixOfString(ptrEmpty, matrixName, 
                                        &iRows, &iCols, 
                                        lengthmatrixString, null);

                // we allocate each string
                matrixString = new string[iRows * iCols];
                for (int i = 0; i < iRows * iCols; i++)
                {
                    matrixString[i] = new string(' ', lengthmatrixString[i]);
                }

                // we get strings from scilab
                SciErr = Scilab_cs_wrapper.readNamedMatrixOfString(ptrEmpty, matrixName, 
                                                &iRows, &iCols,
                                                lengthmatrixString, 
                                                matrixString);
            }
            return matrixString;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of boolean from Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of boolean from scilab. If Variable name does not exist returns null</returns>
        public unsafe Boolean[] getNamedMatrixOfBoolean(string matrixName)
        {
            Boolean[] matrixBoolean = null;
            int[] iDim = getNamedVarDimension(matrixName);

            if (iDim != null)
            {
                int iRows = iDim[0];
                int iCols = iDim[1];
                int[] matrixInt = new int[iRows * iCols];

                System.IntPtr ptrEmpty = new System.IntPtr();

                // get values in matrixDouble
                Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.readNamedMatrixOfBoolean(ptrEmpty, matrixName, 
                                                            &iRows, &iCols, 
                                                            matrixInt);

                if (matrixInt != null)
                {
                    matrixBoolean = new Boolean[iRows * iCols];
                    for (int i = 0; i < iRows * iCols; i++)
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
                }
            }
            return matrixBoolean;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of complex double in Scilab (Real part)
        /// </summary>
        /// <param name="matrixName">variable name</param>
        /// <returns> real part. If Variable name does not exist returns null</returns>
        public unsafe double[] readNamedComplexMatrixOfDoubleRealPart(string matrixName)
        {
            double[] dRealPart = null;
            int[] iDim = getNamedVarDimension(matrixName);
            if (iDim != null)
            {
                int iRows = iDim[0];
                int iCols = iDim[1];

                double[] dImagPart = new double[iRows * iCols];
                dRealPart = new double[iRows * iCols];

                System.IntPtr ptrEmpty = new System.IntPtr();

                Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.readNamedComplexMatrixOfDouble(ptrEmpty, matrixName,
                                           &iRows, &iCols,
                                           dRealPart,
                                           dImagPart);
            }
            return dRealPart;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of complex double in Scilab (Imag part)
        /// </summary>
        /// <param name="matrixName">variable name</param>
        /// <returns> img part. If Variable name does not exist returns null</returns>
        public unsafe double[] readNamedComplexMatrixOfDoubleImgPart(string matrixName)
        {
            double[] dImagPart = null;
            int[] iDim = getNamedVarDimension(matrixName);
            if (iDim != null)
            {
                int iRows = iDim[0];
                int iCols = iDim[1];

                double[] dRealPart = new double[iRows * iCols];
                dImagPart = new double[iRows * iCols];

                System.IntPtr ptrEmpty = new System.IntPtr();

                Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.readNamedComplexMatrixOfDouble(ptrEmpty, matrixName,
                                           &iRows, &iCols,
                                           dRealPart,
                                           dImagPart);
            }
            return dImagPart;
        }
        //=============================================================================
        /// <summary>
        /// Read a named matrix of int(32) in Scilab
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>a matrix of int(32) from scilab. If Variable name does not exist returns null</returns>
        public unsafe int[] readNamedMatrixOfInt32(string matrixName)
        {
            int[] matrixInt = null;
            int[] iDim = getNamedVarDimension(matrixName);
            if (iDim != null)
            {
                int iRows = iDim[0];
                int iCols = iDim[1];

                // we allocate matrixInt array
                matrixInt = new int[iRows * iCols];

                System.IntPtr ptrEmpty = new System.IntPtr();

                // get values in matrixInt
                Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.readNamedMatrixOfInteger32(ptrEmpty, matrixName, &iRows, &iCols, matrixInt);
            }
            return matrixInt;
        }
        //=============================================================================
        /// <summary>
        /// get scilab type of named matrix
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>scilab type (see enum ScilabType)</returns>
        public unsafe int getNamedVarType(string matrixName)
        {
            int iType = 0;
            System.IntPtr ptrEmpty = new System.IntPtr();

            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.getNamedVarType(ptrEmpty, matrixName, &iType);
            if (SciErr.iErr == 0) return iType;
            return 0;
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
            System.IntPtr ptrEmpty = new System.IntPtr();

            Scilab_cs_wrapper.api_Err SciErr = Scilab_cs_wrapper.getVarAddressFromName(ptrEmpty, matrixName, &piAdress);
            if (SciErr.iErr == 0) return true;
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
            return Scilab_cs_wrapper.SendScilabJob("exec('" + scriptFilename + "');");
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
                int ierr = Scilab_cs_wrapper.sciHasFigures();
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
            return SendScilabJob("");
        }
        //=============================================================================
        /// <summary>
        /// get scilab type of named matrix
        /// </summary>
        /// <param name="matrixName"> variable name</param>
        /// <returns>scilab type (see enum ScilabType)</returns>
        public int isNamedVarComplex(string matrixName)
        {
            System.IntPtr ptrEmpty = new System.IntPtr();
            return Scilab_cs_wrapper.isNamedVarComplex(ptrEmpty, matrixName);
        }
        //=============================================================================
    }
}
//=============================================================================
