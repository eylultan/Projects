#include "pt.h"
#include "common.h"
#include <stdlib.h>
#include "tlb.h"
#include "types.h"
#include "ll.h"

const uint32_t TOTAL_PAGES = 256; 
const uint32_t maxTotal_Pages = 1 << 14;

page_t* free_page_list;
page_t* used_page_list;
pt_entry_t* page_table;

// Page table statistics counters.
uint32_t page_table_total_accesses;
uint32_t page_table_faults;
uint32_t page_table_faults_with_dirty_page;



void initialize_pt_system() {
    // Initialize the page table and free page list
    init_pt();
    init_free_page_list(&free_page_list);
}

void init_pt() {
    // Allocate memory for the page table
    page_table = (pt_entry_t *)calloc (maxTotal_Pages, sizeof(pt_entry_t));
    if (page_table == NULL) {
        //fprintf(stderr, "Failed to allocate memory for the page table\n");
        exit(EXIT_FAILURE);
    }
}

void init_free_page_list(page_t** free_page_list){
    for (int i = 255; i >=0; i--)
    {
        page_t *new_page = (page_t *)malloc(sizeof(page_t));
        new_page->ppn = i;
        new_page->next = NULL;
        insert_in_ll(free_page_list, new_page);
    }
}

page_t *find_page_with_ppn(page_t *head, uint32_t ppn) {
    page_t *current = head;
    while (current != NULL) {
        if (current->ppn == ppn) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void move_page_to_head(page_t **head, uint32_t ppn) {
    if (*head == NULL || (*head)->ppn == ppn) {
        return;
    }
    page_t *current = *head;
    page_t *prev = NULL;

    while (current != NULL && current->ppn != ppn) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev != NULL) {
            prev->next = current->next;
        }
        current->next = *head;
        *head = current;
    }
}

void update_used_page_list(uint32_t VPN) {
    uint32_t PPN = page_table[VPN].PPN;

    page_t *page = find_page_with_ppn(used_page_list, PPN);

    if (page == NULL) {
        page = (page_t *)malloc(sizeof(page_t));
        if (page == NULL) {
            //fprintf(stderr, "Failed to allocate memory for a new page\n");
            exit(EXIT_FAILURE);
        }
        page->ppn = PPN;
        page->next = NULL;
        page->page_table_entry = &page_table[VPN];
        insert_in_ll(&used_page_list, page);
    } else {
        move_page_to_head(&used_page_list, PPN);
    }

    page_table[VPN].accessed = 1;
}

int check_page_table(uint32_t address) {
    uint32_t VPN = address >> 12; // Shift right by the offset size 
    page_table_total_accesses++;

    if (VPN >= maxTotal_Pages) {
        //printf("VPN: %x, TOTAL_PAGES: %X", VPN, TOTAL_PAGES);
        return -1;
    }

    
    pt_entry_t entry = page_table[VPN];

    if (entry.present) {
        update_used_page_list(VPN);
        return entry.PPN;
    } else {
        page_table_faults++;
        if (entry.dirty == 1) {
            page_table_faults_with_dirty_page++;
        }
        return -1;
    }
}

void update_page_table(uint32_t address, uint32_t PPN) {
    uint32_t VPN = address >> 12;

    if (VPN < maxTotal_Pages) {
        page_table[VPN].PPN = PPN;
        page_table[VPN].present = 1;
        update_used_page_list(VPN);
        insert_or_update_tlb_entry(address, PPN);
    }
}

void set_dirty_bit_in_page_table(uint32_t address) {
    uint32_t VPN = address >> 12;

    if (VPN < maxTotal_Pages) {
        page_table[VPN].dirty = 1;
    }
}

page_t* get_tail_of_ll(page_t *head) {
    if (head == NULL) return NULL;
    page_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

page_t *get_victim_page() {
    page_t *current = used_page_list;
    page_t *prev = NULL;

    if (current == NULL) {
        //printf("NULL victim page, PPN %u\n", current->ppn);
        return NULL;
    }

    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    if (prev != NULL) {
        prev->next = NULL;  // Remove the last element from the list
    } else {
        // only one element
        used_page_list = NULL;
    }

    
    uint32_t PPN = current->ppn;
    for (uint32_t vpn = 0; vpn < TOTAL_PAGES; vpn++) {
        if (page_table[vpn].present && page_table[vpn].PPN == PPN) {
            page_table[vpn].present = 0;
            page_table[vpn].dirty = 0;
            break; 
        }
    }

    //printf("Selecting victim page, PPN %u\n", current->ppn);
    return current;
}


void access_page(uint32_t VPN) {
    page_t* page = find_page_with_ppn(used_page_list, page_table[VPN].PPN);
    if (page == NULL) {
        page = (page_t*)malloc(sizeof(page_t));
        if (page == NULL) {
            //fprintf(stderr, "Failed to allocate memory for a new page\n");
            exit(EXIT_FAILURE);
        }
        page->ppn = page_table[VPN].PPN;
        insert_in_ll(&used_page_list, page);
    } else {
        move_page_to_head(&used_page_list, page->ppn);
    }
}

page_t *get_free_page() {
    if (free_page_list == NULL) {
        return NULL;
    } else {
        return delete_from_top_of_ll(&free_page_list);
    }
}

void print_pt_entries() {
    printf("\nPage Table Entries (Present-Bit Dirty-Bit VPN PPN)\n");

    for (uint32_t vpn = 0; vpn < maxTotal_Pages; vpn++) {
        pt_entry_t current_entry = page_table[vpn];
        if (current_entry.present) {
            printf("%d %d 0x%05x 0x%05x\n",
                   current_entry.present,
                   current_entry.dirty,
                   vpn,
                   current_entry.PPN);
        }
    }
}

void print_pt_statistics() {
    printf("\n* Page Table Statistics *\n");
    printf("total accesses: %d\n", page_table_total_accesses);
    printf("page faults: %d\n", page_table_faults);
    printf("page faults with dirty bit: %d\n", page_table_faults_with_dirty_page);
}