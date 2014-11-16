#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

#define VBE_MODE_INFO_BLOCK_SIZE 256

#define GET_VBE_INFO 0x01
#define VBE_CALL 0x4F
#define VBE_INT 0x10
#define SET_LINEAR_FB 1<<14

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  

	struct reg86u registries;
	mmap_t buf;

	lm_init();
	lm_alloc(VBE_MODE_INFO_BLOCK_SIZE, &buf);

	registries.u.b.ah = VBE_CALL;
	registries.u.b.al = GET_VBE_INFO;
	registries.u.w.es = PB2BASE(buf.phys);
	registries.u.w.di = PB2OFF(buf.phys);
	registries.u.b.intno = VBE_INT;
	registries.u.w.cx = SET_LINEAR_FB | mode;

	sys_int86(&registries);

	*vmi_p = *(vbe_mode_info_t*)buf.virtual;
	lm_free(&buf);
  
  return 1;
}


