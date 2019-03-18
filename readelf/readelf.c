/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
#define EI_DATA	5

/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int blswitch32(int x)
{
	int ret = 0;
	ret |= ((x&(0xFF))<<24);
	ret |= ((x&(0xFF00))<<8);
	ret |= ((x&(0xFF0000))>>8);
	ret |= ((x&(0xFF000000))>>24);
	return ret;
}
short blswitch16(short x) {
	int ret = 0;
	ret |= ((x&0xFF)<<8);
	ret |= ((x&0xFF00)>>8);
	return ret;
}
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}
int endian_type(Elf32_Ehdr *ehdr)
{
	return ehdr->e_ident[EI_DATA];
}
/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
		Elf32_Off shoff;

        int Nr;
		int big_endian = 0;

        Elf32_Shdr *shdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return -1;
        }
		big_endian = (endian_type(ehdr) == 2);

		/*debug-----------------------------
		if(big_endian) {
			printf("Big endian\n");
		} else {
			printf("Little endian\n");
		}
		*/

        // get section table addr, section header number and section header size.

		shoff = ehdr->e_shoff;
		if(big_endian) {
			shoff = blswitch32(shoff);
		}
		ptr_sh_table = (binary+shoff);
		sh_entry_size = ehdr->e_shentsize;
		sh_entry_count = ehdr->e_shnum;
		if(big_endian) {
			sh_entry_size = blswitch16(sh_entry_size);
			sh_entry_count = blswitch16(sh_entry_count);
		}
        // for each section header, output section number and section addr.
		for (Nr = 0; Nr < sh_entry_count; ++Nr) {
			int off = Nr*sh_entry_size;
			int addr;
			shdr = ((Elf32_Shdr *)(ptr_sh_table+off));
			addr = shdr->sh_addr;
			if(big_endian) {
				addr = blswitch32(addr);
			}
			printf("%d:0x%x\n", Nr, addr);
		}

        return 0;
}

