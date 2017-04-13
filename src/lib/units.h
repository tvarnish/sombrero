// units.h
// Definitions of common values and units

#ifndef __UNITS_H_INCLUDED__
#define __UNITS_H_INCLUDED__

// Real World Units taken from (https://www.iau.org/public/themes/measuring/)
#define AU 149597870700 // Astronomical Unit - Average Earth-Sun Distance
#define PC 3.0857e12 // Parsec
#define MPC 1.0e6 * PC // Megaparsec
// A Parsec is the distance to a star that subtends 1 arcsecond, at an arc length of 1 AU.

#define MINUTE 60 // 1 Minute in seconds
#define HOUR 3600 // 1 Hour in seconds
#define DAY 86400 // 1 Day in seconds
#define YR (365.25 * DAY) // 1 Year in seconds

#define M 1.9891e30 // Mass of the Sun
#define GR 6.67e-11 // Gravitational constant, GR because of image.h
#define PI 3.1415926 // Pi

#endif
