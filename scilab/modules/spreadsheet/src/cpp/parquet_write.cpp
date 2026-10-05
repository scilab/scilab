/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>
#include "arrow_lib.h"
#include "arrow_write_utils.h"

static bool get_compression(const char* comp_str, parquet::Compression::type& compression)
{
    if (!comp_str)
    {
        compression = parquet::Compression::GZIP;
        return true;
    }
    if (strcmp(comp_str, "snappy") == 0)
    {
        compression = parquet::Compression::SNAPPY;
        return true;
    }
    else if (strcmp(comp_str, "gzip") == 0)
    {
        compression = parquet::Compression::GZIP;
        return true;
    }
    else if (strcmp(comp_str, "zstd") == 0)
    {
        compression = parquet::Compression::ZSTD;
        return true;
    }
    else if (strcmp(comp_str, "uncompressed") == 0)
    {
        compression = parquet::Compression::UNCOMPRESSED;
        return true;
    }
    return false;
}

int parquet_write(const char* filename, ArrowData* raw_data, types::String* pCompression)
{
    if (!filename || !raw_data)
    {
        // Invalid arguments
        return -1;
    }

    auto file_result = arrow::io::FileOutputStream::Open(filename);
    if (!file_result.ok())
    {
        // Failed to open file
        return -2;
    }

    auto table_result = createArrowTable(raw_data);
    if (!table_result.ok())
    {
        // Failed to create table
        return -3;
    }

    auto outfile = file_result.ValueOrDie();
    auto table = table_result.ValueOrDie();

    parquet::WriterProperties::Builder props_builder;
    props_builder.version(parquet::ParquetVersion::PARQUET_2_6);

    if (pCompression != nullptr)
    {
        int comp_count = pCompression->getSize();
        if (comp_count == 1)
        {
            char* comp_str = wide_string_to_UTF8(pCompression->get(0));
            parquet::Compression::type compression;
            bool ok = get_compression(comp_str, compression);
            FREE(comp_str);
            if (!ok)
            {
                return -1;
            }
            props_builder.compression(compression);
        }
        else
        {
            int cols = raw_data->pColNames->getSize();
            if (comp_count != cols)
            {
                return -1;
            }
            for (int i = 0; i < cols; ++i)
            {
                char* comp_str = wide_string_to_UTF8(pCompression->get(i));
                parquet::Compression::type compression;
                bool ok = get_compression(comp_str, compression);
                FREE(comp_str);
                if (!ok)
                {
                    return -1;
                }
                char* col_name = wide_string_to_UTF8(raw_data->pColNames->get(i));
                props_builder.compression(col_name, compression);
                FREE(col_name);
            }
        }
    }
    else
    {
        props_builder.compression(parquet::Compression::GZIP);
    }

    auto props = props_builder.build();

    parquet::ArrowWriterProperties::Builder parquet_props_builder;
    auto parquet_props = parquet_props_builder.store_schema()->build();

    auto writer_result = parquet::arrow::FileWriter::Open(
        *table->schema(),
        arrow::default_memory_pool(),
        outfile,
        props,
        parquet_props
    );

    if (!writer_result.ok())
    {
        // Failed to create file writer
        return -4;
    }

    auto writer = std::move(writer_result.ValueOrDie());
    if (!writer->WriteTable(*table).ok())
    {
        // Failed writing table
        return -5;
    }

    if (!writer->Close().ok())
    {
        // Failed to close writer
        return -6;
    }

    if (!outfile->Close().ok())
    {
        // Failed to close file
        return -7;
    }

    return 0;
}
