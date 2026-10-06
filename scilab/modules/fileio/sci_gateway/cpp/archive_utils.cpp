/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2023 - Dassault Systèmes S.E. - Cédric DELAMARRE
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
*
*/

#include "archive_utils.hxx"
#include "string.hxx"
#include "double.hxx"

int fillArchiveOpt(struct archive* ar, types::optional_list& opt, const char* fname, const wchar_t* filename)
{
    int res = 0;
    int level = -1;
    int levelMax = 9;
    bool bFormat = false;
    bool bCompression = false;
    bool bPassword = false;
    bool bWriteMode = filename != nullptr;

    for (const auto& o : opt)
    {
        if (o.first == L"format" && bWriteMode)
        {
            if(o.second->isString() == false || o.second->getAs<types::String>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: Scalar string expected.\n"), fname, "format");
                return 1;
            }

            wchar_t* pFormat = o.second->getAs<types::String>()->get(0);
            if(format.find(pFormat) == format.end())
            {
                Scierror(999, _("%s: Wrong value for input argument #%s: '%ls' not supported.\n"), fname, "format", pFormat);
                return 1;
            }

            if(bWriteMode)
            {
                res = archive_write_set_format(ar, format[pFormat]);
            }
            else
            {
                res = archive_read_set_format(ar, format[pFormat]);
            }

            switch(res)
            {
                case ARCHIVE_OK: break;
                case ARCHIVE_WARN: Sciwarning("%s: Warning: %s", fname, archive_error_string(ar)); break;
                default: Scierror(999, _("%s: %s\n"), fname, archive_error_string(ar)); return 1;
            }

            bFormat = true;
        }
        else if (o.first == L"compression" && bWriteMode)
        {
            if(o.second->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: String expected.\n"), fname, "compression");
                return 1;
            }

            types::String* pStrCompressions = o.second->getAs<types::String>();
            for(int i = 0; i < pStrCompressions->getSize(); ++i)
            {
                wchar_t* pCompression = pStrCompressions->get(i);
                if(compression.find(pCompression) == compression.end())
                {
                    Scierror(999, _("%s: Wrong value for input argument #%s: '%ls' not supported.\n"), fname, "compression", pCompression);
                    return 1;
                }

                if(bWriteMode)
                {
                    res = archive_write_add_filter(ar, compression[pCompression]);
                }
                else
                {
                    res = archive_read_append_filter(ar, compression[pCompression]);
                }

                switch(res)
                {
                    case ARCHIVE_OK: break;
                    case ARCHIVE_WARN: Sciwarning("%s: Warning: %s", fname, archive_error_string(ar)); break;
                    default: Scierror(999, _("%s: %s\n"), fname, archive_error_string(ar)); return 1;
                }

                if(compression[pCompression] == ARCHIVE_FILTER_ZSTD)
                {
                    levelMax = 22;
                }
            }

            bCompression = true;
        }
        else if(o.first == L"password")
        {
            if(o.second->isString() == false || o.second->getAs<types::String>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: Scalar string expected.\n"), fname, "password");
                return 1;
            }

            char* pPassword = wide_string_to_UTF8(o.second->getAs<types::String>()->get(0));
            if(bWriteMode)
            {
                res = archive_write_set_passphrase(ar, pPassword);
            }
            else
            {
                res = archive_read_add_passphrase(ar, pPassword);
            }

            FREE(pPassword);
            switch(res)
            {
                case ARCHIVE_OK: break;
                case ARCHIVE_WARN: Sciwarning("%s: Warning: %s", fname, archive_error_string(ar)); break;
                default: Scierror(999, _("%s: %s\n"), fname, archive_error_string(ar)); return 1;
            }

            bPassword = true;
        }
        else if(o.first == L"level" && bWriteMode)
        {
            if(o.second->isDouble() == false || o.second->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: Scalar expected.\n"), fname, "level");
                return 1;
            }

            level = (int)o.second->getAs<types::Double>()->get(0);
            if(level < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%s: A positive value expected.\n"), fname, "level");
                return 1;
            }
        }
        else
        {
            char* pcArgName = wide_string_to_UTF8(o.first.c_str());
            Scierror(999, _("%s: Wrong optional argument: '%s' not allowed.\n"), fname, pcArgName);
            FREE(pcArgName);
            return 1;
        }
    }

    /*** default values ***/
    if(bWriteMode)
    {
        if(!bFormat && !bCompression)
        {
            // Sets both filters and format based on the output filename.
            // Supported extensions: .7z, .zip, .jar, .cpio, .iso, .a, .ar,
            //                       .tar, .tgz, .tar.gz, .tar.bz2, .tar.xz
            // Note: libarchive does not manage the .zst extension, use the
            // format and compression options to create zstd archives.
            char* f = wide_string_to_UTF8(filename);
            int res = archive_write_set_format_filter_by_ext(ar, f);
            FREE(f);
            switch(res)
            {
                case ARCHIVE_OK: break;
                case ARCHIVE_WARN: Sciwarning("%s: Warning: %s", fname, archive_error_string(ar)); break;
                default: Scierror(999, _("%s: %s\n"), fname, archive_error_string(ar)); return 1;
            }
        }
        else if(!bFormat && bCompression)
        {
            Scierror(999, _("%s: '%s' argument missing.\n"), fname, "format");
            return 1;
        }

        if(level >= 0)
        {
            if(level > levelMax)
            {
                Scierror(999, _("%s: Wrong value for input argument #%s: A value between %d and %d expected.\n"), fname, "level", 0, levelMax);
                return 1;
            }

            std::string option = "compression-level=" + std::to_string(level);
            res = archive_write_set_options(ar, option.data());
            if (res != ARCHIVE_OK)
            {
                Scierror(999, _("%s: %s\n"), fname, archive_error_string(ar));
                return types::Function::Error;
            }
        }
    }
    else
    {
        archive_read_support_format_all(ar);
        archive_read_support_filter_all(ar);
    }

    if(bPassword)
    {
        // only ZIP format has encryption
        if(archive_format(ar) == ARCHIVE_FORMAT_ZIP)
        {
            // https://manpages.debian.org/unstable/libarchive-dev/archive_write_set_options.3.en.html
            res = archive_write_set_options(ar, "encryption");
            if (res != ARCHIVE_OK)
            {
                Scierror(999, _("%s: %s\n"), fname, archive_error_string(ar));
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, "%s: 'password' argument only expected with ZIP format.\n", fname);
        }
    }

    return 0;
}

bool check_error(int res, struct archive* ar, const char* fname)
{
    if(res == ARCHIVE_WARN)
    {
        Sciwarning("%s: Warning: %s", fname, archive_error_string(ar));
    } 
    else if(res != ARCHIVE_OK && res != ARCHIVE_EOF)
    {
        const char* err = archive_error_string(ar);
        if(err)
        {
            Scierror(999, _("%s: %s\n"), fname, err);
        }
        else
        {
            Scierror(999, _("%s: Cannot %s the archive.\n"), fname, fname);
        }
        return true;
    }

    return false;
}

void cleanup(struct archive* ar, struct archive* disk, struct archive_entry* entry)
{
    if(ar)
    {
        archive_write_close(ar);
        archive_write_free(ar);
    }

    if(disk)
    {
        archive_read_close(disk);
        archive_read_free(disk);
    }

    if(entry)
    {
        archive_entry_free(entry);
    }
    
}