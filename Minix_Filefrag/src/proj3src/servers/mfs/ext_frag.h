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


int grsExtFrag(void);
void print_extern_frag_report(void);
unsigned int get_frag(zone_t,int);

int grsIntFrag(void);
void print_internal_frag_report(void);
unsigned int get_internal_frag(zone_t,int);

unsigned int get_internal_filefrag(zone_t grs_zone,int num);
int grsIntFragFile();