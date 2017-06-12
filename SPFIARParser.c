#include "SPFIARParser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

SPCommand spParserPraseLine(const char* str) {
	char* str_copy = malloc(strlen(str)+1);
	strcpy(str_copy, str);
	char* command_text = strtok(str_copy, " \t\n");
	//printf("%s \n", command_text);
	char* command_int = strtok(NULL, " \t\n");
	//printf("%s \n", command_int);

	SPCommand command;
	command.validArg = false;
	command.cmd = SP_INVALID_LINE;

	if (strcmp(command_text, "undo_move") == 0) {
		command.cmd = SP_UNDO_MOVE;
		command.validArg = true;
	}
	if (strcmp(command_text, "restart_game") == 0) {
		command.cmd = SP_RESTART;
		command.validArg = true;
	}
	if (strcmp(command_text, "quit") == 0) {
		command.cmd = SP_QUIT;
		command.validArg = true;
	}
	if ((strcmp(command_text, "add_disc")) == 0) {
		command.cmd = SP_ADD_DISC;
		command.validArg = true;
		if (spParserIsInt(command_int) == true) {
			int tmp = atoi(command_int);
			command.arg = tmp;
		}
	}
	return command;
}

bool spParserIsInt(const char* str) {
	char ch;
	if (*str == '-') {
		++str;
	}
	for (unsigned i = 0; i < strlen(str); i++) {
		ch = str[i];
		if (ch < 48 || ch > 57) {
			return false;
		}
	}
	return true;
}


