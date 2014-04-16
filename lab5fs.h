#ifndef _LAB5FS_H
#define _LAB5FS_H

#define LAB5FS_MAGIC		0xBADC0DE
#define LAB5FS_BITS		10
#define LAB5FS_BSIZE		(1<<LAB5FS_BITS)
#define LAB5FS_MAX_SIZE		(LAB5FS_BSIZE<<10)
#define LAB5FS_MAX_OBJS		1024
#define LAB5FS_MAX_FNAME	16

struct lab5fs_super_block {
	uint32_t _magic;
	uint32_t _free;
};

struct lab5fs_inode {
	mode_t mode;
	uint32_t _inum;
};

#endif /* _LAB5FS_H */
