function [y,Fs,bits]=wavread(wavfile,ext)
  y=[];Fs=[];bits=[];
  //Read Microsoft .wav sound file.
  //y=wavread(wavfile) reads a .wav file specified by the string wavfile,
  //returning the sampled data in y. The .wav extension is appended
  //if no extension is given.  
  //Amplitude values are in the range [-1,+1].
  //[y,fs,bits]=wavread(wavfile) returns the sample rate (fs) in Hertz
  //and the number of bits per sample (bits) used to encode the
  //data in the file.
  //[...]=wavread(wavfile,n) returns only the first n samples from each
  //      channel in the file.
  //[...]=wavread(wavfile,[n1 n2]) returns only samples n1 through n2 from
  //     each channel in the file.
  //siz=wavread(wavfile,'size') returns the size of the audio data contained
  //    in the file in place of the actual audio data, returning the
  //     vector siz=[samples channels].

  // Append .wav extension if necessary
  if strindex(wavfile,'.')==[] then  wavfile = wavfile+'.wav';end
  // Open the file
  [fid,err] = mopen(wavfile,'rb',1);    // Little-endian
  if err<0 then  error('Cannot open '+wavfile), end

  // Handle ext optional argument
  if argn(2)<2 then   ext = [];end

  if type(ext)==10 then
    ext=convstr(ext)
    if ext<>'size' then
      error('second argument must be ""size"", an integer or a vector of 2 integers")
    end
  elseif type(ext)==1 then
    exts = size(ext,'*');
    if exts>2 then
      error('Index range must be specified as a scalar or 2-element vector.');
    end
    if exts==1 then
      if ext==0 then
	ext = 'size';  // synonym for size
      else
	ext = [1,ext];
      end
    end
  else
    error('second argument must be ""size"", an integer or a vector of 2 integers")
  end
  


  Data=[];
  ID=stripblanks(ascii(mget(4,'c',fid)));
  Size=mget(1,'ui',fid);
  if (convstr(ID)~='riff') then
    error('.wav file does not contain the RIFF identifier')
  end
  rifftype=mget(4,'c',fid);
  dtype = convstr(ascii(rifftype)');
  if dtype~='wave' then
    error('.wav file does not contain the wave identifier')
  end
  
  // Find optional chunks
  found_fmt = 0; found_data = 0;
  while ~found_data then
    [ID,Size]=find_cktype(fid);
    select ID
     case 'fact' then
      total_bytes=Size;
      orig_pos = mtell(fid);
      nbytes = 4;
      // # of required bytes in <fact-ck> header
      if total_bytes < nbytes then
	error('Error reading .wav file');
      end
      factdata=mget(1,'ui',fid); // Samples per second
      rbytes = total_bytes-(mtell(fid)-orig_pos);
      if rbytes then
	if mseek(rbytes,fid,'cur')==-1 then
	  error('Error reading <fact-ck> chunk.');
	end
      end

     case 'fmt' then
      found_fmt = 1;
      [wFormatTag,nChannels,nSamplesPerSec,nAvgBytesPerSec,...
       nBlockAlign,nBitsPerSample,cbSize]=read_wavefmt(fid,Size);
     case 'data' then
      found_data = 1;
      if ~found_fmt then
	error('Invalid .wav file: found data before format information.');
      end
      if (ext=='size')|(~(ext==[]))&and(ext==0) then
	// Caller just wants data size:
	samples=read_wavedat(fid, Size ,wFormatTag, nChannels, nBitsPerSample,-1)
	mclose(fid);
	y = [samples,nChannels];
      else
	 y=read_wavedat(fid, Size ,wFormatTag, nChannels,nBitsPerSample,ext)
	 mclose(fid);
      end
    else
       if mseek(Size,fid,'cur')==-1 then
	 error('Incorrect chunk size information in RIFF file.');
       end
    end
  end
  Fs = nSamplesPerSec;
  if wFormatTag==1 then
    bits = nBitsPerSample;
  else
     bits = [];
  end
endfunction


function [ID,Size]=find_cktype(fid)
  ID=stripblanks(ascii(mget(4,'c',fid)));
  Size=mget(1,'ui',fid)
endfunction

function [wFormatTag,nChannels,nSamplesPerSec,nAvgBytesPerSec,nBlockAlign,nBitsPerSample,cbSize]=read_wavefmt(fid,total_bytes)
  orig_pos = mtell(fid); 
  nbytes = 14; // # of required bytes in  header

  if total_bytes<nbytes then
    error('Error reading .wav file');
  end
  
  // Read wav data:
  wFormatTag=mget(1,'us',fid);  // Data encoding format
  nChannels=mget(1,'us',fid); // Number of channels
  nSamplesPerSec=mget(1,'ui',fid); // Samples per second
  nAvgBytesPerSec=mget(1,'ui',fid); // Avg transfer rate
  nBlockAlign=mget(1,'us',fid); // Block alignment
  
  if wFormatTag~=1 then
    error('Invalid wav format');
  else
     [cbSize,nBitsPerSample]=read_fmt_pcm(fid,total_bytes)
  end

  rbytes = total_bytes-(mtell(fid)-orig_pos);
  if rbytes then
    if mseek(rbytes,fid,'cur')==-1 then
      error('Error reading wav file');
    end
  end
endfunction

function [cbSize,nBitsPerSample]=read_fmt_pcm(fid,total_bytes)
  nbytes = 14; cbSize=[]; nBitsPerSample=[]; 
  // # of bytes already read 
  if total_bytes < nbytes+2 then
    error('Error reading wav file');
  end
  nBitsPerSample = mget(1, 'us', fid);
  nbytes = nbytes+2;
  if total_bytes > nbytes then
    if total_bytes>=nbytes+2 then
      cbSize= mget(1,'us',fid);
      nbytes = nbytes+2;
    end
    if total_bytes>nbytes then
      if mseek(total_bytes-nbytes,fid,'cur')==-1 then
	error('Error reading .wav file');
      end
    end
  end
endfunction

function Data=read_wavedat(fid, Size, wFormatTag, nChannels, nBitsPerSample, ext)
  fmt_msg = [];
  select wFormatTag
   case 1 then
    // PCM Format:
    Data=read_dat_pcm(fid, Size, nChannels, nBitsPerSample, ext)
   case 2 then
    fmt_msg = 'Microsoft ADPCM';
   case 6 then
    fmt_msg = 'CCITT a-law';
   case 7 then
    fmt_msg = 'CCITT mu-law';
   case 17 then
    fmt_msg = 'IMA ADPCM';
   case 34 then
    fmt_msg = 'DSP Group TrueSpeech TM';
   case 49 then
    fmt_msg = 'GSM 6.10';
   case 50 then
    fmt_msg = 'MSN Audio';
   case 257 then
    fmt_msg = 'IBM Mu-law';
   case 258 then
    fmt_msg = 'IBM A-law';
   case 259 then
    fmt_msg = 'IBM AVC Adaptive Differential';
  else
     fmt_msg = 'Format #'+string(wFormatTag);
  end
  if ~(fmt_msg==[]) then
    error('Data compression format '+fmt_msg+' is not supported');
  end
endfunction


function Data=read_dat_pcm(fid,total_bytes , nChannels, nBitsPerSample, ext)
// Determine # bytes/sample - format requires rounding
//  to next integer number of bytes:
  BytesPerSample = ceil(nBitsPerSample/8);
  if BytesPerSample==1 then  // unsigned 8-bit
    dtype = 'uc';
  elseif BytesPerSample==2 then // signed 16-bit
     dtype = 's';
  else
     error('Cannot read .wav file  with more than 16 bits per sample');
  end
  // # bytes in this chunk
  total_samples = total_bytes/BytesPerSample;
  SamplesPerChannel = total_samples/nChannels;
  if (~(ext==[]))&(ext==-1) then
    // Just return the samples per channel, and seek past data:
    Data = SamplesPerChannel;
    if mseek(total_bytes,fid,'cur')==-1 then
      error( 'Error reading .wav file');
    end
    return
  end
  if ext==[] then
    ext = [1,SamplesPerChannel];
  else
     if prod(size(ext))~=2 then
       error('Sample limit vector must have 2 entries');
       return
     end
     if ext(1)<1|ext(2)>SamplesPerChannel then
       error('Sample limits out of range.');
     end
     if ext(1)>ext(2) then
       error('Invalid sample limits (use ascending order)');
     end
  end

  if ext(1)>1 then
    // Skip if specified:
    if mseek(BytesPerSample*(ext(1)-1)*nChannels,fid,'cur')==-1 then
      error('Error reading .wav file');
      return
    end
  end
  
  // Read data:
  nSPCext = ext(2)-ext(1)+1;
  // # samples per channel in extraction range
  extSamples = nChannels*nSPCext;
  Data=matrix(mget(nChannels*nSPCext,dtype,fid),[nChannels,nSPCext])

  // Skip trailing samples:
  if mseek(BytesPerSample*(SamplesPerChannel-ext(2))*nChannels,fid,'cur')==-1 then
    error('Error reading .wav file');
    return
  end
  
  // Determine if a pad-byte is appended and skip if present:
  junk = Size;
  if junk-fix(junk./2).*2 then
    mseek(1,fid,'cur');
  end
  Data=Data';
  // Normalize data range in [-1 1] (min will hit -1)
  if BytesPerSample==1 then
    Data=(Data-128)/128;
  else
     Data=Data/32768
  end
endfunction 
