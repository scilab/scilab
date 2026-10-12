/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <algorithm>
#include <string>
#include <sstream>
#include <regex>
#include <iostream>
#include <fstream>
#include <set>

#include "lsp_dap_tools.hxx"
#include "evalVisitor.hxx"
#include "dummyvisitor.hxx"
#include "UTF8.hxx"
#include "callable.hxx"
#include "configvariable.hxx"
#include "struct.hxx"
#include "classdef.hxx"
#include "tlist.hxx"
#include "list.hxx"
#include "cell.hxx"
#include "object.hxx"
#include "json.hxx"
#include "visitor_common.hxx"
#include "runtimeaccess.hxx"
#include "runtimeSnapshot.hxx"

extern "C"
{
    #include "freeArrayOfString.h"
    #include "getdeprecated.h"
    #include "getfunctionsname.h"
    #include "getmacrosname.h"
    #include "getvariablesname.h"
    #include "commandwords.h"
    #include "parsecommand.h"
    #include "completion.h"
    #include "getDictionarySetProperties.h"
    #include "getDictionaryGetProperties.h"
    #include "inlinehelp.h"
    #include "functions_manager.h"
    #include "sci_malloc.h"
}

// Standard LSP CompletionItemKind values. DAP uses the textual `type` field
// from the same completion entries and does not interpret this numeric field.
static constexpr int COMPLETION_METHOD = 2;
static constexpr int COMPLETION_FUNCTION = 3;
static constexpr int COMPLETION_FIELD = 5;
static constexpr int COMPLETION_VARIABLE = 6;
static constexpr int COMPLETION_PROPERTY = 10;
static constexpr int COMPLETION_KEYWORD = 14;
static constexpr int COMPLETION_FILE = 17;
static constexpr int COMPLETION_ENUM_MEMBER = 20;

static bool isDeprecatedCompletion(const std::string& name)
{
    static const std::set<std::string> deprecated = []() {
        std::set<std::string> result;
        int size = 0;
        char** names = getDeprecatedNames(&size);
        for(int index = 0; index < size; ++index)
        {
            result.emplace(names[index]);
        }
        freeArrayOfString(names, size);
        return result;
    }();
    return deprecated.find(name) != deprecated.end();
}

static void appendFieldCompletion(json& fields, const std::wstring& name,
    const char* type, int kind)
{
    fields.push_back({
        {"label", scilab::UTF8::toUTF8(name)},
        {"type", type},
        {"kind", kind}
    });
}

bool isRuntimeValueAvailable(types::InternalType* value)
{
    if (value == nullptr || ConfigVariable::getScilabMode() != SCILAB_NWNI
            || !value->isCallable())
    {
        return true;
    }

    types::Callable* callable = value->getAs<types::Callable>();
    return callable == nullptr || !isNonNwniModule(callable->getModule().c_str());
}

bool isRuntimeValueProtected(const std::wstring& name)
{
    // Context::isprotected() only answers for the current scope, and a pause
    // or a macro opens one above the scope holding the predefined variables.
    symbol::Variable* variable = symbol::Context::getInstance()->getOrCreate(symbol::Symbol(name));
    return variable->empty() == false && variable->top()->protect;
}

static bool appendRuntimeFieldCompletions(types::InternalType* value, json& fields)
{
    if(value->isStruct())
    {
        types::String* names = value->getAs<types::Struct>()->getFieldNames();
        if(names)
        {
            for(int i = 0; i < names->getSize(); ++i)
            {
                appendFieldCompletion(fields, names->get(i), "field", COMPLETION_FIELD);
            }
            names->killMe();
        }
        return true;
    }

    if(value->isObject())
    {
        types::Classdef* classdef = value->getAs<types::Object>()->getClassdef();
        for(const std::wstring& name : classdef->getPublicProperties())
        {
            appendFieldCompletion(fields, name, "property", COMPLETION_PROPERTY);
        }
        for(const std::wstring& name : classdef->getPublicMethods())
        {
            appendFieldCompletion(fields, name, "method", COMPLETION_METHOD);
        }
        return true;
    }

    if(value->isClassdef())
    {
        // Completion on the class name itself (ClassName.), not an instance:
        // only what is reachable without an object, i.e. enumeration entries
        // and public static properties/methods. Instance properties and
        // methods require an instance and are offered on the instance instead.
        types::Classdef* classdef = value->getAs<types::Classdef>();
        for(const std::wstring& name : classdef->getEnumeration())
        {
            appendFieldCompletion(fields, name, "enum", COMPLETION_ENUM_MEMBER);
        }

        for(const auto& entry : classdef->getProperties())
        {
            const types::OBJ_ATTR& attr = std::get<0>(entry.second);
            if(attr.isStatic && attr.hidden == false
                    && attr.access == types::AccessModifier::PUBLIC)
            {
                appendFieldCompletion(fields, entry.first, "property", COMPLETION_PROPERTY);
            }
        }

        for(const auto& entry : classdef->getMethods())
        {
            const types::OBJ_ATTR& attr = std::get<0>(entry.second);
            if(attr.isStatic && attr.hidden == false
                    && attr.access == types::AccessModifier::PUBLIC)
            {
                appendFieldCompletion(fields, entry.first, "method", COMPLETION_METHOD);
            }
        }

        return true;
    }

    return false;
}

class FieldCompletionVisitor : public ast::DummyVisitor
{
public:
    FieldCompletionVisitor(size_t expectedEndColumn, const std::string& marker)
        : m_expectedEndColumn(expectedEndColumn), m_marker(marker)
    {
    }

    void visit(const ast::FieldExp& field) override
    {
        ast::DummyVisitor::visit(field);

        const Location& location = field.getLocation();
        const ast::Exp* tail = field.getTail();
        if(location.last_line != 1
                || location.last_column != static_cast<int>(m_expectedEndColumn)
                || !tail->isSimpleVar())
        {
            return;
        }

        const ast::SimpleVar* fieldName = tail->getAs<ast::SimpleVar>();
        std::string name = scilab::UTF8::toUTF8(fieldName->getSymbol().getName());
        if(!m_marker.empty() && name != m_marker)
        {
            return;
        }

        m_found = true;
        m_token = m_marker.empty() ? name : std::string();
        m_headLocation = field.getHead()->getLocation();
    }

    bool found() const
    {
        return m_found;
    }

    const std::string& token() const
    {
        return m_token;
    }

    const Location& headLocation() const
    {
        return m_headLocation;
    }

private:
    size_t m_expectedEndColumn;
    std::string m_marker;
    bool m_found = false;
    std::string m_token;
    Location m_headLocation;
};

static bool findParsedFieldCompletion(const std::string& source,
    const std::string& parsedSource, const std::string& marker,
    std::string& token, std::string& expression)
{
    ast::Exp* tree = nullptr;
    char* error = parseCommandWithRecovery(parsedSource.data(), reinterpret_cast<void**>(&tree));
    if(error)
    {
        FREE(error);
    }
    if(tree == nullptr)
    {
        return false;
    }

    // AST columns are 1-based and the last column is exclusive.
    FieldCompletionVisitor visitor(parsedSource.size() + 1, marker);
    tree->accept(visitor);
    delete tree;
    if(!visitor.found())
    {
        return false;
    }

    const Location& head = visitor.headLocation();
    if(head.first_line != 1 || head.last_line != 1 || head.first_column < 1
            || head.last_column < head.first_column
            || static_cast<size_t>(head.last_column - 1) > source.size())
    {
        return false;
    }

    token = visitor.token();
    expression = source.substr(static_cast<size_t>(head.first_column - 1),
        static_cast<size_t>(head.last_column - head.first_column));
    return !expression.empty();
}

static bool findParsedFieldCompletion(const std::string& input,
    std::string& token, std::string& expression)
{
    // A partial field name (foo.ba) already forms a valid AST.
    if(findParsedFieldCompletion(input, input, std::string(), token, expression))
    {
        return true;
    }

    // Complete a trailing dot (foo.) with a valid identifier, then use the
    // FieldExp head location to recover foo without relying on delimiters.
    static const std::string marker = "__scilab_completion__";
    return findParsedFieldCompletion(input, input + marker, marker, token, expression);
}

// The dot of "0." or "12." is a decimal point, not a field access. Digits
// ending an identifier, as in "a1.", stay one.
static bool endsWithDecimalPoint(const std::string& input)
{
    static const std::regex decimal_point_reg("(^|[^A-Za-z_%$!#?])[0-9]+\\.$");
    return std::regex_search(input, decimal_point_reg);
}

COMPLETION_KIND completion_info(const std::string& input, std::string& token, std::string& exp)
{
    std::string tmp = input;

    // check if we are inside a string
    bool isInsideQuote = std::count(tmp.begin(), tmp.end(), '\'') % 2 != 0;
    bool isInsideDblQuote = std::count(tmp.begin(), tmp.end(), '\"') % 2 != 0;
    bool isString = isInsideQuote || isInsideDblQuote;
    if(isInsideQuote && isInsideDblQuote)
    {
        // not yet managed
        return NO_COMP;
    }

    std::smatch string_match;
    if(isInsideQuote)
    {
        const std::regex string_reg("'[^']*$");
        std::regex_search(tmp, string_match, string_reg);
    } 
    else if (isInsideDblQuote)
    {
        const std::regex string_reg("\"[^\"]*$");
        std::regex_search(tmp, string_match, string_reg);
    }

    if(isString)
    {
        token = string_match[0].str();
        token.erase(0,1); // remove quote

        // check for string as field
        std::smatch arg_match;
        const std::regex arg_reg("\\([ ]*$");
        // search if whe got a call expression
        std::string arg_prefix = string_match.prefix();
        if(std::regex_search(arg_prefix, arg_match, arg_reg))
        {
            exp = arg_match.prefix();
            return FIELD_STRING;
        }

        return STRING;
    }

    if(endsWithDecimalPoint(tmp))
    {
        return NO_COMP;
    }

    if(findParsedFieldCompletion(tmp, token, exp))
    {
        return FIELD;
    }

    // get last expression
    std::smatch last_exp_match;
    const std::regex last_exp_reg("[^, ;]+$");
    if(std::regex_search(tmp, last_exp_match, last_exp_reg) == false)
    {
        // Nothing typed: an explicit completion request on an empty position
        // asks for the whole dictionary.
        token.clear();
        return DICT;
    }

    std::string lastexp = last_exp_match[0].str();

    // get last argument
    std::smatch last_arg_match;
    const std::regex last_arg_reg("\\([^()]*$");
    bool hasArg = std::regex_search(lastexp, last_arg_match, last_arg_reg);

    if(hasArg)
    {
        token = last_arg_match[0].str();
        token.erase(0,1); // remove (
        return DICT;
    }

    // get last scilab word
    // const std::regex scilab_word_match("[$!%\\w]*");
    std::smatch scilab_word_match;
    const std::regex scilab_word_reg("[$!%\\w]*$");
    if(std::regex_search(lastexp, scilab_word_match, scilab_word_reg) == false)
    {
        return NO_COMP;
    }

    token = scilab_word_match[0].str();
    return DICT;
}

json getFieldCompletion(const std::string& token, const std::string& exp)
{
    RuntimeTryLockGuard runtimeGuard;
    if (!runtimeGuard.ownsLock())
    {
        return json::array({});
    }

    ast::Exp* tree = nullptr;
    char* error = parseCommand(exp.data(), (void**)&tree);
    if (error)
    {
        // parsing error => no completion
        delete tree;
        FREE(error);
        return json::array({});
    }

    // a tree always start by a SeqExp and the SepExp of EvalVisitor catch all exeptions
    ast::EvalVisitor* eval = new ast::EvalVisitor();
    tree->accept(*eval);
    types::InternalType* pIT = eval->getResult();
    if(pIT)
    {
        // avoid double delete when pIT is held by the tree and the visitor
        pIT->IncreaseRef();
        delete tree;
        delete eval;
        pIT->DecreaseRef();

        json runtimeFields = json::array({});
        if(appendRuntimeFieldCompletions(pIT, runtimeFields))
        {
            pIT->killMe();
            return runtimeFields;
        }
        else if(pIT->isMList() || pIT->isTList())
        {
            types::String* pFields = pIT->getAs<types::TList>()->getFieldNames();
            json fields = json::array({});
            if(pFields)
            {
                for(int i = 1; i < pFields->getSize(); ++i)
                {
                    fields.push_back({
                        {"label", scilab::UTF8::toUTF8(pFields->get(i))},
                        {"type", "field"},
                        {"kind", COMPLETION_FIELD}
                    });
                }

                pFields->killMe();
            }

            pIT->killMe();
            return fields;
        }
        else if(pIT->isHandle())
        {
            int iSizeGet = 0;
            int iSizeSet = 0;
            char** graphic_get = getDictionaryGetProperties(&iSizeGet);
            char** graphic_set = getDictionarySetProperties(&iSizeSet);

            json fields = json::array({});
            std::set<std::string> added;
            for (int i=0; i < iSizeGet; ++i)
            {
                if (added.insert(graphic_get[i]).second)
                {
                    fields.push_back({{"label", graphic_get[i]}, {"type", "field"}, {"kind", COMPLETION_FIELD}});
                }
            }

            for (int i=0; i < iSizeSet; ++i)
            {
                if (added.insert(graphic_set[i]).second)
                {
                    fields.push_back({{"label", graphic_set[i]}, {"type", "field"}, {"kind", COMPLETION_FIELD}});
                }
            }

            freeArrayOfString(graphic_get, iSizeGet);
            freeArrayOfString(graphic_set, iSizeSet);

            pIT->killMe();
            return fields;
        }

        pIT->killMe();
        // continue in STRING case
    }
    else
    {
        delete tree;
        delete eval;
        // continue in STRING case
    }

    return {};
}

json getFileCompletion(const std::string& token)
{
    json items = json::array({});
    int iSize = 0;
    char** pcOut = nullptr;

    // file
    pcOut = completionOnFiles(token.data(), &iSize);
    for(int i = 0; i < iSize; ++i)
    {
        std::filesystem::path p(pcOut[i]);
        items.push_back({
            {"label", p.filename()},
            {"text", p.c_str()}, // DAP: text inserted after selection
            {"insertText", p.c_str()}, // LSP: text inserted after selection
            {"detail", p.parent_path()},
            {"type", "file"},
            {"kind", COMPLETION_FILE}
        });
    }
    freeArrayOfString(pcOut, iSize);

    return items;
}

// Shown on the right of a suggestion, where the items collected from the
// edited files say which file they come from.
static const char* const SCILAB_FUNCTION_DETAIL = "Scilab function";
static const char* const SCILAB_MACRO_DETAIL = "Scilab macro";
static const char* const SCILAB_VARIABLE_DETAIL = "Scilab variable";
static const char* const SCILAB_KEYWORD_DETAIL = "Scilab keyword";
static const char* const SCILAB_VALIDATOR_DETAIL = "Scilab validator";

json getDictCompletion(const std::string& token, const RuntimeSnapshot* snapshot)
{
    RuntimeTryLockGuard runtimeGuard;
    if (!runtimeGuard.ownsLock())
    {
        if (snapshot == nullptr || snapshot->empty())
        {
            return json::array({});
        }

        json items = json::array({});
        std::set<std::string> labels;
        auto matches = [&token](const std::string& label) {
            return token.empty() || label.rfind(token, 0) == 0;
        };
        auto appendItem = [&items, &labels, &matches](const std::string& label,
            const char* type, int kind, const char* detail) {
            if (!matches(label) || !labels.insert(label).second)
            {
                return;
            }
            json item = {{"label", label}, {"type", type}, {"kind", kind},
                {"detail", detail}};
            if (kind == COMPLETION_FUNCTION && isDeprecatedCompletion(label))
            {
                item["tags"] = {1};
            }
            items.push_back(std::move(item));
        };

        for (const RuntimeSymbolSnapshot& symbol : snapshot->symbols())
        {
            bool callable = symbol.kind == RuntimeSymbolKind::Function
                || symbol.kind == RuntimeSymbolKind::Macro;
            appendItem(symbol.name, callable ? "function" : "variable",
                callable ? COMPLETION_FUNCTION : COMPLETION_VARIABLE,
                symbol.kind == RuntimeSymbolKind::Macro ? SCILAB_MACRO_DETAIL
                    : callable ? SCILAB_FUNCTION_DETAIL : SCILAB_VARIABLE_DETAIL);
        }

        int size = 0;
        char** values = completionOnCommandWords(token.data(), &size);
        for (int index = 0; index < size; ++index)
        {
            appendItem(values[index], "keyword", COMPLETION_KEYWORD, SCILAB_KEYWORD_DETAIL);
        }
        freeArrayOfString(values, size);

        values = completionOnMustBe(token.data(), &size);
        for (int index = 0; index < size; ++index)
        {
            appendItem(values[index], "keyword", COMPLETION_KEYWORD, SCILAB_VALIDATOR_DETAIL);
        }
        freeArrayOfString(values, size);

        appendItem("SCI", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
        appendItem("SCIHOME", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
        appendItem("home", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
        appendItem("TMPDIR", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
#ifdef _MSC_VER
        appendItem("WSCI", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
#endif
        return items;
    }

    json items = json::array({});
    std::set<std::string> labels;
    int iSize = 0;
    char** pcOut = nullptr;

    auto appendItem = [&items, &labels](const std::string& label, const char* type, int kind,
        const char* detail) {
        if(kind == COMPLETION_FUNCTION)
        {
            symbol::Context* context = symbol::Context::getInstance();
            types::InternalType* value = context->get(symbol::Symbol(scilab::UTF8::toWide(label)));
            if(!isRuntimeValueAvailable(value))
            {
                return;
            }
        }

        if(labels.insert(label).second)
        {
            json item = {
                {"label", label},
                {"type", type},
                {"kind", kind},
                {"detail", detail}
            };
            if(kind == COMPLETION_FUNCTION && isDeprecatedCompletion(label))
            {
                item["tags"] = {1}; // LSP CompletionItemTag.Deprecated
            }
            items.push_back(std::move(item));
        }
    };

    // function
    pcOut = completionOnFunctions(token.data(), &iSize);
    for(int i = 0; i < iSize; ++i)
    {
        appendItem(pcOut[i], "function", COMPLETION_FUNCTION, SCILAB_FUNCTION_DETAIL);
    }
    freeArrayOfString(pcOut, iSize);

    // macros
    pcOut = completionOnMacros(token.data(), &iSize);
    for(int i = 0; i < iSize; ++i)
    {
        appendItem(pcOut[i], "function", COMPLETION_FUNCTION, SCILAB_MACRO_DETAIL);
    }
    freeArrayOfString(pcOut, iSize);

    // variables
    pcOut = completionOnVariables(token.data(), &iSize);
    for(int i = 0; i < iSize; ++i)
    {
        appendItem(pcOut[i], "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
    }
    freeArrayOfString(pcOut, iSize);

    // keywords
    pcOut = completionOnCommandWords(token.data(), &iSize);
    for(int i = 0; i < iSize; ++i)
    {
        appendItem(pcOut[i], "keyword", COMPLETION_KEYWORD, SCILAB_KEYWORD_DETAIL);
    }
    freeArrayOfString(pcOut, iSize);

    // mustbe
    pcOut = completionOnMustBe(token.data(), &iSize);
    for(int i = 0; i < iSize; ++i)
    {
        appendItem(pcOut[i], "keyword", COMPLETION_KEYWORD, SCILAB_VALIDATOR_DETAIL);
    }
    freeArrayOfString(pcOut, iSize);

    // static path
    appendItem("SCI", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
    appendItem("SCIHOME", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
    appendItem("home", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
    appendItem("TMPDIR", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
    #ifdef _MSC_VER
    appendItem("WSCI", "variable", COMPLETION_VARIABLE, SCILAB_VARIABLE_DETAIL);
    #endif

    return items;
}

json getSignatures(const std::string& _word, int _activeParam, const std::string& _sheetFile)
{
    json signatures = json::array();
    if (loadInlineHelpStyleSheet(_sheetFile.c_str()) == FALSE)
    {
        std::cout << "Load Style Sheet failed" << std::endl;
        return signatures;
    }

    wchar_t* rawContent = nullptr;
    int err = getInlineHelp(scilab::UTF8::toWide(_word).c_str(), &rawContent, nullptr);
    if(err)
    {
        return signatures;
    }

    std::wstring content(rawContent ? rawContent : L"");
    FREE(rawContent);

    json help;
    try
    {
        help = json::parse(scilab::UTF8::toUTF8(content));
    }
    catch(const json::parse_error& e)
    {
        std::cout << "exception " << e.id << std::endl;
        std::cout << e.what() << " at position " << e.byte << std::endl;
        return signatures;
    }

    if(help.empty())
    {
        return signatures;
    }

    help = help[0];
    // std::cout << help.dump(2, ' ') << std::endl;

    auto trim = [](std::string value) {
        const char* whitespace = " \t\r\n";
        size_t first = value.find_first_not_of(whitespace);
        if (first == std::string::npos)
        {
            return std::string();
        }
        size_t last = value.find_last_not_of(whitespace);
        return value.substr(first, last - first + 1);
    };

    std::map<std::string, std::string> parameters;
    for(const auto& var : help["parameters"])
    {
        // DocBook may group argument aliases in one term (for example
        // "m1, m2, ..."); map each alias to the shared description.
        std::string names = var["name"];
        std::string description = var["description"];
        size_t begin = 0;
        while (begin <= names.size())
        {
            size_t separator = names.find_first_of(",|", begin);
            std::string alias = trim(names.substr(begin, separator - begin));
            size_t ellipsis = alias.find("...");
            if (ellipsis != std::string::npos)
            {
                alias = trim(alias.substr(0, ellipsis));
            }
            if (!alias.empty())
            {
                parameters[alias] = description;
            }
            if (separator == std::string::npos)
            {
                break;
            }
            begin = separator + 1;
        }
    }

    const std::regex arg_sep_reg("[^, \r\n|\n]*");
    for(const auto& var : help["synopsis"])
    {
        std::string proto(var);
        json params = json::array();

        size_t beg = proto.find("(") + 1;
        size_t end = proto.find(")");
        if(beg < end) // one or more arguments
        {
            std::string args = proto.substr(beg, end - beg);
            if (std::count(args.begin(), args.end(), ',') < _activeParam)
            {
                // skip proto with less argument than the actual position
                continue;
            }

            auto words_beg = std::sregex_iterator(args.begin(), args.end(), arg_sep_reg);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_beg; i != words_end; ++i)
            {
                if((*i).str().empty() == false)
                {
                    std::string arg_name = (*i).str();
                    params.push_back({
                        {"label", arg_name },
                        {"documentation", {
                            { "kind", "markdown"},
                            { "value", parameters[arg_name] }
                        }}
                    });
                }
            }
        }
        else if(_activeParam >= 0)
        {
            // skip zero argument proto when asking for at least one arg
            continue;
        }

        json signature = {
            {"label", var},
            {"parameters", params}
        };
        if (help.contains("purpose") && help["purpose"].is_string())
        {
            signature["documentation"] = {
                { "kind", "markdown" },
                { "value", help["purpose"] }
            };
        }
        signatures.push_back(signature);
    }

    return signatures;
}

std::string getDoc(const std::string& _word, const std::string& _sheetFile)
{
    if (loadInlineHelpStyleSheet(_sheetFile.c_str()) == FALSE)
    {
        return "Load Style Sheet failed";
    }

    std::wstring word = scilab::UTF8::toWide(_word);
    wchar_t* rawContent = nullptr;
    BOOL isScilab = FALSE;
    int err = getInlineHelp(word.c_str(), &rawContent, &isScilab);
    if(err)
    {
        return "";
    }

    std::wstring content(rawContent ? rawContent : L"");
    FREE(rawContent);

    if(isScilab)
    {
        content += L"[Open documentation](scilab-help://" + word + L") · F1";
    }
    else
    {
        content += L"F1 for the full help";
    }

    return scilab::UTF8::toUTF8(content);
}

std::string eval(const std::string& _command, types::InternalType** _pIT)
{
    ast::Exp* tree = nullptr;
    char* error = parseCommand(_command.data(), (void**)&tree);
    if (error)
    {
        delete tree;
        std::string err(error);
        FREE(error);
        return err;
    }

    // a tree always start by a SeqExp and the SepExp of EvalVisitor catch all exeptions
    ast::EvalVisitor* eval = new ast::EvalVisitor();
    tree->accept(*eval);
    *_pIT = eval->getResult();
    std::string err(eval->getLastError());

    if(*_pIT)
    {
        // avoid double delete when pIT is held by the tree and the visitor
        (*_pIT)->IncreaseRef();
    }

    delete tree;
    delete eval;

    if(*_pIT)
    {
        (*_pIT)->DecreaseRef();
    }
    return err;
}

static std::string buildDocumentContent(const std::vector<std::string>& lines)
{
    std::stringstream content;
    for (const auto& line : lines)
    {
        content << line << std::endl;
    }

    return content.str();
}

bool parseDocument(const std::vector<std::string>& lines, ast::Exp** tree)
{
    std::string content = buildDocumentContent(lines);
    char* error = parseCommand(content.data(), reinterpret_cast<void**>(tree));
    if (error)
    {
        FREE(error);
        return false;
    }

    return true;
}

bool parseDocumentWithRecovery(const std::vector<std::string>& lines, ast::Exp** tree)
{
    return parseDocumentWithRecovery(lines, tree, nullptr);
}

bool parseDocumentWithRecovery(const std::vector<std::string>& lines, ast::Exp** tree, std::string* errorMessage)
{
    std::string content = buildDocumentContent(lines);
    char* error = parseCommandWithRecovery(content.data(), reinterpret_cast<void**>(tree));
    if (error)
    {
        if (errorMessage != nullptr)
        {
            *errorMessage = error;
        }

        FREE(error);
        return *tree != nullptr;
    }

    if (errorMessage != nullptr)
    {
        errorMessage->clear();
    }

    return true;
}

bool parseDocumentWithRecoveryWithoutRuntime(const std::vector<std::string>& lines,
    ast::Exp** tree, std::string* errorMessage)
{
    std::string content = buildDocumentContent(lines);
    char* error = parseCommandWithRecoveryWithoutRuntime(content.data(),
        reinterpret_cast<void**>(tree));
    if (error)
    {
        if (errorMessage != nullptr)
        {
            *errorMessage = error;
        }

        FREE(error);
        return *tree != nullptr;
    }

    if (errorMessage != nullptr)
    {
        errorMessage->clear();
    }

    return true;
}

// _character is a 0-based column; std::string::npos asks for the first word of the line.
std::string getWord(const std::string& _line, size_t _character)
{
    std::smatch scilab_word;
    const std::regex scilab_word_reg("[$!%\\w]*");
    auto words_beg = std::sregex_iterator(_line.begin(), _line.end(), scilab_word_reg);
    auto words_end = std::sregex_iterator();
    std::string word;
    for (std::sregex_iterator i = words_beg; i != words_end; ++i)
    {
        std::smatch match = *i;
        const size_t matchPosition = static_cast<size_t>(match.position(0));
        if (_character == std::string::npos
                || (matchPosition <= _character
                    && _character < matchPosition + match.length(0)))
        {
            word = match.str();
            break;
        }
    }
    return word;
}

json convertLocation(Location _loc)
{
    return {
        {"start", {
            {"line", _loc.first_line - 1},
            {"character", _loc.first_column - 1}
        }},
        {"end", {
            {"line", _loc.last_line - 1},
            {"character", _loc.last_column - 1}
        }},
    };
}

std::string getStructFieldsSummary(types::String* fields)
{
    if(fields == nullptr || fields->getSize() == 0)
    {
        return "";
    }

    std::string summary = " with fields: ";
    for(int i = 0; i < fields->getSize(); ++i)
    {
        if(i > 0)
        {
            summary += ", ";
        }
        summary += scilab::UTF8::toUTF8(fields->get(i));
    }

    return summary;
}

std::string getSizeSummary(types::InternalType* pIT)
{
    std::wostringstream os;
    os << printTypeDimsInfo(pIT) << std::endl;
    std::string summary = scilab::UTF8::toUTF8(os.str());
    std::smatch size_match;
    if(std::regex_search(summary, size_match, std::regex("(\\[.*\\])")))
    {
        return size_match[0].str();
    }

    return summary;
}

std::string getStructuredVariableSummary(types::InternalType* pIT, bool includeFields)
{
    switch(pIT->getType())
    {
        case types::InternalType::ScilabSingleStruct:
        {
            if(includeFields)
            {
                return "struct" + getStructFieldsSummary(pIT->getAs<types::SingleStruct>()->getFieldNames());
            }

            return "struct";
        }
        case types::InternalType::ScilabStruct:
        {
            types::Struct* pStr = pIT->getAs<types::Struct>();
            types::String* pFields = pStr->getFieldNames();
            if(pStr->getSize() == 0 || pFields == nullptr || pFields->getSize() == 0)
            {
                return "empty struct";
            }

            if(pStr->isScalar() == false)
            {
                return getSizeSummary(pIT);
            }

            if(includeFields)
            {
                return "struct" + getStructFieldsSummary(pFields);
            }

            return "struct";
        }
        case types::InternalType::ScilabMList:
        case types::InternalType::ScilabTList:
        {
            types::String* pFields = pIT->getAs<types::TList>()->getFieldNames();
            if(pFields && pFields->getSize() > 0)
            {
                return scilab::UTF8::toUTF8(pFields->get(0));
            }

            return scilab::UTF8::toUTF8(pIT->getTypeStr());
        }
        case types::InternalType::ScilabList:
        {
            types::GenericType* pGT = pIT->getAs<types::GenericType>();
            if(pGT->getSize() == 0)
            {
                return "empty list";
            }

            return getSizeSummary(pIT);
        }
        case types::InternalType::ScilabCell:
        {
            types::GenericType* pGT = pIT->getAs<types::GenericType>();
            if(pGT->getSize() == 0)
            {
                return "empty cell";
            }

            return "cell";
        }
        default:
            return "";
    }
}

std::string getVariableDisplayValue(types::InternalType* pIT)
{
    if(pIT == nullptr)
    {
        return "";
    }

    std::string structuredSummary = getStructuredVariableSummary(pIT, true);
    if(structuredSummary.empty() == false)
    {
        return structuredSummary;
    }

    std::wostringstream os;
    if(pIT->isGenericType() == false)
    {
        pIT->toString(os);
        pIT->clearPrintState();
        return scilab::UTF8::toUTF8(os.str());
    }

    types::GenericType* pGT = pIT->getAs<types::GenericType>();
    if(pGT->getSize() > 1)
    {
        os << printTypeDimsInfo(pIT) << std::endl;
    }

    pGT->toString(os);
    pIT->clearPrintState();
    std::string value = scilab::UTF8::toUTF8(os.str());
    if(pGT->getSize() < 2)
    {
        const std::regex blanks("^[ ]*");
        value = std::regex_replace(value, blanks, "");
    }

    return value;
}
