// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// For more information, see the COPYING file which you should have received
// along with this program.

#ifndef XLSX_STREAMING_HXX
#define XLSX_STREAMING_HXX

#include <xlnt/xlnt.hpp>
#include <xlnt/workbook/streaming_workbook_reader.hpp>
#include <string>
#include <vector>

// Fallback for workbooks whose relationships reference parts is absent from the
// archive. workbook::load() opens every referenced part and aborts on the
// missing one, where the streaming reader only reads the workbook and its
// sheets. See https://github.com/xlnt-community/xlnt/issues/165

inline bool xlsx_streaming_titles(const char* filename, std::vector<std::string>& titles)
{
    try
    {
        xlnt::streaming_workbook_reader reader;
        reader.open(std::string(filename));
        titles = reader.sheet_titles();
        return !titles.empty();
    }
    catch (...)
    {
        return false;
    }
}

// Calls on_cell(index, cell) for every cell of each sheet accepted by want(title, index).
template <typename WantFn, typename CellFn>
inline bool xlsx_streaming_scan(const char* filename, std::vector<std::string>& titles, WantFn&& want, CellFn&& on_cell)
{
    try
    {
        xlnt::streaming_workbook_reader reader;
        reader.open(std::string(filename));
        titles = reader.sheet_titles();
        if (titles.empty())
        {
            return false;
        }

        for (int i = 0; i < static_cast<int>(titles.size()); i++)
        {
            if (!want(titles[i], i))
            {
                continue;
            }

            reader.begin_worksheet(titles[i]);
            while (reader.has_cell())
            {
                on_cell(i, reader.read_cell());
            }
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

#endif /* XLSX_STREAMING_HXX */
