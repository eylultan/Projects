#include "tlb.h"
#include <stdlib.h>

// Input parameters to control the tlb.
uint32_t tlb_entries;
uint32_t tlb_associativity;

// TLB statistics counters.
uint32_t tlb_total_accesses;
uint32_t tlb_hits;
uint32_t tlb_misses;
uint32_t numEntries, num_Sets;

tlb_entry_t *tlb_least_recently_used_block=0;
uint32_t tlb_min_last_used= UINT32_MAX;
uint32_t tlb_global_access_time=0;

tlb_entry_t** tlb;

// Check if all the necessary paramaters for the tlb are provided and valid.
// Return 0 when everything is good. Otherwise return -1.
int check_tlb_parameters_valid() {
    
    if (tlb_entries >= 2 && tlb_associativity >= 1 && tlb_associativity <= 4) {
        return 0; 
    } else {
        return -1; 
    }
}
/*
 * Initialize the "tlb" depending on the input parameters T and L. 
 * The "tlb" is declared in as extern in include/tlb.h file.
 */
void initialize_tlb()
{
    uint32_t i;
    if (tlb_associativity == 1) { // Direct mapped
        numEntries = 1;
        num_Sets = tlb_entries;
    } else if (tlb_associativity == 3) { // 2-way set associative
        numEntries = 2;
        num_Sets = tlb_entries / 2; // 2 entries per set
    } else if (tlb_associativity == 4) { // 4-way set associative
        numEntries = 4;
        num_Sets = tlb_entries / 4; // 4 entries per set 
    } else { // Fully associative
        numEntries = tlb_entries;
        num_Sets = 1;
    }

    tlb = (tlb_entry_t**)calloc(num_Sets, sizeof(tlb_entry_t *));
    if (tlb == NULL) {
    //  fprintf(stderr, "Failed to allocate memory for TLB sets\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each set
    for (i = 0; i < num_Sets; i++) {
        tlb[i] = (tlb_entry_t *)calloc(numEntries, sizeof(tlb_entry_t));
        if (tlb[i] == NULL) {
           // fprintf(stderr, "Failed to allocate memory for TLB entries\n");
            exit(EXIT_FAILURE);
        }
    tlb_total_accesses = 0;
    tlb_hits = 0;
    tlb_misses = 0;
    }
}

// Process the T parameter properly and initialize `tlb_entries`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_T(int opt, char *optarg) {
    if (optarg == NULL) {
        //fprintf(stderr, "Error: Missing argument for entry -%c\n", opt);
        return -1;
        
    }

    int entries = atoi(optarg); 

    // at least 2
    if (entries < 2 || (entries & (entries - 1)) != 0) {
       
        return -1;
    }

    tlb_entries = entries; 
    return 0; 
}

// Process the L parameter properly and initialize `tlb_associativity`.
// Return 0 when everything is good. Otherwise return -1.
int process_arg_L(int opt, char *optarg) {
    int associativity = atoi(optarg); 

   
    if (associativity < 1 || associativity > 4) {
        return -1; 
    }

    tlb_associativity = associativity; 
    return 0; 
}

// Check if the tlb hit or miss.
// Extract the VPN from the address and use it.
// Keep associativity in mind while searching.
int check_tlb(uint32_t address) {
    tlb_total_accesses++;  
    uint32_t VPN = address >> 12;  // Determine VPN
    uint32_t index;

    if (tlb_associativity == 1) { // Direct Mapped 
        index = VPN & (tlb_entries - 1);
    } else if (tlb_associativity == 2) { // Fully Associative 
        index = 0; // No index 
    } else if (tlb_associativity == 3) { // 2-way associative 
        index = VPN & ((tlb_entries/2) - 1);
    } else if (tlb_associativity == 4) { // 4-way associative 
        index = VPN & ((tlb_entries/4) - 1); 
    }

    for (int i=0; i< numEntries; i++){  
        // Valid and VPN Match check
        if (tlb[index][i].valid && tlb[index][i].VPN == VPN) {
            // TLB hit
            tlb_hits++;
            tlb[index][i].last_used= ++tlb_global_access_time;
            return tlb[index][i].PPN;
        }
    }

    tlb_misses++;
    return -1;
}
    

void set_dirty_bit_in_tlb(uint32_t address){
    uint32_t VPN = address >> 12;  // Determine VPN
    uint32_t index;

    if (tlb_associativity == 1) { // Direct Mapped 
        index = VPN & (tlb_entries - 1);
    } else if (tlb_associativity == 2) { // Fully Associative 
        index = 0; // No index 
    } else if (tlb_associativity == 3) { // 2-way associative 
        index = VPN & ((tlb_entries/2) - 1);
    } else if (tlb_associativity == 4) { // 4-way associative 
        index = VPN & ((tlb_entries/4) - 1); 
    }

    for (int i=0; i< numEntries; i++){  
        // Valid and VPN Match check
        if (tlb[index][i].valid && tlb[index][i].VPN == VPN) {
            tlb[index][i].dirty=1;
            break;
        }
    }
}

// LRU replacment policy has to be implemented.
void insert_or_update_tlb_entry(uint32_t address, uint32_t PPN){
    // if the entry is free, insert the entry
    // if the entry is not free, identify the victim entry and replace it
    //set PPN for VPN in tlb
    //set valid bit in tlb
    // For LRU
    int free_block_index=-1;
    uint32_t VPN = address >> 12;  // Determine VPN
    uint32_t index;
    tlb_min_last_used =UINT32_MAX;

  

    if (tlb_associativity == 1) { // Direct Mapped 
        index = VPN & (tlb_entries - 1);
    } else if (tlb_associativity == 2) { // Fully Associative 
        index = 0; // No index
    } else if (tlb_associativity == 3) { // 2-way associative 
        index = VPN & ((tlb_entries/2) - 1);
    } else if (tlb_associativity == 4) { // 4-way associative 
        index = VPN & ((tlb_entries/4) - 1); 
    }

    
    for(int i=0;i<numEntries;i++){
            if(!tlb[index][i].valid){
                tlb[index][i].valid=1;
                tlb[index][i].VPN=VPN;
                tlb[index][i].PPN=PPN;
                tlb[index][i].dirty=0;
                tlb[index][i].last_used = ++tlb_global_access_time;
                return;
            }

            else if (tlb[index][i].last_used < tlb_min_last_used){
                //LRU
                tlb_least_recently_used_block=&tlb[index][i];
                tlb_min_last_used=tlb[index][i].last_used;
                free_block_index= i; 

            }
        }

        tlb[index][free_block_index].valid = 1;
        tlb[index][free_block_index].VPN= VPN;
        tlb[index][free_block_index].PPN= PPN;
        tlb[index][free_block_index].dirty = 0; 
        tlb[index][free_block_index].last_used = ++tlb_global_access_time;
}

// print pt entries as per the spec
void print_tlb_entries() {
    printf("\nTLB Entries (Valid-Bit Dirty-Bit VPN PPN)\n");

    for (uint32_t set = 0; set < num_Sets; set++) {

        for (uint32_t entry = 0; entry < numEntries; entry++) {
            if (tlb[set][entry].VPN == 0 && tlb[set][entry].PPN == 0)
            {
                printf("%d %d - -\n", tlb[set][entry].valid, tlb[set][entry].dirty);
            }
            else{
            tlb_entry_t current_entry = tlb[set][entry];
            printf("%d %d 0x%05x 0x%05x\n",current_entry.valid,current_entry.dirty,current_entry.VPN,current_entry.PPN);
            }
    }
}
}

// print tlb statistics as per the spec
void print_tlb_statistics(){
    //print the tlb statistics
    printf("\n* TLB Statistics *\n");
    printf("total accesses: %d\n", tlb_total_accesses);
    printf("hits: %d\n", tlb_hits);
    printf("misses: %d\n", tlb_misses);
}