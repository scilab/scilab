function []=wavwrite(y,Fs,nbits,wavefile)
  [nargout,nargin] = argn(0)
  //WAVWRITE Write Microsoft WAVE ("""".wav"""") sound file.
  //   WAVWRITE(Y,WAVEFILE) writes a WAVE file specified by the
  //   string WAVEFILE.  The data should be arranged with one channel
  //   per column.  Amplitude values outside the range [-1,+1] are
  //   clipped prior to writing.
  // 
  //   WAVWRITE(Y,FS,WAVEFILE) specifies the sample rate FS of the data
  //   in Hertz.
  // 
  //   WAVWRITE(Y,FS,NBITS,WAVEFILE) forces an NBITS-bit file format to
  //   be written, where NBITS<=16.
  // 
  //   Supports multi-channel 8- or 16-bit WAVE data.
  // 
  //   See also WAVREAD, AUWRITE.
  
  //   Copyright (c) 1984-98 by The MathWorks, Inc.
  //   $Revision: 1.3 $  $Date: 2003/03/13 10:13:31 $
  
  //   D. Orofino, 11/95
  
  // Get user default preferences:
  Fs_pref = 8000;
  nbits_pref = 16;
  
  // Parse inputs:
  if nargin<2|nargin>4 then
    error('Incorrect number of input arguments.');
  elseif nargin<3 then
     wavefile = Fs;
     Fs = Fs_pref;
     nbits = nbits_pref;
  elseif nargin<4 then
     wavefile = nbits;
     nbits = nbits_pref;
  end
  
  // Open file for output:
  if ~(type(wavefile)==10) then
    error('wavefile must be a string.');
  end
  if mtlb_findstr(wavefile,'.')==[] then
    wavefile = wavefile+'.wav';
  end
  [fid,%v] = mopen(wavefile,'wb',1)
  if %v<0 then fid = -1;end
  // Little-endian
  if fid==(-1) then
    error('Can''t open WAVE file for output.');
  end
  
  // If input is a vector, force it to be a column:

  if size(y,2) > 2 then
    error('Data array must have 1- or 2-dimensions, only.');
  end
  [samples,channels] = size(y);
  if samples==1 then
    y = y(:);
    [samples,channels] = size(y);
  end
  
  // Clip data to normalized range [-1,+1]:
  i = matrix(find(abs(y)>1),1,-1);
  if ~(i==[]) then
    y(i) = sign(y(i)) 
    warning('Data clipped during write to file.');
  end
  
  // # bytes per sample to write
  bytes_per_sample = ceil(nbits/8);
  total_samples = samples*channels;
  total_bytes = total_samples*bytes_per_sample;
  
  // Determine number of bytes in RIFF chunk
  // (not including pad bytes, if needed):
  // ----------------------------------
  //  'RIFF'           4 bytes
  //  size             4 bytes (ulong)
  //  'WAVE'           4 bytes
  //  'fmt '           4 bytes
  //  size             4 bytes (ulong)
  // <wave-format>     14 bytes
  // <format_specific> 2 bytes (PCM)
  //  'data'           4 bytes
  //  size             4 bytes (ulong)
  // <wave-data>       N bytes
  // ----------------------------------
  riff_cksize = 36+total_bytes;
  // Don't include 'RIFF' or its size field
  fmt_cksize = 16;
  // Don't include 'fmt ' or its size field
  data_cksize = total_bytes;
  // Don't include 'data' or its size field
  
  // Determine pad bytes:
  data_pad = data_cksize-fix(data_cksize./2).*2;
  riff_cksize = riff_cksize+data_pad;
  // + fmt_pad, always 0

  ck= tlist(['ck','fid','Size','ID']) 
  // Write RIFF chunk:
  ck('fid')=fid
  ck('Size')=riff_cksize
  ck('ID')='RIFF';
  write_ckinfo(ck);

  // Write WAVE:
  ck('ID')='WAVE';
  write_ckinfo(ck,1);
  
  // Write <fmt-ck>:
  ck('ID')='fmt ';
  ck('Size')=fmt_cksize
  write_ckinfo(ck);
  // Write <wave-format>:
  fmt=tlist(['fmt','wFormatTag','nChannels','nSamplesPerSec','nAvgBytesPerSec','nBlockAlign','nBitsPerSample'])
  fmt('wFormatTag')=1
  // Data encoding format = PCM
  fmt('nChannels')=channels
  // Number of channels
  fmt('nSamplesPerSec')=Fs
  // Samples per second
  fmt('nAvgBytesPerSec')=channels*bytes_per_sample*Fs
  // Avg transfer rate
  fmt('nBlockAlign')=channels*bytes_per_sample
  // Block alignment
  fmt('nBitsPerSample')=nbits
  // standard <PCM-format-specific> info
  status = write_wavefmt(fid,fmt);
  
  // Write <data-ck>:
  ck('ID') = 'data';
  ck('Size')=data_cksize
  write_ckinfo(ck);
  
  // Write <wave-data>, and its pad byte if needed:
  status = write_wavedat(fid,fmt,y);
  mclose(fid);
  // Close file
  // end of wavwrite()
endfunction

function write_ckinfo(ck,sflg)
  [nargout,nargin] = argn(0)
  // WRITE_CKINFO: Writes next RIFF chunk, but not the chunk data.
  //   If optional sflg is set to nonzero, write SUBchunk info instead.
  //   Expects an open FID pointing to first byte of chunk header,
  //   and a chunk structure.
  //   ck.fid, ck.ID, ck.Size, ck.Data
  if length(ck('ID'))<>4 then error('write_ckinfo: ck has a wrong length');end
  mput(ascii(ck('ID')),'c',ck('fid'))
  // Error condition
  if nargin==1 then
    // Write chunk size (skip if subchunk):
    mput(ck('Size'),'ui',ck('fid'))
  end
endfunction

function [status]=write_wavedat(fid,fmt,data)
  status=[];
  // WRITE_WAVEDAT: Write WAVE data chunk
  //   Assumes fid points to the wave-data chunk
  //   Requires <wave-format> structure to be passed.
  
  status = 0;
  
  if fmt('wFormatTag')==1 then
    // PCM Format:
    // Determine # bytes/sample - format requires rounding
    //  to next integer number of bytes:
    BytesPerSample = ceil(fmt('nBitsPerSample')/8);
    if BytesPerSample==1 then
      dtype = 'uc';
      // unsigned 8-bit
      // Scale data according to bits/samples: [-1,+1] -> [0,255]
      data = round((data+1)*255/2);
    elseif BytesPerSample==2 then
       dtype = 's';
       // signed 16-bit
       // Scale data according to bits/samples: [-1,+1] -> [-32768,+32767]
       data = round((data+1)*65535/2)-32768;
    else
       error('Cannot write WAVE files with more than 16 bits/sample.');
    end
    
    // Write data, one row at a time (one sample from each channel):
    [samples,channels] = size(data);
    total_samples = samples*channels;
    if mput(matrix(data',total_samples,1),dtype,fid)~=0 then
      status = -1;
      return
    end
    // Error condition
    
    // Determine if a pad-byte is appended to data chunk:
    %v2_1$1 = total_samples*BytesPerSample
    if %v2_1$1-fix(%v2_1$1./2).*2 then
      mtlb_fwrite(fid,0,'uchar');
    end
    
    
  else
     // Unknown wave-format for data.
     error('Unknown data format.');
  end
  return
endfunction

function [status]=write_wavefmt(fid,fmt)
  status = 0;
  // WRITE_WAVEFMT: Write WAVE format chunk.
  //   Assumes fid points to the wave-format subchunk.
  //   Requires chunk structure to be passed, indicating
  //   the length of the chunk.
  
  // Create <wave-format> data:

  mput(fmt('wFormatTag'),'us',fid);
  mput(fmt('nChannels'),'us',fid);
  mput(fmt('nSamplesPerSec'),'ui',fid);
  mput(fmt('nAvgBytesPerSec'),'ui',fid);
  mput(fmt('nBlockAlign'),'us',fid);
  
  // Write format-specific info:
  if fmt('wFormatTag')==1 then
    // Write standard <PCM-format-specific> info:
    mput(fmt('nBitsPerSample'),'us',fid)
  else
     error('Unknown data format.');
  end
endfunction
