#include <stdlib.h>
#include "..\ExternDoc.h"
#include "..\UI\Zoom.h"
#include "..\UI\MsgView.h"
#include "..\Graphics\DrawFunc.h"
#include "..\Example\Example.h"

#define ERROR_NUMBER -1

//function prototype

static void drawDirect(CDC *pDC);
static void drawBuffered();

//Start of user code
#include <float.h>
#include <algorithm>
#include <vector>

#define CELL_SIZE 6
#define WALL_WIDTH 1

/**
 * maze data.
 * '+', '-', '|' s are walls and ' ' is path.
 * instead of maintaining horizontal and vertical paths, we can
 * start from (1, 1) and just traverse this maze by adjacent cells.
 */
std::vector<std::vector<char>> field;
int cols, rows;

/*****************************************************************
* function	: bool readFile(const char* filename)
* argument	: cons char* filename - filename to be opened
* return	: true if success, otherwise flase
* remark	: After read data file, phisycal view must be set;
*			  otherwise drawing will not be executed correctly.
*			  The window will be invalidated after readFile()
*			  returns true.
******************************************************************/
bool readFile(const char* filename){
	char buf;
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) return false;

	while (true) {
		std::vector<char> line;
		while (buf = fgetc(fp)) {
			if (buf == '\n') break;
			if (buf == EOF) break;
			line.emplace_back(buf);
		}
		field.emplace_back(line);
		if (buf == EOF) break;
	}
	fclose(fp);

	rows = field.size();
	cols = field[0].size();
	
	// maze has floor(rows / 2) calls and floor(rows / 2) + 1 walls vertically;
	// floor(cols / 2) calls and floor(cols / 2) + 1 walls horizontally.
	setWindow(0, 0,
		CELL_SIZE * (cols / 2) + WALL_WIDTH * (cols - cols / 2),
		CELL_SIZE * (rows / 2) + WALL_WIDTH * (rows - rows / 2),
		1);

	return true;
}

/******************************************************************
* function	: void freeMemory()
*
* remark	: Free dynamically allocated memory
*******************************************************************/
void freeMemory(){
	cols = 0;
	rows = 0;
	field.clear();
}

/**************************************************************
* function	: bool writeFile(const char* filename)
*
* argument	: const char* filename - filename to be written
* return	: true if success, otherwise false
* remark	: Save user data to a file
****************************************************************/
bool writeFile(const char* filename){
	//start of the user code
	bool flag;
	flag = 0;

	return flag;
	//end of usercode
}

/************************************************************************
* fucntion	: void drawMain(CDC* pDC)
*
* argument	: CDC* pDC - device context object pointer
* remark	: Main drawing function. Called by CMFC_MainView::OnDraw()
*************************************************************************/
void drawMain(CDC *pDC){
	//if direct drawing is defined
#if defined(GRAPHICS_DIRECT)
	drawDirect(pDC);
	//if buffered drawing is defined
#elif defined(GRAPHICS_BUFFERED)
	drawBuffered();
#endif
}

/************************************************************************
* function	: static void drawDirect(CDC *pDC
*
* argument	: CDC* pDC - device context object pointer
* remark	: Direct drawing routines here.
*************************************************************************/
static void drawDirect(CDC *pDC){
	//begin of user code
	//Nothing to write currently.
	//end of user code
}

/***********************************************************************
* function	: static void drawBuffered()
*
* argument	: CDC* pDC -0 device object pointer
* remark	: Buffered drawing routines here.
************************************************************************/
static void drawBuffered(){
	// COLORREF cyan = RGB(0x00, 0xbc, 0xd4);
	// COLORREF violet = RGB(0x9c, 0x27, 0xb0);

	for (int i = 0; i < rows; i++) {
		int x = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2);
		for (int j = 0; j < cols; j++) {
			int y = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2);

			// this adds gradients just for fun
			double blend = (double) (i + j) / (rows + cols);
			COLORREF color = RGB(
				0x00 * (1 - blend) + 0x9c * blend,
				0xbc * (1 - blend) + 0x27 * blend,
				0xd4 * (1 - blend) + 0xb0 * blend
				);

			switch (field[i][j]) {
				case '+':
					DrawSolidBox_I(y, x,
						y + WALL_WIDTH, x + WALL_WIDTH,
						0, color, color);
					break;
				case '-':
					DrawSolidBox_I(y, x,
						y + CELL_SIZE, x + WALL_WIDTH,
						0, color, color);
					break;
				case '|':
					DrawSolidBox_I(y, x,
						y + WALL_WIDTH, x + CELL_SIZE,
						0, color, color);
					break;
			}
		}
	}
}

void DFS() {

}

void BFS() {

}