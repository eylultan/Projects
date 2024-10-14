#ifndef PT_H_
#define PT_H_

#include <stdio.h>  
#include <stdint.h>
#include "types.h"

// typedef struct
// {
//     uint8_t present;
//     uint8_t dirty;
//     uint32_t PPN; // this will reflect the physical address of the page
// } pt_entry_t;

extern page_t* free_page_list;
extern page_t* used_page_list;
extern pt_entry_t* page_table;

void update_page_table(uint32_t address, uint32_t PPN);
int check_page_table(uint32_t address);
page_t *get_victim_page();
void initialize_pt_system();
void print_pt_statistics();
void init_pt();
void init_free_page_list(page_t** free_page_list);
void set_dirty_bit_in_page_table(uint32_t address);
page_t *get_free_page();
void print_pt_entries();
void update_used_page_list(uint32_t VPN);
void update_page_table(uint32_t address, uint32_t PPN);
page_t* get_tail_of_ll(page_t *head);
void move_page_to_head(page_t **head, uint32_t ppn);
page_t *find_page_with_ppn(page_t *head, uint32_t ppn);
void access_page(uint32_t VPN);

#endif /* PT_H_ */