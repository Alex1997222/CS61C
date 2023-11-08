#include <stddef.h>
#include <stdio.h>
#include "ex10_ll_cycle.h"

int ll_has_cycle(node *head) {
    if(head == NULL || head->next == NULL) return 0;
    node* fast_ptr = head->next;
    node* slow_ptr = head;
    while(fast_ptr!=NULL){
        if(fast_ptr == slow_ptr) return 1;   
        if(slow_ptr) slow_ptr = slow_ptr->next;  
        if(fast_ptr) fast_ptr = fast_ptr->next;
        if(fast_ptr) fast_ptr = fast_ptr->next;  
    }
    return 0;
}