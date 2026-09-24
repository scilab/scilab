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
#include <climits> // INT_MAX
#include <map>
#include "xlsx_lib.hxx"
#include "xlsx_streaming.hxx"


static bool make_range_from_string(xlnt::column_t::index_t highest_col, xlnt::row_t highest_row, const std::string &range_str, xlnt::range_reference &out_ref, bool &out_of_bounds)
{
    out_of_bounds = false;
    if (range_str.empty())
    {
        return false;
    }

    auto is_digits = [](const std::string &s){ return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit); };
    auto is_alpha = [](const std::string &s){ return !s.empty() && std::all_of(s.begin(), s.end(), ::isalpha); };
    auto get_ref_type = [&](const std::string &s) -> char {
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
    xlnt::column_t::index_t last_col = highest_col;
    xlnt::row_t last_row = highest_row;

    size_t colon = range_str.find(':');
    if (colon == std::string::npos)
    {
        std::string token = range_str;
        char type = get_ref_type(token);

        if (type == 'R')  // row only: "1"
        {
            first_row = static_cast<xlnt::row_t>(std::stoul(token));
            last_row = highest_row;
            first_col = 1;
            last_col = highest_col;
        }
        else if (type == 'C')  // column only: "A"
        {
            try
            {
                xlnt::column_t c(token);
                first_col = c.index;
                last_col = highest_col;
                first_row = 1;
                last_row = highest_row;
            }
            catch (...)
            {
                out_of_bounds = true;
                return true;
            }
        }
        else if (type == 'X')  // cell: "A1"
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
                last_col = highest_col;
                first_row = static_cast<xlnt::row_t>(std::stoul(rowpart));
                last_row = highest_row;
            }
            catch (...)
            {
                out_of_bounds = true;
                return true;
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
            return false; // invalide: ":A" ou "A1:"
        }
        
        std::string start = range_str.substr(0, colon);
        std::string end = range_str.substr(colon + 1);

        char start_type = get_ref_type(start);
        char end_type = get_ref_type(end);

        if (start_type != end_type || start_type == '?')
        {
            return false; // invalide: "A:1", "1:A", "A1:B", "A:B2", etc...
        }

        xlnt::column_t::index_t s_col = 1, e_col = highest_col;
        xlnt::row_t s_row = 1, e_row = highest_row;

        if (start_type == 'R')  // row range: "1:3"
        {
            try
            {
                s_row = static_cast<xlnt::row_t>(std::stoul(start));
                e_row = static_cast<xlnt::row_t>(std::stoul(end));
                s_col = 1;
                e_col = highest_col;
            }
            catch (...)
            {
                out_of_bounds = true;
                return true;
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
                e_row = highest_row;
            }
            catch (...)
            {
                out_of_bounds = true;
                return true;
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
            }
            catch (...)
            {
                out_of_bounds = true;
                return true;
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

        // ancienement normaliser 
        first_col = s_col;
        last_col = e_col;
        first_row = s_row;
        last_row = e_row;    
    }

    if (first_col < 1 || first_row < 1) 
    {
        return false;
    }

    xlnt::column_t::index_t actual_highest_col = highest_col;
    xlnt::row_t actual_highest_row = highest_row;

    if (first_col > actual_highest_col || first_row > actual_highest_row)
    {
        out_of_bounds = true;
        return true;
    }

    if (last_col < 1) 
    {
        last_col = 1;
    }
    if (last_col > actual_highest_col) 
    {
        last_col = actual_highest_col;
    }
    if (first_row > actual_highest_row)
    {
        first_row = actual_highest_row;
    }
    if (last_row > actual_highest_row)
    {
        last_row = actual_highest_row;
    }

    out_ref = xlnt::range_reference(xlnt::cell_reference(xlnt::column_t(first_col), first_row),
                                    xlnt::cell_reference(xlnt::column_t(last_col), last_row));

    return true;
}


XlsxDataType_t detect_cell_type(const xlnt::cell& cell)
{
    if (cell.data_type() == xlnt::cell::type::empty)
        return XLSX_TYPE_STRING;

    if (cell.data_type() == xlnt::cell::type::boolean)
        return XLSX_TYPE_BOOL;

    // verifier is_date() avant de verifier le type number
    // car les dates sont stockees comme des nombres
    if (cell.is_date())
    {
        if (cell.number_format().format_string() == "h:mm:ss")
        {
            return XLSX_TYPE_DURATION;
        }
        return XLSX_TYPE_TIMESTAMP;
    }        

    if (cell.data_type() == xlnt::cell::type::number)
    {
        return XLSX_TYPE_DOUBLE;
    }
    return XLSX_TYPE_STRING;
}

static void get_cell_value(const xlnt::cell& cell, XlsxCell& cell_data)
{
    if (cell.is_date())
    {
        double value = cell.value<double>();
        cell_data.dblValue = value;  
    }
    else
    {
        switch (cell.data_type())
        {
            case xlnt::cell::type::empty: {
                cell_data.value = "";
                break;
            }
            case xlnt::cell::type::boolean: {
                cell_data.value = cell.value<bool>() ? "true" : "false";
                break;
            }
            case xlnt::cell::type::number: {
                double value = cell.value<double>();
                cell_data.dblValue = value;
                break;
            }
            default: {
                cell_data.value = cell.to_string();
                break;
            }
        }
    }
}

static bool should_process_sheet(const std::string& sheet_name, int sheet_idx, const XlsxWriteOptions* options)
{
    if (!options)
        return true;

    if (!options->sheet_name.empty())
        return sheet_name == options->sheet_name;

    if (options->sheet_index >= 0)
        return sheet_idx == options->sheet_index;

    return true;
}

// Streaming counterpart of xlsx_read, for archives workbook::load() rejects.
static int xlsx_read_streaming(const char* filename, XlsxData* data, const XlsxWriteOptions* options)
{
    data->sheets.clear();
    data->sheet_count = 0;
    data->is_loaded = false;

    struct SheetCells
    {
        std::vector<XlsxCell> cells;
        xlnt::column_t::index_t highest_col = 1;
        xlnt::row_t highest_row = 1;
    };

    std::vector<std::string> titles;
    std::map<int, SheetCells> collected;
    std::vector<int> order;

    auto want = [&](const std::string& title, int index)
    {
        if (!should_process_sheet(title, index, options))
        {
            return false;
        }

        collected[index];
        order.push_back(index);
        return true;
    };

    auto on_cell = [&](int index, xlnt::cell cell)
    {
        SheetCells& sheet = collected[index];

        // The extents drive the range resolution, so empty cells count too.
        if (cell.row() > sheet.highest_row)
        {
            sheet.highest_row = cell.row();
        }
        if (cell.column().index > sheet.highest_col)
        {
            sheet.highest_col = cell.column().index;
        }

        if (!cell.has_value())
        {
            return;
        }

        XlsxCell cell_data;
        cell_data.row = static_cast<int>(cell.row());
        cell_data.col = static_cast<int>(cell.column().index);
        cell_data.dblValue = 0.0;
        cell_data.type = detect_cell_type(cell);
        get_cell_value(cell, cell_data);
        sheet.cells.push_back(cell_data);
    };

    if (!xlsx_streaming_scan(filename, titles, want, on_cell))
    {
        return -2;
    }

    for (int index : order)
    {
        SheetCells& sheet = collected[index];
        XlsxSheet sheet_data;
        sheet_data.name = titles[index];
        sheet_data.max_row = 0;
        sheet_data.max_col = 0;

        std::vector<XlsxCell>* source = &sheet.cells;
        std::vector<XlsxCell> in_range;

        if (options && !options->range.empty())
        {
            xlnt::range_reference rr;
            bool out_of_bounds = false;
            if (!make_range_from_string(sheet.highest_col, sheet.highest_row, options->range, rr, out_of_bounds))
            {
                return -5;
            }

            if (out_of_bounds)
            {
                data->sheets.push_back(sheet_data);
                continue;
            }

            const int first_row = static_cast<int>(rr.top_left().row());
            const int first_col = static_cast<int>(rr.top_left().column_index());
            const int last_row = static_cast<int>(rr.bottom_right().row());
            const int last_col = static_cast<int>(rr.bottom_right().column_index());

            for (const auto& cell_data : sheet.cells)
            {
                if (cell_data.row >= first_row && cell_data.row <= last_row &&
                    cell_data.col >= first_col && cell_data.col <= last_col)
                {
                    in_range.push_back(cell_data);
                }
            }
            source = &in_range;
        }

        if (source->empty())
        {
            data->sheets.push_back(sheet_data);
            continue;
        }

        int min_row = INT_MAX, max_row = 0;
        int min_col = INT_MAX, max_col = 0;
        for (const auto& cell_data : *source)
        {
            if (cell_data.row < min_row) min_row = cell_data.row;
            if (cell_data.row > max_row) max_row = cell_data.row;
            if (cell_data.col < min_col) min_col = cell_data.col;
            if (cell_data.col > max_col) max_col = cell_data.col;
        }

        sheet_data.max_row = max_row - min_row + 1;
        sheet_data.max_col = max_col - min_col + 1;

        sheet_data.cells.resize(sheet_data.max_row);
        for (int i = 0; i < sheet_data.max_row; i++)
        {
            sheet_data.cells[i].resize(sheet_data.max_col);
            for (int j = 0; j < sheet_data.max_col; j++)
            {
                sheet_data.cells[i][j].row = min_row + i;
                sheet_data.cells[i][j].col = min_col + j;
                sheet_data.cells[i][j].dblValue = 0.0;
                sheet_data.cells[i][j].value = "";
                sheet_data.cells[i][j].type = XLSX_TYPE_STRING;
            }
        }

        for (const auto& cell_data : *source)
        {
            sheet_data.cells[cell_data.row - min_row][cell_data.col - min_col] = cell_data;
        }

        data->sheets.push_back(sheet_data);
    }

    data->sheet_count = static_cast<int>(data->sheets.size());
    data->is_loaded = true;
    return 0;
}


int xlsx_read(const char* filename, XlsxData* data, const XlsxWriteOptions* options)
{
    if (!filename || !data)
        return -1;

    data->filename = filename;
    data->sheets.clear();
    data->sheet_count = 0;
    data->is_loaded = false;
    
    try
    {
        xlnt::workbook wb;    
        wb.load(filename);
        auto sheet_names = wb.sheet_titles();

        std::vector<std::pair<std::string, int>> sheets_to_process;
        for (int i = 0; i < static_cast<int>(sheet_names.size()); i++)
        {
            if (should_process_sheet(sheet_names[i], i, options))
                sheets_to_process.push_back({sheet_names[i], i});
        }

        data->sheet_count = static_cast<int>(sheets_to_process.size());
        data->sheets.reserve(data->sheet_count);

        for (const auto& sheet_info : sheets_to_process)
        {
            const std::string& sheet_name = sheet_info.first;
            XlsxSheet sheet_data;
            sheet_data.name = sheet_name;
            sheet_data.max_row = 0;
            sheet_data.max_col = 0;

            auto ws = wb.sheet_by_title(sheet_name);
            bool has_data = false;
            int min_row = INT_MAX, max_row = 0;
            int min_col = INT_MAX, max_col = 0;
            bool use_xlnt_range = false;
            std::unique_ptr<xlnt::range> xlnt_range_ptr;

            if (options && !options->range.empty())
            {
                xlnt::range_reference rr;
                bool out_of_bounds = false;
                if (!make_range_from_string(ws.highest_column().index, ws.highest_row(), options->range, rr, out_of_bounds))
                {
                    return -5;
                }

                if (out_of_bounds)
                {
                    data->sheets.push_back(sheet_data);
                    continue;
                }

                try
                {
                    auto tmp = ws.range(rr);
                    xlnt_range_ptr = std::make_unique<xlnt::range>(std::move(tmp));
                    use_xlnt_range = true;
                }
                catch (const std::exception&)
                {
                    return -5;
                }
            }

            if (use_xlnt_range)
            {
                auto rr = xlnt_range_ptr->reference();
                min_row = static_cast<int>(rr.top_left().row());
                min_col = static_cast<int>(rr.top_left().column_index());
                max_row = static_cast<int>(rr.bottom_right().row());
                max_col = static_cast<int>(rr.bottom_right().column_index());

                int actual_min_row = INT_MAX;
                int actual_min_col = INT_MAX;
                int actual_max_row = 0;
                int actual_max_col = 0;

                for (auto row : *xlnt_range_ptr)
                {
                    for (auto cell : row)
                    {
                        if (cell.has_value())
                        {
                            has_data = true;
                            int current_row = static_cast<int>(cell.row());
                            int current_col = static_cast<int>(cell.column().index);
                            if (current_row < actual_min_row) actual_min_row = current_row;
                            if (current_row > actual_max_row) actual_max_row = current_row;
                            if (current_col < actual_min_col) actual_min_col = current_col;
                            if (current_col > actual_max_col) actual_max_col = current_col;
                        }
                    }
                }
                if (has_data)
                {
                    min_row = actual_min_row;
                    min_col = actual_min_col;
                    max_row = actual_max_row;
                    max_col = actual_max_col;
                }
            }
            else
            {
                for (auto row : ws.rows(false)) // false = skip empty rows
                {
                    for (auto cell : row)
                    {
                        if (cell.has_value())
                        {
                            int current_row = static_cast<int>(cell.row());
                            int current_col = static_cast<int>(cell.column().index);
                            has_data = true;
                            if (max_row == 0 || current_row > max_row) max_row = current_row;
                            if (max_col == 0 || current_col > max_col) max_col = current_col;
                            if (current_row < min_row) min_row = current_row;
                            if (current_col < min_col) min_col = current_col;
                        }
                    }
                }
            }

            if (!has_data)
            {
                data->sheets.push_back(sheet_data);
                continue;
            }

            sheet_data.max_row = max_row - min_row + 1;
            sheet_data.max_col = max_col - min_col + 1;

            sheet_data.cells.resize(sheet_data.max_row);
            for (int i = 0; i < sheet_data.max_row; i++)
            {
                sheet_data.cells[i].resize(sheet_data.max_col);
                for (int j = 0; j < sheet_data.max_col; j++)
                {
                    sheet_data.cells[i][j].row = min_row + i;
                    sheet_data.cells[i][j].col = min_col + j;
                    sheet_data.cells[i][j].value = "";
                    sheet_data.cells[i][j].type = XLSX_TYPE_STRING;
                }
            }

            if (use_xlnt_range && xlnt_range_ptr)
            {
                for (auto row : *xlnt_range_ptr)
                {
                    for (auto cell : row)
                    {
                        if (cell.has_value())
                        {
                            int current_row = static_cast<int>(cell.row());
                            int current_col = static_cast<int>(cell.column().index);
                            int array_row = current_row - min_row;
                            int array_col = current_col - min_col;

                            if (array_row >= 0 && array_row < sheet_data.max_row && array_col >= 0 && array_col < sheet_data.max_col)
                            {
                                XlsxCell& cell_data = sheet_data.cells[array_row][array_col];
                                cell_data.row = current_row;
                                cell_data.col = current_col;
                                cell_data.type = detect_cell_type(cell);
                                get_cell_value(cell, cell_data);
                            }
                        }
                    }
                }
            }
            else
            {
                for (auto row : ws.rows(false))
                {
                    for (auto cell : row)
                    {
                        if (cell.has_value())
                        {
                            int current_row = static_cast<int>(cell.row());
                            int current_col = static_cast<int>(cell.column().index);

                            if (current_row < min_row || current_row > max_row || current_col < min_col || current_col > max_col)
                                continue;

                            int array_row = current_row - min_row;
                            int array_col = current_col - min_col;

                            if (array_row >= 0 && array_row < sheet_data.max_row && array_col >= 0 && array_col < sheet_data.max_col)
                            {
                                XlsxCell& cell_data = sheet_data.cells[array_row][array_col];
                                cell_data.row = current_row;
                                cell_data.col = current_col;
                                cell_data.type = detect_cell_type(cell);
                                get_cell_value(cell, cell_data);
                            }
                        }
                    }
                }
            }
            data->sheets.push_back(sheet_data);
        }
        data->is_loaded = true;
        return 0;
    }
    catch (const xlnt::exception& e)
    {
        return xlsx_read_streaming(filename, data, options);
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

void xlsx_free_data(XlsxData* data)
{
    if (data)
    {
        for (auto& sheet : data->sheets)
        {
            sheet.cells.clear();
            sheet.name.clear();
        }
        data->sheets.clear();
        data->sheet_count = 0;
        data->is_loaded = false;
        data->filename.clear();
    }
}
