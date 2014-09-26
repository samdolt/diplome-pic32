
//#include "CircularBuffer.h"



#include "circular_buffer.h"

void buffer_init(buffer_t *buffer, uint8_t *data, const uint8_t BUFFER_SIZE) {
    
    buffer->M_BUFFER_SIZE = BUFFER_SIZE;
    buffer->M_BEGIN = data;

    buffer->M_END = buffer->M_BEGIN + (BUFFER_SIZE - 1);
    buffer->m_read = buffer->M_BEGIN;
    buffer->m_write = buffer->M_BEGIN;
    buffer->m_free_size = buffer->M_BUFFER_SIZE;
    buffer->m_number_of_item = 0;
}

void buffer_put(buffer_t *buffer, uint8_t data) {
    if (buffer_get_free_size(buffer) != 0) {
        *buffer->m_write = data;
        buffer->m_free_size--;
        buffer->m_number_of_item++;

        if (buffer->m_write == buffer->M_END) {
            buffer->m_write = buffer->M_BEGIN;
        } else {
            buffer->m_write++;
        }
    } else {
        // Il n'y a plus de place pour écrire
    }
}

uint8_t buffer_get(buffer_t *buffer) {
    uint8_t data = 0;

    if (buffer_get_number_of_item(buffer) > 0) {
        data = *buffer->m_read;
        buffer->m_free_size++;
        buffer->m_number_of_item--;

        if (buffer->m_read == buffer->M_END) {
            buffer->m_read = buffer->M_BEGIN;
        } else {
            buffer->m_read++;
        }
    } else {
        // Il n'y a plus rien à lire
    }
    return data;
}

uint8_t buffer_get_free_size(buffer_t *buffer) {
    return buffer->m_free_size;
}

uint8_t buffer_get_number_of_item(buffer_t *buffer) {
    return buffer->m_number_of_item;
}

void buffer_delete(buffer_t *buffer) {
    free(buffer);
}

