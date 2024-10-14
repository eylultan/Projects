#include "include/cache.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Cache statistics counters.
uint32_t cache_total_accesses;
uint32_t cache_hits;
uint32_t cache_misses;
uint32_t cache_read_accesses;
uint32_t cache_read_hits;
uint32_t cache_write_accesses;
uint32_t cache_write_hits;

// Input parameters to control the cache.
uint32_t cache_size;
uint32_t cache_associativity;
uint32_t cache_block_size;
uint32_t numSets, numBlocks;

block_t *least_recently_used_block=NULL;
uint32_t min_last_used= UINT32_MAX;

block_t **cache;
uint32_t global_access_time=0;
/*
 * Perform a read from the memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to read from memory.
 */
int read_from_memory(uint32_t pa) 
{
	return 0;
}

/*
 * Perform a write from the cache to memory for a particular address.
 * Since this is a cache-simulation, memory is not involved.
 * No need to implement the logic for this function.
 * Call this function when cache needs to write to memory.
 */
int write_to_memory(uint32_t pa)
{
	return 0;
}

/*
 *
  Please edit the below functions for Part 1. 
  You are allowed to add more functions that may help you
  with your implementation. However, please do not add
  them in any file. All your new code should be in cache.c
  file below this line. 
 *
*/

/*
 * Initialize the cache depending on the input parameters S, A, and B 
 * and the statistics counter. The cache is declared in as extern in 
 * include/cache.h file.
 */
void initialize_cache(){

    // Determine the number of sets and blocks per set
   
	uint32_t i;

    if (cache_associativity == 1) { // Direct mapped
        numSets = 1;
        numBlocks = cache_size / cache_block_size;
    } else if (cache_associativity == 3) { // 2-way set associative
        numSets =2 ;
        numBlocks = cache_size / (2*cache_block_size); // 2 blocks per set 
    } else if (cache_associativity == 4) { // 4-way set associative
        numSets = 4;
        numBlocks = cache_size / (4*cache_block_size); // 4 blocks per set 
    } else { // Fully associative
        numSets = cache_size /cache_block_size;
        numBlocks = 1;
    }

    // Allocate memory for the cache
    cache = (block_t **)calloc(numBlocks, sizeof(block_t *));
    if (cache == NULL) {
        //fprintf(stderr, "Failed to allocate memory for cache sets\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each set in the cache
    for (i = 0; i < numBlocks; i++) {
        cache[i] = (block_t *)calloc(numSets, sizeof(block_t));
        }

    // Initialize cache statistics counters
    cache_total_accesses = 0;
    cache_hits = 0;
    cache_misses = 0;
    cache_read_accesses = 0;
    cache_read_hits = 0;
    cache_write_accesses = 0;
    cache_write_hits = 0;
    }

/*
 * Free the allocated memory for the cache to avoid memory leaks.
 */
void free_cache() {
    // Free memory for each set in the cache
    for (uint32_t i = 0; i < numBlocks; i++) {
        free(cache[i]);
    }

    free(cache);
}


// Print cache statistics.
void print_cache_statistics()
{
    printf("\n* Cache Statistics *\n");
	printf("total accesses: %d\n", cache_total_accesses);
	printf("hits: %d\n", cache_hits);
	printf("misses: %d\n", cache_misses);
	printf("total reads: %d\n", cache_read_accesses);
	printf("read hits: %d\n", cache_read_hits);
	printf("total writes: %d\n", cache_write_accesses);
	printf("write hits: %d\n", cache_write_hits);
}

/*
 * Perform a read from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 * Do seperately for each associativity. For 2 way make sure you divide by 2 not 3 ! 
 */



op_result_t read_from_cache(uint32_t pa) 
{
    
    cache_total_accesses++;
    cache_read_accesses++;
    int free_block_index=-1;
    uint32_t OFFSET_BITS, INDEX_BITS, index, tag;
    uint32_t min_last_used= UINT32_MAX;

    // Calculate OFFSET_BITS and INDEX_BITS 
    OFFSET_BITS = (uint32_t)log2(cache_block_size);
    if (cache_associativity == 1) { // Direct Mapped Cache
        INDEX_BITS = (uint32_t)log2(cache_size / cache_block_size);
    } else if (cache_associativity==2) { // Fully Associative Cache
        INDEX_BITS = 0;
    } else { 
        if (cache_associativity == 3) { // 2-way associative
        INDEX_BITS = (uint32_t)log2(cache_size / (2 * cache_block_size));
        }else if (cache_associativity == 4) { // 4-way associative
        INDEX_BITS = (uint32_t)log2(cache_size / (4 * cache_block_size));
        }
    }
    // Calculate index and tag
    index = (pa >> OFFSET_BITS) & ((1 << INDEX_BITS) - 1);
    tag = pa >> (OFFSET_BITS + INDEX_BITS);
   
    // Check if the cache line is valid and the tags match
    for (int i=0; i< numSets; i++){
        if (cache[index][i].valid && cache[index][i].tag== tag) {
            cache_hits++;
            cache_read_hits++;
            cache[index][i].last_used= ++global_access_time;
            return HIT;
        } 
    } 
        cache_misses++;
        for(int i=0;i<numSets;i++){
            if(!cache[index][i].valid){
                cache[index][i].valid=1;
                cache[index][i].tag=tag;
                cache[index][i].dirty=0;
                cache[index][i].last_used= ++global_access_time;
                write_to_memory(pa);
                return MISS;
            }

            else if (cache[index][i].last_used < min_last_used){
                //Track the least recently used block 
                least_recently_used_block=&cache[index][i];
                min_last_used=cache[index][i].last_used;
                free_block_index= i; 
            }
        }
        
        if (cache[index][free_block_index].dirty)
        {
            write_to_memory(pa);
        }
        read_from_memory(pa);

        cache[index][free_block_index].valid = 1;
        cache[index][free_block_index].tag = tag;
        cache[index][free_block_index].dirty = 0;
        cache[index][free_block_index].last_used= ++global_access_time;
        return MISS;
}


/*
 * Perform a write from the cache for a particular address.
 * Since this is a simulation, there is no data. Ignore the data part.
 * The return value is always a HIT or a MISS and never an ERROR.
 */
op_result_t write_to_cache(uint32_t pa) {
    cache_total_accesses++;
    cache_write_accesses++;
    uint32_t OFFSET_BITS, INDEX_BITS, index, tag;
    int free_block_index=-1;
    uint32_t min_last_used= UINT32_MAX;

    // Calculate OFFSET_BITS and INDEX_BITS 
    OFFSET_BITS = (uint32_t)log2(cache_block_size);
    if (cache_associativity == 1) { // Direct Mapped Cache
        INDEX_BITS = (uint32_t)log2(cache_size / cache_block_size);
    } else if (cache_associativity==2) { // Fully Associative Cache
        INDEX_BITS = 0;
    } else { 
        if (cache_associativity == 3) { // 2-way associative
        INDEX_BITS = (uint32_t)log2(cache_size / (2 * cache_block_size));
        }else if (cache_associativity == 4) { // 4-way associative
        INDEX_BITS = (uint32_t)log2(cache_size / (4 * cache_block_size));
        }
    }
    // Calculate index and tag
    index = (pa >> OFFSET_BITS) & ((1 << INDEX_BITS) - 1);
    tag = pa >> (OFFSET_BITS + INDEX_BITS);

    for (int i=0; i< numSets; i++){
        if (cache[index][i].valid && cache[index][i].tag== tag) {
            cache_hits++;
            cache_write_hits++;
            cache[index][i].dirty=1;
            cache[index][i].last_used=++global_access_time;
            return HIT;
        } 
    } 
        cache_misses++;
        for(int i=0;i<numSets;i++){
            if(!cache[index][i].valid){
                cache[index][i].valid=1;
                cache[index][i].tag=tag;
                cache[index][i].dirty=0;
                cache[index][i].last_used= ++global_access_time;
                write_to_memory(pa);
                return MISS;
            }

            else if (cache[index][i].last_used < min_last_used){
                //Track the least recently used block 
                least_recently_used_block=&cache[index][i];
                min_last_used=cache[index][i].last_used;
                free_block_index= i; 
            }
        }
        
        if (cache[index][free_block_index].dirty)
        {
            write_to_memory(pa);
        }
        read_from_memory(pa);
        
        cache[index][free_block_index].valid = 1;
        cache[index][free_block_index].tag = tag;
        cache[index][free_block_index].dirty = 0;
        cache[index][free_block_index].last_used= ++global_access_time;
        return MISS;
    
}

// Process the S parameter properly and initialize `cache_size`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_S(int opt, char *optarg)
{
	if (optarg == NULL ) {
    	//printf("Error: Null pointer received for option %c\n", opt);
        return -1;
    }
	int value = atoi(optarg); 
    if (value <= 0 || (value & (value - 1)) != 0) {
        // Size is not positive or not a power of 2
        return -1;
    }
    cache_size = value;
    return 0;
}

// Process the A parameter properly and initialize `cache_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_A(int opt, char *optarg){
	if (optarg == NULL) {
    	//printf("Error: Null pointer received for option %c\n", opt);
        return -1;
    }
	int associativity = atoi(optarg); 

    // Check if associativity is valid
    if (associativity < 1 || associativity > 4) {
        //printf("Error: Invalid cache associativity value '%s' for option %c\n", optarg, opt);
        return -1;
    }

    cache_associativity = associativity;

    return 0;
}

// Process the B parameter properly and initialize `cache_block_size`.
// Return 0 when everything is good. Otherwise return -1.
// Multiple of 4 !! 
int process_arg_B(int opt, char *optarg) {
	
	if (optarg == NULL) {
    	//printf("Error: Null pointer received for option %c\n", opt);
        return -1;
    }
    int value = atoi(optarg);
    
    if (value <= 0) {
        //printf("Error: Invalid block size %d provided for option %c\n", value, opt);
        return -1;
    }
    
    if (cache_size % value != 0) {
        //printf("Error: Cache size (%d) is not a multiple of block size (%d)\n", cache_size, value);
        return -1;
    }

    // Check if value is a multiple of 4
    if (value % 4 != 0) {
        //printf("Error: Block size %d is not a multiple of 4 for option %c\n", value, opt);
        return -1;
    }

 
    cache_block_size = value;
    return 0;
}

// When verbose is true, print the details of each operation -- MISS or HIT.
void handle_cache_verbose(memory_access_entry_t entry, op_result_t ret)
{
	if (ret == ERROR) {
		printf("This message should not be printed. Fix your code\n");
	}

    char operation_type = (entry.accesstype == READ) ? 'R' : 'W';
    const char *result_str= (ret == HIT) ? "HIT" : "MISS";

    printf("%c 0x%08X %s\n", operation_type, entry.address,result_str);
}

// Check if all the necessary paramaters for the cache are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_cache_parameters_valid() {

    if (cache_size <= 0 || cache_associativity <= 0 || cache_block_size <= 0) {
        return -1;
    }

  
    if ((cache_size & (cache_size - 1)) != 0 ||
        (cache_block_size & (cache_block_size - 1)) != 0 ) {
        return -1;
    }

    
   
    int effective_associativity;

    if (cache_associativity == 1) {
        effective_associativity = 1; // Direct Mapped
    } else if (cache_associativity == 2) {
        effective_associativity = 1; // Fully Associative
    } else if (cache_associativity == 3) {
        effective_associativity = 2; // 2-way Associative
    } else if (cache_associativity == 4) {
        effective_associativity = 4; // 4-way Associative
    } else {
        return -1; 
    }

    if (cache_associativity !=2 && cache_size % (effective_associativity * cache_block_size) != 0) {
        return -1;
    }
    return 0 ;
}
