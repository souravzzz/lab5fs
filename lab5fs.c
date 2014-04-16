#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>

#define LAB5FS_MAGIC		0xBADC0DE
#define LAB5FS_BITS		10
#define LAB5FS_BSIZE		(1<<LAB5FS_BITS)
#define LAB5FS_MAX_SIZE		(LAB5FS_BSIZE<<10)
#define LAB5FS_MAX_OBJS		1024
#define LAB5FS_MAX_FNAME	16

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sourav Chakraborty");

struct lab5fs_super_block {
	uint32_t _magic;
	uint32_t _free;
};

struct lab5fs_inode {
	mode_t mode;
	uint32_t _inum;
};

static struct inode* lab5fs_get_inode(struct super_block *sb, int mode, dev_t dev)
{
	struct inode *inode = new_inode(sb);

	if (inode) {
		inode->i_mode = mode;
		inode->i_uid = current->fsuid;
		inode->i_gid = current->fsgid;
		inode->i_blksize = LAB5FS_BSIZE;
		inode->i_blocks = 0;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	}
	return inode;
}

static int lab5fs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct buffer_head *bh;
	struct lab5fs_super_block *disk_sb;
	struct inode *inode;

	printk("Mounting lab5fs\n");

	sb_set_blocksize(sb, LAB5FS_BSIZE);
	bh = sb_bread(sb, 0);
	disk_sb = (struct lab5fs_super_block*)bh->b_data;

	printk("magic: %zu, free: %zu\n", disk_sb->_magic, disk_sb->_free);

	sb->s_maxbytes = LAB5FS_MAX_SIZE;
	sb->s_blocksize = LAB5FS_BSIZE;
	sb->s_blocksize_bits = LAB5FS_BITS;
	sb->s_magic = LAB5FS_MAGIC;
	sb->s_fs_info = NULL;

	inode = lab5fs_get_inode(sb, S_IFDIR | 0755, 0);
	sb->s_root = d_alloc_root(inode);

	return 0;
}

static struct super_block* lab5fs_get_sb(struct file_system_type *fs_type,
				   int flags,
				   const char *dev_name,
				   void *data)
{
	return get_sb_bdev(fs_type, flags, dev_name, data, lab5fs_fill_super);
}

static void lab5fs_kill_sb(struct super_block *sb)
{
	printk("Unmounting lab5fs\n");
}

struct file_system_type lab5fs_fs_type = {
	.owner = THIS_MODULE,
	.name = "lab5fs",
	.fs_flags = FS_REQUIRES_DEV,
	.get_sb = lab5fs_get_sb,
	.kill_sb = lab5fs_kill_sb
};

static int __init init_lab5fs(void)
{
	int r;

	printk("Initializing module lab5fs\n");
	r = register_filesystem(&lab5fs_fs_type);
	if(r) {
		printk("Error registering lab5fs: %d\n", r);
	}

	return r;
}

static void __exit exit_lab5fs(void)
{
	printk("Cleaning up module lab5fs\n");
}

module_init(init_lab5fs);
module_exit(exit_lab5fs);

