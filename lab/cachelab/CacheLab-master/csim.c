/*
Name: Yanjie Ze
Student ID: 519021910706
*/

#include "cachelab.h"
#include <getopt.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>


int num_s;// to store number of set index bits
int num_E;// to store number of lines per set
int num_b;// to store number of block bits
char tgt_file[100];// to store target file name
FILE *file_pointer;

int S;// num of sets
int B;// block size
int E;// num of lines per set
int current_time;

/*results param*/
int hits;
int misses;
int evictions;

/* basic data structure for caches*/
typedef struct CACHE_LINE
{
    int valid_bit;
    int tag;
    int time;
}cache_line;


cache_line **cache;

/* function for "-h" command , print help*/
void print_help();
/* initialize cache */
void cache_init();
/* parse each operation in target file */
void operation_parse();
/* free cache */
void cache_free();
/* update cache */
void cache_update(unsigned int address);
/* update time */
void time_update();

int main(int argc, char*argv[])
{
    int command_param; // get the command from getopt()

    num_b = -1;
    num_E = -1;
    num_s = -1;

    // parse command
    while ((command_param = getopt(argc,argv,"s:E:b:t:hv"))!= -1)
    {
        
        switch (command_param)
        {
        case 'h':
            print_help();
            break;

        case 's':
            num_s = atoi(optarg);
            break;

        case 'E':
            num_E = atoi(optarg);
            break;

        case 'b':
            num_b = atoi(optarg);
            break;

        case 't':
            strcpy(tgt_file, optarg);
            break;

        default:
            printf("./csim: Wrong required command line argument\n");
            print_help();
            break;
        }
    }
    
    // Basic Check
    if(num_b<= 0 || num_E<=0 || num_s<=0)
    {
        return -1;
    }
    file_pointer = fopen(tgt_file, "r");
    if(file_pointer==NULL)
    {
        printf("File Error: Can't Open File.\n");
        return -1;
    }


    hits = 0;
    misses = 0;
    evictions = 0;

    S = 1<<num_s;
    B = 1<<num_b;
    E = num_E;

    current_time = 0;

    cache_init();

    operation_parse();
    
    cache_free();

    fclose(file_pointer);

    printSummary(hits, misses, evictions);

    return 0;
}


void cache_init()
{
    cache = (cache_line**)malloc(sizeof(cache_line*) * S);
    for(int i=0; i<S; ++i)
    {
        cache[i] = (cache_line*)malloc(sizeof(cache_line) * E);
        for(int j=0; j<E; ++j)
        {
            cache[i][j].tag = -1;
            cache[i][j].time = -1;
            cache[i][j].valid_bit = 0;// set to invalid
        }
    }
}

void operation_parse()
{
    char operation;
    unsigned int address;
    int size;
    char comma;
    
    while (fscanf(file_pointer, " %c %xu%c%xu", &operation, &address,&comma, &size)>0)
    {
        switch (operation)
        {
        case 'L': // Load Data
            cache_update(address);
            break;
        case 'M':// Modify Data
            cache_update(address);// Modiy also need Store

        case 'S': // Store Data
            cache_update(address);
            break;

        default:
            break;
        }
        time_update();
    }
    
}


void cache_free()
{
    for(int i=0; i<S; ++i)
        free(cache[i]);
    free(cache);
}

void print_help()
{
    printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
 "-h: Optional help flag that prints usage info\n"
 "-s <s>: Number of set index bits (S = 2^s is the number of sets)\n"
 "-E <E>: Associativity (number of lines per set)\n"
 "-b <b>: Number of block bits (B = 2^b is the block size)\n"
 "-t <tracefile>: Name of the valgrindtrace to replay\n");
}

void cache_update(unsigned int address)
{
    int tag_idx = (address>>(num_b+num_s));
    int set_idx = (address>>num_b) & ((0xFFFFFFFF)>>(64-num_s));
    
    for(int i=0; i<E; ++i)// search in set
    {   // if hit
        if((cache[set_idx][i].tag == tag_idx) && cache[set_idx][i].valid_bit==1)
        {
            hits ++;// hit
            cache[set_idx][i].time = current_time;
            return;
        }
    }

    for(int i=0; i<E; ++i)// not hit
    {
        if(cache[set_idx][i].valid_bit == 0)//  invalid, means empty
        {
            cache[set_idx][i].tag = tag_idx;
            cache[set_idx][i].time = current_time;
            cache[set_idx][i].valid_bit = 1;
            misses ++;// miss
            return;
        }
    }

    // not hit and not empty, need eviction
    evictions ++;
    misses ++;
    // We use LRU here
    int min_time = 10000;
    int min_idx;
    for(int i=0; i<E; ++i)
    {   
        if(cache[set_idx][i].time<min_time)
        {
            min_time = cache[set_idx][i].time;
            min_idx = i;
        }
    }
    cache[set_idx][min_idx].tag = tag_idx;
    cache[set_idx][min_idx].time = current_time;
} 

void time_update()
{
    current_time ++;
}