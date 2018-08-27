/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
*
* Copyright (C) 2012 - 2016 - Scilab Enterprises
*
*/

#include "json.hxx"

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <regex>
#include <chrono>


extern "C"
{
#define __API_SCILAB_UNSAFE__
#include "api_scilab.h"
#include "jsmn.h"
#include "os_string.h"
}

//
// toJSON
//

static bool export_data(scilabEnv env, scilabVar var, int indent, std::wostringstream& os);

int level = -1;

template <typename T>
static T printType(T dbl)
{
    return dbl;
}

//to avoid character printing
static int printType(char c)
{
    return (int)c;
}

static unsigned int printType(unsigned char c)
{
    return (unsigned int)c;
}

static void string_replace(std::wstring& str, const std::wstring& f, const std::wstring& r)
{
    size_t pos = str.find(f);
    if (pos == std::wstring::npos)
    {
        return;
    }

    str.replace(pos, f.length(), r);
}

static std::wstring printType(wchar_t* s)
{
    std::wstring str = s;
    string_replace(str, L"\\", L"\\\\");
    string_replace(str, L"\"", L"\\\"");
    string_replace(str, L"\t", L"\\\t");

    return L"\"" + str + L"\"";
}

static std::wstring printType(bool b)
{
    return b ? L"true" : L"false";
}

template<typename T>
static void getDataInLine(const T d, int rows, int cols, int line, std::wostringstream& os)
{
    //row vector [...]
    os << L"[";
    if (cols)
    {
        os << printType(d[line]);
    }

    for (int i = 1; i < cols; ++i)
    {
        os << L", ";
        os << printType(d[i * rows + line]);
    }

    os << L"]";
}

template<typename T>
static bool export_type(int rows, int cols, T data, int indent, std::wostringstream& os)
{
    std::wstring indentStr;
    std::wstring indentStr2;
    if (indent)
    {
        indentStr = L"\n" + std::wstring(indent * level, L' ');
        indentStr2 = L"\n" + std::wstring(indent * (level + 1), L' ');
    }

    if (rows == 0 && cols == 0)
    {
        os << L"[]";
        return true;
    }

    if (rows == 1 && cols == 1)
    {
        os << printType(data[0]);
        return true;
    }

    if (rows == 1)
    {
        getDataInLine(data, rows, cols, 0, os);
        return true;
    }

    if (cols == 1)
    {
        os << L"[";
        os << indentStr2;
        os << L"[" << printType(data[0]) << L"]";

        for (int i = 1; i < rows; ++i)
        {
            os << L", ";
            os << indentStr2;
            os << L"[" << printType(data[i]) << L"]";
        }

        os << indentStr;
        os << L"]";
        return true;
    }

    //matrix

    os << L"[";
    os << indentStr2;
    getDataInLine(data, rows, cols, 0, os);
    for (int i = 1; i < rows; ++i)
    {
        os << L",";
        os << indentStr2;
        getDataInLine(data, rows, cols, i, os);
    }

    os << indentStr;
    os << L"]";

    return true;
}

static bool export_struct(scilabEnv env, int indent, const std::vector<wchar_t*>& fields, const std::vector<scilabVar> data, std::wostringstream& os)
{
    std::wstring indentStr;
    std::wstring indentStr2;
    if (indent)
    {
        indentStr = L"\n" + std::wstring(indent * level, L' ');
        indentStr2 = L"\n" + std::wstring(indent * (level + 1), L' ');
    }

    size_t size = fields.size();
    if (size == 0)
    {
        os << L"{}";
        return true;
    }

    os << L"{";
    os << indentStr2;
    os << L"\"" << fields[0] << L"\": ";
    export_data(env, data[0], indent, os);

    for (size_t i = 1; i < size; ++i)
    {
        os << L",";
        os << indentStr2;
        os << L"\"" << fields[i] << L"\": ";
        export_data(env, data[i], indent, os);
    }

    os << indentStr;
    os << L"}";
    return true;
}

static bool export_struct_array(scilabEnv env, int indent, const std::vector<wchar_t*>& fields, scilabVar var, std::wostringstream& os)
{
    int* dims = nullptr;
    int dim = scilab_getDimArray(env, var, &dims);

    if (dim == 2 && dims[0] == 1 && dims[1] == 1)
    {
        size_t size = fields.size();
        if (size == 0)
        {
            os << L"{}";
            return true;
        }

        std::vector<scilabVar> data(size);
        for (size_t i = 0; i < size; ++i)
        {
            data[i] = scilab_getStructMatrix2dData(env, var, fields[i], 0, 0);
        }
        //scalar
        return export_struct(env, indent, fields, data, os);
    }

    std::wstring indentStr;
    std::wstring indentStr2;
    if (indent)
    {
        indentStr = L"\n" + std::wstring(indent * level, L' ');
        indentStr2 = L"\n" + std::wstring(indent * (level + 1), L' ');
    }

    if (dim == 2 && dims[0] == 0 && dims[1] == 0)
    {
        os << L"{}";
        return true;
    }

    int size = 1;
    for (int i = 0; i < dim; ++i)
    {
        size *= dims[i];
    }

    //build data for each dimensions
    std::vector<std::vector<scilabVar>> d(size);
    int size2d = dims[0] * dims[1];
    for (int i = 0; i < size; ++i)
    {
        //take care of dim > 2
        int extra = (i / size2d);
        int mem = i % (size2d);
        //convert i to row-major 2d index;
        int m = mem % dims[0];
        int n = mem / dims[0];

        //convert to col-major
        int idx = m * dims[1] + n + extra * size2d;

        for (size_t f = 0; f < fields.size(); ++f)
        {
            scilabVar v = scilab_getStructMatrix2dData(env, var, fields[f], m, n);
            d[idx].push_back(v);
        }
    }

    os << L"[";
    os << indentStr2;

    for (int i = 0; i < dims[0]; ++i)
    {
        if (i != 0)
        {
            os << L",";
        }

        if (dims[0] != 1)
        {
            os << L"[";
        }

        for (int j = 0; j < dims[1]; ++j)
        {
            if (j != 0)
            {
                os << L",";
            }
            level++;
            export_struct(env, indent, fields, d[i * dims[1] + j], os);
            level--;
        }

        if (dims[0] != 1)
        {
            os << L"]";
        }
    }
    os << indentStr;
    os << L"]";

    return true;
}


static bool export_data(scilabEnv env, scilabVar var, int indent, std::wostringstream& os)
{
    ++level;

    bool ret = false;
    switch (scilab_getType(env, var))
    {
        case sci_matrix: //double
        {
            os << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
            int rows = 0;
            int cols = 0;
            double* d = nullptr;
            scilab_getDim2d(env, var, &rows, &cols);
            scilab_getDoubleArray(env, var, &d);
            ret = export_type(rows, cols, d, indent, os);
            break;
        }

        case sci_ints: //ints
        {
            int rows = 0;
            int cols = 0;
            scilab_getDim2d(env, var, &rows, &cols);

            int itype = scilab_getIntegerPrecision(env, var);
            switch (itype)
            {
                case SCI_INT8:
                {
                    char* i8 = nullptr;
                    scilab_getInteger8Array(env, var, &i8);
                    ret = export_type(rows, cols, i8, indent, os);
                    break;
                }
                case SCI_UINT8:
                {
                    unsigned char* ui8 = nullptr;
                    scilab_getUnsignedInteger8Array(env, var, &ui8);
                    ret = export_type(rows, cols, ui8, indent, os);
                    break;
                }
                case SCI_INT16:
                {
                    short* i16 = nullptr;
                    scilab_getInteger16Array(env, var, &i16);
                    ret = export_type(rows, cols, i16, indent, os);
                    break;
                }
                case SCI_UINT16:
                {
                    unsigned short* ui16 = nullptr;
                    scilab_getUnsignedInteger16Array(env, var, &ui16);
                    ret = export_type(rows, cols, ui16, indent, os);
                    break;
                }
                case SCI_INT32:
                {
                    int* i32 = nullptr;
                    scilab_getInteger32Array(env, var, &i32);
                    ret = export_type(rows, cols, i32, indent, os);
                    break;
                }
                case SCI_UINT32:
                {
                    unsigned int* ui32 = nullptr;
                    scilab_getUnsignedInteger32Array(env, var, &ui32);
                    ret = export_type(rows, cols, ui32, indent, os);
                    break;
                }
                case SCI_INT64:
                {
                    long long* i64 = nullptr;
                    scilab_getInteger64Array(env, var, &i64);
                    ret = export_type(rows, cols, i64, indent, os);
                    break;
                }
                case SCI_UINT64:
                {
                    unsigned long long* ui64 = nullptr;
                    scilab_getUnsignedInteger64Array(env, var, &ui64);
                    ret = export_type(rows, cols, ui64, indent, os);
                    break;
                }
            }

            break;
        }

        case sci_boolean: //boolean
        {
            int rows = 0;
            int cols = 0;
            int* i32 = nullptr;
            scilab_getDim2d(env, var, &rows, &cols);
            scilab_getBooleanArray(env, var, &i32);

            bool* b = new bool[rows * cols];
            for (int i = 0; i < rows * cols; ++i)
            {
                b[i] = i32[i] == 0 ? false : true;
            }

            ret = export_type(rows, cols, b, indent, os);
            delete[] b;
            break;
        }

        case sci_strings: //string
        {
            int rows = 0;
            int cols = 0;
            wchar_t** s = nullptr;
            scilab_getDim2d(env, var, &rows, &cols);
            scilab_getStringArray(env, var, &s);

            ret = export_type(rows, cols, s, indent, os);
            break;
        }

        case sci_list: //list
        {
            //lists become a heterogeous array
            int count = scilab_getSize(env, var);
            if (count == 0)
            {
                os << L"[]";
                return true;
            }

            std::wstring indentStr;
            std::wstring indentStr2;
            if (indent)
            {
                indentStr = L"\n" + std::wstring(indent * level, L' ');
                indentStr2 = L"\n" + std::wstring(indent * (level + 1), L' ');
            }


            os << L"[";
            os << indentStr2;

            scilabVar item = scilab_getListItem(env, var, 0);
            export_data(env, item, indent, os);
            for (int i = 1; i < count; ++i)
            {
                os << L",";
                os << indentStr2;
                item = scilab_getListItem(env, var, i);
                export_data(env, item, indent, os);
            }

            os << indentStr;
            os << L"]";
            ret = true;
            break;
        }
        case sci_struct: //struct
        {
            //check is a struct or hymermatix
            wchar_t** w = nullptr;
            int size = scilab_getFields(env, var, &w);

            //build a vector for C array
            std::vector<wchar_t*> fields(size);
            for (int i = 0; i < size; ++i)
            {
                fields[i] = w[i];
            }

            delete[] w;

            ret = export_struct_array(env, indent, fields, var, os);
            for (auto f : fields)
            {
                FREE(f);
            }

            break;
        }
    }

    --level;

    return ret;
}

std::string toJSON(scilabEnv env, scilabVar var, int indent)
{
    return toJSON((types::InternalType*)var, indent);
}

std::string toJSON(types::InternalType* it, int indent)
{
    std::wostringstream os;
    scilabVar var = (int*)it;

    export_data(nullptr, var, indent, os);

    char* c = wide_string_to_UTF8(os.str().data());
    std::string s(c);
    FREE(c);
    return s;
}

//
// fromJSON
//

std::string json;
int token_offset = 0;

enum JSONType
{
    JSON_UNDEFINED,
    JSON_EMPTY_MATRIX,
    JSON_DOUBLE,
    JSON_BOOL,
    JSON_STRING,
    JSON_ARRAY,
    JSON_STRUCT,
    JSON_LIST
};

struct JSONVar
{
    JSONType kind;
    std::vector<int> dims;
    bool reduced;
    char* s1;
    double d1;
    int b1;
    std::vector<std::string> fields;
    std::unordered_map<std::string, JSONVar*> o1;

    std::vector<char*> s;
    std::vector<double> d;
    std::vector<int> b;
    std::vector<JSONVar*> a;
    std::unordered_map<std::string, std::vector<JSONVar*>> o;

    JSONVar() : kind(JSON_UNDEFINED), reduced(false), d1(0.0), b1(0)
    {
        s1 = nullptr;
    }

    ~JSONVar()
    {
        for (auto c : s)
        {
            FREE(c);
        }

        if (s1)
        {
            FREE(s1);
        }

        for (auto c : a)
        {
            delete c;
        }

        for (auto c : o1)
        {
            delete c.second;
        }
        // Do not delete content of o
    }
};

void getIndexArray(int idx, const std::vector<int>& dims, std::vector<int>& index)
{
    int iMul = 1;
    size_t size = dims.size();
    for (size_t i = 0; i < size; ++i)
    {
        index[i] = (int)(idx / iMul) % dims[i];
        iMul *= dims[i];
    }
}

scilabVar createScilabVar(scilabEnv env, const JSONVar* v)
{
    scilabVar ret = nullptr;
    switch (v->kind)
    {
        case JSON_EMPTY_MATRIX:
        {
            ret = scilab_createEmptyMatrix(env);
            break;
        }

        case JSON_DOUBLE:
        {
            switch (v->dims.size())
            {
                case 0://scalar
                    ret = scilab_createDouble(env, v->d1);
                    break;

                case 1: //vector
                    ret = scilab_createDoubleMatrix2d(env, 1, v->dims[0], 0);
                    scilab_setDoubleArray(env, ret, v->d.data());
                    break;

                case 2: //maxtrix
                    ret = scilab_createDoubleMatrix2d(env, v->dims[0], v->dims[1], 0);
                    scilab_setDoubleArray(env, ret, v->d.data());
                    break;

                default: // > 2 dims
                    ret = scilab_createDoubleMatrix(env, (int)v->dims.size(), v->dims.data(), 0);
                    scilab_setDoubleArray(env, ret, v->d.data());
                    break;
            }
            break;
        }

        case JSON_BOOL:
        {
            switch (v->dims.size())
            {
                case 0://scalar

                    ret = scilab_createBoolean(env, v->b1);
                    break;

                case 1: //vector
                    ret = scilab_createBooleanMatrix2d(env, 1, v->dims[0]);
                    scilab_setBooleanArray(env, ret, v->b.data());
                    break;

                case 2: //matrix
                    ret = scilab_createBooleanMatrix2d(env, v->dims[0], v->dims[1]);
                    scilab_setBooleanArray(env, ret, v->b.data());
                    break;

                default: // > 2 dims
                    ret = scilab_createBooleanMatrix(env, (int)v->dims.size(), v->dims.data());
                    scilab_setBooleanArray(env, ret, v->b.data());
                    break;
            }
            break;
        }

        case JSON_STRING:
        {
            int size = 1;
            std::vector<wchar_t*> w;
            if (v->dims.size() > 0)
            {
                for (auto i : v->dims)
                {
                    size *= i;
                }

                w.resize(size);
                for (int i = 0; i < size; ++i)
                {
                    w[i] = to_wide_string(v->s[i]);
                }
            }
            else
            {
                w.push_back(to_wide_string(v->s1));
            }


            switch (v->dims.size())
            {
                case 0://scalar
                {
                    ret = scilab_createString(env, w[0]);
                    break;
                }
                case 1: //vector
                {
                    ret = scilab_createStringMatrix2d(env, 1, v->dims[0]);
                    scilab_setStringArray(env, ret, w.data());
                    break;
                }
                case 2: //maxtrix
                {
                    ret = scilab_createStringMatrix2d(env, v->dims[0], v->dims[1]);
                    scilab_setStringArray(env, ret, w.data());
                    break;
                }
                default: // > 2 dims
                {
                    ret = scilab_createStringMatrix(env, (int)v->dims.size(), v->dims.data());
                    scilab_setStringArray(env, ret, w.data());
                    break;
                }
            }

            for (auto wc : w)
            {
                FREE(wc);
            }

            break;
        }

        case JSON_LIST:
        {
            size_t size = v->a.size();
            ret = scilab_createList(env);

            for (size_t i = 0; i < size; ++i)
            {
                scilabVar var = createScilabVar(env, v->a[i]);
                scilab_setListItem(env, ret, i, var);
            }
            break;
        }

        case JSON_STRUCT:
        {
            size_t fsize = v->fields.size();
            std::vector<wchar_t*> fields;
            fields.reserve(fsize);
            for (auto f : v->fields)
            {
                fields.push_back(to_wide_string(f.data()));
            }

            int size = 1;
            for (auto i : v->dims)
            {
                size *= i;
            }

            switch (v->dims.size())
            {
                case 0:
                {
                    ret = scilab_createStruct(env);
                    for (size_t i = 0; i < fsize; ++i)
                    {
                        scilab_addField(env, ret, fields[i]);
                        scilabVar var = createScilabVar(env, v->o1.at(v->fields[i]));
                        scilab_setStructMatrix2dData(env, ret, fields[i], 0, 0, var);
                    }
                    break;

                    case 1:
                    {
                        if (v->dims[0] == 1) //single struct [{"toto":42}]
                        {
                            ret = scilab_createStruct(env);
                            for (size_t i = 0; i < fsize; ++i)
                            {
                                scilab_addField(env, ret, fields[i]);
                                scilabVar var = createScilabVar(env, v->o.at(v->fields[i])[0]);
                                scilab_setStructMatrix2dData(env, ret, fields[i], 0, 0, var);
                            }
                        }
                        else //row vector [{"toto":1},{"toto":2}]
                        {
                            ret = scilab_createStructMatrix2d(env, 1, size);
                            for (size_t i = 0; i < fsize; ++i)
                            {
                                scilab_addField(env, ret, fields[i]);
                                for (int x = 0; x < size; ++x)
                                {
                                    scilabVar var = createScilabVar(env, v->o.at(v->fields[i])[x]);
                                    scilab_setStructMatrix2dData(env, ret, fields[i], 0, x, var);
                                }
                            }
                        }

                        break;
                    }

                    default: //2 and more
                    {
                        ret = scilab_createStructMatrix(env, (int)v->dims.size(), v->dims.data());
                        for (size_t i = 0; i < fsize; ++i)
                        {
                            scilab_addField(env, ret, fields[i]);
                            std::vector<int> index(v->dims.size());
                            for (int x = 0; x < size; ++x)
                            {
                                getIndexArray(x, v->dims, index);
                                scilabVar var = createScilabVar(env, v->o.at(v->fields[i])[x]);
                                scilab_setStructMatrixData(env, ret, fields[i], index.data(), var);
                            }
                        }
                    }
                }
            }

            for (auto wc : fields)
            {
                FREE(wc);
            }

            break;
        }

        default:
        {
            return nullptr;
        }
    }

    return ret;
}

JSONVar* getJSONVar(const jsmntok_t& t)
{
    std::string str(json.substr(t.start, t.end - t.start));
    if (t.type == JSMN_STRING)
    {
        JSONVar* v = new JSONVar();
        v->kind = JSON_STRING;
        v->s1 = os_strdup(str.data());
        return v;
    }

    if (t.type == JSMN_PRIMITIVE)
    {
        if (str == "false" || str == "true")
        {
            JSONVar* v = new JSONVar();
            v->kind = JSON_BOOL;
            v->b1 = (str == "true" ? 1 : 0);
            return v;
        }

        try
        {
            double val = std::stod(str);
            //sciprint("it\'s a double\n");
            JSONVar* v = new JSONVar();
            v->kind = JSON_DOUBLE;
            v->d1 = val;
            return v;
        }
        catch (const std::invalid_argument& e)
        {
            JSONVar* v = new JSONVar();
            v->kind = JSON_STRING;
            v->s1 = os_strdup(str.data());
            return v;
        }
    }

    return nullptr;
}

JSONType getCommonType(JSONVar* v)
{
    JSONType ref = v->a[0]->kind;

    for (auto c : v->a)
    {
        if (c->kind != ref)
        {
            return JSON_UNDEFINED;
        }
    }

    return ref;
}

JSONVar* import_data(const jsmntok_t* t)
{
    switch (t[token_offset].type)
    {
        case JSMN_PRIMITIVE:
        case JSMN_STRING:
        {
            return getJSONVar(t[token_offset++]);
        }

        case JSMN_OBJECT:
        {
            int size = t[token_offset++].size;
            JSONVar* v = new JSONVar();
            v->kind = JSON_STRUCT;
            v->o1.reserve(size);

            for (int i = 0; i < size; ++i)
            {
                JSONVar* s = getJSONVar(t[token_offset++]);
                std::string key(s->s1);
                delete s;

                JSONVar* data = import_data(t);
                v->fields.push_back(key);
                v->o1[key] = data;
            }

            return v;
        }

        case JSMN_ARRAY:
        {
            int size = t[token_offset].size;
            JSONVar* v = new JSONVar();
            token_offset++;
            if (size == 0)
            {
                //emtpy matrix
                v->kind = JSON_EMPTY_MATRIX;
                v->reduced = true;
                return v;
            }

            v->kind = JSON_ARRAY;
            v->a.reserve(size);

            for (int i = 0; i < size; ++i)
            {
                v->a.push_back(import_data(t));
            }

            int type = getCommonType(v);

            // check type !
            if (type != JSON_UNDEFINED)
            {
                //check dims count and dims value
                std::vector<int> ref = v->a[0]->dims;
                int totalSize = 1;
                for (auto i : ref)
                {
                    totalSize *= i;
                }

                for (auto c : v->a)
                {
                    std::vector<int> dims = c->dims;
                    if (ref.size() != dims.size())
                    {
                        //reduce failed, return list
                        v->reduced = true;
                        v->kind = JSON_LIST;
                        return v;
                    }

                    for (size_t i = 0; i < ref.size(); ++i)
                    {
                        if (ref[i] != dims[i])
                        {
                            //reduce failed, return list
                            v->reduced = true;
                            v->kind = JSON_LIST;
                            return v;
                        }
                    }
                }

                //same dims, can be concat

                //first concat dims
                switch (ref.size())
                {
                    case 0: //scalar -> vector
                    {
                        v->dims.push_back((int)v->a.size());
                        break;
                    }

                    case 1:
                    {
                        //vector -> matrix
                        v->dims.resize(2);
                        v->dims[0] = (int)v->a.size();
                        v->dims[1] = ref[0];
                        break;
                    }

                    default:
                    {
                        v->dims = ref;
                        v->dims.push_back((int)v->a.size());
                        break;
                    }
                }

                //try to reduce
                switch (type)
                {
                    case JSON_DOUBLE:
                    {
                        //reduce to double array
                        switch (v->dims.size())
                        {
                            case 1: //to vector
                            {
                                v->d.resize(totalSize * v->a.size());
                                std::transform(v->a.begin(), v->a.end(), v->d.begin(),
                                               [](JSONVar * val)
                                {
                                    return val->d1;
                                });
                                break;
                            }

                            case 2: //to matrix
                            {
                                v->d.resize(totalSize * v->a.size());
                                int rows = v->dims[0];
                                int cols = v->dims[1];
                                for (int i = 0; i < rows; ++i)
                                {
                                    for (int j = 0; j < cols; ++j)
                                    {
                                        double val = v->a[i]->d[j];
                                        v->d[j * rows + i] = val;
                                    }
                                }
                                break;
                            }

                            default: //to hypermatrix
                            {
                                v->d.reserve(totalSize * v->a.size());
                                for (auto c : v->a)
                                {
                                    v->d.insert(v->d.end(), c->d.begin(), c->d.end());
                                }
                                break;
                            }
                        }

                        v->reduced = true;
                        v->kind = JSON_DOUBLE;

                        //clear a compoant
                        for (auto c : v->a)
                        {
                            delete c;
                        }

                        v->a.clear();
                        break;
                    }
                    case JSON_BOOL:
                    {
                        //reduce to bool array
                        switch (v->dims.size())
                        {
                            case 1: //to vector
                            {
                                v->b.resize(totalSize * v->a.size());
                                std::transform(v->a.begin(), v->a.end(), v->b.begin(),
                                               [](JSONVar * val)
                                {
                                    return val->b1;
                                });
                                break;
                            }

                            case 2: //to matrix
                            {
                                v->b.resize(totalSize * v->a.size());
                                int rows = v->dims[0];
                                int cols = v->dims[1];
                                for (int i = 0; i < rows; ++i)
                                {
                                    for (int j = 0; j < cols; ++j)
                                    {
                                        int val = v->a[i]->b[j];
                                        v->b[j * rows + i] = val;
                                    }
                                }
                                break;
                            }

                            default: //to hypermatrix
                            {
                                v->b.reserve(totalSize * v->a.size());
                                for (auto c : v->a)
                                {
                                    v->b.insert(v->b.end(), c->b.begin(), c->b.end());
                                }
                                break;
                            }
                        }

                        v->reduced = true;
                        v->kind = JSON_BOOL;

                        //clear a compoant
                        for (auto c : v->a)
                        {
                            delete c;
                        }

                        v->a.clear();
                        break;
                    }

                    case JSON_STRING:
                    {
                        //reduce to string array
                        switch (v->dims.size())
                        {
                            case 1: //to vector
                            {
                                v->s.resize(totalSize * v->a.size());
                                std::transform(v->a.begin(), v->a.end(), v->s.begin(),
                                               [](JSONVar * val)
                                {
                                    return os_strdup(val->s1);
                                });
                                break;
                            }

                            case 2: //to matrix
                            {
                                v->s.resize(totalSize * v->a.size());
                                int rows = v->dims[0];
                                int cols = v->dims[1];
                                for (int i = 0; i < rows; ++i)
                                {
                                    for (int j = 0; j < cols; ++j)
                                    {
                                        char* val = v->a[i]->s[j];
                                        v->s[j * rows + i] = os_strdup(val);
                                    }
                                }
                                break;
                            }

                            default: //to hypermatrix
                            {
                                v->d.reserve(totalSize * v->a.size());

                                for (auto c : v->a)
                                {
                                    for (auto s : c->s)
                                    {
                                        v->s.push_back(os_strdup(s));
                                    }
                                    //v->b.insert(v->b.end(), c->b.begin(), c->b.end());
                                }

                                break;
                            }
                        }

                        v->reduced = true;
                        v->kind = JSON_STRING;

                        //clear a compoant
                        for (auto c : v->a)
                        {
                            delete c;
                        }

                        v->a.clear();
                        break;
                    }

                    case JSON_STRUCT:
                    {
                        //check field name coherence.
                        std::vector<std::string> fref;

                        //use first element as reference;
                        for (auto c : v->a[0]->fields)
                        {
                            fref.push_back(c);
                        }

                        for (size_t i = 1; i < v->a.size(); ++i)
                        {
                            //same field count
                            if (v->a[i]->fields.size() != fref.size())
                            {
                                //reduce failed, return list
                                v->reduced = true;
                                v->kind = JSON_LIST;
                                return v;
                            }

                            //same fields
                            for (auto c : v->a[i]->fields)
                            {
                                if (std::find(fref.begin(), fref.end(), c) == fref.end())
                                {
                                    //reduce failed, return list
                                    v->reduced = true;
                                    v->kind = JSON_LIST;
                                    return v;
                                }
                            }
                        }
                        //ok same field, go concatenate !

                        //copie each value in data vector of new map.
                        v->o.reserve(fref.size());
                        switch (v->dims.size())
                        {
                            case 1: //row vector
                            {
                                for (auto f : fref)
                                {
                                    for (auto d : v->a)
                                    {
                                        v->o[f].push_back(d->o1[f]);
                                    }
                                }
                                break;
                            }
                            case 2: //matrix
                            {
                                for (auto f : fref)
                                {
                                    for (int j = 0; j < v->dims[1]; ++j)
                                    {
                                        for (int i = 0; i < v->dims[0]; ++i)
                                        {
                                            auto d = v->a[i];
                                            if (d->dims.size() == 0)
                                            {
                                                v->o[f].push_back(d->o1[f]);
                                            }
                                            else
                                            {
                                                v->o[f].push_back(d->o[f][j]);
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                            default: //hypermat
                            {
                                for (auto f : fref)
                                {
                                    for (auto d : v->a)
                                    {
                                        if (d->dims.size() == 0)
                                        {
                                            v->o[f].push_back(d->a[0]->o1[f]);
                                        }
                                        else
                                        {
                                            v->o[f].insert(v->o[f].end(), d->o[f].begin(), d->o[f].end());
                                        }
                                    }
                                }
                                break;
                            }
                        }

                        v->fields = fref;
                        v->reduced = true;
                        v->kind = JSON_STRUCT;
                        break;
                    }
                }
            }
            else
            {
                //heterogeneous -> list
                v->kind = JSON_LIST;
            }
            return v;
        }

        default:
            return nullptr;
    }
}

scilabVar fromJSON(scilabEnv env, const std::string& s)
{
    return (scilabVar)fromJSON(s);
}

types::InternalType* fromJSON(const std::string& s)
{
    json = s;
    jsmn_parser p;
    jsmn_init(&p);
    int r = jsmn_parse(&p, json.data(), (int)json.size(), nullptr, 0);
    if (r <= 0)
    {
        return nullptr;
    }

    //reset parser
    jsmn_init(&p);
    jsmntok_t *t = new jsmntok_t[r];
    jsmn_parse(&p, json.data(), (int)json.size(), t, r);

    if (r > 1 && t[0].type != JSMN_ARRAY && t[0].type != JSMN_OBJECT)
    {
        delete[] t;
        return nullptr;
    }

    token_offset = 0;
    JSONVar* v = import_data(t);

    delete[] t;
    scilabVar var = createScilabVar(nullptr, v);
    delete v;
    if (var == nullptr)
    {
        return nullptr;
    }

    return (types::InternalType*)var;
}
