/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <memory.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.
    
	
	if((NULL == supertab) || (NULL == filetab) || (isbadfd(fd)))
		return SYSERR;

	wait(supertab->sb_dirlock);

/*	if(SYSERR ==*/free((void *)filetab[fd].fn_data);
		//return SYSERR;

	
	filetab[fd].fn_state = FILE_FREE;

	seek(DISK0, supertab->sb_dirlst->db_blocknum);
	 
	if(SYSERR == write(DISK0, supertab->sb_dirlst, sizeof(struct dirblock)))
	{
		signal(supertab->sb_dirlock);
		return SYSERR;
	}	

	signal(supertab->sb_dirlock);

	sbFreeBlock(supertab, filetab[fd].fn_blocknum);
		

	filetab[fd].fn_blocknum = 0;
	
	return OK;
}
