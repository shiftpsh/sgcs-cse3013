#include <stdio.h>
#include <assert.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "..\Util\StringTokenizer.h"
#include "Example.h"

EXAMPLE* g_pHeader = NULL;
double g_xMin = DBL_MAX, g_xMax = DBL_MIN, g_yMin = DBL_MAX, g_yMax = DBL_MIN;

static void getLine(CStringTokenizer& tokenizer);
static void getBox(CStringTokenizer& tokenizer);
static void getSolidBox(CStringTokenizer& tokenizer);
static void getHatchBox(CStringTokenizer& tokenizer);

bool loadExam(const char* pszFileName)
{
	assert(pszFileName);
	CStringTokenizer tokenizer;
	const char* str;

	if (tokenizer.openFile(pszFileName) == false)
		return false;

	while (tokenizer.hasNext())
	{
		str = tokenizer.getNext();
		if (strcmp(str, "LINE") == 0)
			getLine(tokenizer);
		else if (strcmp(str, "BOX") == 0)
			getBox(tokenizer);
		else if (strcmp(str, "SOLIDBOX") == 0)
			getSolidBox(tokenizer);
		else if (strcmp(str, "HATCHBOX") == 0)
			getHatchBox(tokenizer);
		else
		{
			tokenizer.closeFile();
			return false;
		}
	}

	return true;
}

bool saveExam(const char* pszFileName)
{
	char str[200];
	EXAMPLE* exam = g_pHeader;
	FILE* fp;// = fopen(pszFileName, "wt");
	if (0 != fopen_s(&fp,pszFileName,"wt"))
		return false;

	while (exam)
	{
		switch(exam->type)
		{
		case DR_LINE:
			sprintf_s(str, "LINE %f %f %f %f %d 0x%08X\n", exam->xmin, exam->ymin, exam->xmax, exam->ymax, exam->lineWidth, exam->lineColor);
			fputs(str, fp);
			break;
		case DR_BOX:
			sprintf_s(str, "BOX %f %f %f %f %d 0x%08X\n", exam->xmin, exam->ymin, exam->xmax, exam->ymax, exam->lineWidth, exam->lineColor);
			fputs(str, fp);
			break;
		case DR_SOLIDBOX:
			sprintf_s(str, "SOLIDBOX %f %f %f %f %d 0x%08X 0x%08X\n", exam->xmin, exam->ymin, exam->xmax, exam->ymax, exam->lineWidth, exam->lineColor, exam->fillColor);
			fputs(str, fp);
			break;
		case DR_HATCHBOX:
			sprintf_s(str, "HATCHBOX %f %f %f %f %d 0x%08X 0x%08X %d\n", exam->xmin, exam->ymin, exam->xmax, exam->ymax, exam->lineWidth, exam->lineColor, exam->fillColor, exam->hatch);
			fputs(str, fp);
			break;
		}
		exam = exam->next;
	}
	fclose(fp);
	return true;
}

void freeExam()
{
	EXAMPLE* tmp;

	while (g_pHeader)
	{
		tmp = g_pHeader;
		g_pHeader = g_pHeader->next;
		free(tmp);
	}
	g_pHeader = NULL;
}

void changeColor()
{
	EXAMPLE* exam = g_pHeader;

	srand((unsigned int)time(NULL));
	while (exam)
	{
		exam->lineColor = int((rand() / double(RAND_MAX)) * 0x00FFFFFF);
		exam->fillColor = int((rand() / double(RAND_MAX)) * 0x00FFFFFF);
		exam->hatch = int((rand() / double(RAND_MAX)) * 6);
		exam = exam->next;
	}
}

static void getLine(CStringTokenizer& tokenizer)
{
	EXAMPLE* exam = (EXAMPLE*) calloc(1, sizeof(EXAMPLE));
	exam->type = DR_LINE;
	exam->xmin = atof(tokenizer.getNext());
	exam->ymin = atof(tokenizer.getNext());
	exam->xmax = atof(tokenizer.getNext());
	exam->ymax = atof(tokenizer.getNext());
	exam->lineWidth = atoi(tokenizer.getNext());
	sscanf_s(tokenizer.getNext(), "%x", &(exam->lineColor));
	exam->next = g_pHeader;
	g_pHeader = exam;
	if (exam->xmin < g_xMin)
		g_xMin = exam->xmin;
	if (exam->ymin < g_yMin)
		g_yMin = exam->ymin;
	if (exam->xmax > g_xMax)
		g_xMax = exam->xmax;
	if (exam->ymax > g_yMax)
		g_yMax = exam->ymax;
}

static void getBox(CStringTokenizer& tokenizer)
{
	EXAMPLE* exam = (EXAMPLE*) calloc(1, sizeof(EXAMPLE));
	exam->type = DR_BOX;
	exam->xmin = atof(tokenizer.getNext());
	exam->ymin = atof(tokenizer.getNext());
	exam->xmax = atof(tokenizer.getNext());
	exam->ymax = atof(tokenizer.getNext());
	exam->lineWidth = atoi(tokenizer.getNext());
	sscanf_s(tokenizer.getNext(), "%x", &(exam->lineColor));
	exam->next = g_pHeader;
	g_pHeader = exam;
	if (exam->xmin < g_xMin)
		g_xMin = exam->xmin;
	if (exam->ymin < g_yMin)
		g_yMin = exam->ymin;
	if (exam->xmax > g_xMax)
		g_xMax = exam->xmax;
	if (exam->ymax > g_yMax)
		g_yMax = exam->ymax;
}

static void getSolidBox(CStringTokenizer& tokenizer)
{
	EXAMPLE* exam = (EXAMPLE*) calloc(1, sizeof(EXAMPLE));
	exam->type = DR_SOLIDBOX;
	exam->xmin = atof(tokenizer.getNext());
	exam->ymin = atof(tokenizer.getNext());
	exam->xmax = atof(tokenizer.getNext());
	exam->ymax = atof(tokenizer.getNext());
	exam->lineWidth = atoi(tokenizer.getNext());
	sscanf_s(tokenizer.getNext(), "%x", &(exam->lineColor));
	sscanf_s(tokenizer.getNext(), "%x", &(exam->fillColor));
	exam->next = g_pHeader;
	g_pHeader = exam;
	if (exam->xmin < g_xMin)
		g_xMin = exam->xmin;
	if (exam->ymin < g_yMin)
		g_yMin = exam->ymin;
	if (exam->xmax > g_xMax)
		g_xMax = exam->xmax;
	if (exam->ymax > g_yMax)
		g_yMax = exam->ymax;
}

static void getHatchBox(CStringTokenizer& tokenizer)
{
	EXAMPLE* exam = (EXAMPLE*) calloc(1, sizeof(EXAMPLE));
	exam->type = DR_HATCHBOX;
	exam->xmin = atof(tokenizer.getNext());
	exam->ymin = atof(tokenizer.getNext());
	exam->xmax = atof(tokenizer.getNext());
	exam->ymax = atof(tokenizer.getNext());
	exam->lineWidth = atoi(tokenizer.getNext());
	sscanf_s(tokenizer.getNext(), "%x", &(exam->lineColor));
	sscanf_s(tokenizer.getNext(), "%x", &(exam->fillColor));
	exam->hatch = atoi(tokenizer.getNext());
	exam->next = g_pHeader;
	g_pHeader = exam;
	if (exam->xmin < g_xMin)
		g_xMin = exam->xmin;
	if (exam->ymin < g_yMin)
		g_yMin = exam->ymin;
	if (exam->xmax > g_xMax)
		g_xMax = exam->xmax;
	if (exam->ymax > g_yMax)
		g_yMax = exam->ymax;
}
