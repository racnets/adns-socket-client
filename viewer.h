/*
 * viewer.h
 */

#ifndef VIEWER_H
#define VIEWER_H
#include <stdint.h>

int viewer_set_image(const char *img, int width, int height);
void viewer_update(void);
int viewer_init(int *argc, char **argv[]);
	
#endif /* VIEWER_H */
