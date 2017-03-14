/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <device.h>
#include <memory.h>
#include <disk.h>
#include <file.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.
   
	struct freeblock *freeblk, *free2;
	struct dentry *phw;
	int result;
	int diskfd;

	

	if(NULL == psuper)
	{
		return SYSERR;
	}

	phw = psuper->sb_disk;
	if(NULL == phw)
	{
		return SYSERR;
	} 

	diskfd = phw - devtab;	
	freeblk = psuper->sb_freelst;
	

	wait(psuper->sb_freelock);
	if(NULL == freeblk)
	{
		//kprintf("Case 1\r\n");
		freeblk = (struct freeblock *)malloc(sizeof(struct freeblock));
		freeblk->fr_count = 0;
		freeblk->fr_free[freeblk->fr_count] = 0;
		freeblk->fr_next = NULL;
		freeblk->fr_blocknum = block;
		seek(diskfd, freeblk->fr_blocknum);
		if(SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
		{
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		psuper->sb_freelst = (struct freeblock *)freeblk->fr_blocknum;
		seek(diskfd, psuper->sb_blocknum);
           	 if (SYSERR == write(diskfd, psuper, sizeof(struct superblock)))
            	{
			signal(psuper->sb_freelock);
             	  	return SYSERR;
            	}
		psuper->sb_freelst = freeblk;
		signal(psuper->sb_freelock);
		return OK;
	}

	while(freeblk->fr_next != NULL) 
	{
		freeblk = freeblk->fr_next;
	}

	
	if(freeblk == psuper->sb_freelst && freeblk->fr_count == 0)
	{
		//kprintf("Case 4\r\n");
                free2 = (struct freeblock *)malloc(sizeof(struct freeblock));
                free2->fr_count = 0;
                free2->fr_free[free2->fr_count] = 0;
                free2->fr_next = NULL;
                free2->fr_blocknum = block;
                seek(diskfd, free2->fr_blocknum);
                if(SYSERR == write(diskfd, free2, sizeof(struct freeblock)))
                {
                        signal(psuper->sb_freelock);
                        return SYSERR;
                }
                freeblk->fr_next = (struct freeblock *)free2->fr_blocknum;
                seek(diskfd, freeblk->fr_blocknum);
                if(SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
                {
                        signal(psuper->sb_freelock);
                        return SYSERR;
                }

                freeblk->fr_next = free2;
                signal(psuper->sb_freelock);
                return OK;


	}
	else if(freeblk->fr_count < FREEBLOCKMAX)
	{
		//kprintf("Case 2\r\n");
		freeblk->fr_free[freeblk->fr_count] = block;
		freeblk->fr_count++;
		free2 = freeblk->fr_next;
       		
            	freeblk->fr_next = 0;
	/*	if (NULL == freeblk->fr_next)
        	{
        	}*/
        	
	//	else
        //	{
            	//	freeblk->fr_next =
                //		(struct freeblock *)freeblk->fr_next->fr_blocknum;
        //	}
        	seek(diskfd, freeblk->fr_blocknum);
        	if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
        	{
			signal(psuper->sb_freelock);
	            	return SYSERR;
        	}
        	freeblk->fr_next = free2;
		signal(psuper->sb_freelock);
		return OK;
	}
	else
	{
		//kprintf("Case 3\r\n");
		free2 = (struct freeblock *)malloc(sizeof(struct freeblock));
		free2->fr_count = 0;
                free2->fr_free[free2->fr_count] = 0;
                free2->fr_next = NULL;
                free2->fr_blocknum = block;
                seek(diskfd, free2->fr_blocknum);
                if(SYSERR == write(diskfd, free2, sizeof(struct freeblock)))
                {
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		freeblk->fr_next = (struct freeblock *)free2->fr_blocknum;
		seek(diskfd, freeblk->fr_blocknum);
		if(SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
		{
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		
		freeblk->fr_next = free2;
		signal(psuper->sb_freelock);
		return OK;

	}	
	signal(psuper->sb_freelock);
 	return SYSERR;
}
