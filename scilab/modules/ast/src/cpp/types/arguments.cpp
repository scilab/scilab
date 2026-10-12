/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include <algorithm>
#include <cstdarg>
#include <cwctype>

#include "arguments.hxx"
#include "argumentvisitor.hxx"
#include "configvariable.hxx"
#include "runvisitor.hxx"
#include "context.hxx"
#include "object.hxx"
#include "overload.hxx"
#include "opexp.hxx"
#include "fieldexp.hxx"
#include "simplevar.hxx"
#include "int.hxx"
#include "string.hxx"
#include "symbol.hxx"
#include "double.hxx"
#include "types_comparison_eq.hxx"
#include "types_comparison_lt_le_gt_ge.hxx"
#include "types_comparison_ne.hxx"
#include "tlist.hxx"
#include "colon.hxx"

extern "C"
{
#include "FileExist.h"
#include "Scierror.h"
#include "Sciwarning.h"
#include "expandPathVariable.h"
#include "graphicObjectProperties.h"
#include "hasHandleType.h"
#include "hasUIControlStyle.h"
#include "isdir.h"
}

std::wstring var2str(types::InternalType* pIT)
{
    // objects have no expression: their class name
    if (pIT->isObject())
    {
        return pIT->getTypeStr();
    }

    types::InternalType* pCall = symbol::Context::getInstance()->get(symbol::Symbol(L"sci2exp"));
    if (pCall && pCall->isCallable())
    {
        std::wostringstream ostr;
        types::typed_list in = {pIT};
        types::optional_list opt;
        types::typed_list out;

        pIT->IncreaseRef();
        types::Function::ReturnValue ret = types::Function::Error;
        try
        {
            ret = pCall->getAs<types::Callable>()->call(in, opt, 1, out);
        }
        catch (const ast::InternalError&)
        {
            // type not managed by sci2exp: its name
            pIT->DecreaseRef();
            return pIT->getTypeStr();
        }

        if (ret == types::Function::OK)
        {
            if (out.size() == 1 && out[0]->isString())
            {
                std::wstring strOut(out[0]->getAs<types::String>()->get(0));
                out[0]->killMe();
                pIT->DecreaseRef();
                return strOut;
            }
        }

        pIT->toString(ostr);
        pIT->DecreaseRef();
        return ostr.str();
    }

    return L"";
}

std::wstring formatError(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    va_list args2;
    va_copy(args2, args);
    int size = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);

    std::vector<char> buf(size > 0 ? size + 1 : 1, '\0');
    if (size > 0)
    {
        vsnprintf(buf.data(), buf.size(), fmt, args2);
    }
    va_end(args2);

    return scilab::UTF8::toWide(buf.data());
}

bool mustBeEmpty(types::InternalType* x) { return x->isDouble() && x->getAs<types::Double>()->isEmpty(); }
bool mustBeDouble(types::InternalType* x) { return x->isDouble(); }
bool mustBeString(types::InternalType* x) { return x->isString(); }
bool mustBeBool(types::InternalType* x) { return x->isBool(); }
bool mustBeInt(types::InternalType* x) { return x->isInt(); }
bool mustBeInt8(types::InternalType* x) { return x->isInt8(); }
bool mustBeUInt8(types::InternalType* x) { return x->isUInt8(); }
bool mustBeInt16(types::InternalType* x) { return x->isInt16(); }
bool mustBeUInt16(types::InternalType* x) { return x->isUInt16(); }
bool mustBeInt32(types::InternalType* x) { return x->isInt32(); }
bool mustBeUInt32(types::InternalType* x) { return x->isUInt32(); }
bool mustBeInt64(types::InternalType* x) { return x->isInt64(); }
bool mustBeUInt64(types::InternalType* x) { return x->isUInt64(); }
bool mustBePoly(types::InternalType* x) { return x->isPoly(); }
bool mustBeList(types::InternalType* x) { return x->isList(); }
bool mustBeTList(types::InternalType* x) { return x->isTList(); }
bool mustBeMList(types::InternalType* x) { return x->isMList(); }
bool mustBePointer(types::InternalType* x) { return x->isPointer(); }
bool mustBeHandle(types::InternalType* x) { return x->isHandle(); }
bool mustBeStruct(types::InternalType* x) { return x->isStruct(); }
bool mustBeCell(types::InternalType* x) { return x->isCell(); }
bool mustBeLib(types::InternalType* x) { return x->isLibrary(); }
bool mustBeFunction(types::InternalType* x) { return x->isCallable(); }
bool mustBeBuiltin(types::InternalType* x) { return x->isFunction(); }
bool mustBeMacro(types::InternalType* x) { return x->isMacro() || x->isMacroFile(); }
bool mustBeSparse(types::InternalType* x) { return x->isSparse(); }
bool mustBeBoolSparse(types::InternalType* x) { return x->isSparseBool(); }
bool mustBeImplicitList(types::InternalType* x) { return x->isImplicitList(); }
template<int go> bool mustBeHandleType(types::InternalType* x) { return x->isHandle() && hasHandleType(x->getAs<types::GraphicHandle>()->get()[0], go); }
template<int go> bool mustBeUIControlStyle(types::InternalType* x) { return mustBeHandleType<__GO_UICONTROL__>(x) && hasUIControlStyle(x->getAs<types::GraphicHandle>()->get()[0], go); }

std::map<std::wstring, std::function<bool(types::InternalType*)>> typeValidator = {
    {L"empty", mustBeEmpty}, {L"double", mustBeDouble}, {L"constant", mustBeDouble}, {L"bool", mustBeBool}, {L"boolean", mustBeBool}, {L"string", mustBeString}, {L"int", mustBeInt},
    {L"int8", mustBeInt8}, {L"uint8", mustBeUInt8}, {L"int16", mustBeInt16}, {L"uint16", mustBeUInt16}, {L"int32", mustBeInt32}, {L"uint32", mustBeUInt32},
    {L"int64", mustBeInt64}, {L"uint64", mustBeUInt64}, {L"poly", mustBePoly}, {L"polynomial", mustBePoly}, {L"list", mustBeList}, {L"tlist", mustBeTList},
    {L"mlist", mustBeMList}, {L"pointer", mustBePointer}, {L"handle", mustBeHandle}, {L"struct", mustBeStruct}, {L"st", mustBeStruct}, {L"cell", mustBeCell},
    {L"library", mustBeLib}, {L"lib", mustBeLib}, {L"function", mustBeFunction}, {L"builtin", mustBeBuiltin}, {L"gateway", mustBeBuiltin}, {L"macro", mustBeMacro},
    {L"sparse", mustBeSparse}, {L"booleansparse", mustBeBoolSparse}, {L"boolsparse", mustBeBoolSparse}, {L"implicitlist", mustBeImplicitList}, {L"range", mustBeImplicitList},
    {L"axes", mustBeHandleType<__GO_AXES__>}, {L"axis", mustBeHandleType<__GO_AXIS__>}, {L"champ", mustBeHandleType<__GO_CHAMP__>}, {L"compound", mustBeHandleType<__GO_COMPOUND__>},
    {L"fac3d", mustBeHandleType<__GO_FAC3D__>}, {L"fec", mustBeHandleType<__GO_FEC__>}, {L"figure", mustBeHandleType<__GO_FIGURE__>},  {L"grayplot", mustBeHandleType<__GO_GRAYPLOT__>},
    {L"label", mustBeHandleType<__GO_LABEL__>}, {L"legend", mustBeHandleType<__GO_LEGEND__>}, {L"matplot", mustBeHandleType<__GO_MATPLOT__>}, {L"plot3d", mustBeHandleType<__GO_PLOT3D__>},
    {L"polyline", mustBeHandleType<__GO_POLYLINE__>}, {L"rect", mustBeHandleType<__GO_RECTANGLE__>}, {L"segs", mustBeHandleType<__GO_SEGS__>}, /*{L"text", mustBeHandleType<__GO_TEXT__>},*/
    {L"uicontrol", mustBeHandleType<__GO_UICONTROL__>}, {L"uimenu", mustBeHandleType<__GO_UIMENU__>},
    {L"checkbox", mustBeUIControlStyle<__GO_UI_CHECKBOX__>}, {L"edit", mustBeUIControlStyle<__GO_UI_EDIT__>}, {L"spinner", mustBeUIControlStyle<__GO_UI_SPINNER__>},
    {L"frame", mustBeUIControlStyle<__GO_UI_FRAME__>}, {L"image", mustBeUIControlStyle<__GO_UI_IMAGE__>}, {L"listbox", mustBeUIControlStyle<__GO_UI_LISTBOX__>},
    {L"popupmenu", mustBeUIControlStyle<__GO_UI_POPUPMENU__>}, {L"pushbutton", mustBeUIControlStyle<__GO_UI_PUSHBUTTON__>}, {L"radiobutton", mustBeUIControlStyle<__GO_UI_RADIOBUTTON__>},
    {L"slider", mustBeUIControlStyle<__GO_UI_SLIDER__>}, {L"text", mustBeUIControlStyle<__GO_UI_TEXT__>}, {L"layer", mustBeUIControlStyle<__GO_UI_LAYER__>},
    {L"tab", mustBeUIControlStyle<__GO_UI_TAB__>}
};

static types::InternalType* callComparison(std::function<types::InternalType*(types::InternalType*, types::InternalType*)> cmp, ast::OpExp::Oper oper, types::InternalType* x, types::InternalType* y)
{
    types::InternalType* pIT = cmp(x, y);
    if (pIT == nullptr)
    {
        x->IncreaseRef();
        y->IncreaseRef();
        types::typed_list in = {x, y};
        types::typed_list out;
        types::Function::ReturnValue ret = Overload::generateNameAndCall(Overload::getNameFromOper(oper), in, 1, out, true);
        x->DecreaseRef();
        y->DecreaseRef();
        if (ret == types::Function::ReturnValue::OK)
        {
            return out[0];
        }
    }

    return pIT;
}

static types::InternalType* callComparison(std::function<types::InternalType*(types::InternalType*, types::InternalType*, const std::wstring&)> cmp, ast::OpExp::Oper oper, const std::wstring& operstr, types::InternalType* x, types::InternalType* y)
{
    types::InternalType* pIT = cmp(x, y, operstr);
    if (pIT == nullptr)
    {
        x->IncreaseRef();
        y->IncreaseRef();
        types::typed_list in = {x, y};
        types::typed_list out;
        types::Function::ReturnValue ret = Overload::generateNameAndCall(Overload::getNameFromOper(oper), in, 1, out, true);
        x->DecreaseRef();
        y->DecreaseRef();
        if (ret == types::Function::ReturnValue::OK)
        {
            return out[0];
        }
    }

    return pIT;
}

static bool callIsFunction(const wchar_t* name, types::typed_list& x)
{
    types::typed_list in = {x[0]};
    types::typed_list out;
    if (Overload::call(name, in, 1, out) != types::Function::OK)
    {
        return false;
    }

    bool bIsTrue = out[0]->getAs<types::Bool>()->isTrue();
    out[0]->killMe();
    return bIsTrue;
}

template<class T_OUT, class T_IN>
types::InternalType* convertNum(types::InternalType* val)
{
    T_IN* in = val->getAs<T_IN>();
    T_OUT* out = new T_OUT(in->getDims(), in->getDimsArray());
    typename T_OUT::type* pout = out->get();
    typename T_IN::type* pin = in->get();
    for (int i = 0; i < in->getSize(); ++i)
    {
        pout[i] = static_cast<typename T_OUT::type>(pin[i]);
    }

    return out;
}

// the whole string must be a number (surrounding spaces allowed)
static bool parseDouble(const wchar_t* str, double& d)
{
    wchar_t* end = nullptr;
    d = wcstod(str, &end);
    if (end == str)
    {
        return false;
    }

    while (iswspace(*end))
    {
        ++end;
    }

    return *end == L'\0';
}

// non zero is true
template<class T_IN>
types::InternalType* numToBool(types::InternalType* val)
{
    T_IN* in = val->getAs<T_IN>();
    types::Bool* out = new types::Bool(in->getDims(), in->getDimsArray());
    int* pout = out->get();
    typename T_IN::type* pin = in->get();
    for (int i = 0; i < in->getSize(); ++i)
    {
        pout[i] = pin[i] != 0 ? 1 : 0;
    }

    return out;
}

template<class T>
types::InternalType* stringToNum(types::String* in, const std::wstring& name, const char* type)
{
    T* out = new T(in->getDims(), in->getDimsArray());
    typename T::type* pout = out->get();
    for (int i = 0; i < in->getSize(); ++i)
    {
        double d = 0;
        if (parseDouble(in->get()[i], d) == false)
        {
            out->killMe();
            throw ast::InternalError(formatError(_("%ls: Unable to convert \"%ls\" to %s.\n"), name.data(), in->get()[i], type));
        }

        pout[i] = static_cast<typename T::type>(d);
    }

    return out;
}

types::InternalType* toDouble(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabDouble:
            return val;
        case types::InternalType::ScilabBool:
            return convertNum<types::Double, types::Bool>(val);
        case types::InternalType::ScilabInt8:
            return convertNum<types::Double, types::Int8>(val);
        case types::InternalType::ScilabUInt8:
            return convertNum<types::Double, types::UInt8>(val);
        case types::InternalType::ScilabInt16:
            return convertNum<types::Double, types::Int16>(val);
        case types::InternalType::ScilabUInt16:
            return convertNum<types::Double, types::UInt16>(val);
        case types::InternalType::ScilabInt32:
            return convertNum<types::Double, types::Int32>(val);
        case types::InternalType::ScilabUInt32:
            return convertNum<types::Double, types::UInt32>(val);
        case types::InternalType::ScilabInt64:
            return convertNum<types::Double, types::Int64>(val);
        case types::InternalType::ScilabUInt64:
            return convertNum<types::Double, types::UInt64>(val);
        case types::InternalType::ScilabString:
            return stringToNum<types::Double>(val->getAs<types::String>(), name, "double");
        default:
        {
            throw ast::InternalError(formatError(_("%ls: Unable to convert '%ls' to double.\n"), name.data(), val->getTypeStr().data()));
        }
    }
}

template<class T>
types::InternalType* toInt(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabDouble:
            return convertNum<T, types::Double>(val);
        case types::InternalType::ScilabBool:
            return convertNum<T, types::Bool>(val);
        case types::InternalType::ScilabInt8:
            return convertNum<T, types::Int8>(val);
        case types::InternalType::ScilabUInt8:
            return convertNum<T, types::UInt8>(val);
        case types::InternalType::ScilabInt16:
            return convertNum<T, types::Int16>(val);
        case types::InternalType::ScilabUInt16:
            return convertNum<T, types::UInt16>(val);
        case types::InternalType::ScilabInt32:
            return convertNum<T, types::Int32>(val);
        case types::InternalType::ScilabUInt32:
            return convertNum<T, types::UInt32>(val);
        case types::InternalType::ScilabInt64:
            return convertNum<T, types::Int64>(val);
        case types::InternalType::ScilabUInt64:
            return convertNum<T, types::UInt64>(val);
        case types::InternalType::ScilabString:
            return stringToNum<T>(val->getAs<types::String>(), name, "int");
        default:
        {
            throw ast::InternalError(formatError(_("%ls: Unable to convert '%ls' to int.\n"), name.data(), val->getTypeStr().data()));
        }
    }
}

types::InternalType* toBool(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabBool:
            return val;
        case types::InternalType::ScilabDouble:
        {
            types::Double* d = val->getAs<types::Double>();
            if (std::any_of(d->get(), d->get() + d->getSize(), [](double v) { return std::isnan(v); }))
            {
                throw ast::InternalError(formatError(_("%ls: Unable to convert NaN to boolean.\n"), name.data()));
            }

            return numToBool<types::Double>(val);
        }
        case types::InternalType::ScilabInt8:
            return numToBool<types::Int8>(val);
        case types::InternalType::ScilabUInt8:
            return numToBool<types::UInt8>(val);
        case types::InternalType::ScilabInt16:
            return numToBool<types::Int16>(val);
        case types::InternalType::ScilabUInt16:
            return numToBool<types::UInt16>(val);
        case types::InternalType::ScilabInt32:
            return numToBool<types::Int32>(val);
        case types::InternalType::ScilabUInt32:
            return numToBool<types::UInt32>(val);
        case types::InternalType::ScilabInt64:
            return numToBool<types::Int64>(val);
        case types::InternalType::ScilabUInt64:
            return numToBool<types::UInt64>(val);
        default:
        {
            throw ast::InternalError(formatError(_("%ls: Unable to convert '%ls' to boolean.\n"), name.data(), val->getTypeStr().data()));
        }
    }
}

template <class T>
types::String* toStringNum(T* in)
{
    types::String* s = new types::String(in->getDims(), in->getDimsArray());
    typename T::type* pin = in->get();

    for (int i = 0; i < in->getSize(); ++i)
    {
        s->set(i, std::to_wstring(pin[i]).c_str());
    }

    return s;
}

types::InternalType* toStringNum(types::Double* d)
{
    if (d->isEmpty())
    {
        return d;
    }

    types::String* s = new types::String(d->getDims(), d->getDimsArray());
    if (d->isComplex())
    {
        std::wostringstream ostr;
        double* pR = d->get();
        double* pI = d->getImg();
        for (int i = 0; i < d->getSize(); ++i)
        {
            DoubleComplexMatrix2String(&ostr, pR[i], pI[i]);
            s->set(i, ostr.str().c_str());
            ostr.str(L"");
        }
    }
    else
    {
        std::wostringstream ostr;
        double* pR = d->get();
        for (int i = 0; i < d->getSize(); ++i)
        {
            DoubleComplexMatrix2String(&ostr, pR[i], 0);
            s->set(i, ostr.str().c_str());
            ostr.str(L"");
        }
    }

    return s;
}

types::InternalType* toStringBool(types::Bool* b)
{
    types::String* s = new types::String(b->getDims(), b->getDimsArray());
    int* pin = b->get();
    const wchar_t* True = L"T";
    const wchar_t* False = L"F";
    for (int i = 0; i < b->getSize(); ++i)
    {
        s->set(i, pin[i] == 0 ? False : True);
    }

    return s;
}

types::InternalType* toString(types::InternalType* val, const std::wstring& name)
{
    switch (val->getType())
    {
        case types::InternalType::ScilabDouble:
            return toStringNum(val->getAs<types::Double>());
        case types::InternalType::ScilabInt8:
            return toStringNum(val->getAs<types::Int8>());
        case types::InternalType::ScilabUInt8:
            return toStringNum(val->getAs<types::UInt8>());
        case types::InternalType::ScilabInt16:
            return toStringNum(val->getAs<types::Int16>());
        case types::InternalType::ScilabUInt16:
            return toStringNum(val->getAs<types::UInt16>());
        case types::InternalType::ScilabInt32:
            return toStringNum(val->getAs<types::Int32>());
        case types::InternalType::ScilabUInt32:
            return toStringNum(val->getAs<types::UInt32>());
        case types::InternalType::ScilabInt64:
            return toStringNum(val->getAs<types::Int64>());
        case types::InternalType::ScilabUInt64:
            return toStringNum(val->getAs<types::UInt64>());
        case types::InternalType::ScilabBool:
            return toStringBool(val->getAs<types::Bool>());
        case types::InternalType::ScilabString:
            return val;
        default:
        {
            throw ast::InternalError(formatError(_("%ls: Unable to convert '%ls' to string.\n"), name.data(), val->getTypeStr().data()));
        }
    }
}

std::map<std::wstring, std::function<types::InternalType*(types::InternalType*, const std::wstring& name)>> typeConvertors = {
    {L"double", toDouble},
    {L"constant", toDouble},
    {L"int", toInt<types::Int32>},
    {L"uint", toInt<types::UInt32>},
    {L"int8", toInt<types::Int8>},
    {L"uint8", toInt<types::UInt8>},
    {L"int16", toInt<types::Int16>},
    {L"uint16", toInt<types::UInt16>},
    {L"int32", toInt<types::Int32>},
    {L"uint32", toInt<types::UInt32>},
    {L"int64", toInt<types::Int64>},
    {L"uint64", toInt<types::UInt64>},
    {L"bool", toBool},
    {L"boolean", toBool},
    {L"string", toString}
};

std::function<types::InternalType*(types::InternalType*, const std::wstring& name)> getTypeConvertor(const std::wstring& name)
{
    if (typeConvertors.find(name) == typeConvertors.end())
    {
        return nullptr;
    }

    return typeConvertors[name];
}

// this, or field of a variable (this.max, opts.max, MyClass.max): evaluated at each call
bool isCallTimeArgument(ast::Exp* e)
{
    if (e->isSimpleVar())
    {
        return e->getAs<ast::SimpleVar>()->getSymbol().getName() == L"this";
    }

    if (e->isFieldExp() == false)
    {
        return false;
    }

    while (e->isFieldExp())
    {
        ast::FieldExp* f = e->getAs<ast::FieldExp>();
        if (f->getTail()->isSimpleVar() == false)
        {
            return false;
        }

        e = f->getHead();
    }

    return e->isSimpleVar();
}

// source of a call time argument: this or a.b.c
std::wstring callTimeArgumentText(ast::Exp* e)
{
    if (e->isFieldExp())
    {
        ast::FieldExp* f = e->getAs<ast::FieldExp>();
        return callTimeArgumentText(f->getHead()) + L"." + f->getTail()->getAs<ast::SimpleVar>()->getSymbol().getName();
    }

    return e->getAs<ast::SimpleVar>()->getSymbol().getName();
}

std::wstring callTimeArgumentRoot(ast::Exp* e)
{
    while (e->isFieldExp())
    {
        e = e->getAs<ast::FieldExp>()->getHead();
    }

    return e->getAs<ast::SimpleVar>()->getSymbol().getName();
}

namespace
{
// constant parameter of a validator, evaluated when it is defined
types::InternalType* evalConstant(ast::Exp* e, const std::string& where)
{
    ast::ArgumentVisitor check;
    e->accept(check);
    if (check.getStatus() == false)
    {
        throw ast::InternalError(formatError(_("%s: argument must be constant expression.\n"), where.data()), 999, e->getLocation());
    }

    std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
    types::InternalType* pIT = nullptr;
    try
    {
        e->accept(*exec);
        pIT = ((ast::RunVisitor*)exec.get())->getResult();
    }
    catch (const ast::InternalError&)
    {
    }

    if (pIT == nullptr || pIT->isAssignable() == false)
    {
        throw ast::InternalError(formatError(_("%s: Unable to evaluate argument.\n"), where.data()), 999, e->getLocation());
    }

    // kept by the validator
    pIT->IncreaseRef();
    return pIT;
}
}

ARG_VALIDATOR buildValidator(ast::Exp* v, const ValidatorContext& ctx)
{
    const char* where = ctx.where.data();
    ARG_VALIDATOR validator;
    std::wstring name;
    std::vector<int> rhs;

    if (v->isSimpleVar())
    {
        // short form: the checked value is the only parameter
        name = v->getAs<ast::SimpleVar>()->getSymbol().getName();
        std::tie(validator.validator, rhs) = getFunctionValidator(name);
        if (validator.validator == nullptr)
        {
            throw ast::InternalError(formatError(_("%s: Unknown validation function '%s'\n"), where, scilab::UTF8::toUTF8(name).data()), 999, v->getLocation());
        }

        if (rhs[0] != 1)
        {
            throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), where, 1), 999, v->getLocation());
        }

        validator.inputs.push_back({ctx.self, nullptr});
    }
    else if (v->isCallExp() && v->getAs<ast::CallExp>()->getName().isSimpleVar())
    {
        ast::CallExp* c = v->getAs<ast::CallExp>();
        name = c->getName().getAs<ast::SimpleVar>()->getSymbol().getName();
        std::tie(validator.validator, rhs) = getFunctionValidator(name);
        if (validator.validator == nullptr)
        {
            throw ast::InternalError(formatError(_("%s: \"%ls\" is not a validation function.\n"), where, name.data()), 999, v->getLocation());
        }

        int size = static_cast<int>(c->getArgs().size());
        if (rhs.size() == 1 && rhs[0] != size && rhs[0] != -1)
        {
            throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), where, rhs[0]), 999, v->getLocation());
        }

        if (rhs.size() > 1 && (size < rhs[0] || size > rhs[1]))
        {
            throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): between %d and %d expected.\n"), where, rhs[0], rhs[1]), 999, v->getLocation());
        }

        for (auto&& in : c->getArgs())
        {
            // an input
            if (in->isSimpleVar() && ctx.input(in->getAs<ast::SimpleVar>()->getSymbol().getName()) != -1)
            {
                validator.inputs.push_back({ctx.input(in->getAs<ast::SimpleVar>()->getSymbol().getName()), nullptr});
                continue;
            }

            // evaluated when checked: this, field of this, of an input or of a class
            if (isCallTimeArgument(in) && (ctx.property == false || ctx.input(callTimeArgumentRoot(in)) == -1))
            {
                if (ctx.property)
                {
                    // a check would not follow the changes of the object
                    if (callTimeArgumentRoot(in) == L"this")
                    {
                        throw ast::InternalError(formatError(_("%s: The validation of a property can not use the object.\n"), where), 999, in->getLocation());
                    }

                    validator.callTime[static_cast<int>(validator.inputs.size())] = in->clone();
                }
                else
                {
                    // part of the body of the function
                    validator.callTime[static_cast<int>(validator.inputs.size())] = in;
                }

                validator.inputs.push_back({-2, nullptr});
                continue;
            }

            // in a function, a name must be an input or a predefined constant
            if (ctx.property == false && in->isSimpleVar())
            {
                std::vector<std::wstring> allowedVar = {L"%eps", L"%i", L"%inf", L"%nan"};
                std::wstring var = in->getAs<ast::SimpleVar>()->getSymbol().getName();
                if (std::find(allowedVar.begin(), allowedVar.end(), var) == allowedVar.end())
                {
                    throw ast::InternalError(formatError(_("%s: Identifier '%s' must be an input argument.\n"), where, scilab::UTF8::toUTF8(var).data()), 999, in->getLocation());
                }
            }

            validator.inputs.push_back({-1, evalConstant(in, ctx.where)});
        }
    }
    else
    {
        throw ast::InternalError(formatError(_("%s: argument must be constant expression.\n"), where), 999, v->getLocation());
    }

    // error message parameters: #position (functions), the content of a constant,
    // or the content of the validator parameter when checked
    validator.error = ctx.property ? getPropertyErrorValidator(name) : getErrorValidator(name);
    for (auto&& e : getErrorArgs(name))
    {
        int idx = std::get<0>(e);
        if (std::abs(idx) >= static_cast<int>(validator.inputs.size()))
        {
            continue;
        }

        int index = std::get<0>(validator.inputs[std::abs(idx)]);
        types::InternalType* pIT = std::get<1>(validator.inputs[std::abs(idx)]);
        if (idx > 0 && ctx.property == false && index == -2)
        {
            validator.errorArgs.push_back({-1, scilab::UTF8::toUTF8(callTimeArgumentText(validator.callTime[idx]))});
        }
        else if (idx > 0 && ctx.property == false && index >= 0)
        {
            validator.errorArgs.push_back({-1, std::to_string(index + 1)});
        }
        else if (pIT)
        {
            validator.errorArgs.push_back({-1, scilab::UTF8::toUTF8(var2str(pIT))});
        }
        else
        {
            validator.errorArgs.push_back({std::abs(idx), ""});
        }
    }

    return validator;
}

bool isOfBuiltinType(types::InternalType* x, const std::wstring& type)
{
    static const std::map<std::wstring, types::InternalType::ScilabType> types = {
        {L"double", types::InternalType::ScilabDouble},
        {L"constant", types::InternalType::ScilabDouble},
        {L"int", types::InternalType::ScilabInt32},
        {L"uint", types::InternalType::ScilabUInt32},
        {L"int8", types::InternalType::ScilabInt8},
        {L"uint8", types::InternalType::ScilabUInt8},
        {L"int16", types::InternalType::ScilabInt16},
        {L"uint16", types::InternalType::ScilabUInt16},
        {L"int32", types::InternalType::ScilabInt32},
        {L"uint32", types::InternalType::ScilabUInt32},
        {L"int64", types::InternalType::ScilabInt64},
        {L"uint64", types::InternalType::ScilabUInt64},
        {L"bool", types::InternalType::ScilabBool},
        {L"boolean", types::InternalType::ScilabBool},
        {L"string", types::InternalType::ScilabString}};

    auto it = types.find(type);
    return it != types.end() && x->getType() == it->second;
}

bool isOfClassType(types::InternalType* x, const std::wstring& type, const std::wstring& name)
{
    if (x->isObject() && x->getAs<types::Object>()->isA(type))
    {
        return true;
    }

    // known at call time only: the class can be defined after the function
    if (symbol::Context::getInstance()->getClassdef(type) == nullptr)
    {
        throw ast::InternalError(formatError(_("%s: Unknown conversion function '%s'\n"), scilab::UTF8::toUTF8(name).data(), scilab::UTF8::toUTF8(type).data()));
    }

    return false;
}

bool andBool(types::InternalType* pIT)
{
    if (pIT == nullptr)
    {
        return false;
    }

    bool iRet = true;
    if(pIT->isBool())
    {
        types::Bool* b = pIT->getAs<types::Bool>();
        for (int i = 0; i < b->getSize(); ++i)
        {
            if (b->get()[i] == 0)
            {
                iRet = false;
                break;
            }
        }
    }

    pIT->killMe();
    return iRet;
}

bool orBool(types::InternalType* pIT)
{
    if (pIT == nullptr)
    {
        return false;
    }

    int iRet = false;
    if(pIT->isBool())
    {
        types::Bool* b = pIT->getAs<types::Bool>();
        for (int i = 0; i < b->getSize(); ++i)
        {
            if (b->get()[i] == 1)
            {
                iRet = true;
                break;
            }
        }
    }

    pIT->killMe();
    return iRet;
}

bool mustBePositive(types::typed_list& x)
{
    types::Double* pDbl = new types::Double(0);
    types::InternalType* pComp = callComparison(GenericGreater, ast::OpExp::Oper::gt, x[0], pDbl);
    pDbl->killMe();
    return andBool(pComp);
}

bool mustBeNonpositive(types::typed_list& x)
{
    types::Double* pDbl = new types::Double(0);
    types::InternalType* pComp = callComparison(GenericLessEqual, ast::OpExp::Oper::le, x[0], pDbl);
    pDbl->killMe();
    return andBool(pComp);
}

bool mustBeNonnegative(types::typed_list& x)
{
    types::Double* pDbl = new types::Double(0);
    types::InternalType* pComp = callComparison(GenericGreaterEqual, ast::OpExp::Oper::ge, x[0], pDbl);
    pDbl->killMe();
    return andBool(pComp);
}

bool mustBeNegative(types::typed_list& x)
{
    types::Double* pDbl = new types::Double(0);
    types::InternalType* pComp = callComparison(GenericLess, ast::OpExp::Oper::le, x[0], pDbl);
    pDbl->killMe();
    return andBool(pComp);
}

bool mustBeNumeric(types::typed_list& x)
{
    return x[0]->isDouble() || x[0]->isInt();
}

bool mustBeFinite(types::typed_list& x)
{
    if (mustBeNumeric(x) == false)
    {
        return false;
    }

    if (x[0]->isDouble())
    {
        double* p = x[0]->getAs<types::Double>()->get();
        for (int i = 0; i < x[0]->getAs<types::Double>()->getSize(); ++i)
        {
            if (std::isfinite(p[i]) == false)
            {
                return false;
            }
        }
    }

    return true;
}

bool mustBeNonNan(types::typed_list& x)
{
    if (mustBeNumeric(x) == false)
    {
        return callIsFunction(L"isnan", x) == false;
    }

    if (x[0]->isDouble())
    {
        double* p = x[0]->getAs<types::Double>()->get();
        for (int i = 0; i < x[0]->getAs<types::Double>()->getSize(); ++i)
        {
            if (std::isnan(p[i]))
            {
                return false;
            }
        }
    }

    return true;
}

bool mustBeNonzero(types::typed_list& x)
{
    types::Double* pDbl = new types::Double(0);
    types::InternalType* pComp = callComparison(GenericComparisonNonEqual, ast::OpExp::Oper::ne, x[0], pDbl);
    pDbl->killMe();
    return andBool(pComp);
}

bool mustBeNonsparse(types::typed_list& x)
{
    return x[0]->isSparse() == false;
}

static bool isReal(const double* val, size_t size, double eps)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (abs(val[i]) > eps)
        {
            return false;
        }
    }

    return true;
}

bool mustBeReal(types::typed_list& x)
{
    if (x[0]->isDouble() || x[0]->isPoly() || x[0]->isSparse())
    {
        if (x[0]->isDouble() && x[0]->getAs<types::Double>()->isComplex())
        {
            types::Double* d = x[0]->getAs<types::Double>();
            return isReal(d->getImg(), d->getSize(), x.size() > 1 ? x[1]->getAs<types::Double>()->get()[0] : 0);
        }

        if (x[0]->isPoly() && x[0]->getAs<types::Polynom>()->isComplex())
        {
            types::Polynom* p = x[0]->getAs<types::Polynom>();
            types::Double* d = p->getCoef();
            int ret = isReal(d->getImg(), d->getSize(), x.size() > 1 ? x[1]->getAs<types::Double>()->get()[0] : 0);
            d->killMe();
            return ret;
        }

        if (x[0]->isSparse() && x[0]->getAs<types::Sparse>()->isComplex())
        {
            types::Sparse* sp = x[0]->getAs<types::Sparse>();
            size_t nonZeros = sp->nonZeros();
            std::vector<double> NonZeroR(nonZeros);
            std::vector<double> NonZeroI(nonZeros);
            sp->outputValues(NonZeroR.data(), NonZeroI.data());

            return isReal(NonZeroI.data(), nonZeros, x.size() > 1 ? x[1]->getAs<types::Double>()->get()[0] : 0);
        }
    }

    return true;
}

bool mustBeInteger(types::typed_list& x)
{
    if (mustBeNumeric(x) == false)
    {
        return false;
    }

    if (x[0]->isDouble())
    {
        double* p = x[0]->getAs<types::Double>()->get();
        for (int i = 0; i < x[0]->getAs<types::Double>()->getSize(); ++i)
        {
            if (floor(p[i]) != p[i])
            {
                return false;
            }
        }
    }

    return true;
}

bool mustBeMember(types::typed_list& x)
{
    types::InternalType* tmp = nullptr;
    if (x[1]->isCell())
    {
        types::Cell* ce = x[1]->getAs<types::Cell>();
        types::Bool* tmp2 = new types::Bool(1, ce->getSize());
        for (int i = 0; i < ce->getSize(); ++i)
        {
            tmp2->set(i, andBool(GenericComparisonEqual(x[0], ce->get(i))) ? 1 : 0);
        }

        tmp = tmp2;
    }
    else
    {
        types::InternalType* x1 = x[1];
        bool killMe = false;
        if (x[1]->isImplicitList())
        {
            types::ImplicitList* pIL = x[1]->getAs<types::ImplicitList>();
            if (pIL->isComputable())
            {
                x1 = pIL->extractFullMatrix();
                killMe = true;
            }
        }
        tmp = GenericComparisonEqual(x[0], x1);

        if (killMe)
        {
            x1->killMe();
        }
    }

    return orBool(tmp);
}

bool mustBeGreaterThan(types::typed_list& x)
{
    return andBool(callComparison(GenericGreater, ast::OpExp::Oper::gt, x[0], x[1]));
}

bool mustBeGreaterThanOrEqual(types::typed_list& x)
{
    return andBool(callComparison(GenericGreaterEqual, ast::OpExp::Oper::ge, x[0], x[1]));
}

bool mustBeLessThan(types::typed_list& x)
{
    return andBool(callComparison(GenericLess, ast::OpExp::Oper::lt, x[0], x[1]));
}

bool mustBeLessThanOrEqual(types::typed_list& x)
{
    return andBool(callComparison(GenericLessEqual, ast::OpExp::Oper::le, x[0], x[1]));
}

bool mustBeA(types::typed_list& x)
{
    types::String* types = x[1]->getAs<types::String>();
    for (int i = 0; i < types->getSize(); ++i)
    {
        if (typeValidator.find(types->get()[i]) != typeValidator.end())
        {
            if (typeValidator[types->get()[i]](x[0]))
            {
                return true;
            }
        }
        else
        {
            std::wstring type;
            if (x[0]->isUserType())
            {
                type = x[0]->getAs<types::UserType>()->getTypeStr();
            }

            if (x[0]->isTList() || x[0]->isMList())
            {
                type = x[0]->getAs<types::TList>()->getTypeStr();
            }

            if (type == types->get()[i])
            {
                return true;
            }
        }
    }

    return false;
}

bool mustBeNumericOrLogical(types::typed_list& x)
{
    return mustBeNumeric(x) || x[0]->isBool();
}

bool mustBeNonempty(types::typed_list& x)
{
    return (x[0]->isDouble() && x[0]->getAs<types::Double>()->isEmpty()) == false;
}

bool mustBeScalar(types::typed_list& x)
{
    if(x[0]->isGenericType() && x[0]->getAs<types::GenericType>()->isScalar())
    {
        return true;
    }

    return callIsFunction(L"isscalar", x);
}

bool mustBeScalarOrEmpty(types::typed_list& x)
{
    if(x[0]->isGenericType() && (x[0]->getAs<types::GenericType>()->isScalar() || x[0]->getAs<types::GenericType>()->getSize() == 0))
    {
        return true;
    }

    return callIsFunction(L"isscalar", x) || callIsFunction(L"isempty", x);
}

bool mustBeVector(types::typed_list& x)
{
    return x[0]->isGenericType() && x[0]->getAs<types::GenericType>()->isVector();
}

bool mustBeRow(types::typed_list& x)
{
    return x[0]->isGenericType() && x[0]->getAs<types::GenericType>()->getDims() == 2 && x[0]->getAs<types::GenericType>()->getDimsArray()[0] == 1;
}

bool mustBeColumn(types::typed_list& x)
{
    return x[0]->isGenericType() && x[0]->getAs<types::GenericType>()->getDims() == 2 && x[0]->getAs<types::GenericType>()->getDimsArray()[1] == 1;
}

bool mustBeSquare(types::typed_list& x)
{
    if (x[0]->isGenericType() == false)
    {
        return false;
    }

    types::GenericType* gt = x[0]->getAs<types::GenericType>();
    if (gt->isDouble() && gt->getAs<types::Double>()->isEmpty())
    {
        return false;
    }

    if (gt->getDims() != 2 || gt->getRows() != gt->getCols())
    {
        return false;
    }

    return true;
}

bool mustBeInRange(types::typed_list& x)
{
    #define checkFunc(name, op) [](types::InternalType* x1, types::InternalType* x2) { \
        return callComparison(name, op, x1, x2); \
    }

    typedef std::function<types::InternalType*(types::InternalType*, types::InternalType*)> checker;
    checker checkLeft = checkFunc(GenericGreaterEqual, ast::OpExp::Oper::ge);
    checker checkRight = checkFunc(GenericLessEqual, ast::OpExp::Oper::le);

    if (x.size() == 4)
    {
        std::wstring bounds = x[3]->getAs<types::String>()->get()[0];
        if (bounds == L"exclusive")
        {
            checkLeft = checkFunc(GenericGreater, ast::OpExp::Oper::gt);
            checkRight = checkFunc(GenericLess, ast::OpExp::Oper::lt);
        }
        else if (bounds == L"exclude-lower")
        {
            checkLeft = checkFunc(GenericGreater, ast::OpExp::Oper::gt);
        }
        else if (bounds == L"exclude-upper")
        {
            checkRight = checkFunc(GenericLess, ast::OpExp::Oper::lt);
        }
    }

    return andBool(checkLeft(x[0], x[1])) && andBool(checkRight(x[0], x[2]));
}

bool mustBeFile(types::typed_list& x)
{
    if (x[0]->isString())
    {
        wchar_t* f = x[0]->getAs<types::String>()->get()[0];
        wchar_t* e = expandPathVariableW(f);
        if (e == nullptr)
        {
            return false;
        }

        std::wstring exp(e);
        FREE(e);
        return isdirW(exp.data()) == false && FileExistW(exp.data());
    }

    return false;
}

bool mustBeFolder(types::typed_list& x)
{
    if (x[0]->isString())
    {
        wchar_t* f = x[0]->getAs<types::String>()->get()[0];
        wchar_t* e = expandPathVariableW(f);
        if (e == nullptr)
        {
            return false;
        }

        std::wstring exp(e);
        FREE(e);
        return isdirW(exp.data());
    }

    return false;
}

bool mustBeNonzeroLengthText(types::typed_list& x)
{
    if (x[0]->isString() && x[0]->getAs<types::String>()->isScalar())
    {
        return wcslen(x[0]->getAs<types::String>()->get()[0]) > 0;
    }

    return false;
}

bool mustBeValidVariableName(types::typed_list& x)
{
    if (x[0]->isString() && x[0]->getAs<types::String>()->isScalar())
    {
        return symbol::Context::getInstance()->isValidVariableName(x[0]->getAs<types::String>()->get()[0]);
    }

    return false;
}

bool mustBeEqualDims(types::typed_list& x)
{
    types::typed_list in1 = {x[0]};
    types::typed_list out1;
    if (Overload::call(L"size", in1, 1, out1) != types::Function::OK)
    {
        return false;
    }

    types::typed_list in2 = {x[1]};
    types::typed_list out2;
    if (Overload::call(L"size", in2, 1, out2) != types::Function::OK)
    {
        return false;
    }

    types::Double* p1 = out1[0]->getAs<types::Double>();
    std::vector<int> dims1(p1->get(), p1->get() + p1->getSize());
    p1->killMe();

    types::Double* p2 = out2[0]->getAs<types::Double>();
    std::vector<int> dims2(p2->get(), p2->get() + p2->getSize());
    p2->killMe();

    std::vector<int> ref = {-1};
    if (x.size() == 3)
    {
        types::Double* pref = x[2]->getAs<types::Double>();
        ref.clear();
        ref.reserve(pref->getSize());
        for (int i = 0; i < pref->getSize(); ++i)
        {
            ref.push_back(static_cast<int>(pref->get()[i]));
        }
    }

    if (ref.size() >= 1 && ref[0] != -1)
    {
        for (int i = 0; i < ref.size(); ++i)
        {
            if (dims1.size() < ref[i] || dims2.size() < ref[i])
            {
                return false;
            }
        }
    }
    else
    {
        if (dims1.size() != dims2.size())
        {
            return false;
        }
    }

    if (ref.size() >= 1 && ref[0] != -1)
    {
        for (int i = 0; i < ref.size(); ++i)
        {
            if (dims1[ref[i] - 1] != dims2[ref[i] - 1])
            {
                return false;
            }
        }
    }
    else
    {
        for (int i = 0; i < dims1.size(); ++i)
        {
            if (dims1[i] != dims2[i])
            {
                return false;
            }
        }
    }

    return true;
}

bool mustBeSameType(types::typed_list& x)
{
    if (x[0]->isInt() && x[1]->isInt())
    {
        return true;
    }

    return x[0]->getType() == x[1]->getType();
}

bool mustBeEqualDimsOrEmpty(types::typed_list& x)
{
    if(mustBeEqualDims(x))
    {
        return true;
    }

    types::typed_list in1 = {x[0]};
    if(callIsFunction(L"isempty", in1))
    {
        return true;
    }

    types::typed_list in2 = {x[1]};
    return callIsFunction(L"isempty", in2);
}

bool mustBeEqualDimsOrScalar(types::typed_list& x)
{
    if(mustBeEqualDims(x))
    {
        return true;
    }

    types::typed_list in1 = {x[0]};
    if(callIsFunction(L"isscalar", in1))
    {
        return true;
    }

    types::typed_list in2 = {x[1]};
    return callIsFunction(L"isscalar", in2);
}

std::map<std::wstring, std::tuple<std::function<int(types::typed_list&)>, std::vector<int>>> functionValidators = {
    {L"mustBePositive", {mustBePositive, {1}}},
    {L"mustBeNonpositive", {mustBeNonpositive, {1}}},
    {L"mustBeNonnegative", {mustBeNonnegative, {1}}},
    {L"mustBeNegative", {mustBeNegative, {1}}},
    {L"mustBeFinite", {mustBeFinite, {1}}},
    {L"mustBeNonNan", {mustBeNonNan, {1}}},
    {L"mustBeNonzero", {mustBeNonzero, {1}}},
    {L"mustBeNonsparse", {mustBeNonsparse, {1}}},
    {L"mustBeReal", {mustBeReal, {1, 2}}},
    {L"mustBeInteger", {mustBeInteger, {1}}},
    {L"mustBeGreaterThan", {mustBeGreaterThan, {2}}},
    {L"mustBeLessThan", {mustBeLessThan, {2}}},
    {L"mustBeGreaterThanOrEqual", {mustBeGreaterThanOrEqual, {2}}},
    {L"mustBeLessThanOrEqual", {mustBeLessThanOrEqual, {2}}},
    {L"mustBeA", {mustBeA, {2}}},
    {L"mustBeNumeric", {mustBeNumeric, {1}}},
    {L"mustBeNumericOrLogical", {mustBeNumericOrLogical, {1}}},
    {L"mustBeNumericOrBoolean", {mustBeNumericOrLogical, {1}}},
    {L"mustBeNonempty", {mustBeNonempty, {1}}},
    {L"mustBeScalar", {mustBeScalar, {1}}},
    {L"mustBeScalarOrEmpty", {mustBeScalarOrEmpty, {1}}},
    {L"mustBeVector", {mustBeVector, {1}}},
    {L"mustBeRow", {mustBeRow, {1}}},
    {L"mustBeColumn", {mustBeColumn, {1}}},
    {L"mustBeSquare", {mustBeSquare, {1}}},
    {L"mustBeMember", {mustBeMember, {2}}},
    {L"mustBeInRange", {mustBeInRange, {3, 4}}},
    {L"mustBeFile", {mustBeFile, {1}}},
    {L"mustBeFolder", {mustBeFolder, {1}}},
    {L"mustBeNonzeroLengthText", {mustBeNonzeroLengthText, {1}}},
    {L"mustBeValidVariableName", {mustBeValidVariableName, {1}}},
    {L"mustBeEqualDims", {mustBeEqualDims, {2, 3}}},
    {L"mustBeSameType", {mustBeSameType, {2}}},
    {L"mustBeEqualDimsOrEmpty", {mustBeEqualDimsOrEmpty, {-1}}},
    {L"mustBeEqualDimsOrScalar", {mustBeEqualDimsOrScalar, {-1}}}
};

std::tuple<std::function<int(types::typed_list&)>, std::vector<int>> getFunctionValidator(const std::wstring& name)
{
    if (functionValidators.find(name) == functionValidators.end())
    {
        return {nullptr, {0}};
    }

    return functionValidators[name];
}

std::map<std::wstring, std::tuple<std::string, int>> errorValidators = {
    {L"mustBePositive", {"%s: Wrong value for input argument #%d: Positive numbers expected.\n", 2}},
    {L"mustBeNonpositive", {"%s: Wrong value for input argument #%d: Non positive numbers expected.\n", 2}},
    {L"mustBeNonnegative", {"%s: Wrong value for input argument #%d: Non negative numbers expected.\n", 2}},
    {L"mustBeNegative", {"%s: Wrong value for input argument #%d: Negative numbers expected.\n", 2}},
    {L"mustBeFinite", {"%s: Wrong value for input argument #%d: Finite numbers expected.\n", 2}},
    {L"mustBeNonNan", {"%s: Wrong value for input argument #%d: Nan are not allowed.\n", 2}},
    {L"mustBeNonzero", {"%s: Wrong value for input argument #%d: Zero are not allowed.\n", 2}},
    {L"mustBeNonsparse", {"%s: Wrong value for input argument #%d: Sparse are not allowed.\n", 2}},
    {L"mustBeReal", {"%s: Wrong value for input argument #%d: Real numbers expected.\n", 2}},
    {L"mustBeInteger", {"%s: Wrong value for input argument #%d: Integer numbers expected.\n", 2}},
    {L"mustBeMember", {"%s: Wrong value for input argument #%d: Must be in %s.\n", 3}},
    {L"mustBeGreaterThan", {"%s: Wrong value for input argument #%d: Must be > %s.\n", 3}},
    {L"mustBeGreaterThanOrEqual", {"%s: Wrong value for input argument #%d: Must be >= %s.\n", 3}},
    {L"mustBeLessThan", {"%s: Wrong value for input argument #%d: Must be < %s.\n", 3}},
    {L"mustBeLessThanOrEqual", {"%s: Wrong value for input argument #%d: Must be <= %s.\n", 3}},
    {L"mustBeA", {"%s: Wrong type for input argument #%d: Must be in %s.\n", 3}},
    {L"mustBeNumeric", {"%s: Wrong type for input argument #%d: Must be numeric values.\n", 2}},
    {L"mustBeNumericOrLogical", {"%s: Wrong type for input argument #%d: Must be numeric values or boolean.\n", 2}},
    {L"mustBeNumericOrBoolean", {"%s: Wrong type for input argument #%d: Must be numeric values or boolean.\n", 2}},
    {L"mustBeNonempty", {"%s: Wrong type for input argument #%d: Must not be empty.\n", 2}},
    {L"mustBeScalar", {"%s: Wrong type for input argument #%d: Must be a scalar.\n", 2}},
    {L"mustBeScalarOrEmpty", {"%s: Wrong type for input argument #%d: Must be a scalar or empty.\n", 2}},
    {L"mustBeVector", {"%s: Wrong type for input argument #%d: Must be a vector.\n", 2}},
    {L"mustBeRow", {"%s: Wrong type for input argument #%d: Must be a row vector.\n", 2}},
    {L"mustBeColumn", {"%s: Wrong type for input argument #%d: Must be a column vector.\n", 2}},
    {L"mustBeSquare", {"%s: Wrong type for input argument #%d: Must be a square matrix.\n", 2}},
    {L"mustBeMember", {"%s: Wrong type for input argument #%d: Must be member of %s.\n", 3}},
    {L"mustBeInRange", {"%s: Wrong value for input argument #%d: Must be in range [%s, %s].\n", 4}},
    {L"mustBeFile", {"%s: Wrong type for input argument #%d: Must be a file.\n", 2}},
    {L"mustBeFolder", {"%s: Wrong type for input argument #%d: Must be a folder.\n", 2}},
    {L"mustBeNonzeroLengthText", {"%s: Wrong type for input argument #%d: Must not be an empty string.\n", 2}},
    {L"mustBeValidVariableName", {"%s: Wrong type for input argument #%d: Must be a valid variable name.\n", 2}},
    {L"mustBeEqualDims", {"%s: Wrong size for input argument #%d: Must be of the same dimensions of #%s.\n", 3}},
    {L"mustBeSameType", {"%s: Wrong type for input argument #%d: Must be same type of #%s.\n", 3}},
    {L"mustBeEqualDimsOrEmpty", {"%s: Wrong size for input argument #%d: Must be of the same dimensions of #%s or empty.\n", -3}},
    {L"mustBeEqualDimsOrScalar", {"%s: Wrong size for input argument #%d: Must be of the same dimensions of #%s or scalar.\n", -3}},
};

std::tuple<std::string, int> getErrorValidator(const std::wstring& name)
{
    return errorValidators[name];
}

// classdef properties: "%s: ... property '%s'..." with class and property names
std::map<std::wstring, std::tuple<std::string, int>> errorPropertyValidators = {
    {L"mustBePositive", {"%s: Wrong value for property '%s': Positive numbers expected.\n", 2}},
    {L"mustBeNonpositive", {"%s: Wrong value for property '%s': Non positive numbers expected.\n", 2}},
    {L"mustBeNonnegative", {"%s: Wrong value for property '%s': Non negative numbers expected.\n", 2}},
    {L"mustBeNegative", {"%s: Wrong value for property '%s': Negative numbers expected.\n", 2}},
    {L"mustBeFinite", {"%s: Wrong value for property '%s': Finite numbers expected.\n", 2}},
    {L"mustBeNonNan", {"%s: Wrong value for property '%s': Nan are not allowed.\n", 2}},
    {L"mustBeNonzero", {"%s: Wrong value for property '%s': Zero are not allowed.\n", 2}},
    {L"mustBeNonsparse", {"%s: Wrong value for property '%s': Sparse are not allowed.\n", 2}},
    {L"mustBeReal", {"%s: Wrong value for property '%s': Real numbers expected.\n", 2}},
    {L"mustBeInteger", {"%s: Wrong value for property '%s': Integer numbers expected.\n", 2}},
    {L"mustBeMember", {"%s: Wrong value for property '%s': Must be in %s.\n", 3}},
    {L"mustBeGreaterThan", {"%s: Wrong value for property '%s': Must be > %s.\n", 3}},
    {L"mustBeGreaterThanOrEqual", {"%s: Wrong value for property '%s': Must be >= %s.\n", 3}},
    {L"mustBeLessThan", {"%s: Wrong value for property '%s': Must be < %s.\n", 3}},
    {L"mustBeLessThanOrEqual", {"%s: Wrong value for property '%s': Must be <= %s.\n", 3}},
    {L"mustBeA", {"%s: Wrong type for property '%s': Must be in %s.\n", 3}},
    {L"mustBeNumeric", {"%s: Wrong type for property '%s': Must be numeric values.\n", 2}},
    {L"mustBeNumericOrLogical", {"%s: Wrong type for property '%s': Must be numeric values or boolean.\n", 2}},
    {L"mustBeNumericOrBoolean", {"%s: Wrong type for property '%s': Must be numeric values or boolean.\n", 2}},
    {L"mustBeNonempty", {"%s: Wrong type for property '%s': Must not be empty.\n", 2}},
    {L"mustBeScalar", {"%s: Wrong type for property '%s': Must be a scalar.\n", 2}},
    {L"mustBeScalarOrEmpty", {"%s: Wrong type for property '%s': Must be a scalar or empty.\n", 2}},
    {L"mustBeVector", {"%s: Wrong type for property '%s': Must be a vector.\n", 2}},
    {L"mustBeRow", {"%s: Wrong type for property '%s': Must be a row vector.\n", 2}},
    {L"mustBeColumn", {"%s: Wrong type for property '%s': Must be a column vector.\n", 2}},
    {L"mustBeSquare", {"%s: Wrong type for property '%s': Must be a square matrix.\n", 2}},
    {L"mustBeInRange", {"%s: Wrong value for property '%s': Must be in range [%s, %s].\n", 4}},
    {L"mustBeFile", {"%s: Wrong type for property '%s': Must be a file.\n", 2}},
    {L"mustBeFolder", {"%s: Wrong type for property '%s': Must be a folder.\n", 2}},
    {L"mustBeNonzeroLengthText", {"%s: Wrong type for property '%s': Must not be an empty string.\n", 2}},
    {L"mustBeValidVariableName", {"%s: Wrong type for property '%s': Must be a valid variable name.\n", 2}},
    {L"mustBeEqualDims", {"%s: Wrong size for property '%s': Must be of the same dimensions of %s.\n", 3}},
    {L"mustBeSameType", {"%s: Wrong type for property '%s': Must be same type of %s.\n", 3}},
    {L"mustBeEqualDimsOrEmpty", {"%s: Wrong size for property '%s': Must be of the same dimensions of %s or empty.\n", -3}},
    {L"mustBeEqualDimsOrScalar", {"%s: Wrong size for property '%s': Must be of the same dimensions of %s or scalar.\n", -3}},
};

std::tuple<std::string, int> getPropertyErrorValidator(const std::wstring& name)
{
    return errorPropertyValidators[name];
}

// < 0 to print varaible/constant content
// > 0 to print #num (variable position)
std::map<std::wstring, std::vector<std::tuple<int, std::string>>> errorArgs = {
    {L"mustBeMember", {{-1, ""}}},
    {L"mustBeGreaterThan", {{-1, ""}}},
    {L"mustBeGreaterThanOrEqual", {{-1, ""}}},
    {L"mustBeLessThan", {{-1, ""}}},
    {L"mustBeLessThanOrEqual", {{-1, ""}}},
    {L"mustBeA", {{-1, ""}}},
    {L"mustBeInRange", {{-1, ""}, {-2, ""}}},
    {L"mustBeEqualDims", {{1, ""}}},
    {L"mustBeSameType", {{1, ""}}},
    {L"mustBeEqualDimsOrEmpty", {{1, ""}}},
    {L"mustBeEqualDimsOrScalar", {{1, ""}}},
};

std::vector<std::tuple<int, std::string>> getErrorArgs(const std::wstring& name)
{
    return errorArgs[name];
}

types::InternalType* transposevar(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    if (x->isArrayOf())
    {
        types::GenericType* gt = x->getAs<types::GenericType>();
        // if dims == (1, x) ou (x, 1) & input is a vector, transpose it to match good shape
        if (dims.size() == 2)
        {
            std::vector<int> transposeDims;

            for (int i = 0; i < 2; ++i)
            {
                std::vector<int> dim;
                symbol::Variable* v;
                std::tie(dim, v) = dims[i];

                if (v != nullptr)
                {
                    types::InternalType* pIT = v->get();
                    if (pIT && pIT->isDouble())
                    {
                        types::Double* d = pIT->getAs<types::Double>();
                        if (d->isScalar())
                        {
                            transposeDims.push_back(static_cast<int>(d->get()[0]));
                        }
                    }
                }
                else
                {
                    if (dim.size() == 1)
                    {
                        transposeDims.push_back(dim[0]);
                    }
                }
            }

            if (transposeDims.size() == 2)
            {
                // row expected, column given
                if (transposeDims[0] == 1 && gt->getCols() == 1)
                {
                    if (transposeDims[1] == -1 || transposeDims[1] == gt->getRows())
                    {
                        types::InternalType* transposed;
                        gt->transpose(transposed);
                        return transposed;
                    }
                }
                // column expected, row given
                else if (transposeDims[1] == 1 && gt->getRows() == 1)
                {
                    if (transposeDims[0] == -1 || transposeDims[0] == gt->getCols())
                    {
                        types::InternalType* transposed;
                        gt->transpose(transposed);
                        return transposed;
                    }
                }
            }
        }
    }

    return nullptr;
}

types::InternalType* expandvar(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims, bool isStatic)
{
    if (x->isArrayOf())
    {
        std::vector<int> convertDims;
        types::GenericType* gt = x->getAs<types::GenericType>();
        if (gt->isScalar())
        {
            if (isStatic)
            {
                for (auto&& d : dims)
                {
                    convertDims.push_back(std::get<0>(d)[0]);
                }
            }
            else
            {
                for (auto&& d : dims)
                {
                    std::vector<int> dim;
                    symbol::Variable* v;
                    std::tie(dim, v) = d;
                    if (v == nullptr)
                    {
                        if (dim.size() == 1 && dim[0] != -1)
                        {
                            convertDims.push_back(dim[0]);
                        }
                        else
                        {
                            convertDims.clear();
                        }
                    }
                    else
                    {
                        types::InternalType* pIT = v->get();
                        if (pIT && pIT->isDouble())
                        {
                            types::Double* d = pIT->getAs<types::Double>();
                            if (d->isScalar())
                            {
                                convertDims.push_back(static_cast<int>(d->get()[0]));
                            }
                            else
                            {
                                convertDims.clear();
                            }
                        }
                        else
                        {
                            convertDims.clear();
                        }
                    }
                }
            }
        }

        if (convertDims.size() != 0)
        {
            int size = 1;
            std::for_each(convertDims.begin(), convertDims.end(), [&size](int v) { size *= v; });

            if (size != 1)
            {
                // clone to keep the same type of input argument
                types::GenericType* clone = gt->clone();
                clone->resize(convertDims.data(), static_cast<int>(convertDims.size()));

                types::typed_list in;
                types::InternalType* colon = new types::Colon();
                in.push_back(colon);

                clone = clone->insert(&in, gt);
                colon->killMe();
                return clone;
            }
        }
    }

    return x;
}

namespace
{
// dimensions of x, false if it has none
bool getDims(types::InternalType* x, std::vector<int>& dims1)
{
    if (x->isGenericType() == false)
    {
        return false;
    }

    if (x->isArrayOf())
    {
        types::GenericType* g = x->getAs<types::GenericType>();
        dims1.assign(g->getDimsArray(), g->getDimsArray() + g->getDims());
        return true;
    }

    types::typed_list in1 = {x};
    types::typed_list out1;
    if (Overload::call(L"size", in1, 1, out1) != types::Function::OK)
    {
        return false;
    }

    types::Double* p1 = out1[0]->getAs<types::Double>();
    dims1.assign(p1->get(), p1->get() + p1->getSize());
    p1->killMe();
    return true;
}

// dims1 is one of the declared dimensions (alternatives, ':' or input), as is
bool matchDims(const std::vector<int>& dims1, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    bool status = true;
    for (int i = 0; i < dims.size(); ++i)
    {
        std::vector<int> dim;
        symbol::Variable* v;
        std::tie(dim, v) = dims[i];
        int ref = i < dims1.size() ? dims1[i] : 1;
        bool ok = false;
        if (v != nullptr)
        {
            types::InternalType* pIT = v->get();
            if (pIT && pIT->isDouble())
            {
                types::Double* d = pIT->getAs<types::Double>();

                for (int j = 0; j < d->getSize(); ++j)
                {
                    if (d->get()[j] == ref)
                    {
                        ok = true;
                        break;
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < dim.size(); ++j)
            {
                if (dim[j] == -1 || dim[j] == ref)
                {
                    ok = true;
                    break;
                }
            }
        }

        status &= ok;
    }

    return status;
}

// one declared dimension: number of elements
bool matchLength(const std::vector<int>& dims1, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    auto&& d = std::get<0>(dims[0]);
    return std::find(d.begin(), d.end(), dims1[0]) != d.end();
}
}

types::InternalType* checksize(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims, bool isStatic)
{
    std::vector<int> dims1;
    if (getDims(x, dims1) == false || dims1.size() > dims.size())
    {
        return nullptr;
    }

    if (dims.size() == 1 && std::get<1>(dims[0]) == nullptr)
    {
        return matchLength(dims1, dims) ? x : nullptr;
    }

    if (x->getAs<types::GenericType>()->isScalar())
    {
        return expandvar(x, dims, isStatic);
    }

    if (matchDims(dims1, dims))
    {
        return x;
    }

    // a vector with the wrong orientation can still match once transposed
    if (dims1.size() == 2 && (dims1[0] == 1 || dims1[1] == 1))
    {
        return transposevar(x, dims);
    }

    return nullptr;
}

bool hasDims(types::InternalType* x, const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    std::vector<int> dims1;
    if (getDims(x, dims1) == false || dims1.size() > dims.size())
    {
        return false;
    }

    if (dims.size() == 1 && std::get<1>(dims[0]) == nullptr)
    {
        return matchLength(dims1, dims);
    }

    return matchDims(dims1, dims);
}

std::vector<std::vector<int>> todims(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    std::vector<std::vector<int>> ret;
    for (auto&& s : dims)
    {
        std::vector<int> d;
        symbol::Variable* v;
        std::tie(d, v) = s;
        if (v == nullptr)
        {
            ret.push_back(d);
        }
        else
        {
            ret.push_back({static_cast<int>(v->top()->m_pIT->getAs<types::Double>()->get()[0])});
        }
    }

    return ret;
}

std::wstring dims2str(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    std::wstring res = L"";

    std::vector<std::vector<int>> c = todims(dims);
    for (int i = 0; i < c.size(); ++i)
    {
        auto s = c[i];
        if (res.empty() == false)
        {
            res += L" x ";
        }

        if (s.size() == 1)
        {
            res += s[0] == -1 ? std::wstring(1, L'm' + i) : std::to_wstring(s[0]);
        }
        else
        {
            std::wstring res2;
            for (auto&& d : s)
            {
                if (res2.empty() == false)
                {
                    res2 += L", ";
                }

                res2 += std::to_wstring(d);
            }

            res += L"[" + res2 + L"]";
        }
    }

    return res;
}
