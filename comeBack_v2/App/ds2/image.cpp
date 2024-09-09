#include "image.h"

image::image(const std::string& path)
{
	_img.loadFromFile(path);
}

void image::detect_edges()
{

}

double image::score(vl::vec2d pt, vl::vec2d prev, double radius)
{
	
}

