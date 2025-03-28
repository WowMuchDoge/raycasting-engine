#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>

#include "line.h"

typedef struct {
	double distance;

	// Whether the given segments actually intersect
	bool isValid;
} VerticalSegment;

typedef struct {
	LineSegment* walls;
	int size;
} Map;

// Creates a map with the given properties
Map CreateMap(LineSegment walls[], int size);

// Calculates the intersection of the ray against every wall and
// returns the closest intersection point
VerticalSegment GetIntersection(Map map, LineSegment ray);

// Frees the dynamically allocaated memory in the map
void FreeMap(Map map);

#endif
