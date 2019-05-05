#ifndef __DEFINE_H__
#define __DEFINE_H__

#define MAX	1024

#define gXmin		100
#define gYmin		100
#define gXmax		600
#define gYmax		500
#define wWeight		gXmax + gXmin
#define wHeight		gYmax + gYmin + gYmin/2

#define lineSize	20
#define cirsize		2

typedef struct Line__{
	int xl, yl, xr, yr;
} Line;

typedef struct Point__ {
	float x, y;
} Point;

#endif
