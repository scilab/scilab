// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================
// WAVE Audio File Format
// http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
// http://www.digitalpreservation.gov/formats/fdd/fdd000001.shtml
// =============================================================================
function savewave(filename,x,rate,nbits)

    // create_wavheader, write_ckinfo, write_wavedat functions only defined in savewave.sci

    function [x,fmt] = create_wavheader(x,rate,nbits)

        [channels, samples] = size(x);
        if (samples == 1) then
            x = x';
            [channels, samples] = size(x);
        end

        // Clip data to normalized range [-1,+1]:
        i = matrix(find(abs(x)>1),1,-1);
        if ~(i == []) then
            // Data clipped during write to file.
            x(i) = sign(x(i));
        end

        // # bytes per sample to write
        bytes_per_sample = ceil(nbits/8);
        total_samples = samples * channels;
        total_bytes = total_samples * bytes_per_sample;

        riff_cksize = 36 + total_bytes;
        // Don't include 'RIFF' or its size field
        fmt_cksize = 16;
        // Don't include 'fmt' or its size field
        data_cksize = total_bytes;
        // Don't include 'data' or its size field

        // Determine pad bytes:
        data_pad = data_cksize - fix(data_cksize./2).*2;
        riff_cksize = riff_cksize + data_pad;
        // + fmt_pad, always 0

        ck= tlist(["ck","fid","Size","ID"]) ;
        // Write RIFF chunk:
        ck("fid") = fid;
        ck("Size") = riff_cksize;
        ck("ID") = "RIFF";
        write_ckinfo(ck);

        // Write WAVE:
        ck("ID") = "WAVE";
        write_ckinfo(ck,1);

        // Write <fmt-ck>:
        ck("ID") = "fmt ";
        ck("Size") = fmt_cksize;
        write_ckinfo(ck);
        // Write <wave-format>:
        fmt = tlist(["fmt","wFormatTag","nChannels","nSamplesPerSec","nAvgBytesPerSec","nBlockAlign","nBitsPerSample"]);
        fmt("wFormatTag") = 1;
        // Data encoding format = PCM
        fmt("nChannels") = channels;
        // Number of channels
        fmt("nSamplesPerSec") = rate;
        // Samples per second
        fmt("nAvgBytesPerSec") = channels * bytes_per_sample * rate;
        // Avg transfer rate
        fmt("nBlockAlign") = channels * bytes_per_sample;
        // Block alignment
        fmt("nBitsPerSample") = nbits;

        // standard <PCM-format-specific> info
        status = write_wavefmt(fid,fmt);

        // Write <data-ck>:
        ck("ID") = "data";
        ck("Size") = data_cksize;
        write_ckinfo(ck);

    endfunction
    // =============================================================================
    function write_ckinfo(ck,sflg)
        [nargout,nargin] = argn(0)
        // WRITE_CKINFO: Writes next RIFF chunk, but not the chunk data.
        //   If optional sflg is set to nonzero, write SUBchunk info instead.
        //   Expects an open FID pointing to first byte of chunk header,
        //   and a chunk structure.
        //   ck.fid, ck.ID, ck.Size, ck.Data
        if length(ck("ID"))<>4 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d.\n"),"write_ckinfo",1));
        end

        mput(ascii(ck("ID")),"c",ck("fid"));

        // Error condition
        if (nargin == 1) then
            // Write chunk size (skip if subchunk):
            mput(ck("Size"),"ui",ck("fid"));
        end
    endfunction
    // =============================================================================
    function [status] = write_wavedat(fid,fmt,data)
        status = [];
        // WRITE_WAVEDAT: Write WAVE data chunk
        //   Assumes fid points to the wave-data chunk
        //   Requires <wave-format> structure to be passed.

        status = 0;

        if fmt("wFormatTag") == 1 then
            // PCM Format:
            // Determine # bytes/sample - format requires rounding
            //  to next integer number of bytes:
            BytesPerSample = ceil(fmt("nBitsPerSample")/8);

            select BytesPerSample
            case 1 then
                dtype = "uc"; // unsigned 8-bit
                // Scale data according to bits/samples: [-1,+1] -> [0,255]
                data = round((data+1)*255/2);
            case 2 then
                dtype = "s";
                // signed 16-bit
                // Scale data according to bits/samples: [-1,+1] -> [-32768,+32767]
                data = round((data+1)*65535/2)-32768;
            case 3 then
                dtype="c"
                // signed 24-bit
                // Scale data according to bits/samples: [-1,+1] -> [-8 388 608,+8 388 607]
                data = round((data+1)*(2^24-1)/2)-(2^23);
            case 4 then
                dtype="i"
                // signed 32-bit
                // Scale data according to bits/samples: [-1,+1] -> [-2 147 483 648,+2 147 483 647]
                data = round((data+1)*(2^32-1)/2)-(2^31);

            else
                error(msprintf(gettext("%s: An error occurred: %s\n"),"savewave",gettext("only 8/16/24/32 bits for the encoding.")));
            end

            // Write data, one row at a time (one sample from each channel):
            [channels,samples] = size(data);
            total_samples = samples * channels;

            //24-bits needs special treatment
            if (BytesPerSample == 3) then
                oct3 = (floor((data)/(2^16)));//msb
                oct2 = (floor((data-(oct3*2^16))/(2^8)));
                oct1 = (floor(data-(oct3*2^16)-(oct2*2^8)));//lsb
                data_line = zeros(3*total_samples,1);
                select channels
                case 1
                    data_line(1:3:$) = oct1(1,:)';
                    data_line(2:3:$) = oct2(1,:)';
                    data_line(3:3:$) = oct3(1,:)';
                case 2
                    data_line(1:6:$) = oct1(1,:)';
                    data_line(2:6:$) = oct2(1,:)';
                    data_line(3:6:$) = oct3(1,:)';
                    data_line(4:6:$) = oct1(2,:)';
                    data_line(5:6:$) = oct2(2,:)';
                    data_line(6:6:$) = oct3(2,:)';
                end
                data_line = data_line';
            else
                data_line = data;
            end


            try
                mput(data_line,dtype,fid);
            catch
                status = -1;
                return
            end
            // Error condition
            // Determine if a pad-byte is appended to data chunk:
            %v2_1$1 = total_samples * BytesPerSample;
            if ( %v2_1$1 - fix(%v2_1$1./2).*2 ) then
                mput(0,"uc",fid);
            end
        else
            // Unknown wave-format for data.
            error(msprintf(gettext("%s: An error occurred: %s\n"),"write_wavedat",gettext("Unknown data format.")));
        end
        return
    endfunction
    // =============================================================================
    function [status]=write_wavefmt(fid,fmt)
        status = 0;
        // WRITE_WAVEFMT: Write WAVE format chunk.
        //   Assumes fid points to the wave-format subchunk.
        //   Requires chunk structure to be passed, indicating
        //   the length of the chunk.

        // Create <wave-format> data:

        mput(fmt("wFormatTag"),"us",fid);
        mput(fmt("nChannels"),"us",fid);
        mput(fmt("nSamplesPerSec"),"ui",fid);
        mput(fmt("nAvgBytesPerSec"),"ui",fid);
        mput(fmt("nBlockAlign"),"us",fid);

        // Write format-specific info:
        if ( fmt("wFormatTag") == 1 ) then
            // Write standard <PCM-format-specific> info:
            mput(fmt("nBitsPerSample"),"us",fid)
        else
            error("Unknown data format.");
        end
    endfunction
    // =============================================================================

    // savewave main
    lhs = argn(1);
    rhs = argn(2);

    if (rhs < 4) then
        nbits = 16;
    end;

    if (rhs < 3) then
        rate = 22050;
    end;

    if ~(type(filename) == 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n" ),"savewave",1));
    end

    if strindex(filename,".")==[] then
        filename = filename+".wav";
    end

    [fid,%v] = mopen(filename,"wb",1);

    if (%v < 0) then
        fid = -1;
    end

    if ( fid == (-1) ) then
        error(msprintf(gettext("%s: Cannot open file %s.\n"),"savewave",filename));
    end

    [x,fmt] = create_wavheader(x,rate,nbits);
    status = write_wavedat(fid,fmt,x);
    mclose(fid);

endfunction
// =============================================================================
