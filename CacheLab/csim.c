/* inhyeok hwang 2021058995*/

#include <getopt.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cachelab.h"

#define addlen 64

typedef unsigned long long int mem_add;

typedef struct cache_line{
	char valid;
	mem_add tag;
	unsigned long long int time;
}cache_line_t;

typedef cache_line_t* cache_set_t;
typedef cache_set_t* cache_t;

int s = 0;
int b = 0;
int E = 0;

int set_num;
int block_size;

char *file = NULL;


void cache_init();
void simulate(char* trace);
void update(mem_add address);

cache_t cache;
mem_add set_mask;

int miss = 0; 
int hit = 0;
int eviction = 0;
unsigned long long int time = 1;


int main(int argc, char* argv[]){
	char opt;

	while((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
		switch(opt){
			case 's':
				s = atoi(optarg);
				break;
			case 'E':
				E = atoi(optarg);
				break;
			case 'b':
				b = atoi(optarg);
				break;
			case 't':
				file = optarg;
				break;
			default:
				exit(1);
		}
	}
	
	set_num = (unsigned int)(1 << s);
	block_size = (unsigned int)(1 << b);

	cache_init();
	
	simulate(file);

	printSummary(hit, miss, eviction);
	
	for(int i = 0; i < set_num; i++){
		free(cache[i]);
	}
	free(cache);

	return 0;
}

void cache_init() {
	cache = (cache_set_t*)malloc(sizeof(cache_set_t) * set_num);
	for (int i = 0; i < set_num; i++){
		cache[i] = (cache_line_t*)malloc(sizeof(cache_line_t)*E);
		for(int j = 0; j < E; j++) {
			cache[i][j].valid = 0;
			cache[i][j].tag = 0;
			cache[i][j].time = 0;
		}
	}
	set_mask = (mem_add)((1<<s) - 1);
}

void simulate(char* trace){
	FILE* tp = fopen(trace, "r");
	
	char operation;
	mem_add address;
	int block;

	while(fscanf(tp, " %c %llx,%d", &operation, &address, &block) == 3) {
		switch(operation){
			case 'L':
				update(address);
				break;
			case 'S':
				update(address);
				break;
			case 'M':
				update(address);
				update(address);
				break;
			default:
				break;
			}
	}
	fclose(tp);
}

void update(mem_add address){
	unsigned long long int eviction_time = ULONG_MAX;
	unsigned int eviction_line = 0;
	mem_add set_index = (address >> b) & set_mask;
	mem_add tag = address >> (s + b);
	
	cache_set_t cache_set = cache[set_index];

	for(int i = 0; i < E; i++){
		if(cache_set[i].valid){
			if(cache_set[i].tag == tag){
				cache_set[i].time = time++;
				hit++;
				return;
			}
		}
	}

	miss++;



	for(int i = 0; i < E; i++){
		if(eviction_time > cache_set[i].time){
			eviction_line = i;
			eviction_time = cache_set[i].time;
		}
	}
	
	if(cache_set[eviction_line].valid){
		eviction++;
	}

	cache_set[eviction_line].valid = 1;
	cache_set[eviction_line].tag = tag;
	cache_set[eviction_line].time = time++;
}
