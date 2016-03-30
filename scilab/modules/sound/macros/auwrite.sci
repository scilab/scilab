// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=auwrite(y,Fs,nbits,method,aufile)
    //Write .au sound file.
    //auwrite(y,aufile) writes a sound file specified by the
    //string aufile.  The data should be arranged with one channel
    //per column.  Amplitude values outside the range [-1,+1] are
    //clipped prior to writing.
    //
    //Supports multi-channel data for 8-bit mu-law, and 8- and
    //16-bit linear formats:
    //
    //auwrite(y,Fs,aufile) specifies the sample rate of the data
    //in Hertz.
    //
    //auwrite(y,Fs,bits,aufile) selects the number of bits in
    //the encoder.  Allowable settings are bits=8 and bits=16.
    //
    //auwrite(y,Fs,bits,method,aufile) allows selection of the
    //encoding method, which can be either 'mu' or 'linear'.
    //Note that mu-law files must be 8-bit. By default, method='mu'.

    // test :
    // auwrite(y,44100,8,'mu','poo.au')
    // aplay -c 1 -f MU_LAW --rate=22050 poo.au
    //

    // Get default:
    [nargout,nargin] = argn(0)
    Fs_pref = 22050;
    nbits_pref = 8;
    method_pref = "mu";

    if nargin==1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"auwrite",2,5));
    elseif nargin>5 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"auwrite",2,5));
    elseif nargin==4 then
        aufile = method;
        method = method_pref;
    elseif nargin==3 then
        aufile = nbits;
        method = method_pref;
        nbits = nbits_pref;
    elseif nargin==2 then
        aufile = Fs;
        method = method_pref;
        nbits = nbits_pref;
        Fs = Fs_pref;
    end

    if ~(type(aufile)==10) then
        error(msprintf(gettext("%s: Wrong values for input argument: Filename must be a string.\n"),"auwrite"));
    end
    if strindex(aufile,".")==[] then
        aufile = aufile+".au";
    end

    [fid,junk] = mopen(aufile,"wb",0) // Big-endian
    if junk<0 then
        error(msprintf(gettext("%s: Cannot open file %s.\n"),"auwrite",aufile));
    end

    if length(size(y)) > 2 then
        error(msprintf(gettext("%s: An error occurred: %s\n"),"auwrite",gettext("Data array must have 1- or 2-dimensions only.")));
    end
    if size(y,2)==1 then
        y = y';
    end

    // Clip data to normalized range [-1,+1]:
    i = matrix(find(abs(y)>1),1,-1);
    if ~(i==[]) then
        y(i) = sign(y(i));
        warning(gettext("Data clipped during write to file."));
    end

    snd = write_sndhdr(fid,Fs,nbits,method,size(y));

    if write_sndata(fid,snd,y) then
        error(msprintf(gettext("%s: An error occurred: %s\n"),"auwrite",gettext("Error while writing sound file.")));
    end
    mclose(fid);
endfunction

function [status]=write_sndata(fid,snd,data)
    status = 0;
    if snd("format")==1 then
        dtype = "uc";
        data = lin2mu(data);
    elseif snd("format")==2 then
        dtype = "c";// 'int8';
        data = round(data*(2^7-1));
    elseif snd("format")==3 then
        dtype = "sb";// 'int16"
        data = round(data*(2^15-1));
    elseif snd("format")==5 then
        dtype = "ib";// 'int32"
        data = round(data*(2^31-1));
    elseif snd("format")==6 then
        dtype = "fb";
        //
    elseif snd("format")==7 then
        dtype = "db";// double precision
        //
    else
        status = -1;
        return
    end
    total_samples = snd("samples")*snd("chans");
    mput(data,dtype,fid);
endfunction

function [snd]=write_sndhdr(fid,Fs,nbits,method,sz)
    // write header part
    if method=="mu" then
        if nbits~=8 then
            error(msprintf(gettext("%s: An error occurred: %s\n"),"auwrite",gettext("Mu-law can only be used with 8 bit data. Use method=''linear'' instead.")));
        end
        snd.format = 1;
        snd.bits = 8;
    elseif method=="linear" then
        if nbits==8 then
            snd.format=2  // 8-bit linear
            snd.bits=8
        elseif nbits==16 then
            snd.format=3  // 16-bit linear
            snd.bits=16
        elseif nbits==32 then
            snd.format=5 //  32-bit linear
            snd.bits=32;
        elseif nbits==64 then
            snd.format=7;  // Double-precision
            snd.bits=64;
        else
            error(msprintf(gettext("%s: An error occurred: %s\n"),"auwrite",gettext("Unrecognized data format.")));
            return
        end
    else
        error(msprintf(gettext("%s: An error occurred: %s\n"),"auwrite",gettext("Unrecognized data format.")));
    end

    // Define sound header structure:
    snd("samples")=sz(2)
    snd("chans")=sz(1)
    total_samples = snd("samples")*snd("chans");
    bytes_per_sample = ceil(snd("bits")/8);
    snd("rate")=Fs
    snd("databytes")=bytes_per_sample*total_samples
    snd("offset")=28
    snd("info")="SCI0";

    mput(ascii(".snd"),"c",fid); // magic number
    mput(snd("offset"),"uib",fid); // data location
    mput(snd("databytes"),"uib",fid); // size in bytes
    mput(snd("format"),"uib",fid); // data format
    //
    mput(snd("rate")/snd("chans"),"uib",fid); // sample rate
    mput(snd("chans"),"uib",fid); // channels
    mput(ascii(snd("info")),"c",fid);  // info
endfunction
