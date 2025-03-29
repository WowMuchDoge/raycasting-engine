#ifndef PLAYER_H_
#define PLAYER_H_

#include <time.h>
#include <stdio.h>

#include "constants.h"
#include "map.h"

// Units in meters or radians

typedef struct {
	double vx;
	double vy;
	double omega;
} Speeds;

typedef struct {
	double x;
	double y;
	double rot;
	Speeds currentSpeeds;

	// Used to determine deltatime for player movement
	struct timespec _currentTime;

	// Used for rendering after raycasting math is complete
	VerticalSegment screenSegments[SCREEN_WIDTH];
} Player;

// Creates a player with the given conditions at a speed of 0. Remember rotation
// is in radians
Player CreatePlayer(double initialX, double initialY, double initialRot);

// Moves the player according to its current speeds
void IteratePlayer(Player* player);

// Each component of the provided speeds will be added to the player speeds
void AdjustSpeeds(Speeds speeds, Player *player);

// Sets each of the players speeds to the provided speeds
void SetSpeeds(Speeds speeds, Player *player);

// Performs the raycasting calculation based on player rotation and position
void RaycastPlayer(Player* player, Map map);

#endif
