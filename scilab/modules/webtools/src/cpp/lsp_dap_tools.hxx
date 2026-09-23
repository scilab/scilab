/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __LSP_DAP_TOOLS_HXX__
#define __LSP_DAP_TOOLS_HXX__

#include <algorithm>
#include <string>
#include <regex>
#include <vector>

#include "internal.hxx"
#include "nlohmann_json.hxx"
using json = nlohmann::json;

namespace ast
{
class Exp;
}

class RuntimeSnapshot;

enum COMPLETION_KIND {
    NO_COMP, // do not complete
    STRING,  // inside a string. ie: complete file path
    FIELD,   // field completion. ie: struct.field
    FIELD_STRING,   // field completion. ie: struct("field")
    DICT     // complete on static dictionary
};

// input: text to auto complete
// token: what we try to complete
// exp: exp to eval
// return: kind of completion
// ie: input => elem.fie, token => fie, exp => elem, return FIELD
COMPLETION_KIND completion_info(const std::string& input, std::string& token, std::string& exp);
json getDictCompletion(const std::string& token, const RuntimeSnapshot* snapshot = nullptr);
json getFileCompletion(const std::string& token);
json getFieldCompletion(const std::string& token, const std::string& exp);
json getSignatures(const std::string& _word, int _activParam, const std::string& _sheetFile);
std::string getDoc(const std::string& _word, const std::string& _sheetFile);
bool isRuntimeValueAvailable(types::InternalType* value);
// json getLintInfo(const std::string&);
std::string eval(const std::string& _command, types::InternalType** _pIT);
bool parseDocument(const std::vector<std::string>& lines, ast::Exp** tree);
bool parseDocumentWithRecovery(const std::vector<std::string>& lines, ast::Exp** tree);
bool parseDocumentWithRecovery(const std::vector<std::string>& lines, ast::Exp** tree, std::string* errorMessage);
bool parseDocumentWithRecoveryWithoutRuntime(const std::vector<std::string>& lines,
    ast::Exp** tree, std::string* errorMessage);
std::string getWord(const std::string& _line, size_t _character);
json convertLocation(Location _loc);
std::string getVariableDisplayValue(types::InternalType* pIT);
std::string getStructuredVariableSummary(types::InternalType* pIT, bool includeFields);
#endif /* !__LSP_DAP_TOOLS_HXX__ */
