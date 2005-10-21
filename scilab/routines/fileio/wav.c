/*
 * April 15, 1992
 * Copyright 1992 Rick Richardson
 * Copyright 1991 Lance Norskog And Sundry Contributors
 * This source code is freely redistributable and may be used for
 * any purpose.  This copyright notice must be maintained. 
 * Lance Norskog And Sundry Contributors are not responsible for 
 * the consequences of using this software.
 */

/*
 * Windows 3.0 .wav format driver
 */

/*
 * Fixed by various contributors:
 * 1) Little-endian handling
 * 2) Skip other kinds of file data
 * 3) Handle 16-bit formats correctly
 * 4) Not go into infinite loop
 */

/*
 * User options should overide file
 * header :- we assumed user knows what
 * they are doing if they specify options.
 * Enhancements and clean up
 * by Graeme W. Gill, 93/5/17
 */

#include <string.h>

#include "st.h"
#include "wav.h"
extern void sciprint __PARAMS((char *fmt, ...));

/* Private data for .wav file */
typedef struct wavstuff 
{
  long	samples;
  int	second_header; /* non-zero on second header write */
} *wav_t;

static char *wav_format_str(unsigned int wFormatTag); 

/*
 * Do anything required before you start reading samples.
 * Read file header. 
 *	Find out sampling rate, 
 *	size and style of samples, 
 *	mono/stereo/quad.
 */

void wavstartread(ft_t ft, WavInfo *Wi, int flag)
{
  wav_t	wav = (wav_t) ft->priv;
  char	magic[4];
  unsigned len;  
  int	littlendian = 1;
  char	*endptr;
  
  /* wave file characteristics */

  
  endptr = (char *) &littlendian;
  if (!*endptr) ft->swap = 1;
  /** sciprint("Read swap status  %d\r\n",ft->swap); **/
  /* If you need to seek around the input file. */
  if (0 && ! ft->seekable)
    {
      sciprint("Sorry, .wav input file must be a file, not a pipe\r\n");
      ft->ierr=1;return;
    }
  
  if (   fread(magic, 1, 4, ft->fp) != 4
      || strncmp("RIFF", magic, 4))
    {
      sciprint("Sorry, not a RIFF file\r\n");
      ft->ierr=1;return;
    }
  
  len = rllong(ft);
  
  if (   fread(magic, 1, 4, ft->fp) != 4
      || strncmp("WAVE", magic, 4))
    {
      sciprint("Sorry, not a WAVE file\r\n");
      ft->ierr=1;return;
    }
  
  /* Now look for the format chunk */
  for (;;)
    {
      if ( fread(magic, 1, 4, ft->fp) != 4 )
	{
	  sciprint("Sorry, missing fmt spec\r\n");
	  ft->ierr=1;return;
	}
      len = rllong(ft);
      if (strncmp("fmt ", magic, 4) == 0)
	break;			/* Found the format chunk */
      while (len > 0 && !feof(ft->fp)) /* skip to next chunk */
	{
	  getc(ft->fp);
	  len--;
	}
    }
  
  if ( len < 16 )
    {
      sciprint("Sorry, fmt chunk is too short\r\n");
      ft->ierr=1;return;
    }
  
  Wi->wFormatTag = rlshort(ft);
  switch ( Wi->wFormatTag)
    {
    case WAVE_FORMAT_UNKNOWN:
      {
	sciprint("Sorry, this WAV file is in Microsoft Official Unknown format.\r\n");
	ft->ierr=1;return;
      }
    case WAVE_FORMAT_PCM_SCI: 	/* this one, at least, I can handle */
      if (ft->info.style != -1 && ft->info.style != UNSIGNED && ft->info.style != SIGN2)
	sciprint("User options overiding style read in .wav header\r\n");
      break;
    case WAVE_FORMAT_ADPCM:
      {
	sciprint("Sorry, this WAV file is in Microsoft ADPCM format.\r\n");
	ft->ierr=1;return;
      }

    case WAVE_FORMAT_ALAW:	/* Think I can handle this */
      if (ft->info.style == -1 || ft->info.style == ALAW)
	ft->info.style = ALAW;
      else
	sciprint("User options overiding style read in .wav header\r\n");
      break;
    case WAVE_FORMAT_MULAW:	/* Think I can handle this */
      if (ft->info.style == -1 || ft->info.style == ULAW)
	ft->info.style = ULAW;
      else
	sciprint("User options overiding style read in .wav header\r\n");
      break;
    case WAVE_FORMAT_OKI_ADPCM:
      {
	sciprint("Sorry, this WAV file is in OKI ADPCM format.\r\n");
	ft->ierr=1;return;
      }

    case WAVE_FORMAT_DIGISTD:
      {
	sciprint("Sorry, this WAV file is in Digistd format.\r\n");
	ft->ierr=1;return;
      }
    case WAVE_FORMAT_DIGIFIX:
      {
	sciprint("Sorry, this WAV file is in Digifix format.\r\n");
	ft->ierr=1;return;
      }
    case IBM_FORMAT_MULAW:
      {
	sciprint("Sorry, this WAV file is in IBM U-law format.\r\n");
	ft->ierr=1;return;
      }
    case IBM_FORMAT_ALAW:
      {
	sciprint("Sorry, this WAV file is in IBM A-law format.\r\n");
	ft->ierr=1;return;
      }
    case IBM_FORMAT_ADPCM:
      {
	sciprint("Sorry, this WAV file is in IBM ADPCM format.\r\n");
	ft->ierr=1;return;
      }
    default:	
      {
	sciprint("Sorry, don't understand format\r\n");
	ft->ierr=1;return;
      }
    }
  Wi->wChannels = rlshort(ft);
  /* User options take precedence */
  if (ft->info.channels == -1 || ft->info.channels == Wi->wChannels)
    ft->info.channels = Wi->wChannels;
  else
    sciprint("User options overiding channels read in .wav header\r\n");
  Wi->wSamplesPerSecond = rllong(ft);
  if (ft->info.rate == (unsigned int)0 || ft->info.rate == Wi->wSamplesPerSecond)
    ft->info.rate = Wi->wSamplesPerSecond;
  else
    sciprint("User options overiding rate read in .wav header\r\n");
  Wi->wAvgBytesPerSec = rllong(ft);	/* Average bytes/second */
  Wi->wBlockAlign = rlshort(ft);	/* Block align */
  Wi->wBitsPerSample =  rlshort(ft); /* bits per sample per channel */
  Wi->bytespersample = (Wi->wBitsPerSample + 7)/8;
  switch (Wi->bytespersample)
    {
    case 1:
      /* User options take precedence */
      if (ft->info.size == -1 || ft->info.size == BYTESCI)
	ft->info.size = BYTESCI;
      else
	sciprint("User options overiding size read in .wav header\r\n");
      if (ft->info.style == -1 || ft->info.style == UNSIGNED)
	ft->info.style = UNSIGNED;
      else if (ft->info.style != ALAW && ft->info.style != ULAW) 
	sciprint("User options overiding style read in .wav header\r\n");
      break;
    case 2:
      if (ft->info.size == -1 || ft->info.size == WORDSCI)
	ft->info.size = WORDSCI;
      else
	sciprint("User options overiding size read in .wav header\r\n");
      if (ft->info.style == -1 || ft->info.style == SIGN2)
	ft->info.style = SIGN2;
      else
	sciprint("User options overiding style read in .wav header\r\n");
      break;
    case 4:
      if (ft->info.size == -1 || ft->info.size == LONGSCI)
	ft->info.size = LONGSCI;
      else
	sciprint("User options overiding size read in .wav header\r\n");
      if (ft->info.style == -1 || ft->info.style == SIGN2)
	ft->info.style = SIGN2;
      else
	sciprint("User options overiding style read in .wav header\r\n");
      break;
    default:
      {
	sciprint("Sorry, don't understand .wav size\r\n");
	ft->ierr=1;return;
      }
    }
  len -= 16;
  while (len > 0 && !feof(ft->fp))
    {
      getc(ft->fp);
      len--;
    }
  
  /* Now look for the wave data chunk */
  for (;;)
    {
      if ( fread(magic, 1, 4, ft->fp) != 4 )
	{
	  sciprint("Sorry, missing data chunk\r\n");
	  ft->ierr=1;return;
	}
      len = rllong(ft);
      if (strncmp("data", magic, 4) == 0)
	break;			/* Found the data chunk */
      while (len > 0 && !feof(ft->fp)) /* skip to next chunk */
	{
	  getc(ft->fp);
	  len--;
	}
    }
  Wi->data_length = len;
  wav->samples = Wi->data_length/ft->info.size; /* total samples */
  
  if ( flag == 1 ) 
    {
      sciprint("Reading Wave file: %s format, %d channel%s, %d samp/sec\r\n",
	       wav_format_str(Wi->wFormatTag), Wi->wChannels,
	       Wi->wChannels == 1 ? "" : "s", Wi->wSamplesPerSecond);
      sciprint("        %d byte/sec, %d block align, %d bits/samp, %u data bytes\r\n",
	       Wi->wAvgBytesPerSec, Wi->wBlockAlign,Wi->wBitsPerSample, Wi->data_length);
    }
  Wi->wav_format = wav_format_str(Wi->wFormatTag);
}

/*
 * Read up to len samples from file.
 * Convert to signed longs.
 * Place in buf[].
 * Return number of samples read.
 */
#if defined(__alpha)|defined(__ia64__)
int wavread(ft_t ft,int *buf, long int len)
#else 
int wavread(ft_t ft, long int *buf, long int len)
#endif
{
  wav_t	wav = (wav_t) ft->priv;
  int	done;

  if (len > wav->samples) len = wav->samples;
  if (len == 0)
    return 0;
  done = rawread(ft, buf, len);
  if (done == 0)
    sciprint("Premature EOF on .wav input file\r\n");
  wav->samples -= done;
  return done;
}

/*
 * Do anything required when you stop reading samples.  
 * Don't close input file! 
 */

void wavstopread(ft_t ft)
{
}

void wavstartwrite(ft_t ft)
{
  wav_t	wav = (wav_t) ft->priv;
  int	littlendian = 1;
  char	*endptr;
  
  endptr = (char *) &littlendian;
  if (!*endptr) ft->swap = 1;
  
  wav->samples = 0;
  wav->second_header = 0;
  if (! ft->seekable)
    sciprint("Length in output .wav header will wrong since can't seek to fix it\r\n");
  wavwritehdr(ft);
}

void wavwritehdr(ft_t ft)
{
  wav_t	wav = (wav_t) ft->priv;
  /* wave file characteristics */
  unsigned short wFormatTag;	/* data format */
  unsigned short wChannels;	/* number of channels */
  unsigned long  wSamplesPerSecond; /* samples per second per channel */
  unsigned long  wAvgBytesPerSec; /* estimate of bytes per second needed */
  unsigned short wBlockAlign;	/* byte alignment of a basic sample block */
  unsigned short wBitsPerSample; /* bits per sample */
  unsigned long  data_length;	/* length of sound data in bytes */
  unsigned long  bytespersample; /* bytes per sample (per channel) */
  
  switch (ft->info.size)
    {
    case BYTESCI:
      wBitsPerSample = 8;
      if (ft->info.style == -1 || ft->info.style == UNSIGNED)
	ft->info.style = UNSIGNED;
      else if (!wav->second_header && ft->info.style != ALAW && ft->info.style != ULAW) 
	sciprint("User options overiding style written to .wav header\r\n");
      break;
    case WORDSCI:
      wBitsPerSample = 16;
      if (ft->info.style == -1 || ft->info.style == SIGN2)
	ft->info.style = SIGN2;
      else if (!wav->second_header)
	sciprint("User options overiding style written to .wav header\r\n");
      break;
    case LONGSCI:
      wBitsPerSample = 32;
      if (ft->info.style == -1 || ft->info.style == SIGN2)
	ft->info.style = SIGN2; 
      else if (!wav->second_header)
	sciprint("User options overiding style written to .wav header\r\n");
      break;
    default:
      wBitsPerSample = 32;
      if (ft->info.style == -1)
	ft->info.style = SIGN2; 
      if (!wav->second_header)
	sciprint("Sciprinting - writing bad .wav file using %s\r\n",sizes[ft->info.size]);
      break;
    }
  
  switch (ft->info.style)
    {
    case UNSIGNED:
      wFormatTag = WAVE_FORMAT_PCM_SCI;
      if (wBitsPerSample != 8 && !wav->second_header)
	sciprint("Sciprinting - writing bad .wav file using unsigned data and %d bits/sample\r\n",wBitsPerSample);
      break;
    case SIGN2:
      wFormatTag = WAVE_FORMAT_PCM_SCI;
      if (wBitsPerSample == 8 && !wav->second_header)
	sciprint("Sciprinting - writing bad .wav file using signed data and %d bits/sample\r\n",wBitsPerSample);
      break;
    case ALAW:
      wFormatTag = WAVE_FORMAT_ALAW;
      if (wBitsPerSample != 8 && !wav->second_header)
	sciprint("Sciprinting - writing bad .wav file using A-law data and %d bits/sample\r\n",wBitsPerSample);
      break;
    case ULAW:
      wFormatTag = WAVE_FORMAT_MULAW;
      if (wBitsPerSample != 8 && !wav->second_header)
	sciprint("Sciprinting - writing bad .wav file using U-law data and %d bits/sample\r\n",wBitsPerSample);
      break;
    }
  
  
  wSamplesPerSecond = ft->info.rate;
  bytespersample = (wBitsPerSample + 7)/8;
  wAvgBytesPerSec = ft->info.rate * ft->info.channels * bytespersample;
  wChannels = ft->info.channels;
  wBlockAlign = (unsigned short) (ft->info.channels * bytespersample);
  if (!wav->second_header)	/* use max length value first time */
    data_length = 0x7fffffff - (8+16+12);
  else				/* fixup with real length */
    data_length = bytespersample * wav->samples;
  
  /* figured out header info, so write it */
  fputs("RIFF", ft->fp);
  wllong(ft, data_length + 8+16+12); /* Waveform chunk size: FIXUP(4) */
  fputs("WAVE", ft->fp);
  fputs("fmt ", ft->fp);
  wllong(ft, (long)16);		/* fmt chunk size */
  wlshort(ft, wFormatTag);
  wlshort(ft, wChannels);
  wllong(ft, wSamplesPerSecond);
  wllong(ft, wAvgBytesPerSec);
  wlshort(ft, wBlockAlign);
  wlshort(ft, wBitsPerSample);
  
  fputs("data", ft->fp);
  wllong(ft, data_length);	/* data chunk size: FIXUP(40) */
  
  if (!wav->second_header) 
    {
      sciprint("Writing Wave file: %s format, %d channel%s, %d samp/sec\r\n",
	       wav_format_str(wFormatTag), wChannels,
	       wChannels == 1 ? "" : "s", wSamplesPerSecond);
      sciprint("        %d byte/sec, %d block align, %d bits/samp\r\n",
	       wAvgBytesPerSec, wBlockAlign, wBitsPerSample);
    } 
  else
    sciprint("Finished writing Wave file, %u data bytes\r\n",data_length);
}

void wavwrite(ft_t ft, long int *buf, long int len)
{
  wav_t	wav = (wav_t) ft->priv;
  wav->samples += len;
  rawwrite(ft, buf, len);
}

void wavstopwrite(ft_t ft)
{
  /* All samples are already written out. */
  /* If file header needs fixing up, for example it needs the */
  /* the number of samples in a field, seek back and write them here. */
  if (!ft->seekable)
    return;
  if (fseek(ft->fp, 0L, 0) != 0)
    {
      sciprint("Sorry, can't rewind output file to rewrite .wav header.\r\n");
      ft->ierr=1;return;
    }
  ((wav_t) ft->priv)->second_header = 1;
  wavwritehdr(ft);
}

/*
 * Return a string corresponding to the wave format type.
 */
static char *
wav_format_str(unsigned int wFormatTag)
{
  switch (wFormatTag)
    {
    case WAVE_FORMAT_UNKNOWN:
      return "Microsoft Official Unknown";
    case WAVE_FORMAT_PCM_SCI:
      return "Microsoft PCM";
    case WAVE_FORMAT_ADPCM:
      return "Microsoft ADPCM";
    case WAVE_FORMAT_ALAW:
      return "Microsoft A-law";
    case WAVE_FORMAT_MULAW:
      return "Microsoft U-law";
    case WAVE_FORMAT_OKI_ADPCM:
      return "OKI ADPCM format.";
    case WAVE_FORMAT_DIGISTD:
      return "Digistd format.";
    case WAVE_FORMAT_DIGIFIX:
      return "Digifix format.";
    case IBM_FORMAT_MULAW:
      return "IBM U-law format.";
    case IBM_FORMAT_ALAW:
      return "IBM A-law";
    case IBM_FORMAT_ADPCM:
      return "IBM ADPCM";
    default:
      return "Unknown";
    }
}

