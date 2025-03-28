#include "map.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define MAX_WALL_DISTANCE 1e+10

Map CreateMap(LineSegment walls[], int size) {
	Map map;

	map.size = size;
	map.walls = malloc(size * sizeof(LineSegment));

	memcpy(map.walls, walls, size * sizeof(LineSegment));

	return map;
}

VerticalSegment GetIntersection(Map map, LineSegment ray) {
	double closest = MAX_WALL_DISTANCE;

	bool isValid = false;

	for (int i = 0; i < map.size; i++) {
		LineSegment wall = map.walls[i];

		Point intersection = LineIntersect(wall, ray);

		printf("Intersection X: %f, Intersection Y: %f\n", intersection.x, intersection.y);

		if (intersection.x != NAN && intersection.y != NAN
			&& hypot(intersection.x, intersection.y) < closest) {
			
			closest = intersection.y;

			isValid = true;
		}
	}

	return (VerticalSegment){closest, isValid};
}

void FreeMap(Map map) {
	free(map.walls);
}
