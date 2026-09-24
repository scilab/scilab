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
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include "xlsx_lib.hxx"
#include <ctime>
#include <cctype>


static bool make_write_range_from_string(const std::string &range_str, xlnt::range_reference &out_ref, int max_row, int max_col)
{
    if (range_str.empty())
    {
        out_ref = xlnt::range_reference(xlnt::cell_reference(1, 1), xlnt::cell_reference(xlnt::column_t(max_col), max_row));
        return true;
    }

    auto is_digits = [](const std::string &s){ return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit); };
    auto is_alpha = [](const std::string &s){ return !s.empty() && std::all_of(s.begin(), s.end(), ::isalpha); };
    auto get_ref_type = [&](const std::string &s) -> char
    {
        if (s.empty())
        {
            return '?';
        }
        if (is_digits(s))
        {
            return 'R'; // row only (ex: "1", "3")
        }
        if (is_alpha(s))
        {
            return 'C'; // column only (ex: "A", "B")
        }
        size_t pos = 0;
        while (pos < s.size() && std::isalpha(static_cast<unsigned char>(s[pos])))
        {
            pos++;
        }
        if (pos > 0 && pos < s.size() && is_digits(s.substr(pos)))
        {
            return 'X'; // cell reference
        }
        return '?';  // invalid
    };

    xlnt::column_t::index_t first_col = 1;
    xlnt::row_t first_row = 1;
    xlnt::column_t::index_t last_col = max_col;
    xlnt::row_t last_row = max_row;

    size_t colon = range_str.find(':');

    if (colon == std::string::npos)
    {
        std::string token = range_str;
        char type = get_ref_type(token);

        if (type == 'R')  // row only: "1" -> 1:max_row
        {
            try
            {
                first_row = static_cast<xlnt::row_t>(std::stoul(token));
                if (first_row > max_row || first_row < 1)
                {
                    return false;
                }
                last_row = max_row;
                first_col = 1;
                last_col = max_col;
            }
            catch (...)
            {
                return false;
            }
        }
        else if (type == 'C')  // column only: "A" -> A:max_col
        {
            try
            {
                xlnt::column_t c(token);
                first_col = c.index;
                last_col = max_col;
                first_row = 1;
                last_row = max_row;
            }
            catch (...)
            {
                return false;
            }
        }
        else if (type == 'X')  // cell: "A1" -> A1:max
        {
            size_t pos = 0;
            while (pos < token.size() && std::isalpha(static_cast<unsigned char>(token[pos])))
            {
                pos++;
            }
            std::string colpart = token.substr(0, pos);
            std::string rowpart = token.substr(pos);
            try
            {
                xlnt::column_t c(colpart);
                first_col = c.index;
                last_col = max_col;
                first_row = static_cast<xlnt::row_t>(std::stoul(rowpart));
                if (first_row > max_row || first_row < 1)
                {
                    return false;
                }
                last_row = max_row;
            }
            catch (...)
            {
                return false;
            }
        }
        else
        {
            return false; // invalid format
        }
    }
    else
    {
        if (colon == 0 || colon == range_str.size() - 1)
        {
            return false;
        }

        std::string start = range_str.substr(0, colon);
        std::string end = range_str.substr(colon + 1);

        char start_type = get_ref_type(start);
        char end_type = get_ref_type(end);

        if (start_type != end_type || start_type == '?')
        {
            return false;
        }

        xlnt::column_t::index_t s_col = 1, e_col = max_col;
        xlnt::row_t s_row = 1, e_row = max_row;

        if (start_type == 'R')  // row range: "1:3"
        {
            try
            {
                s_row = static_cast<xlnt::row_t>(std::stoul(start));
                e_row = static_cast<xlnt::row_t>(std::stoul(end));
                if (s_row < 1 || e_row < 1 || s_row > max_row || e_row > max_row)
                {
                    return false;
                }
                s_col = 1;
                e_col = max_col;
            }
            catch (...)
            {
                return false;
            }
        }
        else if (start_type == 'C')  // column range: "A:B"
        {
            try
            {
                xlnt::column_t c_start(start);
                xlnt::column_t c_end(end);
                s_col = c_start.index;
                e_col = c_end.index;
                s_row = 1;
                e_row = max_row;
            }
            catch (...)
            {
                return false;
            }
        }
        else if (start_type == 'X')  // cell range: "A1:B2"
        {
            try
            {
                size_t pos = 0;
                while (pos < start.size() && std::isalpha(static_cast<unsigned char>(start[pos])))
                {
                    pos++;
                }
                xlnt::column_t c_start(start.substr(0, pos));
                s_col = c_start.index;
                s_row = static_cast<xlnt::row_t>(std::stoul(start.substr(pos)));

                pos = 0;
                while (pos < end.size() && std::isalpha(static_cast<unsigned char>(end[pos])))
                {
                    pos++;
                }
                xlnt::column_t c_end(end.substr(0, pos));
                e_col = c_end.index;
                e_row = static_cast<xlnt::row_t>(std::stoul(end.substr(pos)));
                
                if (s_row < 1 || e_row < 1 || s_row > max_row || e_row > max_row)
                {
                    return false;
                }
            }
            catch (...)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        if (s_col > e_col || s_row > e_row)
        {
            return false;
        }
        
        first_col = s_col;
        last_col = e_col;
        first_row = s_row;
        last_row = e_row;
    }

    if (first_col < 1 || first_row < 1)
    {
        return false;
    }
    out_ref = xlnt::range_reference(xlnt::cell_reference(first_col, first_row),
                                    xlnt::cell_reference(last_col, last_row));

    return true;
}

static xlnt::worksheet get_worksheet(xlnt::workbook& wb, const std::string& sheet_name, int sheet_index, bool is_new_file = false) {
    if (sheet_index >= 0)
    {
        auto sheet_titles = wb.sheet_titles();
        if (sheet_index < static_cast<int>(sheet_titles.size()))
        {
            return wb.sheet_by_title(sheet_titles[sheet_index]);
        }
        else
        {
            throw std::runtime_error("sheet_index_not_found");
        }
    }

    if (!sheet_name.empty())
    {
        for (const auto& ws : wb)
        {
            if (ws.title() == sheet_name)
            {
                return ws;
            }
        }
        if (is_new_file)
        {
            if (wb.sheet_count() > 0)
            {
                xlnt::worksheet default_sheet = wb.sheet_by_index(0);
                if (default_sheet.title() == "Sheet1" || default_sheet.title() == "Sheet")
                {
                    wb.remove_sheet(default_sheet);
                }
            }
            xlnt::worksheet new_ws = wb.create_sheet();
            new_ws.title(sheet_name);
            return new_ws;
        }
        else
        {
            throw std::runtime_error("sheet_name_not_found:" + sheet_name);
        }
    }

    if (wb.sheet_count() > 0)
    {
        return wb.active_sheet();
    }
    else
    {
        return wb.create_sheet();
    }
}


static void apply_metadata_to_new_workbook(xlnt::workbook& wb, const XlsxMetadata& metadata)
{
    wb.core_property(xlnt::core_property::creator, metadata.creator);
    wb.core_property(xlnt::core_property::last_modified_by, metadata.last_modified_by);
    wb.core_property(xlnt::core_property::title, metadata.title);
    wb.core_property(xlnt::core_property::subject, metadata.subject);
    wb.core_property(xlnt::core_property::description, metadata.description);
    wb.core_property(xlnt::core_property::created, metadata.created_date);
    wb.core_property(xlnt::core_property::modified, metadata.modified_date);
}

static void apply_metadata_to_existing_workbook(xlnt::workbook& wb, const XlsxMetadata& metadata)
{
    wb.core_property(xlnt::core_property::last_modified_by, metadata.last_modified_by);
    wb.core_property(xlnt::core_property::modified, metadata.modified_date);
    if (!metadata.title.empty())
        wb.core_property(xlnt::core_property::title, metadata.title);
    if (!metadata.subject.empty())
        wb.core_property(xlnt::core_property::subject, metadata.subject);
    if (!metadata.description.empty())
        wb.core_property(xlnt::core_property::description, metadata.description);
}

static void set_xlnt_cell_value(xlnt::cell& cell, const XlsxCell& xlsx_cell)
{
    switch (xlsx_cell.type)
    {
        case XLSX_TYPE_DOUBLE: {
            double value = xlsx_cell.dblValue;
            cell.value(value);
            break;
        }
        case XLSX_TYPE_INT64: {
            int64_t value = std::strtoll(xlsx_cell.value.c_str(), nullptr, 10);
            cell.value(static_cast<double>(value));
            break;
        }
        case XLSX_TYPE_INT32: {
            int32_t value = std::strtol(xlsx_cell.value.c_str(), nullptr, 10);
            cell.value(static_cast<double>(value));
            break;
        }
        case XLSX_TYPE_BOOL: {
            bool value = (xlsx_cell.value == "true" || xlsx_cell.value == "1");
            cell.value(value);
            break;
        }
        case XLSX_TYPE_TIMESTAMP: {
            double scilab_serial = xlsx_cell.dblValue;
            // convert scilab to Excel 0000-01-01 -> 1900-01-01
            // 693960 days (from year 0 to 1900)
            if (scilab_serial == -1)
            {
                cell.value("");
                break;
            }
            cell.value(scilab_serial - 693960.0);
            cell.number_format(xlnt::number_format::date_datetime());
            break;
        }
        case XLSX_TYPE_DURATION: {
            double value = xlsx_cell.dblValue;
            // xlnt::time t = xlnt::time::from_number(xlsx_cell.dblValue);
            // cell.value(t);

            cell.value(std::floor(value) / MS_BY_DAY);
            cell.number_format(xlnt::number_format::date_time6());
            break;
        }
        case XLSX_TYPE_STRING:
        default: {
            cell.value(xlsx_cell.value);
            break;
        }
    }
}


int xlsx_write(const char* filename, const XlsxData* data, const XlsxMetadata* metadata, const XlsxWriteOptions* options)
{
    if (!filename || !data || !data->is_loaded || data->sheet_count == 0)
        return -1;
    xlnt::range_reference write_range;
    bool has_range = false;
    if (options && !options->range.empty())
    {
        // max excel dimensions (1048576 rows x 16384 columns = XFD) 2 ^ 14 = 16384 / 2 ^ 20 = 1048576
        if (!make_write_range_from_string(options->range, write_range, 1048576, 16384))
            return -5;
        has_range = true;
    }

    XlsxMetadata default_metadata;
    const XlsxMetadata& final_metadata = metadata ? *metadata : default_metadata;

    try
    {
        xlnt::workbook wb;
        bool file_exists = false;
        // An empty file holds nothing to lose, so treat it as a new one.
        std::ifstream test_file(filename, std::ios::binary | std::ios::ate);
        if (test_file.good() && test_file.tellg() > 0)
        {
            test_file.close();
            try
            {
                wb.load(filename);
                file_exists = true;
            }
            catch (...)
            {
                // Starting from a blank workbook here would overwrite whatever
                // the file still holds, so refuse rather than destroy it.
                return -8;
            }
        }

        if (file_exists)
            apply_metadata_to_existing_workbook(wb, final_metadata);
        else
            apply_metadata_to_new_workbook(wb, final_metadata);

        if (options && (!options->sheet_name.empty() || options->sheet_index >= 0))
        {
            try
            {
                xlnt::worksheet target_ws = get_worksheet(wb, options->sheet_name, options->sheet_index, !file_exists);
                if (options->clear_sheet)
                {
                    xlnt::range_reference clear_range(xlnt::cell_reference(1, 1), xlnt::cell_reference(target_ws.highest_column(), target_ws.highest_row()));
                    target_ws.range(clear_range).clear_cells();
                }

                int start_row = has_range ? static_cast<int>(write_range.top_left().row()) : 1;
                int start_col = has_range ? static_cast<int>(write_range.top_left().column_index()) : 1;
                int end_row = has_range ? static_cast<int>(write_range.bottom_right().row()) : -1;
                int end_col = has_range ? static_cast<int>(write_range.bottom_right().column_index()) : -1;

                if (options->append_mode)
                {
                    if (target_ws.calculate_dimension() == xlnt::range_reference("A1:A1") && !target_ws.cell("A1").has_value())
                        start_row = 1;  // empty sheet
                    else
                        start_row = static_cast<int>(target_ws.highest_row()) + 1;  // append after last row
                    start_col = 1;
                    has_range = false;
                }

                int row_offset = 0;
                for (int sheet_idx = 0; sheet_idx < data->sheet_count; sheet_idx++)
                {
                    const XlsxSheet& sheet = data->sheets[sheet_idx];
                    if (sheet.max_row > 0 && sheet.max_col > 0 && !sheet.cells.empty())
                    {
                        for (int row = 0; row < sheet.max_row; row++) {
                            if (row < static_cast<int>(sheet.cells.size())) {
                                for (int col = 0; col < sheet.max_col; col++) {
                                    if (col < static_cast<int>(sheet.cells[row].size())) {
                                        const XlsxCell& xlsx_cell = sheet.cells[row][col];
                                        if ((!xlsx_cell.value.empty() && xlsx_cell.type == XLSX_TYPE_STRING) || xlsx_cell.type != XLSX_TYPE_STRING)
                                        {
                                            int target_row = start_row + row + row_offset;
                                            int target_col = start_col + col;
                                            if (has_range && end_row < 1048576 && target_row > end_row)
                                                continue;
                                            if (has_range && end_col < 16384 && target_col > end_col)
                                                continue;
                                            xlnt::cell cell = target_ws.cell(target_col, target_row);
                                            set_xlnt_cell_value(cell, xlsx_cell);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    row_offset += sheet.max_row;
                }
            }
            catch (const std::runtime_error& e)
            {
                std::string error_msg = e.what();
                if (error_msg == "sheet_index_not_found")
                    return -6;  // sheet index not found
                else if (error_msg.find("sheet_name_not_found:") == 0)
                    return -7;  // sheet name not found
                else
                    return -6;  // other sheet access error
            }
            catch (const std::exception& e)
            {
                return -6;
            }
        }
        else
        {
            if (!file_exists)
            {
                while (wb.sheet_count() > 0)
                    wb.remove_sheet(wb.sheet_by_index(0));
            }

            for (int sheet_idx = 0; sheet_idx < data->sheet_count; sheet_idx++)
            {
                const XlsxSheet& sheet = data->sheets[sheet_idx];
                xlnt::worksheet ws;
                ws = wb.create_sheet();

                if (!sheet.name.empty())
                {
                    try
                    {
                        ws.title(sheet.name);
                    }
                    catch (const xlnt::exception& e)
                    {
                        ws.title("Sheet" + std::to_string(sheet_idx + 1));
                    }
                }
                else
                    ws.title("Sheet" + std::to_string(sheet_idx + 1));

                int start_row = has_range ? static_cast<int>(write_range.top_left().row()) : 1;
                int start_col = has_range ? static_cast<int>(write_range.top_left().column_index()) : 1;
                int end_row = has_range ? static_cast<int>(write_range.bottom_right().row()) : -1;
                int end_col = has_range ? static_cast<int>(write_range.bottom_right().column_index()) : -1;

                if (sheet.max_row > 0 && sheet.max_col > 0 && !sheet.cells.empty())
                {
                    for (int row = 0; row < sheet.max_row; row++) {
                        if (row < static_cast<int>(sheet.cells.size())) {
                            for (int col = 0; col < sheet.max_col; col++) {
                                if (col < static_cast<int>(sheet.cells[row].size())) {    
                                    const XlsxCell& xlsx_cell = sheet.cells[row][col];
                                    if (!xlsx_cell.value.empty())
                                    {
                                        int target_row = start_row + row;
                                        int target_col = start_col + col;
                                        if (has_range && end_row < 1048576 && target_row > end_row)
                                            continue;
                                        if (has_range && end_col < 16384 && target_col > end_col)
                                            continue;
                                        
                                        xlnt::cell cell = ws.cell(target_col, target_row);
                                        set_xlnt_cell_value(cell, xlsx_cell);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        wb.save(filename);
        return 0; // success
    }
    catch (const xlnt::exception& e)
    {
        return -2; // XLNT error 
    }
    catch (const std::exception& e)
    {
        return -3; // general error
    }
    catch (...)
    {
        return -4; // unknown error
    }
}
