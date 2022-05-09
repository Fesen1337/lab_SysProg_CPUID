#include <stdio.h>
#include "cpuid.h"

int main()
{
	struct vendor_name vendor;
	struct cpu_info info;
	struct brand_name brna;
	get_brand_name(&brna);
	get_vendor_name(&vendor);
	
	printf("Vendor Name is %s\n", vendor.name);
	printf("%s\n", brna.name);
	get_cpu_info(&info);
	printf("CPU INFO:\n");
	printf("\tstepping id: 0x%x\n", info.stepping_id);
	printf("\tmodel: 0x%x\n", info.model_id);
	printf("\tfamily: 0x%x\n", info.family_id);
	printf("\ttype: 0x%x\n", info.type_id);
	printf("\textended model id: 0x%x\n", info.extended_model_id);
	printf("\textended family id: 0x%x\n", info.extended_family_id);
	printf("\tbrand index: 0x%x\n", info.brand_id);
	printf("\tcache line size: %u bytes\n", info.cache_line_size);
	printf("\tmax number of addressable IDs for logical CPUs: %u\n", info.max_lcpu_ids);
	printf("\tinitial APIC ID: %u\n", info.initial_apic_id);
	printCache();
	return 0;
}
