#pragma once

typedef struct _texture_data
{
	_texture_data() : width(0), height(0), data(0) {}
	unsigned int width;
	unsigned int height;
	unsigned char* data;
} texture_data;

texture_data load_png(const char* path);
texture_data load_jpg(const char* path);
void free_texture_data(texture_data data);