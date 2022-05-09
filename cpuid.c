#include "cpuid.h"
#include <stdio.h>

static void cpuid(ulong *eax, ulong *ebx, ulong *ecx, ulong *edx)
{
	__asm__ ("movl %0, %%eax \n\t"
		 "movl %1, %%ebx \n\t"
		 "movl %2, %%ecx \n\t"
		 "movl %3, %%edx \n\t"
		 "cpuid          \n\t"
		 "movl %%eax, %0 \n\t"
		 "movl %%ebx, %1 \n\t"
		 "movl %%ecx, %2 \n\t"
		 "movl %%edx, %3 \n\t"
		 : "+m"(*eax), "+m"(*ebx), "+m"(*ecx), "+m"(*edx));
}



void get_vendor_name(struct vendor_name *vendor)
{
	ulong eax = 0, ebx = 0, ecx = 0, edx = 0;

	cpuid(&eax, &ebx, &ecx, &edx);
	vendor->name[0]  = (ebx & 0x000000ff);
	vendor->name[1]  = (ebx & 0x0000ff00) >> 8;
	vendor->name[2]  = (ebx & 0x00ff0000) >> 16;
	vendor->name[3]  = (ebx & 0xff000000) >> 24;
	vendor->name[4]  = (edx & 0x000000ff);
	vendor->name[5]  = (edx & 0x0000ff00) >> 8;
	vendor->name[6]  = (edx & 0x00ff0000) >> 16;
	vendor->name[7]  = (edx & 0xff000000) >> 24;
	vendor->name[8]  = (ecx & 0x000000ff);
	vendor->name[9]  = (ecx & 0x0000ff00) >> 8;
	vendor->name[10] = (ecx & 0x00ff0000) >> 16;
	vendor->name[11] = (ecx & 0xff000000) >> 24;
	vendor->name[12] = 0;
}



void get_brand_name(struct brand_name *brand)
{
	
	
	ulong eax = 0x80000002, ebx = 0, ecx = 0, edx = 0, flag = 0, i;
	for(i = 0x80000002; i < 0x80000005; i++)
	{
		eax = i, ebx = 0, ecx = 0, edx = 0;
		cpuid(&eax, &ebx, &ecx, &edx);
		
		brand->name[flag * 16 + 0]   = (eax & 0x000000ff);
		brand->name[flag * 16 + 1]   = (eax & 0x0000ff00) >> 8;
		brand->name[flag * 16 + 2]   = (eax & 0x00ff0000) >> 16;
		brand->name[flag * 16 + 3]   = (eax & 0xff000000) >> 24;
		brand->name[flag * 16 + 4]   = (ebx & 0x000000ff);
		brand->name[flag * 16 + 5]   = (ebx & 0x0000ff00) >> 8;
		brand->name[flag * 16 + 6]   = (ebx & 0x00ff0000) >> 16;
		brand->name[flag * 16 + 7]   = (ebx & 0xff000000) >> 24;
		brand->name[flag * 16 + 8]   = (ecx & 0x000000ff);
		brand->name[flag * 16 + 9]   = (ecx & 0x0000ff00) >> 8;
		brand->name[flag * 16 + 10]  = (ecx & 0x00ff0000) >> 16;
		brand->name[flag * 16 + 11]  = (ecx & 0xff000000) >> 24;
		brand->name[flag * 16 + 12]  = (edx & 0x000000ff);
		brand->name[flag * 16 + 13]  = (edx & 0x0000ff00) >> 8;
		brand->name[flag * 16 + 14]  = (edx & 0x00ff0000) >> 16;
		brand->name[flag * 16 + 15]  = (edx & 0xff000000) >> 24;
		flag++;
	}
	brand->name[48] = 0;
}

void printCache(){
	unsigned int a = 0x80000000, b, c, d;
	a = 4;
	asm ( 	"cpuid\n"
		: "=a" (a), "=b" (b), "=c" (c), "=d" (d)

		: "a" (a), "c" (0)
	);
	int i = 0;
	while (a & 0b11111){
		a = 4;
		asm ( "cpuid\n"
		: "=a" (a), "=b" (b), "=c" (c), "=d" (d)
		: "a" (a), "c" (i)
		);
		// printf("%x %x %x %x\n", a, b, c, d);
		char *str;
		if ((a & 0b11111) == 1) str = "Data";
		else if ((a & 0b11111) == 2) str = "Instruction";
		else if ((a & 0b11111) == 3) str = "Unified";
		else continue;
		printf("L%d %s cache\n", (a >> 5) & 0b111, str);
		unsigned int sz = c + 1;
		sz *= ((b & 0xFFF) + 1);
		printf("Line size: %d Bytes\n", ((b & 0xFFF) + 1));
		sz *= (((b >> 12) & 0b1111111111) + 1);
		sz *= (((b >> 22) & 0b1111111111) + 1);
		printf("Size: %d KBytes\n\n", sz/1024);
		i++;
	}
}

void get_cpu_info(struct cpu_info *info)
{
	ulong eax = 1, ebx = 0, ecx = 0, edx = 0;

	cpuid(&eax, &ebx, &ecx, &edx);
	info->stepping_id = eax & 0x0f;
	info->model_id = eax & 0xf0;
	info->family_id = (eax & 0x0f00) >> 8;
	info->type_id = (eax & 0x3000) >> 12;
	info->extended_model_id = (eax & 0x0f0000) >> 16;
	info->extended_family_id = (eax & 0x0ff00000) >> 20;
	info->brand_id = ebx & 0xff;
	info->cache_line_size = (ebx & 0xff00) >> 5;
	info->max_lcpu_ids = (ebx & 0xff0000) >> 16;
	info->initial_apic_id = (ebx & 0xff000000) >> 24;
}
