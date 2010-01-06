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

(* Compilation message translation dictionary *)

let msgs = [
  ("_UnknownIdentifier",
    [("ENG", "Unknown identifier")]);
  ("_EnumTypeDefWithDuplicLit",
    [("ENG", "Enumeration type definition with duplicated literal")]);
  ("_NotYetImplemented",
    [("ENG", "Use of not yet implemented feature:")]);
  ("_EncapsulatedCannotBeAppliedTo",
    [("ENG", "\"encapsulated\" keyword cannot be applied to")]);
  ("_UnspecifiedEnumLits",
    [("ENG", "Enumeration literals not specified")]);
  ("_CommandLine",
    [("ENG", "command line")]);
  ("_CharacterPosition",
    [("ENG", "Character position")]);
  ("_LineNumber",
    [("ENG", "Line number")]);
  ("_ColumnNumber",
    [("ENG", "Column number")]);
  ("_Context",
    [("ENG", "Context")]);
  ("_UntranslatedText",
    [("ENG", "Untranslated text")]);
  ("_NonBooleanIfCondExpr",
    [("ENG", "Non-Boolean \"if\" condition expression")]);
  ("_TypeConflictsInIfAlternExprs",
    [("ENG", "If alternative expressions must be of the same type")]);
  ("_InvalidTypeInRangeExpr",
    [("ENG", "Invalid type of element in range expression")]);
  ("_NonIntegerRangeExpr",
    [("ENG", "Non-integer range expression")]);
  ("_TypeConflictsInVectorExpr",
    [("ENG", "Elements of different types in vector expression")]);
  ("_NonPredefTypeVectorExpr",
    [("ENG", "Only vector expressions of predefined type elements are supported")]);
  ("_ArrayDimMustAgreeToPerform",
    [("ENG", "Array dimensions must agree to perform")]);
  ("_ImplicitIterRange",
    [("ENG", "Implicit iteration range")]);
  ("_UnaryOperPLUS",
    [("ENG", "Unary operator +")]);
  ("_MatrixExpr",
    [("ENG", "Matrix expression")]);
  ("_BinaryOperDIFF",
    [("ENG", "Binary operator <>")]);
  ("_BinaryOperEQUEQU",
    [("ENG", "Binary operator ==")]);
  ("_TopLevelExpr",
    [("ENG", "expression statement")]);
  ("_TopLevelAlgorithm",
    [("ENG", "algorithmic statement")]);
  ("_WithinClause",
    [("ENG", "within clause")]);
  ("_ImportClause",
    [("ENG", "import clause")]);
  ("_InvalidTypeDef",
    [("ENG", "Invalid \"type\" definition")]);
  ("_ShortClassDef",
    [("ENG", "Short class definition")]);
  ("_ClassDefByExtension",
    [("ENG", "Class definition by extension")]);
  ("_InvalidUseOfEnumKeyword",
    [("ENG", "Invalid use of \"enumeration\" keyword")]);
  ("_ClassExtendsDef",
    [("ENG", "Class definition by extension")]);
  ("_OperAppliedToNonNumericExpr",
    [("ENG", "operator applied to non numeric expression")]);
  ("_PowerOperOnNonSquareArray",
    [("ENG", "Cannot perform power operation on non square array")]);
  ("_ArrayDimMustAgreeToPerform",
    [("ENG", "Array dimensions must agree to perform")]);
  ("_EachAppliedToNonArrayElem",
    [("ENG", "Cannot apply \"each\" keyword to non-array type element")]);
  ("_EquTermsNotOfTheSameType",
    [("ENG", "Equation terms must be of the same type")]);
  ("_ClassElemFoundInExpr",
    [("ENG", "Component element expected, but class element found")]);
  ("_InvalidExtensionDef",
    [("ENG", "Invalid extension definition")]);
  ("_VariablityConflictsInCompDef",
    [("ENG", "Variability conflicts in component definition")]);
  ("_CausalityConflictsInCompDef",
    [("ENG", "Causality conflicts in component definition")]);
  ("_TypeConflictsInAssign",
    [("ENG", "Type conflicts in assignment expression")]);
  ("_InvalidExprInElemModif",
    [("ENG", "Invalid expression in element modification")]);
  ("_FieldAccessInElemModifExpr",
    [("ENG", "Field access in element modification expression")]);
  ("_ElementRedeclaration",
    [("ENG", "Element redeclaration")]);
  ("_InvalidTypeOfClassSpec",
    [("ENG", "Invalid type of class specifier")]);
  ("_RedeclarePredefTypeAttrib",
    [("ENG", "Redeclaration not allowed for predefined type attributes")]);
  ("_InvalidClassElemModif",
    [("ENG", "Invalid modification of class element")]);
  ("_FinalElemModifNotAllowed",
    [("ENG", "Cannot modify final element")]);
  ("_InvalidElemModifDef",
    [("ENG", "Invalid element modification definition")]);
  ("_ArrayDimMismatchInEqu",
    [("ENG", "Array dimensions do not agree in equation")]);
  ("_ComponentTypeEqu",
    [("ENG", "Component type equation")]);
  ("_AlgoClause",
    [("ENG", "Algorithm clause")]);
  ("_InvalidKeyWordEndInExpr",
    [("ENG", "Invalid use of expression \"end\"")]);
  ("_InvalidTypeOfFuncValueInEqu",
    [("ENG", "Invalid type of function value in equation")]);
  ("_FuncArgumentReduction",
    [("ENG", "Function argument reduction")]);
  ("_TooManyArgsInFuncCall",
    [("ENG", "Too many arguments in function call")]);
  ("_FuncCallWithDuplicateArg",
    [("ENG", "Function call with duplicate named argument")]);
  ("_MixedPositAndNamedFuncArgPass",
    [("ENG", "Mixed positional and named function argument passing not allowed")]);
  ("_NonInputFuncArgElem",
    [("ENG", "Function called with non input argument")]);
  ("_NoInnerDeclForOuterElem",
    [("ENG", "Missed inner declaration for outer element")]);
  ("_BlockElem",
    [("ENG", "Block element")]);
  ("_ExpandableConnector",
    [("ENG", "Expandable connector")]);
  ("_PredefinedTypeAttribModif",
    [("ENG", "Modification of predefined type attribute")]);
  ("_UnsupportedFeature",
    [("ENG", "Use of unsupported feature:")]); (*to be documented*)
  ("_OperAppliedToNonBoolExpr",
    [("ENG", "operator applied to non-Boolean expression")]);
  ("_TypeDef",
    [("ENG", "\"type\" definition")]);
  ("_AddOper",
    [("ENG", "addition operation")]);
  ("_TypeInconsistWithComparOper",
    [("ENG", "Type of operands inconsistent with comparison operation")]);
  ("_OperBetweenScalarAndArray",
    [("ENG", "operator between scalar and array type elements")]);
  ("_Addition",
    [("ENG", "addition")]);
  ("_Subtraction",
    [("ENG", "subtraction")]);
  ("_TypeInconsistentWithDivOper",
    [("ENG", "Type of operands inconsistent with division operation")]);
  ("_ArrayDimsNonCompatibleWithMult",
    [("ENG", "Arrays do not have compatible dimensions to be multiplied")]);
  ("_ERROR",
    [("ENG", "ERROR")]);
  ("_ElemExpected",
    [("ENG", "element expected")]);
  ("_UseOfTypePrefixInShortClassDef",
    [("ENG", "Use of type prefix not allowed in short class definition")]);
  ("_UseOfSubsInShortClassDef",
    [("ENG", "Use of subscripts not allowed in short class definition")]);
  ("_NonEmptyFuncCallUsedAsAnEqu",
    [("ENG", "Non-empty function call cannot be used as an equation")]);
  ("_DuplicatedModifOfElem",
    [("ENG", "Duplicated modification of element")]);
  ("_ComponentTypeElemInstant",
    [("ENG", "Component type element instantiation")]);
  ("_PredefinedTypeElemInstant",
    [("ENG", "Predefined type element instantiation")]);
  ("_Component",
    [("ENG", "Component")]);
  ("_CannotAccessProtectElem",
    [("ENG", "Cannot access protected element")]);
  ("_CannotAccessOuterElem",
    [("ENG", "Cannot access outer element")]);
  ("_UnknownFunction",
    [("ENG", "Unknown function")]);
  ("_ZeroRaisedToTheZeroPower",
    [("ENG", "Zero raised to the zero power")]);
  ("_IntegerRaisedToIntegerPower",
    [("ENG", "Integer raised to an integer power")]);
  ("_RealRaisedToIntegerPower",
    [("ENG", "Real raised to an integer power")]);
  ("_VectorRaisedToIntegerPower",
    [("ENG", "Vector raised to an integer power")]);
  ("_NonIntegerArrayDim",
    [("ENG", "Non-Integer array dimension")]);
  ("_EnumType",
    [("ENG", "Enumeration type")]);
  ("_StringType",
    [("ENG", "String type")]);
  ("_BooleanType",
    [("ENG", "Boolean type")]);
  ("_BooleanOperator",
    [("ENG", "Boolean operator")]);
  ("_DynamicArrayType",
    [("ENG", "Dynamic array type")]);
  ("_StaticArrayType",
    [("ENG", "Static array type")]);
  ("_InstanceType",
    [("ENG", "Instance type")]);
  ("_ConditionalEqu",
    [("ENG", "Conditional equation")]);
  ("_FieldAccessExpr",
    [("ENG", "Field access expression")]);
  ("_IndexedAccessExpr",
    [("ENG", "Indexed access expression")]);
  ("_RangeExpr",
    [("ENG", "Range expression")]);
  ("_Expr",
    [("ENG", "Expression")]);
  ("_ExprOfType",
    [("ENG", "Expression of type")]);
  ("_TupleExpr",
    [("ENG", "Tuple expression")]);
  ("_VectorReduct",
    [("ENG", "Vector reduction")]);
  ("_LoopVar",
    [("ENG", "Loop variable")]);
  ("_PredefinedTypeClassRef",
    [("ENG", "Predefined type class reference")]);
  ("_NonExternalCallClassRef",
    [("ENG", "Use of class reference in expression is allowed only for external function call")]);
  ("_ExternalProcedureCall",
    [("ENG", "External procedure call")]);
  ("_ExternalCallToLanguage",
    [("ENG", "External call to language")]);
  ("_ExternalCallWithLeftHandSideExpr",
    [("ENG", "External call with left hand side expression")]);
  ("_AssignExprInElemModif",
    [("ENG", "Assignment expression in element modification")]);
  ("_CannotSubscriptANonArrayTypeElem",
    [("ENG", "Cannot subscript a non array type element")]);
  ("_NonIntegerArraySubscript",
    [("ENG", "Non-Integer array subscript")]);
  ("_RangeStepValueCannotBeNull",
    [("ENG", "Range step value cannot be null")]);
  ("_TypeOfA",
    [("ENG", "Type of A")]);
  ("_TypeOfB",
    [("ENG", "Type of B")]);
  ("_Source",
    [("ENG", "Source")]);
  ("_ClassName",
    [("ENG", "Class name")]);
  ("_FunctionCallExpr",
    [("ENG", "Function call expression")]);
  ("_VectorExpr",
    [("ENG", "Vector expression")]);
  ("_ExprKind",
    [("ENG", "Expression kind")]);
  ("_TypeOfThenBranche",
    [("ENG", "Type of then branche")]);
  ("_TypeOfElseBranche",
    [("ENG", "Type of else branche")]);
  ("_TypePrefix",
    [("ENG", "Type prefix")]);
  ("_ElemFound",
    [("ENG", "Element found")]);
  ("_TypeSpecifierVariability",
    [("ENG", "Type specifier variability")]);
  ("_TypeSpecifierCausality",
    [("ENG", "Type specifier causality")]);
  ("_TypeOfCondition",
    [("ENG", "Type of \"condition\"")]);
  ("_TypeFound",
    [("ENG", "Type found")]);
  ("_ComponentElement",
    [("ENG", "Component element")]);
  ("_ExpectedType",
    [("ENG", "Expected type")]);
  ("_ClassElement",
    [("ENG", "Class element")]);
  ("_ArrayType",
    [("ENG", "Array type")]);
  ("_ClassSpecifier",
    [("ENG", "Class specifier")]);
  ("_TypeOfFuncValue",
    [("ENG", "Type of function value")]);
  ("_TypeOfFunctionOutput",
    [("ENG", "Type of function output")]);
  ("_CannotInheritFrom",
    [("ENG", "Cannot inherit from")]);
  ("_ComponentTypeElement",
    [("ENG", "Component type element")]);
  ("_PredefinedTypeElement",
    [("ENG", "Predefined type element")]);
  ("_AlreadyDeclaredInParentClass",
    [("ENG", "already declared in parent class")]);
  ("_InheritFromDiffClassKindsNotAllowed",
    [("ENG", "Inheritance from different class kinds not allowed")]);
  ("_MismatchingTypes",
    [("ENG", "Mismatching types")]);
  ("_InheritFromFunctionNotAllowed",
    [("ENG", "Inheritance from function not allowed")]);
  ("_InvalidAnnOfInvFunc",
    [("ENG", "Invalid annotation of inverse functions:")]);
  ("_RedeclarationNotAllowed",
    [("ENG", "Redeclaration not allowed")]);
  ("_UseOfEachKeywordNotAllowed",
    [("ENG", "Use of \"each\" keyword not allowed")]);
  ("_UseOfFinalKeywordNotAllowed",
    [("ENG", "Use of \"final\" keyword not allowed")]);
  ("_UnspecifiedModification",
    [("ENG", "Unspecified modification")]);
  ("_InvalidModifExpr",
    [("ENG", "Invalid modification expression")]);
  ("_InvalidFuncCallExpr",
    [("ENG", "Invalid function call expression")]);
  ("_InvalidTypeOfFuncCallExpr",
    [("ENG", "Invalid type of function call expression")]);
  ("_ClassType",
    [("ENG", "Class type")]);
  ("_Function",
    [("ENG", "Function")]);
  ("_FuncArgReductionNotAllowed",
    [("ENG", "Function argument reduction not allowed")]);
  ("_CannotUseUnnamedFuncArg",
    [("ENG", "Cannot use unnamed function argument")]);
  ("_InvalidFuncArgModif",
    [("ENG", "Invalid function argument modification")]);
  ("_UnknownArgName",
    [("ENG", "Unknown argument name")]);
  ("_CannotUseCausPrefixInGenClass",
    [("ENG", "Input or output component found in generic class")]);
  ("_FuncDefInNonInstantiatedClass",
    [("ENG", "Function defined in non instantiated class")]);
  ("_InvalidTypeOfArgInConnectStat",
    [("ENG", "Invalid type of argument in connect statement")]);
  ("_CannotConnectFlowAndNonFlowComp",
    [("ENG", "Cannot connect flow and non-flow components")]);
  ("_InvalidTypeOfWhenCond",
    [("ENG", "Invalid type of when condition")]);
  ("_NameResolution",
    [("ENG", "Name resolution")]);
  ("_InstantiationOfComponent",
    [("ENG", "Instantiation of component")]);
  ("_CodeGenerationForComponent",
    [("ENG", "Code generation for component")]);
  ("_ExecutionStep",
    [("ENG", "Execution step")]);
  ("_MissingDeclEquForFixedId",
    [("ENG", "Missing declaration equation for fixed identifier")]);
  ("_ComponentFuncInvocation",
    [("ENG", "Component function invocation")]);
  ("_FuncWithManyOutputs",
    [("ENG", "Function with many outputs")]);
  ("_InstantiationOfClass",
    ["ENG", "Instantiation of class"]);
  ("_CodeGenerationForClass",
    ["ENG", "Code generation for class"]);
  ("_InstanceUsedInConnection",
    [("ENG", "instance used in connection statement")]);
  ("_Unclosed",
    [("ENG", "Unclosed")]);
  ("_InvalidMatrixConstruct",
    [("ENG", "Invalid matrix construction")]);
  ("_InvalidArrayConstruct",
    [("ENG", "Invalid array construction")]);
  ("_SyntaxError",
    [("ENG", "Syntax error")]);
  ("_SyntacticAnalysis",
    [("ENG", "Syntactic analysis")]);
  ("_PredefType",
    [("ENG", "Predefined type")]);
  ("_WhenClausesCannotBeNested",
    [("ENG", "When clauses cannot be nested")]);
  ("_InvalidWhenEquation",
    [("ENG", "Invalid form of equation within when clause")]);
  ("_WhenConditionMustBeDiscrete",
    [("ENG", "When condition must be discrete-time expression")]);
  ("_ArgTypeMismatch",
    [("ENG", "Argument type mismatch")]);
  ("_VariabilityOfA",
    [("ENG", "Variability of A")]);
  ("_VariabilityOfB",
    [("ENG", "Variability of B")]);
  ("_VariabilityConflicts",
    [("ENG", "Variability conflicts")]);
  ("_CannotUseNamedArgWithBuiltInOper",
    [("ENG", "Cannot use named argument with a built-in operator or function")]);
  ("_OperArgMustBeAVar",
    [("ENG", "operator argument must be a variable")]);
  ("_ArgVariabilityMismatch",
    [("ENG", "Argument variability mismatch")]);
  ("_ExpectedVariability",
    [("ENG", "Expected variability")]);
  ("_VariabilityFound",
    [("ENG", "Variability found")]);
  ("_EquNotAllowedInTheDefOf",
    [("ENG", "Equations not allowed in the definition of")]);
  ("_OperCannotBeUsedWithinFuncDef",
    [("ENG", "operator cannot be used within function definition")]);
  ("_ArgDimMismatchInVectCall",
    [("ENG", "Arguments dimensions mismatch in vectorized function call")]);
  ("_ArgDimMismatch",
    [("ENG", "Argument dimension mismatch")]);
  ("_TooFewArgsInFuncCall",
    [("ENG", "Too few arguments in function call")]);
  ("_LHSOfDiscreteEquMustBeAVar",
    [("ENG", "Left hand side of discrete equation must be a variable")]);
  ("_InvalidVarOfRangeExpr",
    [("ENG", "Invalid variability of range expression")]);
  ("_RealExponentOfNegativeNumber",
    [("ENG", "Real exponentiation of negative number")]);
  ("_ZeroRaisedToNegativePower",
    [("ENG", "Zero raised to negative power")]);
  ("_IllegalCharacter",
    [("ENG", "Illegal character")]); (*to be documented*)
  ("_InvalidExternalFuncName",
    [("ENG", "Invalid external function name")]); (*to be documented*)
  ("_LHSOfExternalCall",
    [("ENG", "Left hand side of external call")]); (*to be documented*)
  ("_InvalidArgOfExternalCall",
    [("ENG", "Invalid argument of external call")]); (*to be documented*)
  ("_CannotAccessToPredefTypeAttrib",
    [("ENG", "Cannot access predefined type attribute")]);
  ("_InvalidCondEquation",
    [("ENG", "Invalid conditional equation")]);
  ("_IndexOutOfBound",
    [("ENG", "Index out of bound")]);
  ("_DivisionByZero",
    [("ENG", "Division by zero")]);
  ("_PredefinedOperator",
    [("ENG", "Predefined operator")]);
  ("_InvalidArgOfOper",
    [("ENG", "Invalid argument of operator")]);
  ("_UnsupportedDerOperArg",
    [("ENG", "Unsupported \"der\" operator argument")]);
  ("_InvalidInteger",
    [("ENG", "Invalid Integer representation")]);
  ("_NonSupportedTypeOfFuncInOut",
    [("ENG", "Non supported type of function input or output")]);
  ("_DuplicateDeclarationOfElement",
    [("ENG", "Duplicate declaration of element")]) (* to be documented *)
  ]

let translate msg =
  try
    List.assoc "ENG" (List.assoc msg msgs)
  with
    exn -> msg;
