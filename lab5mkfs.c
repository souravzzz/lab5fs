#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include "lab5fs.h"

int main()
{
	int fd, len;
	struct lab5fs_super_block sb;

	sb._magic = LAB5FS_MAGIC;
	sb._free = ~0;

	fd = open("image", O_RDWR);
	len = write(fd, &sb, sizeof sb);
	len = write(fd, 0, LAB5FS_BSIZE-len);	
	close(fd);

	return 0;
}
