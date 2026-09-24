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
#include "string.hxx"
#include "struct.hxx"
#include "xlsx_lib.hxx"
#include "spreadsheet_gw.hxx"

extern "C"
{
    #include "Scierror.h"
    #include "localization.h"
    #include "FileExist.h"
}

#define FUNCTION_NAME "xlsxInfo"

types::Function::ReturnValue sci_xlsxInfo(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1 || in.size() > 2) 
    {
        Scierror(77, _("%s: Wrong number of input arguments: %d to %d expected.\n"), FUNCTION_NAME, 1, 2);
        return types::Function::Error;
    }

    if (!in[0]->isString()) 
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), FUNCTION_NAME, 1);
        return types::Function::Error;
    }

    types::String* filename_str = in[0]->getAs<types::String>();
    if (filename_str->getSize() != 1) 
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), FUNCTION_NAME, 1);
        return types::Function::Error;
    }

    char* filename = wide_string_to_UTF8(filename_str->get(0));
    if (!filename) 
    {
        Scierror(999, _("%s: Memory allocation error.\n"), FUNCTION_NAME);
        return types::Function::Error;
    }

    if (!FileExist(filename))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: The file \"%s\" does not exist.\n"), FUNCTION_NAME, 1, filename);
        free(filename);
        return types::Function::Error;
    }

    char* sheet_name = nullptr;
    std::string sheet_name_str;
    int sheet_index = -1;
    if (in.size() == 2)
    {
        if (in[1]->isString())
        {
            // name
            types::String* sheet_str = in[1]->getAs<types::String>();
            if (sheet_str->getSize() != 1)
            {
                free(filename);
                Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), FUNCTION_NAME, 2);
                return types::Function::Error;
            }
            sheet_name = wide_string_to_UTF8(sheet_str->get(0));
            if (sheet_name)
            {
                sheet_name_str.assign(sheet_name);
                // empty strings
                if (sheet_name_str.empty())
                {
                    free(filename);
                    free(sheet_name);
                    Scierror(999, _("%s: Wrong value for input argument #%d: Sheet name cannot be empty.\n"), FUNCTION_NAME, 2);
                    return types::Function::Error;
                }
            }
        }
        else if (in[1]->isDouble())
        {
            // index
            types::Double* idx = in[1]->getAs<types::Double>();
            if (idx->getSize() != 1)
            {
                free(filename);
                Scierror(999, _("%s: Wrong size for input argument #%d: scalar expected.\n"), FUNCTION_NAME, 2);
                return types::Function::Error;
            }
            sheet_index = static_cast<int>(idx->get(0));
            if (sheet_index < 1)
            {
                free(filename);
                Scierror(999, _("%s: Wrong value for input argument #%d: Sheet index must be >= 1.\n"), FUNCTION_NAME, 2);
                return types::Function::Error;
            }
        }
        else
        {
            free(filename);
            Scierror(999, _("%s: Wrong type for input argument #%d: A string or double expected.\n"), FUNCTION_NAME, 2);
            return types::Function::Error;
        }
    }

    XlsxMetadata metadata;
    int result = xlsx_read_metadata(filename, &metadata, sheet_name, sheet_index);
    if (sheet_name)
    {
        free(sheet_name);
    }

    if (result != 0)
    {
        switch (result)
        {
            // -1 means a null argument in xlsx_read_metadata, which cannot happen here
            // leave it to the default branch, which reports the raw code.
            case -2:
                Scierror(999, _("%s: The file '%s' cannot be read as an XLSX file.\n"), FUNCTION_NAME, filename);
                break;
            case -3:
                Scierror(999, _("%s: Error reading metadata of file '%s'.\n"), FUNCTION_NAME, filename);
                break;
            case -4:
                Scierror(999, _("%s: Unknown error reading metadata.\n"), FUNCTION_NAME);
                break;
            case -5:
                if (!sheet_name_str.empty())
                    Scierror(999, _("%s: Sheet \"%s\" not found.\n"), FUNCTION_NAME, sheet_name_str.c_str());
                else if (sheet_index != -1)
                    Scierror(999, _("%s: Sheet index %d out of range.\n"), FUNCTION_NAME, sheet_index);
                else
                    Scierror(999, _("%s: Sheet not found.\n"), FUNCTION_NAME);
                break;
            default:
                Scierror(999, _("%s: Error reading XLSX metadata (code %d).\n"), FUNCTION_NAME, result);
        }
        free(filename);
        return types::Function::Error;
    }

    free(filename);

    types::Struct* metadata_struct = new types::Struct(1, 1);
    
    if (metadata.sheet_info.is_sheet_info)
    {
        // return sheet information
        metadata_struct->addField(L"sheet_name");
        metadata_struct->addField(L"sheet_index");
        
        if (metadata.sheet_info.is_empty)
        {
            metadata_struct->addField(L"sheet_empty");
        }
        else
        {
            metadata_struct->addField(L"max_row");
            metadata_struct->addField(L"max_col");
            metadata_struct->addField(L"cell_count");
        }

        types::SingleStruct* single_struct = metadata_struct->get(0);

        types::String* name_field = new types::String(metadata.sheet_info.sheet_name.c_str());
        single_struct->set(L"sheet_name", name_field);

        types::Double* index_field = new types::Double(static_cast<double>(metadata.sheet_info.sheet_index));
        single_struct->set(L"sheet_index", index_field);

        if (metadata.sheet_info.is_empty)
        {
            types::Bool* empty_field = new types::Bool(1);
            single_struct->set(L"sheet_empty", empty_field);
        }
        else
        {
            types::Double* max_row_field = new types::Double(static_cast<double>(metadata.sheet_info.max_row));
            single_struct->set(L"max_row", max_row_field);

            types::Double* max_col_field = new types::Double(static_cast<double>(metadata.sheet_info.max_col));
            single_struct->set(L"max_col", max_col_field);

            types::Double* cell_count_field = new types::Double(static_cast<double>(metadata.sheet_info.cell_count));
            single_struct->set(L"cell_count", cell_count_field);
        }
    }
    else
    {
        // return file metadata
        metadata_struct->addField(L"filename");
        metadata_struct->addField(L"total_sheets");
        metadata_struct->addField(L"sheet_names");
        metadata_struct->addField(L"creator");
        metadata_struct->addField(L"last_modified_by");
        metadata_struct->addField(L"created_date");
        metadata_struct->addField(L"modified_date");
        metadata_struct->addField(L"title");
        metadata_struct->addField(L"subject");
        metadata_struct->addField(L"description");
        metadata_struct->addField(L"file_size");
        
        types::SingleStruct* single_struct = metadata_struct->get(0);

        types::String* filename_field = new types::String(metadata.filename.c_str());
        single_struct->set(L"filename", filename_field);

        types::Double* total_sheets_field = new types::Double(static_cast<double>(metadata.total_sheets));
        single_struct->set(L"total_sheets", total_sheets_field);

        if (metadata.sheet_names.size() > 0)
        {
            types::String* sheet_names_array = new types::String(1, static_cast<int>(metadata.sheet_names.size()));
            for (size_t i = 0; i < metadata.sheet_names.size(); i++)
            {
                sheet_names_array->set(0, static_cast<int>(i), metadata.sheet_names[i].c_str());
            }                
            single_struct->set(L"sheet_names", sheet_names_array);
        }
        else
        {
            types::String* empty_array = new types::String(1, 0);
            single_struct->set(L"sheet_names", empty_array);
        }

        types::String* creator_field = new types::String(metadata.creator.c_str());
        single_struct->set(L"creator", creator_field);
        
        types::String* last_modified_field = new types::String(metadata.last_modified_by.c_str());
        single_struct->set(L"last_modified_by", last_modified_field);

        types::String* created_field = new types::String(metadata.created_date.c_str());
        single_struct->set(L"created_date", created_field);

        types::String* modified_field = new types::String(metadata.modified_date.c_str());
        single_struct->set(L"modified_date", modified_field);

        types::String* title_field = new types::String(metadata.title.c_str());
        single_struct->set(L"title", title_field);

        types::String* subject_field = new types::String(metadata.subject.c_str());
        single_struct->set(L"subject", subject_field);

        types::String* description_field = new types::String(metadata.description.c_str());
        single_struct->set(L"description", description_field);

        types::Double* file_size_field = new types::Double(static_cast<double>(metadata.file_size));
        single_struct->set(L"file_size", file_size_field);
    }

    out.push_back(metadata_struct);
    return types::Function::OK;
}
