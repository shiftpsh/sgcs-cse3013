#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

// primitive type
#define DR_LINE		0
#define DR_BOX		1
#define DR_SOLIDBOX	2
#define DR_HATCHBOX	3

typedef struct EXAMPLE
{
	short type;
	short hatch;
	int lineWidth;
	unsigned int lineColor;
	unsigned int fillColor;
	double xmin, ymin, xmax, ymax;
	EXAMPLE* next;
} EXAMPLE;

// global variable
extern EXAMPLE* g_pHeader;
extern double g_xMin, g_xMax, g_yMin, g_yMax;

// function prototype
extern bool loadExam(const char* pszFileName);
extern void freeExam();
extern void changeColor();
extern bool saveExam(const char* pszFileName);

#endif	// __EXAMPLE_H__