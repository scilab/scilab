// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

#include "function.hxx"
#include "double.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "mlist.hxx"
#include "xlsx_lib.hxx"
#include "spreadsheet_gw.hxx"

extern "C"
{
    #include "getversion.h"
    #include "Scierror.h"
    #include "sciprint.h"
    #include "localization.h"
}

#define FUNCTION_NAME "xlsxWrite"

static XlsxMetadata create_default_metadata(const char* filename, const XlsxData* data)
{
    XlsxMetadata metadata;
    metadata.filename = filename ? filename : "";
    char* scilab_version_utf8 = getScilabVersionAsString();
    std::string creator_name = "Scilab";
    if (scilab_version_utf8)
    {
        creator_name = std::string(scilab_version_utf8);
        free(scilab_version_utf8);
    }
    metadata.creator = creator_name;
    metadata.last_modified_by = creator_name;

    time_t now = time(nullptr);
    tm* utc_tm = gmtime(&now);
    char date_buffer[32];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%dT%H:%M:%SZ", utc_tm);
    metadata.created_date = std::string(date_buffer);
    metadata.modified_date = std::string(date_buffer);

    metadata.title = "";
    metadata.subject = "";
    metadata.description = "";
    metadata.total_sheets = data ? data->sheet_count : 0;
    
    if (data && data->is_loaded)
    {
        for (int i = 0; i < data->sheet_count; i++)
        {
            metadata.sheet_names.push_back(data->sheets[i].name.empty() ? \
                "Sheet" + std::to_string(i + 1) : data->sheets[i].name);
        }
    }

    metadata.file_size = 0;
    return metadata;
}

static bool convert_scilab_to_cell(types::InternalType* scilab_value, XlsxCell& cell, int row, int col)
{
    cell.row = row + 1; // index 1 based
    cell.col = col + 1;

    if (!scilab_value)
    {
        cell.value = "";
        cell.type = XLSX_TYPE_STRING;
        return true;
    }

    // check for datetime object
    if (scilab_value->isMList())
    {
        types::MList* mlist = scilab_value->getAs<types::MList>();
        types::String* type_str = mlist->getFieldNames();
        
        if (type_str && type_str->getSize() > 0)
        {
            wchar_t* type_name = type_str->get(0);
            if (wcscmp(type_name, L"datetime") == 0)
            {
                if (mlist->getSize() > 1)
                {
                    types::InternalType* date_field = mlist->get(1);
                    if (date_field && date_field->isDouble())
                    {
                        types::Double* date_double = date_field->getAs<types::Double>();
                        types::Double* time_double = mlist->get(2)->getAs<types::Double>();
                        if (date_double->getSize() > 0)
                        {
                            // tranform to datenum value
                            double value = date_double->get(0)  + (time_double->get(0) / 86400);
                            cell.dblValue = value;
                            cell.type = XLSX_TYPE_TIMESTAMP;
                            return true;
                        }
                    }
                }
            }
            else if (wcscmp(type_name, L"duration") == 0)
            {
                types::InternalType* data_field = mlist->get(1);
                if (data_field->isDouble())
                {
                    types::Double* duration_double = data_field->getAs<types::Double>();
                    if (duration_double->getSize() > 0)
                    {
                        // excel expects day values
                        double value = duration_double->get(0);
                        cell.dblValue = value;
                        cell.type = XLSX_TYPE_DURATION;
                        return true;
                    }
                }
            }
        }
    }

    if (scilab_value->isDouble())
    {
        types::Double* double_val = scilab_value->getAs<types::Double>();
        if (double_val->getSize() > 0)
        {
            double value = double_val->get(0);
            if (std::isnan(value) || std::isinf(value))
            {
                cell.value = "";
                cell.type = XLSX_TYPE_STRING;
                return true;
            }
            
            cell.type = XLSX_TYPE_DOUBLE;
            cell.dblValue = value;
        }
        else
        {
            cell.value = "";
            cell.type = XLSX_TYPE_STRING;
        }
        return true;
    }
    
    if (scilab_value->isBool())
    {
        types::Bool* bool_val = scilab_value->getAs<types::Bool>();
        if (bool_val->getSize() > 0)
        {
            bool value = bool_val->get(0);
            cell.value = value ? "true" : "false";
            cell.type = XLSX_TYPE_BOOL;
        }
        else
        {
            cell.value = "";
            cell.type = XLSX_TYPE_STRING;
        }
        return true;
    }

    if (scilab_value->isString())
    {
        types::String* string_val = scilab_value->getAs<types::String>();
        if (string_val->getSize() > 0)
        {
            char* utf8_str = wide_string_to_UTF8(string_val->get(0));
            if (utf8_str)
            {
                cell.value = std::string(utf8_str);
                free(utf8_str);
            }
            else
                cell.value = "";
            cell.type = XLSX_TYPE_STRING;
        }
        else
        {
            cell.value = "";
            cell.type = XLSX_TYPE_STRING;
        }
        return true;
    }

    cell.value = "";
    cell.type = XLSX_TYPE_STRING;
    return true;
}

static bool convert_scilab_struct_to_xlsx_data(types::Struct* scilab_struct, XlsxData& xlsx_data, XlsxMetadata& xlsx_metadata)
{
    if (!scilab_struct || scilab_struct->getSize() == 0)
    {
        return false;
    }

    types::SingleStruct* single_struct = scilab_struct->get(0);
    if (!single_struct)
    {
        return false;
    }

    xlsx_data.sheets.clear();
    xlsx_data.sheet_count = 0;
    xlsx_data.is_loaded = false;
    xlsx_data.filename = "";

    types::InternalType* filename_field = nullptr;
    filename_field = single_struct->get(L"filename");

    if (filename_field && filename_field->isString())
    {
        types::String* filename_str = filename_field->getAs<types::String>();
        if (filename_str->getSize() > 0)
        {
            char* utf8_filename = wide_string_to_UTF8(filename_str->get(0));
            if (utf8_filename)
            {
                xlsx_data.filename = std::string(utf8_filename);
                free(utf8_filename);
            }
        }
    }

    types::InternalType* title_field = single_struct->get(L"title");
    if (title_field && title_field->isString())
    {
        types::String* title_str = title_field->getAs<types::String>();
        if (title_str->getSize() > 0)
        {
            char* utf8_title = wide_string_to_UTF8(title_str->get(0));
            if (utf8_title)
            {
                xlsx_metadata.title = std::string(utf8_title);
                free(utf8_title);
            }
        }
    }

    types::InternalType* subject_field = single_struct->get(L"subject");
    if (subject_field && subject_field->isString())
    {
        types::String* subject_str = subject_field->getAs<types::String>();
        if (subject_str->getSize() > 0)
        {
            char* utf8_subject = wide_string_to_UTF8(subject_str->get(0));
            if (utf8_subject)
            {
                xlsx_metadata.subject = std::string(utf8_subject);
                free(utf8_subject);
            }
        }
    }

    types::InternalType* description_field = single_struct->get(L"description");
    if (description_field && description_field->isString())
    {
        types::String* description_str = description_field->getAs<types::String>();
        if (description_str->getSize() > 0)
        {
            char* utf8_description = wide_string_to_UTF8(description_str->get(0));
            if (utf8_description)
            {
                xlsx_metadata.description = std::string(utf8_description);
                free(utf8_description);
            }
        }
    }

    types::InternalType* sheets_field = single_struct->get(L"sheets");
    if (!sheets_field || !sheets_field->isList())
    {
        return false;
    }

    types::List* sheets_list = sheets_field->getAs<types::List>();
    xlsx_data.sheet_count = static_cast<int>(sheets_list->getSize());
    xlsx_data.sheets.reserve(xlsx_data.sheet_count);

    for (int sheet_idx = 0; sheet_idx < xlsx_data.sheet_count; sheet_idx++)
    {
        types::InternalType* sheet_item = sheets_list->get(sheet_idx);
        types::Struct* sheet_struct = sheet_item->getAs<types::Struct>();
        types::SingleStruct* single_sheet = sheet_struct->get(0);

        XlsxSheet sheet;
        types::InternalType* name_field = single_sheet->get(L"name");
        if (name_field && name_field->isString())
        {
            types::String* name_str = name_field->getAs<types::String>();
            if (name_str->getSize() > 0)
            {
                char* utf8_name = wide_string_to_UTF8(name_str->get(0));
                if (utf8_name)
                {
                    sheet.name = std::string(utf8_name);
                    free(utf8_name);
                }
            }
        }

        types::InternalType* max_row_field = single_sheet->get(L"max_row");
        types::InternalType* max_col_field = single_sheet->get(L"max_col");

        if (max_row_field && max_row_field->isDouble())
        {
            types::Double* max_row_double = max_row_field->getAs<types::Double>();
            if (max_row_double->getSize() > 0)
            {
                sheet.max_row = static_cast<int>(max_row_double->get(0));
            }
        }

        if (max_col_field && max_col_field->isDouble())
        {
            types::Double* max_col_double = max_col_field->getAs<types::Double>();
            if (max_col_double->getSize() > 0)
            {
                sheet.max_col = static_cast<int>(max_col_double->get(0));
            }
        }

        types::InternalType* data_field = single_sheet->get(L"data");
        if (data_field && (data_field->isDouble() || data_field->isString() || data_field->isCell()) && sheet.max_row > 0 && sheet.max_col > 0)
        {
            sheet.cells.resize(sheet.max_row);
            for (int i = 0; i < sheet.max_row; i++)
            {
                sheet.cells[i].resize(sheet.max_col);
            }

            if (data_field->isDouble())
            {
                types::Double* pDbl = data_field->getAs<types::Double>();
                int actual_rows = static_cast<int>(pDbl->getRows());
                int actual_cols = static_cast<int>(pDbl->getCols());

                for (int row = 0; row < std::min(sheet.max_row, actual_rows); row++)
                {
                    for (int col = 0; col < std::min(sheet.max_col, actual_cols); col++)
                    {
                        double dblvalue = pDbl->get(row, col);

                        if (std::isnan(dblvalue) || std::isinf(dblvalue))
                        {
                            sheet.cells[row][col].value = "";
                            sheet.cells[row][col].type = XLSX_TYPE_STRING;
                            continue;
                        }
                            
                        sheet.cells[row][col].type = XLSX_TYPE_DOUBLE;
                        sheet.cells[row][col].dblValue = dblvalue;
                    }
                }
            }
            else if (data_field->isString())
            {
                types::String* pStr = data_field->getAs<types::String>();
                int actual_rows = static_cast<int>(pStr->getRows());
                int actual_cols = static_cast<int>(pStr->getCols());

                for (int row = 0; row < std::min(sheet.max_row, actual_rows); row++)
                {
                    for (int col = 0; col < std::min(sheet.max_col, actual_cols); col++)
                    {
                        char* str = wide_string_to_UTF8(pStr->get(row, col));

                        if (str)
                        {
                            sheet.cells[row][col].value = std::string(str);
                            free(str);
                        }
                        else
                        {
                            sheet.cells[row][col].value = "";
                        }
                                
                        sheet.cells[row][col].type = XLSX_TYPE_STRING;
                    }
                }
            }
            else
            {
                // data_field is a cell
                types::Cell* data_matrix = data_field->getAs<types::Cell>();
                int actual_rows = static_cast<int>(data_matrix->getRows());
                int actual_cols = static_cast<int>(data_matrix->getCols());

                for (int row = 0; row < std::min(sheet.max_row, actual_rows); row++)
                {
                    for (int col = 0; col < std::min(sheet.max_col, actual_cols); col++)
                    {
                        types::InternalType* cell_value = data_matrix->get(row, col);
                        convert_scilab_to_cell(cell_value, sheet.cells[row][col], row, col);
                    }
                }
            }
        }
        else
        {
            sheet.max_row = 0;
            sheet.max_col = 0;
            sheet.cells.clear();
        }
        xlsx_data.sheets.push_back(sheet);
    }
    xlsx_data.is_loaded = true;
    return true;
}

types::Function::ReturnValue sci_xlsxWrite(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 2 || in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input arguments: expected %d to %d arguments, found %d\n"), FUNCTION_NAME, 2, 6, in.size());
        return types::Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), FUNCTION_NAME, 1);
        return types::Function::Error;
    }

    if (!in[1]->isStruct())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: struct expected.\n"), FUNCTION_NAME, 2);
        return types::Function::Error;
    }

    std::string sheet_name;
    int sheet_index = -1;
    bool has_sheet_name = false;
    bool has_sheet_index = false;
    bool clear_sheet = false;
    bool append_mode = false;
    std::string range;

    if (in.size() >= 3) 
    {
        if (in[2]->isString()) {
            types::String* sheet_name_str = in[2]->getAs<types::String>();
            if (sheet_name_str->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: single string expected.\n"), FUNCTION_NAME, 3);
                return types::Function::Error;
            }
            char* utf8_sheet_name = wide_string_to_UTF8(sheet_name_str->get(0));
            if (utf8_sheet_name)
            {
                sheet_name = std::string(utf8_sheet_name);
                free(utf8_sheet_name);
                has_sheet_name = true;
            }
        }
        else if (in[2]->isDouble())
        {
            types::Double* sheet_index_double = in[2]->getAs<types::Double>();
            if (sheet_index_double->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: single number expected.\n"), FUNCTION_NAME, 3);
                return types::Function::Error;
            }
            double idx_val = sheet_index_double->get(0);
            if (idx_val < 1 || idx_val != static_cast<int>(idx_val))
            {
                Scierror(999, _("%s: Sheet index must be a positive integer >= 1.\n"), FUNCTION_NAME);
                return types::Function::Error;
            }
            sheet_index = static_cast<int>(idx_val) - 1;
            has_sheet_index = true;
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string or number expected.\n"), FUNCTION_NAME, 3);
            return types::Function::Error;
        }
    }

    if (in.size() >= 4)
    {
        if (!has_sheet_name && !has_sheet_index)
        {
            Scierror(999, _("%s: Range parameter (argument #4) can only be used when sheet name or index (argument #3) is provided.\n"), FUNCTION_NAME);
            return types::Function::Error;
        }
        if (!in[3]->isString())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), FUNCTION_NAME, 4);
            return types::Function::Error;
        }
        types::String* range_str = in[3]->getAs<types::String>();
        if (range_str->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: single string expected.\n"), FUNCTION_NAME, 4);
            return types::Function::Error;
        }
        char* utf8_range = wide_string_to_UTF8(range_str->get(0));
        if (utf8_range)
        {
            range = std::string(utf8_range);
            free(utf8_range);
        }
    }

    if (in.size() >= 5)
    {
        if (!in[4]->isBool())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: boolean expected.\n"), FUNCTION_NAME, 5);
            return types::Function::Error;
        }
        types::Bool* clear_bool = in[4]->getAs<types::Bool>();
        if (clear_bool->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: single boolean expected.\n"), FUNCTION_NAME, 5);
            return types::Function::Error;
        }
        clear_sheet = clear_bool->get(0);
    }

    if (in.size() == 6)
    {
        if (!in[5]->isBool())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: boolean expected.\n"), FUNCTION_NAME, 6);
            return types::Function::Error;
        }
        types::Bool* append_bool = in[5]->getAs<types::Bool>();
        if (append_bool->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: single boolean expected.\n"), FUNCTION_NAME, 6);
            return types::Function::Error;
        }
        append_mode = append_bool->get(0);
    }

    types::String* filename_str = in[0]->getAs<types::String>();
    if (filename_str->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: single string expected.\n"), FUNCTION_NAME, 1);
        return types::Function::Error;
    }

    types::Struct* data_struct = in[1]->getAs<types::Struct>();
    char* filename = wide_string_to_UTF8(filename_str->get(0));
    if (!filename)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), FUNCTION_NAME);
        return types::Function::Error;
    }

    XlsxData xlsx_data;
    XlsxMetadata custom_metadata = create_default_metadata(filename, nullptr);

    if (!convert_scilab_struct_to_xlsx_data(data_struct, xlsx_data, custom_metadata))
    {
        free(filename);
        Scierror(999, _("%s: Error converting Scilab structure to XLSX data.\n"), FUNCTION_NAME);
        return types::Function::Error;
    }

    custom_metadata.filename = filename;
    custom_metadata.total_sheets = xlsx_data.sheet_count;
    custom_metadata.sheet_names.clear();
    if (xlsx_data.is_loaded)
    {
        for (int i = 0; i < xlsx_data.sheet_count; i++)
        {
            custom_metadata.sheet_names.push_back(xlsx_data.sheets[i].name.empty() ? "Sheet" + std::to_string(i + 1) : xlsx_data.sheets[i].name);
        }
    }

    XlsxWriteOptions* write_options = nullptr;
    XlsxWriteOptions options;
    if (has_sheet_name || has_sheet_index)
    {
        if (has_sheet_name)
        {
            options.sheet_name = sheet_name;
            options.sheet_index = -1;
        }
        else
        {
            options.sheet_name = "";
            options.sheet_index = sheet_index;
        }
        options.range = range;
        options.clear_sheet = clear_sheet;
        options.append_mode = append_mode;
        write_options = &options;
    }
    else if (clear_sheet || append_mode)
    {
        // use defaults
        options.sheet_name = "";
        options.sheet_index = -1;
        options.range = "";
        options.clear_sheet = clear_sheet;
        options.append_mode = append_mode;
        write_options = &options;
    }

    int result = xlsx_write(filename, &xlsx_data, &custom_metadata, write_options);
    free(filename);

    if (result != 0)
    {
        xlsx_free_data(&xlsx_data);
        switch (result)
        {
            case -1:
                Scierror(999, _("%s: Invalid parameters.\n"), FUNCTION_NAME);
                break;
            case -2:
                Scierror(999, _("%s: XLNT library error.\n"), FUNCTION_NAME);
                break;
            case -3:
                Scierror(999, _("%s: General error writing file.\n"), FUNCTION_NAME);
                break;
            case -4:
                Scierror(999, _("%s: Unknown error writing file.\n"), FUNCTION_NAME);
                break;
            case -5:
                Scierror(999, _("%s: Invalid range format. Use formats like 'A1:B2', 'A:C', '1:3', 'B5', 'B', or '3'.\n"), FUNCTION_NAME);
                break;
            case -6:
                if (has_sheet_index)
                {
                    Scierror(999, _("%s: Sheet index %d does not exist in the file.\n"), FUNCTION_NAME, sheet_index + 1);
                }
                else
                {
                    Scierror(999, _("%s: Error accessing target sheet.\n"), FUNCTION_NAME);
                }
                break;
            case -7:
                if (has_sheet_name)
                {
                    Scierror(999, _("%s: Sheet \"%s\" does not exist in the file.\n"), FUNCTION_NAME, sheet_name.c_str());
                }
                else
                {
                    Scierror(999, _("%s: Sheet does not exist in the file.\n"), FUNCTION_NAME);
                }
                break;
            case -8:
                Scierror(999, _("%s: The file '%s' exists but cannot be read: writing would destroy its content.\n"), FUNCTION_NAME, custom_metadata.filename.c_str());
                break;
            default:
                Scierror(999, _("%s: Error writing XLSX file (code %d).\n"), FUNCTION_NAME, result);
        }
        return types::Function::Error;
    }
    xlsx_free_data(&xlsx_data);

    return types::Function::OK;
}
