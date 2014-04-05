#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

#define LAB5FS_MAGIC	2441139
#define LAB5FS_BS_BITS	10
#define LAB5FS_BS	(1UL<<LAB5FS_BS_BITS)
#define LAB5FS_MAXSZ	(LAB5FS_BS<<10)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sourav Chakraborty");

static struct inode* lab5fs_get_inode(struct super_block *sb, int mode, dev_t dev)
{
	struct inode * inode = new_inode(sb);

	if (inode) {
		inode->i_mode = mode;
		inode->i_uid = current->fsuid;
		inode->i_gid = current->fsgid;
		inode->i_blksize = LAB5FS_BS;
		inode->i_blocks = 0;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	}
	return inode;
}

static int lab5fs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode * inode;
	struct dentry * root;

	sb->s_maxbytes = LAB5FS_MAXSZ;
	sb->s_blocksize = LAB5FS_BS;
	sb->s_blocksize_bits = LAB5FS_BS_BITS;
	sb->s_magic = LAB5FS_MAGIC;
	inode = lab5fs_get_inode(sb, S_IFDIR | 0755, 0);
	if (!inode)
		return -EFAULT;

	root = d_alloc_root(inode);
	if (!root) {
		iput(inode);
		return -EFAULT;
	}
	sb->s_root = root;
	return 0;

}

static struct super_block* lab5fs_get_sb(struct file_system_type *fs_type,
				   int flags,
				   const char *dev_name,
				   void *data)
{
	printk("Mounting lab5fs\n");
	return get_sb_single(fs_type, flags, data, lab5fs_fill_super);
}

static void lab5fs_kill_super(struct super_block *sb)
{
	printk("Unmounting lab5fs\n");
	kill_litter_super(sb);
}

struct file_system_type lab5fs_fs_type = {
	.owner = THIS_MODULE,
	.name = "lab5fs",
	.fs_flags = FS_REQUIRES_DEV,
	.get_sb = lab5fs_get_sb,
	.kill_sb = lab5fs_kill_super
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

