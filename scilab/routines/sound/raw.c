/****************************************************************
 * July 5, 1991
 * Copyright 1991 Lance Norskog And Sundry Contributors
 * This source code is freely redistributable and may be used for
 * any purpose.  This copyright notice must be maintained. 
 * Lance Norskog And Sundry Contributors are not responsible for 
 * the consequences of using this software.
 ****************************************************************
 * jpc : modif for dec alpha on which long int are 64bits long 
 ****************************************************************/

/****************************************************************
 * Sound Tools raw format file.
 * Includes .ub, .uw, .sb, .sw, and .ul formats at end
 *
 * Notes: most of the headerless formats set their handlers to raw
 * in their startread/write routines.  
 ****************************************************************/

#include "st.h"
#include "libst.h"

extern void sciprint __PARAMS((char *fmt, ...));

/****************************************************************
 * Read raw file data, and convert it to 
 * the sox internal signed long format. 
 * Warning : on some dec-alpha long x = LEFT(int y,16) 
 *  does not give the proper result (first shift then cast ) 
 * but the result seams to be given by first cast then shift which 
 * is not what we want. Thus we have changed 
 *  *buf++ = LEFT(datum,XXX) into 
 *  *buf++ = datum =  LEFT(datum,XXX) to force the result we want
 ****************************************************************/
#if defined(__alpha)
int rawread(ft_t ft, int *buf, long int nsamp)
#else 
int rawread(ft_t ft, long int *buf, long int nsamp)
#endif 

{
  int count;
#if defined(__alpha)
  register int datum;
#else
  register long datum;
#endif
  int done = 0;
  count=0;
  switch(ft->info.size) 
    {
    case BYTE: switch(ft->info.style) 
      {
      case SIGN2:
	while(done < nsamp) 
	  {
	    datum = getc(ft->fp);
	    if (feof(ft->fp))
	      return done;
	    /* scale signed up to long's range */
	    *buf++ = datum = LEFT(datum, 24);
	    done++;
	  }
	return done;
      case UNSIGNED:
	while(done < nsamp) 
	  {
	    datum = getc(ft->fp);
	    if (feof(ft->fp))
	      return done;
	    /* Convert to signed */
	    datum ^= 128;
	    /* scale signed up to long's range */
	    *buf++ = datum =  LEFT(datum, 24);
	    done++;
	  }
	return done;
      case ULAW:
	while(done < nsamp) 
	  {
	    datum = getc(ft->fp);
	    if (feof(ft->fp))
	      return done;
	    datum = st_ulaw_to_linear(datum);
	    /* scale signed up to long's range */
	    *buf++ = datum =  LEFT(datum, 16);
	    done++;
	  }
	return done;
      case ALAW:
	while(done < nsamp) 
	  {
	    datum = getc(ft->fp);
	    if (feof(ft->fp))
	      return done;
	    datum = st_Alaw_to_linear(datum);
	    /* scale signed up to long's range */
	    *buf++ = datum = LEFT(datum, 16);
	    done++;
	  }
	return done;
      }
    case WORD: 
      switch(ft->info.style) 
	{
	case SIGN2:
	  while(done < nsamp)
	    {
	      datum = rshort(ft);
	      if (feof(ft->fp))
		return done;
	      /* scale signed up to long's range */
	      *buf++ = datum =  LEFT(datum, 16);
	      /** if ( count < 5 ) {
		sciprint("datum %d %d %d \r\n",*(buf-1),sizeof(long),sizeof(int));
		count++;
		}
	      **/
	      done++;
	    }
	  return done;
	case UNSIGNED:
	  while(done < nsamp) 
	    {
	      datum = rshort(ft);
	      if (feof(ft->fp))
		return done;
	      /* Convert to signed */
	      datum ^= 0x8000;
	      /* scale signed up to long's range */
	      *buf++ = datum = LEFT(datum, 16);
	      done++;
	    }
	  return done;
	case ULAW:
	  sciprint("No U-Law support for shorts\r\n");
	  ft->ierr=1;
	  return done;
	case ALAW:
	  sciprint("No A-Law support for shorts\r\n");
	  ft->ierr=1;
	  return done;
	}
    case FLOAT:
      while(done < nsamp) 
	{
	  datum = rfloat(ft);
	  if (feof(ft->fp))
	    return done;
	  *buf++ = datum =  LEFT(datum, 16);
	  done++;
	}
      return done;
    default:
      sciprint("Drop through in rawread!\r\n");
      ft->ierr=1;
    }
  sciprint("Sorry, don't have code to read %s, %s\r\n",
	   styles[ft->info.style], sizes[ft->info.size]);
  ft->ierr=1;
  return done;
}


/****************************************************************
 * Convert the sox internal signed long format 
 * to the raw file data, and write it.
 ****************************************************************/

void rawwrite(ft_t ft, long int *buf, long int nsamp)
{
  register int datum;
  int done = 0;
  switch(ft->info.size) 
    {
    case BYTE: switch(ft->info.style) 
      {
      case SIGN2:
	while(done < nsamp) 
	  {
	    /* scale signed up to long's range */
	    datum = RIGHT(*buf++, 24);
	    putc(datum, ft->fp);
	    done++;
	  }
	return;
      case UNSIGNED:
	while(done < nsamp) 
	  {
	    /* scale signed up to long's range */
	    datum = RIGHT(*buf++, 24);
	    /* Convert to unsigned */
	    datum ^= 128;
	    putc(datum, ft->fp);
	    done++;
	  }
	return;
      case ULAW:
	while(done < nsamp) 
	  {
	    /* scale signed up to long's range */
	    datum = RIGHT(*buf++, 16);
	    /* round up to 12 bits of data */
	    datum += 0x8;	/* + 0b1000 */
	    datum = st_linear_to_ulaw(datum);
	    putc(datum, ft->fp);
	    done++;
	  }
	return;
      case ALAW:
	while(done < nsamp) 
	  {
	    /* scale signed up to long's range */
	    datum = RIGHT(*buf++, 16);
	    /* round up to 12 bits of data */
	    datum += 0x8;	/* + 0b1000 */
	    datum = st_linear_to_Alaw(datum);
	    putc(datum, ft->fp);
	    done++;
	  }
	return;
      }
    case WORD: switch(ft->info.style) {
    case SIGN2:
      while(done < nsamp) {
	/* scale signed up to long's range */
	datum = RIGHT(*buf++, 16);
	wshort(ft, datum);
	done++;
      }
      return;
    case UNSIGNED:
      while(done < nsamp) {
	/* scale signed up to long's range */
	datum = RIGHT(*buf++, 16);
	/* Convert to unsigned */
	datum ^= 0x8000;
	wshort(ft, datum);
	done++;
      }
      return;
    case ULAW:
      sciprint("No U-Law support for shorts (try -b option ?)\r\n");
      ft->ierr=1;
      return;
    case ALAW:
      sciprint("No A-Law support for shorts (try -b option ?)\r\n");
      ft->ierr=1;
      return;
    }
    case FLOAT:
      while(done < nsamp) {
	/* scale signed up to long's range */
	datum = RIGHT(*buf++, 16);
	wfloat(ft, (double) datum);
	done++;
      }
      return;
    default: 
      {
	sciprint("Drop through in rawwrite!\r\n");
	ft->ierr=1;
      }
      
    }
  sciprint("Sorry, don't have code to write %s, %s\r\n",
	   styles[ft->info.style], sizes[ft->info.size]);
  ft->ierr=1;
}







