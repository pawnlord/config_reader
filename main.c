#include <stdio.h>
#include <string.h>
#include "config_reader.h"

int main(void) {
	config cfg;
	cfg.eol = '\n';
	char buff[1000];
	file_reader("example.cfg", buff);
	config_reader(buff, &cfg);
	char eol[2] = {cfg.eol, 0};
	for(int i = 0; strcmp(cfg.words[i], ""); i++) {
		if(cfg.words[i][0] == '\n'){
			printf("\\n");
		}
		printf("%d: %s\n", i, cfg.words[i]);
	}
	char field1[255][255] = {""};
	int b = get_field(cfg, "FIELD2", field1);
	printf("BBBB: %d\n", b);
	for(int i = 0; strcmp(field1[i], ""); i++) {
		printf("%d: %s\n", i, field1[i]);
	}
	int a = get_field(cfg, "FIELD1", field1);
	printf("AAAA: %d\n", a);
	for(int i = 0; strcmp(field1[i], ""); i++) {
		if(field1[i][0] == '\n'){
			printf("\\n");
		}
		printf("%d: %s\n", i, field1[i]);
	}
	char val[10][50][50];
	get_attr(cfg, field1, "MAX", val);
	printf("Max: %s\n", field1[4]);
	printf("Max: ", val[0][0]);
	return 0;
}
