#include "endian.h"
#include "fdt.h"
#include "print.h"
#include "string.h"
#include "types.h"

/* Yes, this is a dirty hack.  But it's simple, and fast. */

int find_str(char *mem, u32 memsz, const char *str)
{
	u32 len = strlen(str);
	const char *last = mem + memsz - len;
	const char *p;

	for (p = mem; p < last; p++) {
		if (*p == *str)
			if (strncmp(p, str, len) == 0)
				return 1;
	}

	return 0;
}

void *find_dtb(void *dtbs, const char *compat)
{
	struct fdt_header *d = (struct fdt_header *)dtbs;

	while (d->magic == be_to_cpu(FDT_MAGIC)) {
		if (find_str((char *)d, be_to_cpu(d->totalsize), compat) == 1)
			return d;

		d = (struct fdt_header *)((char *)d + be_to_cpu(d->totalsize));

		/* align to 4-bytes */
		d = (struct fdt_header *)((((unsigned int)d + 0x3) & ~0x3));

	}

	return NULL;
}
