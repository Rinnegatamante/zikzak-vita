#include "Gamestate.h"

Uint32 Gamestate::tickTimeLeft() {
	static Uint32 next_time = 0;
	Uint32 now;
	
	now = SDL_GetTicks();
	if(next_time <= now) {
		next_time = now+20;
		return 0;
	}
	return next_time-now;
}

int Gamestate::getTimeLeft(Level &l) {
	int timeleft = startTime + l.getSeconds() - (unsigned)time(0);
	if(currentState != PLAY)
		resetTimer();
	return timeleft;
}

void Gamestate::loopDelay() {
	SDL_Delay(tickTimeLeft());
}

void Gamestate::resetTimer() {
	startTime = (unsigned)time(0);
}

Operationstate Gamestate::getOperationState() {
	return currentState;
}

void Gamestate::setOperationState(Operationstate state) {
	currentState = state;
	if(state == PLAY)
		resetTimer();
}

Gamestate::Gamestate() {
	setOperationState(TITLE);
}

Gamestate::~Gamestate() { }
