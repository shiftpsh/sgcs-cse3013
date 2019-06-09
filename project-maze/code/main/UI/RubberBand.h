/**************************************************************************
***    
***  Copyright (c) 2000 Design Automation Lab at the Sogang University,
***
***  Affiliation:   DA Lab, Sogang University, Computer Science Department,
***                 Seoul, Korea
***
***  Permission is hereby granted, free of charge, to any person obtaining 
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation 
***  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
***  and/or sell copies of the Software, and to permit persons to whom the 
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/

#ifndef __RUBBER_BAND_H__
#define __RUBBER_BAND_H__

// rubber band type
#define RBT_DEFAULT		0
#define RBT_ZOOM		1
#define RBT_RESIZE		2

// rubber band status
#define RBS_BEGIN		0
#define RBS_LOOP		1
#define RBS_END			2

// rubber band drag information
#define RBD_DEFAULT		0
#define RBD_ZI			1
#define RBD_ZO			2

// cursor position index
#define RB_POS_UKN			0
#define RB_POS_TL			1
#define RB_POS_TR			2
#define RB_POS_BR			3
#define RB_POS_BL			4
#define RB_POS_LE			5
#define RB_POS_RE			6
#define RB_POS_TE			7
#define RB_POS_BE			8

#define RB_NO_DRAWN			0
#define RB_IS_DRAWN			1
#define RB_WAS_DRAWN		2

// global variables
extern CRect g_rcRB;
extern CRect g_rcPrevRB;
extern int g_nResizePos;
extern int g_nDrawnRB;

// global functions
extern bool initRB(CWnd* pWnd);
extern void clearRB();
extern int beginRB(const POINT& pt, short type = RBT_DEFAULT);
extern int endRB(const POINT& pt);
extern void endResizableRB();
extern void redoRB();
extern void dragRB(const POINT& pt);
extern void drawRB();

#endif	// __RUBBER_BAND_H__