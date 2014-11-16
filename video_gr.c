#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR    0xD0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  16

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

#define GET_VBE_INFO 0x02
#define VBE_CALL 0x4F
#define VBE_INT 0x10
#define SET_LINEAR_FB 1<<14

void * vg_init(unsigned long mode) {

	struct reg86u registries;
	struct mem_range range;
	vbe_mode_info_t vmi;

	registries.u.b.intno = VBE_INT;
	registries.u.b.ah = VBE_CALL;
	registries.u.b.al = GET_VBE_INFO;
	registries.u.w.bx = SET_LINEAR_FB | mode;
	sys_int86(&registries);

	vbe_get_mode_info(mode, &vmi);

	range.mr_base = vmi.PhysBasePtr;

	range.mr_limit = range.mr_base
			+ vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8);
	sys_privctl(SELF, SYS_PRIV_ADD_MEM, &range);

	video_mem = vm_map_phys(SELF, (void *) range.mr_base,
			vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8));

	return NULL;
}

int vg_draw(const char * fpath, int height, int width, int x, int y) {
	FILE *pfile;
	unsigned char *ptr, *ptr2;
	unsigned int i, j;
	unsigned int tamanho = 759750;


	if (x >= H_RES || y >= V_RES) {
		printf("Out of bounds!");
		return 1;
	}

	//Open file
	if ((pfile = fopen(fpath, "r")) == NULL) {
		printf("Erro");
	}

	//Allocate memory
	ptr2 = (char *) malloc((tamanho + 1) * sizeof(char));

	if (!ptr2) {
		fprintf(stderr, "Memory error!");
		fclose(pfile);
		return;
	}

	//Read file contents into ptr2
	fread(ptr2, sizeof(char), tamanho, pfile);
	fclose(pfile);

	ptr = video_mem;

	for (i = 0; i < height; i++) {
		ptr = video_mem + (x + (y + i) * H_RES) * 2;
		for (j = 0; j < width; j++) {
			*ptr = *ptr2;
			ptr++;
			ptr2++;
			*ptr = *ptr2;
			ptr++;
			ptr2++;
		}
	}

	return 0;
}

int vg_draw_image(int height, int width, char *arr, int x, int y) {
	char *ptr, *ptr2;
	int i, j;

	if (x >= H_RES || y >= V_RES) {
		printf("Out of bounds!");
		return 1;
	}

	ptr2 = arr;
	ptr = video_mem;

	for (i = 0; i < height; i++) {
		ptr = video_mem + (x + (y + i) * H_RES) * 2;
		for (j = 0; j < width; j++) {
			*ptr = *ptr2;
			ptr++;
			ptr2++;
			*ptr = *ptr2;
			ptr++;
			ptr2++;
		}
	}

	return 0;
}

int vg_draw_all(deck *d, int x, int y) {
	unsigned int i;
	card * aux;

	for (i=0 ; i<10 ; i++) {
		aux = d->cards[i];
		vg_draw(aux->card_paths[0], aux->height, aux->width, d->posx[i], d->posy[i]);
	}

	/*card * aux;
	int distancia_x = 0;
	int distancia_y = 0;
	int xaux = x;

	switch (d->level) {
	case 1:
		distancia_x = 50;
		distancia_y = 150;
		break;
	case 2:
		distancia_x = 50;
		distancia_y = 150;
		break;
	case 3:
		distancia_x = 50;
		distancia_y = 80;
		break;
	default:
		break;
	}

	for (i = 0; i < 10; i++) {
		aux = d->cards[i];
		if (i == (d->num_cards / 2)) {
			y += distancia_y + 220;
			x = xaux;
		}
		vg_draw(aux->card_paths[0], aux->height, aux->width, x, y);
		x += distancia_x + 150;
	}*/
}

/*int vg_draw_cards(int n, image** cards, int x, int y) {
	int altura = 220;
	int largura = 150;
	int i;
	int count = 0;

	for (i = 0; i < n; i++) {
		vg_draw_image(cards[i]->height, cards[i]->width, cards[i]->pixel_data,
				x, y);
		x += 50 + largura;
		count++;
	}
}*/

int vg_fill(unsigned long color) {
	char *ptr;
	int i;
	ptr = video_mem;

	for (i = 0; i < H_RES * V_RES; i++) {
		*ptr = color;
		ptr++;
		*ptr = color >> 8;
		ptr++;
	}

	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {
	char *ptr;
	ptr = video_mem;

	if (x >= H_RES || y >= V_RES) {
		printf("Out of bounds!");
		return 1;
	}

	ptr += x + y * H_RES * 2;
	*ptr = color;
	ptr++;
	*ptr = color >> 8;
	ptr++;

	return 0;
}

unsigned long vg_get_pixel(unsigned long x, unsigned long y) {
	char *ptr;
	ptr = video_mem;
	unsigned long color;

	if (x >= H_RES || y >= V_RES) {
		printf("Out of bounds!");
		return 1;
	}

	ptr += x + y * H_RES * 2;
	color |= (*ptr << 8);
	ptr++;
	color |= *ptr;
	ptr++;

	return color;

}

int vg_draw_line(unsigned long xi, unsigned long yi, unsigned long xf,
		unsigned long yf, unsigned long color) {

	unsigned long x, y;
	int sx, sy;

	if (xi >= H_RES || yi >= V_RES || xf >= H_RES || yf >= V_RES) {
		return 1;
	}
	if (color >= pow(2.0, (double) BITS_PER_PIXEL)) {
		return 3;
	}
	x = xi;
	y = yi;
	sx = 1;
	sy = 1;

	while (x != xf || y != yf) {
		vg_set_pixel(x, y, color);
		if (xf < xi) {
			sx = -1;
		}
		if (yf < yi) {
			sy = -1;
		}
		if (x != xf)
			x += sx;
		if (y != yf)
			y += sy;
	}

	return 0;
}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}
