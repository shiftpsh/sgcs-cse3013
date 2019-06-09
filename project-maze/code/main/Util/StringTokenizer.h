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

#ifndef __STRING_TOKENIZER_H__
#define __STRING_TOKENIZER_H__

#define STRING_BUFFER_SIZE		128
#define MAX_TOKEN_SIZE			64

class CStringTokenizer
{
public:
	CStringTokenizer() : m_pFile(NULL) {};
	~CStringTokenizer() { if (m_pFile) fclose(m_pFile); };

	void initialize()
	{ m_nBufIdx = 0; m_cBuffer[0] = '\0'; m_cToken[0] = '\0'; };

	bool openFile(const char* pszFileName)
	{
		
			if (0 != fopen_s(&m_pFile,pszFileName, "rt"))
				return false;
			initialize();
			prepareNext();
			return true;
	};

	void closeFile()
	{ fclose(m_pFile); m_pFile = NULL; };

	bool hasNext()
	{ return (m_cNext[0] != '\0'); };

	const char* getNext()
	{
		if (m_cNext[0] == '\0') return NULL;
		strcpy_s(m_cToken, m_cNext);
		prepareNext();
		return m_cToken;
	};

	void prepareNext();

public:
	FILE* m_pFile;
	int m_nBufIdx;
	char m_cBuffer[STRING_BUFFER_SIZE];
	char m_cToken[MAX_TOKEN_SIZE];
	char m_cNext[MAX_TOKEN_SIZE];
};

#endif	// __STRING_TOKENIZER_H__