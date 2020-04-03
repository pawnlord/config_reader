#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../header/config_reader.h"

void init_str(char** str, int dim){
	*str = malloc(sizeof(char)*dim);
	
	for(int i = 0; i < dim; i++){
		(*str)[i] = 0;
	}

}
void free_str(char** str){
	free(*str);
}
void init_strptr(char*** strptr, int dim1, int dim2) {
	*strptr = malloc(sizeof(char*)*dim1);
	for(int i = 0; i < dim1; i++){
		(*strptr)[i] = malloc(sizeof(char)*dim2);
		for(int j = 0; j < dim2; j++){
			(*strptr)[i][j] = 0;
		}
	}
}
void free_strptr(char*** strptr, int dim1){
	for(int i = 0; i < dim1; i++){
		free((*strptr)[i]);
	}
	free((*strptr));
}

void init_strptrptr(char**** strptrptr, int dim1, int dim2, int dim3){
	*strptrptr = malloc(sizeof(char**)*dim1);
	for(int i = 0; i < dim1; i++){
		(*strptrptr)[i] = malloc(sizeof(char*)*dim2);
		for(int j = 0; j < dim2; j++){
			(*strptrptr)[i][j] = malloc(sizeof(char)*dim3);
			for(int k = 0; k < dim3; k++){
					
				(*strptrptr)[i][j][k] = 0;
			}
		}
	}
}

void free_strptrptr(char**** strptrptr, int dim1, int dim2){
	for(int i = 0; i < dim1; i++){
		for(int j = 0; j < dim2; j++){
			free((*strptrptr)[i][j]);
		}
		free((*strptrptr)[i]);
	}
	free((*strptrptr));
}

void insert_string_array(char** originalp, char** newp, int index, int originalsize, int newsize){
	char** original = originalp;
	char** new = newp;
	char** copy = malloc(sizeof(char*)*originalsize);
	int original_index = index;
	int copy_index = 0;
	for(; strcmp(original[index], ""); index++){
		strcpy(copy[copy_index], original[index]);
		copy_index++;
	}
	index = original_index;
	for(int i = 0; strcmp(new[i], ""); i++){
		strcpy(original[index], new[i]);
		index++;
	}
	for(int i = 0; i < copy_index; i++){
		strcpy(original[index], copy[i]);
		index++;	
	}
}

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
	c = fgetc((FILE*)fp);
	// repeat until c is EOF
	while(c != EOF )
	{
		buffer[i] = c;
		i++;
		c = fgetc((FILE*)fp);
	}
	buffer[i] = 0;
	fclose(fp);
}

int config_reader(char* filename, config* cfg) {
	char* buffer;
	init_str(&buffer, 1000);
	file_reader(filename, buffer);
	init_strptr(&cfg->words, MAX_SIZE, MAX_SIZE);
	int word_counter = 0;
	int current_word = 0;
	char* eol_str;
	init_str(&eol_str, 2);
	eol_str[0] = cfg->eol;
	int i = 0;
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
			strcpy(cfg->words[current_word], eol_str);
			/* add array for next word */
			word_counter = 0;
			current_word++;
		} else {
			cfg->words[current_word][word_counter] = buffer[i];
			word_counter++;
		}
	}
	word_counter = 0;
	current_word++;
	free_str(&buffer);
	free_str(&eol_str);
}

int get_field(config cfg, char* field_name, char** field) {
	char* full_field_name;
	init_str(&full_field_name, 50);
	strcat(full_field_name, "[");
	strcat(full_field_name, field_name);
	strcat(full_field_name, "]");
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
	free_str(&full_field_name);
	return 1;
}

int get_attr(config cfg, char** field, char* attr, char*** val) {
	char* new_line;
	init_str(&new_line, 2);
	new_line[0] = cfg.eol;
	int current_val  = 0;
	int current_val_number = 0;
	int found = 0;
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], attr) == 0) {
			found = 1;
			i+=1;
			int curr_i = i;
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val_number][current_val], field[i]);
				current_val++;
			}
			i = curr_i;
			current_val = 0;
			current_val_number++;
		}
	}
	free_str(&new_line);
	return found;
}
int get_first_attr(config cfg, char** field, char* attr, char** val) {
	char* new_line;
	init_str(&new_line, 2);
	new_line[0] = cfg.eol;
	int current_val  = 0;
	int found = 0;
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], attr) == 0) {
			found = 1;
			i+=1;
			int curr_i = i;
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val], field[i]);
				current_val++;
			}
			break;
		}
	}
	free_str(&new_line);
	return found;
}
int get_last_attr(config cfg, char** field, char* attr, char** val) {
	char* new_line;
	init_str(&new_line, 2);
	new_line[0] = cfg.eol;
	int current_val  = 0;
	int found = 0;
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], attr) == 0) {
			found = 1;
			i+=1;
			int curr_i = i;
			for(int j = 0; strcmp(val[j], ""); j++){
				strcpy(val[j], "");
			}
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val], field[i]);
				current_val++;
			}
			i = curr_i;
			current_val = 0;
		}
	}
	free_str(&new_line);
	return found;
}
int dir_get_attr(config cfg, char* fieldname, char* attr, char*** val) {
	char** field;
	init_strptr(&field, MAX_SIZE, MAX_SIZE);
	get_field(cfg, fieldname, field);
	get_attr(cfg, field, attr, val);
	free_strptr(&field, MAX_SIZE);
}

int dir_get_first_attr(config cfg, char* fieldname, char* attr, char** val) {
	char** field;
	init_strptr(&field, MAX_SIZE, MAX_SIZE);
	get_field(cfg, fieldname, field);
	get_first_attr(cfg, field, attr, val);
	free_strptr(&field, MAX_SIZE);
}
int dir_get_last_attr(config cfg, char* fieldname, char* attr, char** val) {
	char** field;
	init_strptr(&field, MAX_SIZE, MAX_SIZE);
	get_field(cfg, fieldname, field);
	get_last_attr(cfg, field, attr, val);
	free_strptr(&field, MAX_SIZE);
}

void set_cfg_field(config* cfg, char begin, char end) {
	cfg->begin_field = begin;
	cfg->end_field = end;
}; 

void set_cfg_eol(config* cfg, char eol) {
	cfg->eol= eol;
}; 

void auto_cfg_setup(config* cfg) {
	set_cfg_field(cfg, '[', ']');
	set_cfg_eol(cfg, '\n');
}

void cfg_setup(config* cfg, char eol, char begin, char end) {
	set_cfg_field(cfg, begin, end);
	set_cfg_eol(cfg, eol);
}


int save_config(config* cfg, char* filename){
	FILE* fp = fopen(filename, "w");
	char* eol_str;
	init_str(&eol_str, 2);
	eol_str[0] = cfg->eol;
	char* copy_word = malloc(MAX_SIZE*sizeof(char));
	for(int i = 0; strcmp(cfg->words[i], ""); i++){
		if(strcmp(cfg->words[i], eol_str) != 0) {
			strcat(strcpy(copy_word, cfg->words[i]), " ");
		} else {
			strcpy(copy_word, cfg->words[i]);
		}
		printf(copy_word);
		
		if(fputs(copy_word, fp) == EOF){
			perror("FAILED TO WRITE CONFIG TO ");
			perror(filename);
			return 0;
		}
	}
	//free_str(&eol_str);
	fclose(fp);
	return 1;
}

int set_field_attr(config* cfg, char* field_name, char* attr, char** new_val) {
	char* full_field_name = "";
	init_str(&full_field_name, 50);
	strcat(full_field_name, "[");
	strcat(full_field_name, field_name);
	strcat(full_field_name, "]");
	int i = 0;
	int found = 0;
	for(i = 0; strcmp(cfg->words[i], ""); i++) {
		if(strcmp(cfg->words[i], full_field_name) == 0) {
			i+=2; /* two to go down a line */
			found = 1;
			break;
		}
	}
	if(found == 0) {
		perror("COULD NOT FIND FIELD ");
		perror(full_field_name);
		return 0;
	}
	char** fieldp = (cfg->words)+i; /* pointer to field for ease of use */
	char* new_line;
	init_str(&new_line, 2);
	new_line[0] = cfg->eol;
	new_line[1] = 0;
	int current_val  = 0;
	int current_val_number = 0;
	found = 0;
	for(i = 0; strcmp(fieldp[i], "") && !is_field(fieldp[i]); i++) {
		if(strcmp(fieldp[i], attr) == 0) {
			found = 1;
			i+=1;
			int curr_i = i;
			int curr_new_val = 0;
			for(; strcmp(fieldp[i], "") && strcmp(fieldp[i], new_line) && !is_field(fieldp[i]); i++) {
				strcpy(fieldp[i], new_val[curr_new_val]);
				curr_new_val++;
			}
			if(strcmp(new_val[curr_new_val], "")) { /* we need to insert values now! */
				insert_string_array(fieldp, new_val+curr_new_val, i, MAX_SIZE, 50);
			}
			i = curr_i;
			current_val = 0;
			current_val_number++;
		}
	}
	if(found == 0){
		perror("COULD NOT FIND VALUE IN ");
		perror(full_field_name);
		return 0;
	}
	free_str(&full_field_name);
	free_str(&new_line);
}

/*TEST
int main(){
	config cfg;
	auto_cfg_setup(&cfg);
	config_reader("config.cfg", &cfg);
	char** field;
	init_strptr(&field, MAX_SIZE, MAX_SIZE);
	char*** val;
	init_strptrptr(&val, 20, 10, 50);
	char** fval;
	init_strptr(&fval, 10, 50);
	char** lval;
	init_strptr(&lval, 10, 50);
	get_field(cfg, "FIELD1", field);
	printf("FIELD1: \n");
	for(int i = 0; strcmp(field[i], ""); i++) {
		printf("%s ", field[i]);
	}
	printf("attr MIN: \n");
	dir_get_attr(cfg, "FIELD1", "MIN", val);
	for(int i = 0; strcmp(val[i][0], ""); i++) {
		for(int j = 0; strcmp(val[i][j], ""); j++) {
			printf("%d: %s ", j, val[i][j]);
		}
		printf("\n");
	}
	char** new_val;
	init_strptr(&new_val, 4, 50);
	new_val[0] = "10";
	new_val[1] = "sad";
	new_val[2] = "dad";
	set_field_attr(&cfg, "FIELD1", "MIN", new_val);
	for(int i = 0; strcmp(cfg.words[i], ""); i++) {
		printf("%s ", cfg.words[i]);
	}
	
	save_config(&cfg, "new.cfg");
	printf("HELP");
	free_strptr(&cfg.words, MAX_SIZE);
	return 0;
}*/