#ifndef WAV_WAVH
#define WAV_WAVH

typedef struct wavinfo {
  char *wav_format;             /* data format as a string */
  unsigned short wFormatTag;	/* data format */
  unsigned short wChannels;	/* number of channels */
  unsigned long  wSamplesPerSecond; /* samples per second per channel */
  unsigned long  wAvgBytesPerSec; /* estimate of bytes per second needed */
  unsigned short wBlockAlign;	/* byte alignment of a basic sample block */
  unsigned short wBitsPerSample; /* bits per sample */
  unsigned long  data_length;	/* length of sound data in bytes */
  unsigned long  bytespersample; /* bytes per sample (per channel) */
} WavInfo;

/* purloined from public Microsoft RIFF docs */

#define	WAVE_FORMAT_UNKNOWN		(0x0000)
#define	WAVE_FORMAT_PCM_SCI			(0x0001) 
#define	WAVE_FORMAT_ADPCM		(0x0002)
#define	WAVE_FORMAT_ALAW		(0x0006)
#define	WAVE_FORMAT_MULAW		(0x0007)
#define	WAVE_FORMAT_OKI_ADPCM		(0x0010)
#define	WAVE_FORMAT_DIGISTD		(0x0015)
#define	WAVE_FORMAT_DIGIFIX		(0x0016)
#define	IBM_FORMAT_MULAW         	(0x0101)
#define	IBM_FORMAT_ALAW			(0x0102)
#define	IBM_FORMAT_ADPCM         	(0x0103)

#endif
