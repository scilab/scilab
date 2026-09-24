// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

#include <xlnt/xlnt.hpp>
#include <algorithm>
#include <cstring>
#include <fstream>
#include "xlsx_lib.hxx"
#include "xlsx_streaming.hxx"

int xlsx_valid_file(const char* filename)
{
    if (!filename || filename[0] == '\0')
    {
        return 0; // xlsx_is_valid_file: empty or NULL filename
    }

    std::ifstream file_check(filename, std::ios::binary);
    if (!file_check.good())
    {
        return -1; // xlsx_is_valid_file: cannot open file
    }
    file_check.close();

    try
    {
        xlnt::workbook wb;
        wb.load(filename);

        if (wb.sheet_count() == 0)
        {
            return -2; // xlsx_is_valid_file: file contains no sheets
        }
        try
        {
            auto active_sheet = wb.active_sheet();
            (void)active_sheet; // avoid unused variable warning
        }
        catch (const std::exception& e)
        {
            return -3; // xlsx_is_valid_file: active sheet access error
        }

        return 1; // all good
    }
    catch (const xlnt::exception& e)
    {
        std::vector<std::string> titles;
        if (xlsx_streaming_titles(filename, titles))
        {
            return 1; // readable through the streaming fallback
        }
        return -4; // xlsx_is_valid_file: xlnt error when loading
    }
    catch (const std::exception& e)
    {
        return -5; // xlsx_is_valid_file: general error when loading
    }
    catch (...)
    {
        return -6; // xlsx_is_valid_file: unknown error when loading
    }
}


static long get_file_size(const char* filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        return -1;
    return static_cast<long>(file.tellg());
}

std::string datetime_to_string(const xlnt::datetime& dt)
{
    try
    {
        return std::to_string(dt.year) + "-" + 
               (dt.month < 10 ? "0" : "") + std::to_string(dt.month) + "-" +
               (dt.day < 10 ? "0" : "") + std::to_string(dt.day) + " " +
               (dt.hour < 10 ? "0" : "") + std::to_string(dt.hour) + ":" +
               (dt.minute < 10 ? "0" : "") + std::to_string(dt.minute) + ":" +
               (dt.second < 10 ? "0" : "") + std::to_string(dt.second);
    }
    catch (...)
    {
        return "Unknown";
    }
}

// Streaming counterpart of xlsx_read_metadata. Core properties stay empty:
// the streaming reader exposes only the sheet titles and their cells.
static int xlsx_read_metadata_streaming(const char* filename, XlsxMetadata* metadata, const char* sheet_name, int sheet_index)
{
    std::vector<std::string> titles;

    if (sheet_name == nullptr && sheet_index < 0)
    {
        if (!xlsx_streaming_titles(filename, titles))
        {
            return -2;
        }

        metadata->total_sheets = static_cast<int>(titles.size());
        metadata->sheet_names = titles;
        return 0;
    }

    metadata->sheet_info.is_sheet_info = true;

    std::string target;
    if (sheet_name != nullptr && strlen(sheet_name) > 0)
    {
        target = sheet_name;
    }

    int found = -1;
    int max_row = 0;
    int max_col = 0;
    int count = 0;

    auto want = [&](const std::string& title, int index)
    {
        if (!target.empty())
        {
            if (title != target)
            {
                return false;
            }
        }
        else if (sheet_index <= 0 || index != sheet_index - 1)
        {
            return false;
        }

        found = index;
        return true;
    };

    auto on_cell = [&](int, xlnt::cell cell)
    {
        // The reference path sizes the sheet from its declared dimension, so
        // trailing cells that carry only a style still count here.
        if (static_cast<int>(cell.row()) > max_row)
        {
            max_row = static_cast<int>(cell.row());
        }
        if (static_cast<int>(cell.column().index) > max_col)
        {
            max_col = static_cast<int>(cell.column().index);
        }

        if (!cell.to_string().empty())
        {
            count++;
        }
    };

    if (!xlsx_streaming_scan(filename, titles, want, on_cell))
    {
        return -2;
    }

    if (found < 0)
    {
        return -5; // sheet not found or index out of range
    }

    metadata->sheet_info.sheet_name.assign(titles[found]);
    metadata->sheet_info.sheet_index = found + 1;
    metadata->sheet_info.cell_count = count;
    metadata->sheet_info.is_empty = (count == 0);
    metadata->sheet_info.max_row = count > 0 ? max_row : 0;
    metadata->sheet_info.max_col = count > 0 ? max_col : 0;

    return 0;
}

int xlsx_read_metadata(const char* filename, XlsxMetadata* metadata, const char* sheet_name, int sheet_index)
{
    if (!filename || !metadata)
        return -1;

    metadata->filename.assign(filename);
    metadata->creator.assign("");
    metadata->last_modified_by.assign("");
    metadata->created_date.assign("");
    metadata->modified_date.assign("");
    metadata->title.assign("");
    metadata->subject.assign("");
    metadata->description.assign("");
    metadata->total_sheets = 0;
    metadata->sheet_names.clear();
    metadata->file_size = get_file_size(filename);
    metadata->sheet_info.is_sheet_info = false;
    metadata->sheet_info.sheet_name.assign("");
    metadata->sheet_info.sheet_index = -1;
    metadata->sheet_info.is_empty = false;
    metadata->sheet_info.max_row = 0;
    metadata->sheet_info.max_col = 0;
    metadata->sheet_info.cell_count = 0;

    try
    {
        xlnt::workbook wb;
        wb.load(filename);

        // if sheet_name or sheet_index is provided, return the sheet info ?
        if (sheet_name != nullptr || sheet_index >= 0)
        {
            metadata->sheet_info.is_sheet_info = true;
            xlnt::worksheet ws;

            if (sheet_name != nullptr && strlen(sheet_name) > 0)
            {
                if (!wb.contains(sheet_name))
                    return -5; // sheet not found
                ws = wb.sheet_by_title(sheet_name);
                metadata->sheet_info.sheet_name.assign(sheet_name);

                // find sheet index
                auto titles = wb.sheet_titles();
                for (size_t i = 0; i < titles.size(); i++)
                {
                    if (titles[i] == sheet_name)
                    {
                        metadata->sheet_info.sheet_index = static_cast<int>(i) + 1;
                        break;
                    }
                }
            }
            else if (sheet_index > 0)
            {
                if (sheet_index > static_cast<int>(wb.sheet_count()))
                    return -5; // index out of range
                ws = wb.sheet_by_index(static_cast<size_t>(sheet_index - 1));
                metadata->sheet_info.sheet_name.assign(ws.title());
                metadata->sheet_info.sheet_index = sheet_index;
            }

            // sheet information
            auto used_range = ws.calculate_dimension();
            int row_count = static_cast<int>(used_range.bottom_right().row());
            int col_count = static_cast<int>(used_range.bottom_right().column_index());
            
            // check if the sheet is empty
            if (row_count == 1 && col_count == 1)
            {
                auto cell = ws.cell(1, 1);
                if (cell.to_string().empty())
                {
                    metadata->sheet_info.is_empty = true;
                    metadata->sheet_info.max_row = 0;
                    metadata->sheet_info.max_col = 0;
                }
                else
                {
                    metadata->sheet_info.is_empty = false;
                    metadata->sheet_info.max_row = row_count;
                    metadata->sheet_info.max_col = col_count;
                }
            }
            else
            {
                metadata->sheet_info.is_empty = false;
                metadata->sheet_info.max_row = row_count;
                metadata->sheet_info.max_col = col_count;
            }

            int count = 0;
            for (auto row : ws.rows(false))
            {
                for (auto cell : row)
                {
                    if (!cell.to_string().empty())
                        count++;
                }
            }
            metadata->sheet_info.cell_count = count;
            
            return 0;
        }

        if (wb.has_core_property(xlnt::core_property::creator))
            metadata->creator.assign(wb.core_property(xlnt::core_property::creator).get<std::string>());
        if (wb.has_core_property(xlnt::core_property::last_modified_by))
            metadata->last_modified_by.assign(wb.core_property(xlnt::core_property::last_modified_by).get<std::string>());
        if (wb.has_core_property(xlnt::core_property::title))
            metadata->title.assign(wb.core_property(xlnt::core_property::title).get<std::string>());
        if (wb.has_core_property(xlnt::core_property::subject))
            metadata->subject.assign(wb.core_property(xlnt::core_property::subject).get<std::string>());
        if (wb.has_core_property(xlnt::core_property::description))
            metadata->description.assign(wb.core_property(xlnt::core_property::description).get<std::string>());
        if (wb.has_core_property(xlnt::core_property::created))
        {
            try
            {
                auto created = wb.core_property(xlnt::core_property::created);
                if (created.is(xlnt::variant::type::lpstr))
                    metadata->created_date.assign(created.get<std::string>());
                else if (created.is(xlnt::variant::type::date))
                {
                    auto dt = created.get<xlnt::datetime>();
                    metadata->created_date.assign(datetime_to_string(dt));
                }
                else
                    metadata->created_date.assign("Created date found (unknown type)");
            }
            catch (...)
            {
                metadata->created_date.assign("Unknown");
            }
        }
        if (wb.has_core_property(xlnt::core_property::modified))
        {
            try
            {
                auto modified = wb.core_property(xlnt::core_property::modified);
                if (modified.is(xlnt::variant::type::lpstr))
                    metadata->modified_date.assign(modified.get<std::string>());
                else if (modified.is(xlnt::variant::type::date))
                {
                    auto dt = modified.get<xlnt::datetime>();
                    metadata->modified_date.assign(datetime_to_string(dt));
                }
                else
                    metadata->modified_date.assign("Modified date found (unknown type)");
            }
            catch (...)
            {
                metadata->modified_date.assign("Unknown");
            }
        }

        metadata->total_sheets = static_cast<int>(wb.sheet_count());
        auto sheet_titles = wb.sheet_titles();
        for (const auto& title : sheet_titles)
            metadata->sheet_names.push_back(title);

        return 0; // success
    }
    catch (const xlnt::exception& e)
    {
        return xlsx_read_metadata_streaming(filename, metadata, sheet_name, sheet_index);
    }
    catch (const std::exception& e)
    {
        // general error
        return -3;
    }
    catch (...)
    {
        // unknown error
        return -4;
    }
}


int xlsx_sheet(const char* filename, const XlsxSheetOptions* options)
{
    if (filename == nullptr || options == nullptr)
        return -1;

    try
    {
        xlnt::workbook wb;
        bool file_exists = false;

        std::ifstream test_file(filename);
        if (test_file.good())
        {
            file_exists = true;
            test_file.close();
            wb.load(filename);
        }
        else
        {
            test_file.close();
            if (options->operation != XLSX_SHEET_CREATE)
                return -1;
        }

        switch (options->operation)
        {
            case XLSX_SHEET_CREATE:
            {
                if (!options->sheet_name.empty())
                {
                    if (wb.contains(options->sheet_name))
                        return -2; // name already exists
                }

                xlnt::worksheet new_ws;
                
                if (!file_exists)
                {
                    if (wb.sheet_count() > 0)
                        wb.remove_sheet(wb.sheet_by_index(0));
                    new_ws = wb.create_sheet();
                    new_ws.title(options->sheet_name);
                }
                else if (options->use_index && options->sheet_index > 0)
                {
                    int target_index = options->sheet_index - 1;
                    int sheet_count = static_cast<int>(wb.sheet_count());

                    if (target_index > sheet_count)
                        target_index = sheet_count; // append at end if index is too large
                    // create the sheet
                    new_ws = wb.create_sheet(target_index);
                    new_ws.title(options->sheet_name);
                }
                else
                {
                    new_ws = wb.create_sheet();
                    new_ws.title(options->sheet_name);
                }
                break;
            }

            case XLSX_SHEET_DELETE:
            {
                if (wb.sheet_count() <= 1)
                    return -5; // cannot delete the only remaining sheet
                
                xlnt::worksheet ws_to_delete;
                bool found = false;

                if (options->use_index && options->sheet_index > 0)
                {
                    // index
                    int target_index = options->sheet_index - 1; // convert 0-based
                    if (target_index < 0 || target_index >= static_cast<int>(wb.sheet_count()))
                        return -4; // invalid index

                    ws_to_delete = wb.sheet_by_index(target_index);
                    found = true;
                }
                else if (!options->sheet_name.empty())
                {
                    // name
                    if (wb.contains(options->sheet_name))
                    {
                        ws_to_delete = wb.sheet_by_title(options->sheet_name);
                        found = true;
                    }
                }

                if (!found)
                    return -3; // sheet not found

                wb.remove_sheet(ws_to_delete);
                break;
            }
            
            case XLSX_SHEET_RENAME:
            {
                bool found = false;
                std::string old_title;
                std::size_t sheet_idx = 0;

                if (options->use_index && options->sheet_index > 0)
                {
                    // index
                    int target_index = options->sheet_index - 1; // convert 0-based
                    if (target_index < 0 || target_index >= static_cast<int>(wb.sheet_count()))
                        return -4; // invalid index

                    sheet_idx = static_cast<std::size_t>(target_index);
                    old_title = wb.sheet_by_index(sheet_idx).title();
                    found = true;
                }
                else if (!options->sheet_name.empty())
                {
                    // name
                    if (wb.contains(options->sheet_name))
                    {
                        old_title = options->sheet_name;
                        for (std::size_t i = 0; i < wb.sheet_count(); ++i)
                        {
                            if (wb.sheet_by_index(i).title() == options->sheet_name)
                            {
                                sheet_idx = i;
                                break;
                            }
                        }
                        found = true;
                    }
                }

                if (!found)
                    return -3; // sheet not found
                if (wb.contains(options->new_sheet_name))
                    return -6; // new name already exists

                try
                {
                    wb.sheet_by_title(old_title).title(options->new_sheet_name);
                }
                catch (const std::exception& e)
                {
                    //std::cerr << "Error renaming sheet: " << e.what() << std::endl;
                    return -1;
                }

                break;
            }

            case XLSX_SHEET_INFO: // I already have metadata, maybe I can call it here..?
            default:
                return 0;
        }
        try
        {
            wb.save(filename);
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error saving workbook: " << e.what() << std::endl;
            return -1;
        }

        return 0; // success
    }
    catch (const std::exception& e)
    {
        //std::cerr << "Error in xlsx_sheet: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        //std::cerr << "Unknown error in xlsx_sheet" << std::endl;
        return -1;
    }
}
