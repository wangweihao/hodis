/*======================================================
    > File Name: hodis_lockfree_list.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月08日 星期二 13时25分50秒
 =======================================================*/

#ifndef _HODIS_KFIFO_H_
#define _HODIS_KFIFO_H_

/* lockfree list 
 * A reader and a writter guarantee 
 * thread safely */

#include <malloc.h>

struct kfifo;
unsigned int roundup_pow_of_two(unsigned int size);
int kfifo_alloc(struct kfifo *fifo, unsigned int size, size_t esize, size_t mask);

struct kfifo {
    unsigned int    in;         /* write index */
    unsigned int    out;        /* read  index*/
    unsigned int    mask;
    unsigned int    esize;      /* one element of size */
    void            *data;      /* real storage space */
};

int kfifo_alloc(struct kfifo *fifo, unsigned int size, 
        size_t esize, size_t mask)
{
    /* 
     * round down to the next power of 2
     * */
    size = roundup_pow_of_two(size);

    fifo->in = 0;
    fifo->out = 0;
    fifo->esize =  esize;

    if (size < 2) {
        fifo->data = NULL;
        fifo->mask = 0;
        return -1;
    }

    fifo->data = malloc(size * esize);

    if(!fifo->data) {
        fifo->mask = 0;
        return -1;
    }

    return 0;
}



unsigned int roundup_pow_of_two(unsigned int size){
    /*  0000001 */
    unsigned int rel_size = 1;
    while (rel_size < size) {
        rel_size <<= 1;
    }

    return rel_size;
}

#endif
