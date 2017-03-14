/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
	//TODO: Test your O/S.
    	printf("This is where you should put some testing code.\n");	  
	int fd[100]; 
	int x;
	for(x = 0; x < 62; x++)
	{
		
		fd[x] = sbGetBlock(supertab);
		//sbFreeBlock(supertab, fd);
	//	xsh_diskstat(0,NULL);
	}
	//xsh_diskstat(0, NULL);
	//sbFreeBlock(supertab, fd); 
	xsh_diskstat(0, NULL);
	

	for(x = 0; x<62; x++)
	{
		sbFreeBlock(supertab, fd[x]);
	}
	xsh_diskstat(0,NULL);
	return OK;
}
