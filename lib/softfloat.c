#include <inttypes.h>
#include <stddef.h>

uint64_t __udivmoddi4(uint64_t num,
                      uint64_t den,
                      uint64_t *rem_p)
{
  uint64_t quot = 0, qbit = 1;

  if ( den == 0 ) {
    return 1/((unsigned)den); /* Intentional divide by zero, without
                                 triggering a compiler warning which
                                 would abort the build */
  }

  /* Left-justify denominator and count shift */
  while ( (int64_t)den >= 0 ) {
    den <<= 1;
    qbit <<= 1;
  }

  while ( qbit ) {
    if ( den <= num ) {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }

  if ( rem_p )
    *rem_p = num;

  return quot;
}

int64_t __divdi3(int64_t num, int64_t den)
{
  int minus = 0;
  int64_t v;

  if ( num < 0 ) {
    num = -num;
    minus = 1;
  }
  if ( den < 0 ) {
    den = -den;
    minus ^= 1;
  }

  v = __udivmoddi4(num, den, NULL);
  if ( minus )
    v = -v;

  return v;
}

int64_t __moddi3(int64_t num, int64_t den)
{
  int minus = 0;
  int64_t v = 0;

  if ( num < 0 ) {
    num = -num;
    minus = 1;
  }
  if ( den < 0 ) {
    den = -den;
    minus ^= 1;
  }

  (void) __udivmoddi4(num, den, (uint64_t *)&v);
  if ( minus )
    v = -v;

  return v;
}

uint64_t __udivdi3(uint64_t num, uint64_t den)
{
  return __udivmoddi4(num, den, NULL);
}

uint64_t __umoddi3(uint64_t num, uint64_t den)
{
  uint64_t v = 0;

  (void) __udivmoddi4(num, den, &v);
  return v;
}
