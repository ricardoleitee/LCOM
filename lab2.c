#include <minix/sysutil.h>
#include <stdio.h>
#include "video_gr.h"
#include "game.h"

#define DELAY_US    175000

int main() {
	sef_startup();
	game();

	//image *p_image, *p_image2, *p_image3, *p_image4, *p_image5;
	//p_image = &image_1c;

	//write to file
	/*FILE *fp;
	unsigned char *ptr;

	if ((fp = fopen("/usr/src/drivers/proj/images/cronometro/image_11.mg", "w")) == NULL) {
		printf("Erro");
	}
	ptr = image_11.pixel_data;
	//unsigned int tamanho = (unsigned int )(image_1c.width * image_1c.height);
	unsigned int tamanho = 759750;
	fwrite(ptr, sizeof(char), tamanho, fp);
	fclose(fp);*/

	//vg_draw_image(p_image->height,p_image->width, p_image->pixel_data,25,25);
	//const char * path = "/usr/src/drivers/proj/images/image_title.mg";
	 //vg_draw(path, 156, 413, 225, 25);
	/*p_image2 = &image_2c;
	 p_image3 = &image_3c;
	 p_image4 = &image_back;
	 p_image5 = &image_title;*/

	/*int n = 5;
	 image* cards[5];
	 cards[0] = &image_1c;
	 cards[1] = &image_2c;
	 cards[2] = &image_3c;
	 cards[3] = &image_back;
	 cards[4] = &image_title;

	 vg_draw_cards(n, cards, 25, 25);*/

	//vg_draw_image(p_image->height,p_image->width, p_image->pixel_data,25,25);
	/*tickdelay(micros_to_ticks(DELAY_US));
	 vg_draw_image(p_image2->height,p_image2->width, p_image2->pixel_data,25,25);
	 tickdelay(micros_to_ticks(DELAY_US));
	 vg_draw_image(p_image3->height,p_image3->width, p_image3->pixel_data,25,25);
	 tickdelay(micros_to_ticks(DELAY_US));
	 vg_draw_image(p_image4->height,p_image4->width, p_image4->pixel_data,25,25);
	 tickdelay(micros_to_ticks(DELAY_US));
	 vg_draw_image(p_image5->height,p_image5->width, p_image5->pixel_data,25,25);
	 tickdelay(micros_to_ticks(DELAY_US));*/

	/*game();*/


	return 0;
}
