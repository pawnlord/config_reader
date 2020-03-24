#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_reader.h"

int is_field(char* str) {
	int last = 0;
	if(str[0] != '[') {
		return 0;
	}
	for(; str[last] != 0; last++){}
	if(last == 1) {
		return 0;
	}
	if(str[last-1] != ']') {
		return 0;
	}
	return 1;
}

int file_reader(char* filename, char* buffer) {
	int ret;
	FILE* fp = fopen(filename, "r");
	char c = 0;
	int i = 0;
	// repeat until c is EOF
	while(c != EOF )
	{
		c = fgetc((FILE*)fp);
		buffer[i] = c;
		i++;
	}
	buffer[i] = 0;
	fclose(fp);
}

int config_reader(char* buffer, config* cfg) {
	int word_counter = 0;
	int current_word = 0;
	int i;
	for(i = 0; i < MAX_SIZE; i++){
		for(int j = 0; j < MAX_SIZE; j++){
			cfg->words[i][j] = 0;
		}
	}
	for(i = 0; buffer[i] != 0; i++) {
		if(buffer[i] == ' ') {
			/* add array for next word */
			if(strcmp(cfg->words[current_word], "")) {
				word_counter = 0;
				current_word++;
			}
		} else if( buffer[i] == cfg->eol){
			/* add eol as word */
			if(strcmp(cfg->words[current_word], "")) {
				word_counter = 0;
				current_word++;
			}
			char eol_str[2] = {cfg->eol, 0};
			strcpy(cfg->words[current_word], eol_str);
			/* add array for next word */
			word_counter = 0;
			current_word++;
		} else {
			cfg->words[current_word][word_counter] = buffer[i];
			word_counter++;
		}
	}
	printf("Values: %d, %d, %d, %d\n", word_counter, current_word, cfg->words[current_word][word_counter], MAX_SIZE);
	word_counter = 0;
	current_word++;
	free((void*)i);
	free((void*)word_counter);
	free((void*)current_word);
}

int get_field(config cfg, char* field_name, char field[][255]) {
	char full_field_name[255] = "";
	strcat(full_field_name, "[");
	strcat(full_field_name, field_name);
	strcat(full_field_name, "]");
	printf("%s\n", full_field_name);
	int i = 0;
	int found = 0;
	for(i = 0; strcmp(cfg.words[i], ""); i++) {
		if(strcmp(cfg.words[i], full_field_name) == 0) {
			i+=2; /* two to go down a line */
			found = 1;
			break;
		}
	}
	if(found == 0) {
		return 0;
	}
	int index = 0;
	for(; strcmp(cfg.words[i], "") && !is_field(cfg.words[i]); i++) {
		strcpy(field[index], cfg.words[i]);
		index++;
	}
	free((void*)i);
	free((void*)found);
	free((void*)index);
	return 1;
}

int get_attr(config cfg, char field[][255], char attr[], char val[][50][50]) {
	char new_line[2] = {cfg.eol, 0};
	int current_val  = 0;
	int current_val_number = 0;
	int found = 0;
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], attr) == 0) {
			found = 1;
			i+1;
			int curr_i = i;
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val_number][current_val], field[i]);
				current_val++;
			}
			current_val = 0;
			current_val_number++;
		}
	}
	return found;
}