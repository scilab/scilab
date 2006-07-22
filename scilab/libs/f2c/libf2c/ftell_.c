#include "f2c.h"
#include "fio.h"

 static FILE *
#ifdef KR_headers
unit_chk(unit, who) integer unit; char *who;
#else
unit_chk(integer unit, char *who)
#endif
{
	if (unit >= MXUNIT || unit < 0)
		f__fatal(101, who);
	return f__units[unit].ufd;
	}

 integer
#ifdef KR_headers
ftell_(unit) integer *unit;
#else
ftell_(integer *unit)
#endif
{
	FILE *f;
	return (f = unit_chk(*unit, "ftell")) ? ftell(f) : -1L;
	}

 int
#ifdef KR_headers
fseek_(unit, offset, whence) integer *unit, *offset, *whence;
#else
fseek_(integer *unit, integer *offset, integer *whence)
#endif
{
	FILE *f;
	return	!(f = unit_chk(*unit, "fseek"))
		|| fseek(f, *offset, (int)*whence) ? 1 : 0;
	}
