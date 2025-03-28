#include "player.h"

#include <math.h>

#define NANO_SEC_PER_SEC 1000000000L
#define EPSILON 1e-6

Player CreatePlayer(double initialX, double initialY, double initialRot) {
	Player player;

	player.x = initialX;
	player.y = initialY;
	player.rot = initialRot;

	player.currentSpeeds = (Speeds){.vx = 0.0, .vy = 0.0, .omega = 0.0};

	clock_gettime(CLOCK_REALTIME, &player._currentTime);

	return player;
}

void IteratePlayer(Player* player) {
	struct timespec curTime;

	clock_gettime(CLOCK_REALTIME, &curTime);

	// In seconds
	double deltaTime = (curTime.tv_sec + ((double)curTime.tv_nsec / NANO_SEC_PER_SEC))
						- (player->_currentTime.tv_sec + ((double)player->_currentTime.tv_nsec / NANO_SEC_PER_SEC));

	Speeds adjustedSpeeds;

	// Essentially just turning this into polar coordinates
	
	double distance = hypot(player->currentSpeeds.vx, player->currentSpeeds.vy);
	double currentAngle = atan2(player->currentSpeeds.vy, player->currentSpeeds.vx);

	printf("Current angle %f\n", currentAngle);

	adjustedSpeeds.vx = distance * cos(currentAngle + player->rot);
	adjustedSpeeds.vy = distance * sin(currentAngle + player->rot);
	adjustedSpeeds.omega = player->currentSpeeds.omega;

	player->x += deltaTime * adjustedSpeeds.vx;
	player->y += deltaTime * adjustedSpeeds.vy;
	player->rot += deltaTime * adjustedSpeeds.omega;

	player->_currentTime = curTime;
}

void AdjustSpeeds(Speeds speeds, Player* player) {
	player->currentSpeeds.vx += speeds.vx;
	player->currentSpeeds.vy += speeds.vy;
	player->currentSpeeds.omega += speeds.omega;
}

void SetSpeeds(Speeds speeds, Player* player) {
	player->currentSpeeds = speeds;
}