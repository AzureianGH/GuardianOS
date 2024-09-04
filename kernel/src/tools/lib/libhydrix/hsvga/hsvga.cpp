#include <libhydrix/hsvga/svga.h>
#define VMWARE_MAGIC  0x564D5868
#define VMWARE_PORT   0x5658
#define VMWARE_PORTHB 0x5659
void vmware_send(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORT;
	asm volatile("in %%dx, %0" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}

static void vmware_send_hb(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORTHB;
	asm volatile("cld; rep; outsb" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}

static void vmware_get_hb(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORTHB;
	asm volatile("cld; rep; insb" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}

int is_vmware_backdoor(void) {
	vmware_cmd cmd;
	cmd.bx = ~VMWARE_MAGIC;
	cmd.command = CMD_GETVERSION;
	vmware_send(&cmd);
	
	if (cmd.bx != VMWARE_MAGIC || cmd.ax == 0xFFFFFFFF) {
		/* Not a backdoor! */
		return 0;
	}
	
	return 1;
}