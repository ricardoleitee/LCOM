#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct{
	int width;
	int height;
	const char * card_paths[11];
	int selected;
	int upcard;
	int value;
}card;

typedef struct{
	unsigned int num_cards;
	card * cards[10];
	int level;
	int posx[10];
	int posy[10];
}deck;

/*! @brief Estrutura utilizada para criar as imagens. */
typedef struct {
	/*! @brief Largura da image. */
	unsigned int 	 width;
	/*! @brief Altura da imagem. */
	unsigned int 	 height;
	/*! @brief Bytes por pixel. */
	unsigned int 	 bytes_per_pixel;
	/*! @brief Informacao sobre as cores da imagem. */
	unsigned char	 *pixel_data;
} image;

#endif /* _STRUCTS_H_ */
