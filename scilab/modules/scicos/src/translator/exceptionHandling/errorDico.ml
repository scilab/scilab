(*
 *  Translator from Modelica 2.x to flat Modelica
 *
 *  Copyright (C) 2005 - 2007 Imagine S.A.
 *  For more information or commercial use please contact us at www.amesim.com
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *)

(* Compilation error dictionary
  @author D. TALBI 
  @since 05/02/2007
  *)

type error_description =
  {
    err_msg: string list;
    err_info: (string * string) list;
    err_ctx: err_ctx
  }

and err_ctx =
  {
    path: Instantiation.path;
    location: Parser.location;
    instance_nature: Instantiation.instance_nature
  }

exception GenericError of error_description

(* list of compilation errors*)
let ccodes = [
  ("_UnknownIdentifier", "0001");
  ("_EnumTypeDefWithDuplicLit", "0002");
  ("_EncapsulatedCannotBeAppliedTo", "0003");
  ("_InvalidTypeDef", "0004");
  ("_UnspecifiedEnumLits", "0005");
  ("_NotYetImplemented", "0006");
  ("_UnsupportedFeature", "0007"); (*to be documented*)
  ("_TypeConflictsInAssign", "0008"); (*to be documented*)
  ("_OperBetweenScalarAndArray", "0009");
  ("_ArrayDimMustAgreeToPerform", "0010");
  ("_FuncCallWithDuplicateArg", "0011");
  ("_TooManyArgsInFuncCall", "0012");
  ("_MixedPositAndNamedFuncArgPass", "0013");
  ("_TypeInconsistWithComparOper", "0014");
  ("_EquTermsNotOfTheSameType", "0015");
  ("_NonInputFuncArgElem", "0016");
  ("_OperAppliedToNonNumericExpr", "0017");
  ("_ArrayDimsNonCompatibleWithMult", "0018");
  ("_PowerOperOnNonSquareArray", "0019");
  ("_NonBooleanIfCondExpr", "0020");
  ("_TypeConflictsInIfAlternExprs", "0021");
  ("_OperAppliedToNonBoolExpr", "0022");
  ("_TypeInconsistentWithDivOper", "0023");
  ("_ElemExpected", "0024");
  ("_FinalElemModifNotAllowed", "0025");
  ("_TypeConflictsInVectorExpr", "0026");
  ("_EachAppliedToNonArrayElem", "0027");
  ("_InvalidExprInElemModif", "0028");
  ("_ClassElemFoundInExpr", "0029");
  ("_ArrayDimMismatchInEqu", "0030");
  ("_InvalidKeyWordEndInExpr", "0031");
  ("_InvalidTypeInRangeExpr", "0032");
  ("_InvalidExtensionDef", "0033");
  ("_InvalidUseOfEnumKeyword", "0034");
  ("_UseOfTypePrefixInShortClassDef", "0035");
  ("_UseOfSubsInShortClassDef", "0036");
  ("_NonEmptyFuncCallUsedAsAnEqu", "0037");
  ("_DuplicatedModifOfElem", "0038");
  ("_InvalidClassElemModif", "0039");
  ("_CannotAccessProtectElem", "0040");
  ("_CannotAccessOuterElem", "0041");
  ("_CannotSubscriptANonArrayTypeElem", "0042");
  ("_NonIntegerArraySubscript", "0043");
  ("_RangeStepValueCannotBeNull", "0044");
  ("_CannotInheritFrom", "0045");
  ("_AlreadyDeclaredInParentClass", "0046");
  ("_InheritFromDiffClassKindsNotAllowed", "0047");
  ("_InheritFromFunctionNotAllowed", "0048");
  ("_InvalidAnnOfInvFunc", "0049");
  ("_CannotUseCausPrefixInGenClass", "0050");
  ("_InvalidTypeOfArgInConnectStat", "0051");
  ("_CannotConnectFlowAndNonFlowComp", "0052");
  ("_InvalidTypeOfWhenCond", "0053");
  ("_InstanceUsedInConnection", "0054");
  ("_WhenClausesCannotBeNested", "0055");
  ("_InvalidWhenEquation", "0056");
  ("_WhenConditionMustBeDiscrete", "0057");
  ("_ArgTypeMismatch", "0058");
  ("_VariabilityConflicts", "0059"); (*to be documented*)
  ("_CannotUseNamedArgWithBuiltInOper", "0060");
  ("_OperArgMustBeAVar", "0061");
  ("_ArgVariabilityMismatch", "0062");
  ("_EquNotAllowedInTheDefOf", "0063");
  ("_OperCannotBeUsedWithinFuncDef", "0064");
  ("_ArgDimMismatchInVectCall", "0065");
  ("_ArgDimMismatch", "0066");
  ("_TooFewArgsInFuncCall", "0067");
  ("_LHSOfDiscreteEquMustBeAVar", "0068");
  ("_InvalidVarOfRangeExpr", "0069");
  ("_InvalidExternalFuncName", "0070"); (*to be documented*)
  ("_InvalidArgOfExternalCall", "0071"); (*to be documented*)
  ("_DuplicateDeclarationOfElement", "0072"); (* to be documented *)
  ("_InvalidArgOfOper", "0096"); (*to be documented*)
  ("_InvalidInteger", "0097") (*to be documented*)
  ]

(* list of instantiation errors*)
let icodes = [
  ("_NotYetImplemented", "1000");
  ("_ZeroRaisedToTheZeroPower", "1001");
  ("_MissingDeclEquForFixedId", "1002");
  ("_RealExponentOfNegativeNumber", "1003");
  ("_ZeroRaisedToNegativePower", "1004");
  ("_CannotAccessToPredefTypeAttrib", "1005"); (*to be documented*)
  ("_InvalidCondEquation", "1006"); (*to be documented*)
  ("_IndexOutOfBound", "1007"); (*to be documented*)
  ("_DivisionByZero", "1008") (*to be documented*)
  ]

(* list of generic errors*)
let gcodes = [
  ("_NotYetImplemented", "2000")
  ]

(* list of syntactic errors*)
let scodes = [
  ("_Unclosed", "3000");
  ("_InvalidMatrixConstruct", "3001");
  ("_InvalidArrayConstruct", "3002");
  ("_SyntaxError", "3003");
  ("_IllegalCharacter", "3004")
  ]

let getCode exn msg =
  try
    match exn with
      | NameResolve.CompilError _ -> List.assoc msg ccodes
      | Instantiation.InstantError _ -> List.assoc msg icodes
      | GenericError _ -> List.assoc msg gcodes
      | Parser.SyntacticError _ -> List.assoc msg scodes
      | _ -> ""
  with
    exn -> ""
