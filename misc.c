#include "misc.h"

void long2string (long nr, char *s)
{
  char c, i, imax, imax2, i2;

  i2 = 0; i = 0;
  if (nr == 0) s[i++] = '0';
  if (nr < 0) {i2 = 1; nr = 0 - nr; }
    
  while (nr>0)
  {
    c = nr % 10;
    nr = nr / 10;
    s[i++] = '0' + c;
  }
  if (i2) s[i++] = '-';
  s[i] = 0;       
  
  imax = i;  
  imax2 = imax/2;
  for (i=0; i<imax2; ++i) {
    i2 = imax-i-1;
    c = s[i];
    s[i] = s[i2];
    s[i2] = c;
  }
}

U8 addchar (char *s, char c)
{
  U8 i;
  i = strlen (s);
  s[i] = c; s[i+1] = 0;
  return i+1;
}

void add_point (char *s, U8 dec)
{
  U8 i, nr, i2=0;
  nr = strlen (s);

  if (!dec || !nr) return;
   
  // insert missing '0'
  if (nr <= dec) // 0 must added
  {
    i2 = dec-nr+1; 
    for (i=nr+i2; i>=i2; --i) s[i] = s[i-i2];  // shift
    for (i=0; i<i2; ++i) s[i] = '0';           // add zeros
  }

  // insert point
  nr = strlen (s);
  for (i=nr+1; i>nr-dec; --i) s[i] = s[i-1];
  s[i] = '.';
}

void format_volt (U16 volt, char *s, U8 dec, char unit)
{
  long2string (volt, s);
  add_point (s, dec);
  addchar (s, unit);
}

