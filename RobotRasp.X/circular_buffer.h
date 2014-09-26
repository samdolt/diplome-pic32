/* 
 * File:   circular_buffer.h
 * Author: doltsa
 *
 * Created on 18. septembre 2014, 13:14
 */

#ifndef CIRCULAR_BUFFER_H
#define	CIRCULAR_BUFFER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t M_BUFFER_SIZE;
    uint8_t * M_BEGIN;
    uint8_t * M_END;
    uint8_t * m_read;
    uint8_t * m_write;
    uint8_t m_number_of_item;
    uint8_t m_free_size;
} buffer_t;

void buffer_init(buffer_t *buffer, uint8_t *data, const uint8_t BUFFER_SIZE);
void buffer_put(buffer_t *buffer, uint8_t data);
uint8_t buffer_get(buffer_t *buffer);
uint8_t buffer_get_free_size(buffer_t *buffer);
uint8_t buffer_get_number_of_item(buffer_t *buffer);
void buffer_delete(buffer_t *buffer);

#endif	/* CIRCULAR_BUFFER_H */

