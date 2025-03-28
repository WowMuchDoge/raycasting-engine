#include "line.h"

#include <stdio.h>
#include <math.h>

#define NAN_POINT (Point){NAN, NAN}

static double min(double a, double b) {
	return a < b ? a : b;
}

static double max(double a, double b) {
	return a > b ? a : b;
}

LineSegment CreateLineSegmentFromPoints(double x1, double y1, double x2, double y2) {
	LineSegment line;

	line.p1.x = x1;
	line.p1.y = y1;
	line.p2.x = x2;
	line.p2.y = y2;

	line.m = (y2 - y1) / (x2 - x1);
	line.b = line.m * (-x1) + y1;

	printf("Slope of %f, Y-intercept of %f\n", line.m, line.b);

	return line;
}

LineSegment CreateLineSegmentFromAngle(Point start, double angle, double distance) {
	double x1 = start.x;
	double y1 = start.y;

	double x2 = distance * cos(angle);
	double y2 = distance * sin(angle);

	return CreateLineSegmentFromPoints(x1, y2, x2, y2);
}

Point LineIntersect(LineSegment ls1, LineSegment ls2) {
	if (ls1.m == ls2.m)
		return NAN_POINT;

	double minP1X = min(ls1.p1.x, ls1.p2.x);
	double minP1Y = min(ls1.p1.y, ls1.p2.y);

	double maxP1X = max(ls1.p1.x, ls1.p2.x);
	double maxP1Y = max(ls1.p1.y, ls1.p2.y);

	double minP2X = min(ls2.p1.x, ls2.p2.x);
	double minP2Y = min(ls2.p1.y, ls2.p2.y);

	double maxP2X = max(ls2.p1.x, ls2.p2.x);
	double maxP2Y = max(ls2.p1.y, ls2.p2.y);

	if (maxP1X < minP2X || maxP2X < minP1X ||
		maxP1Y < minP2Y || maxP2Y < minP1Y) {
			
		return NAN_POINT;
	}

	double intersectedX = (ls2.b - ls1.b) / (ls1.m - ls2.m);
	double intersectedY = (ls1.m * intersectedX) + ls1.b;

	return (Point){intersectedX, intersectedY};
}
