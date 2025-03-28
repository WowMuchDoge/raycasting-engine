#ifndef LINE_H_
#define LINE_H_

typedef struct {
	double x;
	double y;
} Point;

typedef struct {
	Point p1;
	Point p2;

	// While these can be calculated from both points, it's a relatively
	// expensive floating-point calculation so let's just do it once in
	// initialization since this is intended to be immutable
	double m;
	double b;
} LineSegment;

// Creates a line segment with the given endpoints
LineSegment CreateLineSegmentFromPoints(double x1, double y1, double x2, double y2);

// Creates line segment with a length, direction and starting point. Useful for
// creating line segments for rays. Angle is in radians and distance is in meters
LineSegment CreateLineSegmentFromAngle(Point start, double angle, double distance);

// Finds the point where two straight lines intersect; returned point will return
// with `nan` if lines do not intersect or if they intersect at an infinite amount
// of points
Point LineIntersect(LineSegment ls1, LineSegment ls2);

#endif
