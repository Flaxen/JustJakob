#include <pic32mx.h>
#include <stdint.h>

#include "headerfiles/displayFunctions.h"
#include "headerfiles/i2cFunctions.h"
#include "headerfiles/LSM6DS3addresses.h"
#include "headerfiles/gameFunctions.h"
#include "headerfiles/menuFunctions.h"
#include "animations/animationFunctions.h"

int main(void) {

	// initializations
	spiInit();
	i2cInit();
	buttonsInit();

	display_init();

	START:
	// set the map to logo for intro screen
	setMap(logo);

	// dog runs in and "eats" logo
	int dogX = 129;
	int dogY = 5;
	while(dogX > (-TILE_WIDTH)) {
		animateDog(dogX, dogY, 180000);
		dogX -= 4;
	}

	MENU:
	delay(1000000);
	// enter menu
	int menuRow = 0;
	while(1){
		menuRow = stateMenu(menuRow);
		menu(menuRow);
		if(exitMenu(menuRow)){
			goto GAME;
		}
		if(enterSettings(menuRow)){
			goto SETTINGS;

		}
		delay(1000000);
	}

	SETTINGS:
	delay(1000000);
	// enter settings
	int statesettingsRow = 0;
	while(1){
		statesettingsRow = stateSettings(statesettingsRow);
		settings(statesettingsRow);
		if(exitSettings(statesettingsRow)){
			goto MENU;
		}
		if(enterMap(statesettingsRow)){
			goto MAP;
		}
		delay(1000000);
	}

	MAP:
	delay(1000000);
	// enter Map
	int mapRow = 0;
	while(1){
		mapRow = stateMap(mapRow);
		map(mapRow);
		if(exitMap(mapRow)){
			goto SETTINGS;
		}
		changMap(mapRow);
		delay(1000000);
	}

	GAME:
	// initialize actually playable map
	setMap(maps[currentMap]);
	setPlayerStartPos();


	// enter game
	while(1) {
		updateTiltPos();
		drawMove(xMoveDirection(), yMoveDirection());
		display_image(0, MAP_CHOICE);

		if(winGame()){
			break;
		}
		// rerstarts the game
		if(getButton() == 1){
			goto START;
		}
		delay(150000);
	}

	// empty screen for last screen
	setMap(emptyMap);

	// dog runs in and stops in middle
	dogX = 129;
	dogY = 10;
	while(dogX > (75)) {
		animateDog(dogX, dogY, 180000);
		dogX -= 4;
	}

	// place speaking bubble
	drawTile(dogX-(3*TILE_WIDTH)+7, dogY-8, bubbleTile1);
	drawTile(dogX-(2*TILE_WIDTH)+7, dogY-8, bubbleTile2);
	drawTile(dogX-TILE_WIDTH+7, dogY-8, bubbleTile3);
	drawTile(dogX, dogY, dogIdleTile);
	display_image(0, MAP_CHOICE);

	// restarts the game
	while (1) {
		if(awaitAndRestart()){
			goto START;
		}
	}



	return 0;
}
