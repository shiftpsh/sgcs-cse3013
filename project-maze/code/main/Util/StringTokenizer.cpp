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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "StringTokenizer.h"

void CStringTokenizer::prepareNext()
{
	int i;
	// go to next token
	do
	{
		while (m_nBufIdx < STRING_BUFFER_SIZE && isspace(m_cBuffer[m_nBufIdx]))
			m_nBufIdx++;
		// load next line if needed
		if (m_nBufIdx >= STRING_BUFFER_SIZE || '\0' == m_cBuffer[m_nBufIdx])
		{
			// check if EOF
			if (NULL == fgets(m_cBuffer, STRING_BUFFER_SIZE, m_pFile))
			{
				m_cNext[0] = '\0';
				return;
			}
			m_nBufIdx = 0;
		}
	} while (isspace(m_cBuffer[m_nBufIdx]));

	for (i = 0; m_nBufIdx < STRING_BUFFER_SIZE && isspace(m_cBuffer[m_nBufIdx]) == 0; m_nBufIdx++, i++)
		m_cNext[i] = m_cBuffer[m_nBufIdx];
	m_cNext[i] = '\0';
}
