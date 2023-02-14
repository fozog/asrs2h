#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "sys_regs.h"

void dummy(void)
{
	__asm("mrs x0, vbar_el1");
}

typedef struct reg {
	uint16_t id;
	char* name;
	char* description;
} reg_t;

#define REG(x)	AARCH64_ ##x, AARCH64_ ## x ## _NAME, AARCH64_ ## x ## _DESC

reg_t sysregs[] = {
	REG(ESR_EL2),
	REG(VBAR_EL1),
};

int main(int argc, char* argv[])
{
	int i;
	uint32_t* instruction = (int32_t*)dummy;
	printf("%x %s %s\n", AARCH64_ESR_EL2, AARCH64_ESR_EL2_NAME, AARCH64_ESR_EL2_DESC);
	printf("instruction: %x\n", *instruction);
	for (i=0; i < sizeof(sysregs) / sizeof(reg_t); i++) {
		uint16_t key = (((*instruction) >> 5 ) & 0x7FFF) | 0x8000;
		if (sysregs[i].id == key) {
			printf("Using %s (%x): %s\n", sysregs[i].name, key, sysregs[i].description);
		}
	}
	return 0;
}
