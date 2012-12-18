(*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2012 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *)

(* from types/includes/internal.hxx *)

type realType =
            (* Internal Type *)
| RealInternal
            (* Generic Types *)
| RealGeneric
| RealInt8
| RealUInt8
| RealInt16
| RealUInt16
| RealInt32
| RealUInt32
| RealInt64
| RealUInt64
| RealString
| RealDouble
| RealBool
| RealFloat
| RealPoly
| RealSinglePoly
            (* Callable *)
| RealFunction
| RealMacro
| RealMacroFile
            (* Implicit List *)
| RealImplicitList
            (* Container *)
| RealContainer
| RealList
| RealTList
| RealMList
| RealSingleStruct
| RealStruct
| RealCell
            (* User *)
| RealUserType
            (*For list operation*)
| RealListOperation (* parent type *)
| RealListInsertOperation
| RealListDeleteOperation
| RealListUndefinedOperation
| RealFile
| RealColon
| RealDollar
| RealThreadId
| RealSparse
| RealSparseBool
| RealSingleHandle
| RealHandle

type genericType = {
  m_iRealType : realType;
(* from types/includes/types.hxx *)
  m_iRows : int;
  m_iCols : int;
  m_iSize : int;
  m_iSizeMax : int;
  m_piDims : int array; (* [50]; *)
  m_iDims : int;
}


(* in types/includes/arrayof.hxx
    bool                    m_bComplex;
    T*                      m_pRealData;
    T*                      m_pImgData;
*)
(* A Double is an ArrayOf of double *)
(* A Struct is an ArrayOf of SingleStruct/s *)
(* in types/includes/singlestruct.hxx
        std::map<std::wstring, InternalType *>* m_pData;
*)
(* A cell is an ArrayOf of InternalType/s *)

(* The environment is defined in:
   modules/symbol/includes/context.hxx
   modules/symbol/src/cpp/context.cpp
*)
