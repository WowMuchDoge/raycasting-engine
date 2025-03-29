#include "player.h"

#include <math.h>
#include <stdint.h>

#include "constants.h"

#define NANO_SEC_PER_SEC 1000000000L
#define EPSILON 1e-6

const uint8_t simpleMap[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
};

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

void RaycastPlayer(Player* player, Map map) {
	double realScreenWidth = (double)SCREEN_WIDTH / PIXELS_PER_METER;

	double screenDistanceFromPlayer = (realScreenWidth / 2) / (tan(FOV / 2));

	double anglePerPixel = FOV / SCREEN_WIDTH;

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		double rayAngle = ((i * anglePerPixel) + ((PI / 2) - (FOV / 2))) + player->rot;
		Point rayPosition = (Point){.y = player->y, .x = player->x};

		double dx = ITERATE_RAY_DISTANCE * cos(rayAngle);
		double dy = ITERATE_RAY_DISTANCE * sin(rayAngle);

		int iterations = (int) RENDER_DISTANCE / ITERATE_RAY_DISTANCE;

		for (int j = 0; j < iterations; j++) {
			rayPosition.x += dx;
			rayPosition.y += dy;

			double distanceX = j * dx;
			double distanceY = j * dy;

			if ((int)rayPosition.x >= 8 || (int)rayPosition.x < 0 ||
				(int)rayPosition.y >= 8 || (int)rayPosition.y < 0) {
				
				player->screenSegments[i].isValid = false;
				break;
			}
			
			if (simpleMap[(int)rayPosition.y][(int)rayPosition.x] == 1) {
				player->screenSegments[i] = (VerticalSegment){.distance = hypot(rayPosition.x, rayPosition.y)};
				player->screenSegments[i].isValid = true;
				break;
			}
		}

		player->screenSegments->isValid = false;
	}
}
