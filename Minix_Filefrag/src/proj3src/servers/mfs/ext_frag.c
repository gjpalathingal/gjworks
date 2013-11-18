#include <stdio.h>
#include <stdlib.h>
#include "fs.h"
#include <string.h>
#include <assert.h>
#include <minix/com.h>
#include <minix/u64.h>
#include <minix/bdev.h>
#include "buf.h"
#include "inode.h"
#include "super.h"
#include "const.h"
//#include "glo.h"

int grsExtFrag(void);
void print_extern_frag_report(void);
unsigned int get_frag(zone_t,int);

extern dev_t fs_dev;

unsigned int max_extern_frag = 0;
unsigned int min_extern_frag = 0;
unsigned int avg_extern_frag = 0;

void print_extern_frag_report()
{
  printf("--------- EXTERNAL FRAGMENTATION REPORT--------\n");
  printf("MINIMUM EXTERNAL FRAGMENTATION : %d\n",min_extern_frag);
  printf("MAXIMUM EXTERNAL FRAGMENTATION : %d\n",max_extern_frag);
  printf("AVERAGE EXTERNAL FRAGMENTATION : %d\n",avg_extern_frag);
}

unsigned int get_frag(zone_t grs_zone,int num)
{
 short j = 0, count = 1, temp =0;
 unsigned int frag = 0;
 block_t b;
 struct buf* bp1;
 b = (block_t)grs_zone << num;
 temp = num;
 if(temp == 0) { count = 1; }
 else {
		 while(temp > 0)
		 {
			count *= 2;
			temp--;
		 }
 }
 for(j = 0;j < count;j++,b++)
 {
	bp1 = get_block(fs_dev,b,NORMAL);
	if(bp1->b_bytes == 0)
	{
		frag++;
	}
 }
 if(frag > 0)
 {
	printf("Fragment detected\n");
	if(min_extern_frag == 0 | frag < min_extern_frag)
	min_extern_frag = frag;
	if(max_extern_frag == 0 | frag > min_extern_frag)
	max_extern_frag = frag;
	avg_extern_frag =(max_extern_frag+min_extern_frag)/2 ;
 }
 return frag;
}


int grsExtFrag()
{
  struct super_block* sp;
  unsigned int in_numb[100] = {0};
  unsigned int frag_cnt = 0,count = 0, scale = 0;
  struct inode *rip;
  block_t start_block;		/* first bit block */
  block_t block;
  bit_t map_bits;		/* how many bits are there in the bit map? */
  short bit_blocks;		/* how many blocks are there in the bit map? */
  bit_t origin = 1;			/* number of bit to start searching at */
  int c = 0, n = 0, d = 0, j = 0;
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
    for (wptr = &bp->b_bitmap[word]; wptr < wlim; wptr++) {

      /* Does this word contain a free bit? */
      if (*wptr == (bitchunk_t) 0) continue;
	  
	  k = (bitchunk_t) conv4(sp->s_native, (int) *wptr);

      for (i = 0; i < 8*sizeof(k); ++i) {
        /* Bit number from the start of the bit map. */
        b = ((bit_t) block * FS_BITS_PER_BLOCK(sp->s_block_size))
            + (wptr - &bp->b_bitmap[0]) * FS_BITCHUNK_BITS
            + i;

        /* Don't count bits beyond the end of the map. */
        if (b >= map_bits) {
          break;
        } 
        if ((k & (1 << i)) == 1) {
          in_numb[n++] = b;
        }
	}	
		 if (b >= map_bits) break;
  }
	++block;
	word = 0;
	--bcount;	
}

  for(n=0;in_numb[n]!=0;n++)
  {
	rip = get_inode(fs_dev, in_numb[n]);
	scale = rip->i_sp->s_log_zone_size;
	for(j=0;j<7;j++)
	{
	   if(rip->i_zone[j] == NO_ZONE)
		{
		printf("The last zone is %d\n",rip->i_zone[j]);
	    frag_cnt = get_frag(rip->i_zone[j],scale);
		}
		else if(j==7)
		{
			b = (block_t)rip->i_zone[7] << scale;
			bp = get_block(rip->i_dev,b,NORMAL);
			count = (rip->i_sp->s_block_size)/32;
			for(d=0;d<count;d++)
			{
				if(bp->bp->b__v1_ind[d] == NO_ZONE)
				{
				frag_cnt = get_frag(bp->bp->b__v1_ind[d],scale);
				}
			}
		}
		
  }
 }
  print_extern_frag_report();
  return 0;
}