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
 
// Get default:
[nargout,nargin] = argn(0)
Fs_pref = 8000;
nbits_pref = 8;
method_pref = 'mu';
 
if nargin==1 then
  error('Incorrect number of input arguments.');
elseif nargin>5 then
  error('Incorrect number of input arguments.');
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
  error('Filename must be a string.');
end
if strindex(aufile,'.')==[] then
  aufile = aufile+'.au';
end

[fid,junk] = mopen(aufile,'wb',0) // Big-endian
if junk<0 then
  error('Cannot open .au sound file');
end
 
if length(size(y)) > 2 then
  error('Data array must have 1- or 2-dimensions, only.');
end
if size(y,1)==1 then
  y = y(:);
end
 
// Clip data to normalized range [-1,+1]:
i = matrix(find(abs(y)>1),1,-1);
if ~(i==[]) then
  y(i) = sign(y(i));
  warning('Data clipped during write to file.');
end
 
snd = write_sndhdr(fid,Fs,nbits,method,size(y));

if write_sndata(fid,snd,y) then
  error('Error while writing sound file.');
end
mclose(fid);

endfunction
function [status]=write_sndata(fid,snd,data)
status = 0;
if snd('format')==1 then
  dtype = 'uc';
  data = lin2mu(data);
elseif snd('format')==2 then
  dtype = 'int8';
  data = round(data*(2^7-1));
elseif snd('format')==3 then
  dtype = 'int16';
  data = round(data*(2^15-1));
else
  status = -1;
  return
end
total_samples = snd('samples')*snd('chans');
err=mput(matrix(data',total_samples,1),dtype,fid);
if err~=0 then
  status = -1;
end

endfunction
function [snd]=write_sndhdr(fid,Fs,nbits,method,sz)
if method=='mu' then
  if nbits~=8 then
    error('Mu-law can only be used with 8 bit data.'+' Use method=''linear'' instead.');
  end
  snd.format = 1;
  snd.bits = 8;
elseif method=='linear' then
  if nbits==8 then
    snd.format=2  // 8-bit linear
    snd.bits=8
  elseif nbits==16 then 
    snd.format=3  // 16-bit linear
    snd.bits=16
  else
    error('Wavwrite: unrecognized data format');
    return
  end
else
  error('Unrecognized data format');
end
 
// Define sound header structure:
snd('samples')=sz(1)
snd('chans')=sz(2)
total_samples = snd('samples')*snd('chans');
bytes_per_sample = ceil(snd('bits')/8);
snd('rate')=Fs
snd('databytes')=bytes_per_sample*total_samples
snd('offset')=28
snd('info')='SCI0'; 
 
mput('.snd','c',fid); // magic number
mput(snd('offset'),'ul',fid); // data location
mput(snd('databytes'),'ul',fid); // size in bytes
mput(snd('format'),'ul'); // data format
mput(snd('rate'),'ul'); // sample rate
mput(snd('chans'),'ul'); // channels
mput(snd('info'),'c');  // info
endfunction
