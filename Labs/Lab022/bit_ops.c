#include <stdio.h>
#include "bit_ops.h"

// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
    return (x & ((unsigned)1 << n)) >> n;
}
// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
    if(v == 1){
        *x |= ((unsigned)1 << n); 
    }else{
        *x &= ~((unsigned)1 << n);
    }
}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
    (*x) ^= ((unsigned)1 << n);
}

