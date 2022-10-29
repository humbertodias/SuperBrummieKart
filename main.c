// Note to self: In this program +Z axis is up

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>

#define GAME_WIDTH 640
#define GAME_HEIGHT 480

typedef struct rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb;

typedef struct vec2 {
	float x;
	float y;
} vec2;

typedef struct vec3 {
	float x;
	float y;
	float z;
} vec3;

vec3 vec3_cross(vec3 a, vec3 b) {
	return (vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* frameTexture;
void* textureData;
int rowPitch;

bool gameRunning;

typedef struct Camera {
	vec3 position;
	float yaw;
	float pitch;
	vec2 forward_2d;
	vec3 forward;
	vec3 up;
	vec3 right;
	float fov_x;
	float fov_y;
	float cam_dist;
} Camera;

void Camera_SetYawPitch(Camera* cam, float yaw, float pitch) {
	cam->yaw = yaw;
	cam->pitch = pitch;

	float cy = cosf(yaw);
	float sy = sinf(yaw);
	float cp = cosf(pitch);
	float sp = sinf(pitch);

	cam->forward_2d = (vec2){cy, sy};
	cam->forward = (vec3){cy * cp, sy * cp, sp};
	cam->right = (vec3){-sy, cy, 0.0f};
	cam->up = vec3_cross(cam->forward, cam->right);
}

void Camera_SetFovX(Camera* cam) {

}

void Camera_SetFovY(Camera* cam) {

}

Camera mainCamera;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    printf("SDL Version: %d.%d.%d\n", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
    
    window = SDL_CreateWindow("HackTheMidlands", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	SDL_RenderSetLogicalSize(renderer, GAME_WIDTH, GAME_HEIGHT);
	SDL_RenderSetIntegerScale(renderer, true);

	frameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, GAME_WIDTH, GAME_HEIGHT);

	Camera* cam = &mainCamera;
	Camera_SetYawPitch(cam, 0, 0);
	for (int i = 0; i < 10; i++) {
		Camera_SetYawPitch(cam, cam->yaw, cam->pitch + 0.1f);
	}

    gameRunning = true;
    while (gameRunning) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
				gameRunning = false; 
                break;

            default:
                break;
            }
        }

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_LockTexture(frameTexture, NULL, (void**)&textureData, &rowPitch);
		// All rendering must be in here
		memset(textureData, 127, rowPitch * GAME_HEIGHT);
		uint8_t* pixelData = textureData;
		for (int i = 0; i < GAME_HEIGHT; i++) {
			for (int j = 0; j < GAME_WIDTH; j++) {
				// pixelData[i * rowPitch + j * 3 + 0] = 255;
				// pixelData[i * rowPitch + j * 3 + 1] = 255;
				// pixelData[i * rowPitch + j * 3 + 2] = 255;
			}
		}

		SDL_UnlockTexture(frameTexture);

		SDL_RenderCopy(renderer, frameTexture, NULL, NULL);

		SDL_RenderPresent(renderer);
    }

    return 0;
}
