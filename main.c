/*
 * main.c
 *
 *  Created on: 10 ���� 2017
 *      Author: lironlondon
 */
#include "SPMainAux.h"
#include "SPMiniMax.h"

int main(){
	int difficulty = initialize();
	gameplay(difficulty);
	return 0;
}

