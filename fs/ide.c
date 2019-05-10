/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
	void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int a = 0;

	while (offset_begin + offset < offset_end) {
		// Your code here
		syscall_write_dev(&diskno, 0x13000010, sizeof(u_int));
		syscall_write_dev(&offset, 0x13000000, sizeof(u_int));
		a = 0;
		syscall_write_dev(&a, 0x13000020, sizeof(int));
		syscall_read_dev(&a, 0x13000030, sizeof(int));

		if(a == 0) {
			user_panic("ide_read error\n");
		}
		syscall_read_dev(dst + offset, 0x13004000, BY2SECT);

		offset += BY2SECT;
		// error occurred, then panic.
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
	void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * BY2SECT;
	int offset_end = (secno + nsecs) * BY2SECT;
	int offset = 0;
	int a;
	writef("diskno: %d\n", diskno);
	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.
		syscall_write_dev(&diskno, 0x13000010, sizeof(u_int));
		syscall_write_dev(&offset, 0x13000000, sizeof(u_int));
		syscall_write_dev(src + offset, 0x13004000, BY2SECT);
		a = 1;
		syscall_write_dev(&a, 0x13000020, sizeof(int));
		syscall_read_dev(&a, 0x13000030, sizeof(int));
		if(a == 0) {
			user_panic("ide_write error\n");
		}
	}
}

