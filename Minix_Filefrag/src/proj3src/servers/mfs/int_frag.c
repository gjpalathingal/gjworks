#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioc_memory.h>
#include <sys/svrctl.h>
#include <sys/select.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/keymap.h>
#include <minix/const.h>
#include <minix/endpoint.h>
#include <minix/u64.h>
#include <minix/bdev.h>
#include "buf.h"
#include "inode.h"

#include "super.h"
#include "super.h"
#include "const.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <assert.h> 


#define TRUE  1
#define FALSE 0
#define OK    0
#define DEBUG TRUE
#define DEBUG TRUE

#define ERROR_FILE_NOT_PRESENT 		-1
#define ERROR_DIRECTORY_NOT_PRESENT     -2


int grsIntFrag(void);
void print_internal_frag_report(void);
unsigned int get_internal_frag(zone_t,int);

unsigned int get_internal_filefrag(zone_t grs_zone,int num);
int grsIntFragFile();

extern dev_t fs_dev;

unsigned int max_internal_frag = 0;
unsigned int min_internal_frag = 0;
unsigned int avg_internal_frag = 0;


void print_internal_frag_report()
{
  printf("--------- INTERNAL FRAGMENTATION REPORT--------\n");
  printf("MINIMUM INTERNAL FRAGMENTATION : %d\n",max_internal_frag);
  printf("MAXIMUM INTERNAL FRAGMENTATION : %d\n",min_internal_frag);
  printf("AVERAGE INTERNAL FRAGMENTATION : %d\n",avg_internal_frag);
}


unsigned int get_internal_filefrag(zone_t grs_zone,int num)
{
 short j = 0;
 unsigned int frag = 0;
 block_t b;
 struct buf* bp1;


 b = (block_t)grs_zone << num;
 for(j=b;b< b+num;b++)
 {
	bp1 = get_block(fs_dev,b,NORMAL);
	if(bp1->b_dev == NO_DEV)
	{
		frag++;
	}
 }

 return frag;
}

int grsIntFragFile()
{
    printf("\n======================================================\n");
    printf("\n System Call Invoked : filefraginternal_file \n");

    struct inode *rip ;
    struct stat sb;    
    block_t b;
   struct buf *bp;
    //char *fname="test.c";
    long r;

    int d=0, j=0, count =0, frag_cnt =0, scale =0;

   char _fname[20] = {0};

sys_datacopy(SELF, (vir_bytes) fs_m_in.m1_p1, MFS_PROC_NR, (vir_bytes) _fname, (phys_bytes) fs_m_in.m1_i3);

    if (stat(_fname, &sb) == -1) {
        printf("file not found");
        return -1;
    }

   r = (long) sb.st_ino;
     printf("r => %ld ",r);

   rip = get_inode(fs_dev,r);
	scale = rip->i_sp->s_log_zone_size;
	for(j=0;j<7;j++)
	{
	    b = (block_t)rip->i_zone[7] << scale;
	    bp = get_block(rip->i_dev,b,NORMAL);
	    count = (rip->i_sp->s_block_size)/32;
	    for(d=0;d<count;d++)
	    {
		if(bp->b_v1_ind[d] == NO_ZONE)
		{
			frag_cnt = frag_cnt + get_internal_filefrag(bp->b_v1_ind[d],scale);
		}
	    }		
	  }

     printf("\n Internal file fragmentation : %d ",frag_cnt);	
     
    printf("\n======================================================\n");
 return 0;
}


int grsIntFragDir()
{
    printf("\n======================================================\n");
    printf("\n System Call Invoked : filefraginternal_dir \n");
    printf("\n======================================================\n");
    return 0;

}

unsigned int get_internal_frag(zone_t grs_zone,int num)
{
 short j = 0;
 unsigned int frag = 0;
 block_t b;
 struct buf* bp1;

 b = (block_t)grs_zone << num;
 for(j=b;b< b+num;b++)
 {
	bp1 = get_block(fs_dev,b,NORMAL);
	if(bp1->b_dev == NO_DEV)
	{
		frag++;
	}
 }
 if(frag > 0)
 {
	printf("Internal Fragment detected\n");
	if(max_internal_frag == 0 | frag < max_internal_frag)
	min_internal_frag = frag;
	if(min_internal_frag == 0 | frag > min_internal_frag)
	max_internal_frag = frag;
	avg_internal_frag =(max_internal_frag+min_internal_frag)/2 ;
 }
 return frag;
}

int grsIntFrag()
{

  printf("\n======================================================\n");
    printf("\n System Call Invoked : grsIntFrag \n");

  struct super_block* sp;
  unsigned int in_numbs[100] = {0};
  struct inode *rip;
  block_t start_block;		/* first bit block */
  block_t block;
  bit_t map_bits;		/* how many bits are there in the bit map? */
  short bit_blocks;		/* how many blocks are there in the bit map? */
  bit_t origin = 1;			/* number of bit to start searching at */
  int c = 0, n = 0, d = 0, count =0, j=0, frag_cnt =0 , scale =0;

  unsigned word, bcount;
  struct buf *bp;
  bitchunk_t *wptr, *wlim, k;
  bit_t i, b;
  
  sp = get_super(fs_dev);
  c = read_super(sp);  
  printf("The block sz currently in use : %d\n",sp->s_block_size);
  if(sp->s_log_zone_size == 0)
  {
	printf("One block/zone on this filesystem. So no external fragmentation\n");
	return 0;
  }
  start_block = START_BLOCK;
  map_bits = (bit_t) (sp->s_ninodes + 1);
  bit_blocks = sp->s_imap_blocks;
  
    /* Locate the starting place. */
  block = (block_t) (origin / FS_BITS_PER_BLOCK(sp->s_block_size));
  word = (origin % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
  
  bcount = bit_blocks;
while(bcount > 0)  
{
    bp = get_block(sp->s_dev, start_block + block, NORMAL);
   assert(bp);
    wlim = &bp->b_bitmap[FS_BITMAP_CHUNKS(sp->s_block_size)];

    /* Iterate over the words in block. */
    for (wptr = &bp->b_bitmap[word]; wptr < wlim; wptr++) 
    {

      /* Does this word contain a free bit? */
      if (*wptr == (bitchunk_t) 0) continue;
	  
	  k = (bitchunk_t) conv4(sp->s_native, (int) *wptr);

      for (i = 0; i < 8*sizeof(k); ++i) 
      {
        /* Bit number from the start of the bit map. */
        b = ((bit_t) block * FS_BITS_PER_BLOCK(sp->s_block_size))
            + (wptr - &bp->b_bitmap[0]) * FS_BITCHUNK_BITS
            + i;

        /* Don't count bits beyond the end of the map. */
        if (b >= map_bits) {
          break;
        } 
        if ((k & (1 << i)) == 1) {
          in_numbs[n++] = b;
        }
      }	
      if (b >= map_bits) break;
   }
   ++block;
   word = 0;
   --bcount;	
}

  for(n=0;in_numbs[n]!=0;n++)
  {
	rip = get_inode(fs_dev, in_numbs[n]);
	scale = rip->i_sp->s_log_zone_size;
	for(j=0;j<7;j++)
	{
	    b = (block_t)rip->i_zone[7] << scale;
	    bp = get_block(rip->i_dev,b,NORMAL);
	    count = (rip->i_sp->s_block_size)/32;
	    for(d=0;d<count;d++)
	    {
		if(bp->b_v1_ind[d] == NO_ZONE)
		{
			frag_cnt = get_internal_frag(bp->b_v1_ind[d],scale);
		}
	    }		
	  }
  }
  print_internal_frag_report();
  return 0;

  printf("\n======================================================\n");
}
