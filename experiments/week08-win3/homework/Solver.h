#ifndef _SOLVER_
#define _SOLVER_

extern void Read_File(const char* filename);
extern char* Check_Tree(int tc);
extern void Init();
extern void Error_Exit(char* s);

extern int currCase;
extern int cases;

#endif