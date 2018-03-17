#include "buffer.h"

#include "hal_general.h"

void Push(buffer_t *buffer, char data) {
	uint8_t overwrite = 0;
	// disable interrupts to make the function interrupt safe
	BlockInterrupts();
	
	/*	If front = rear and the buffer is not empty then overwrite what is on
		the front, this allows use as a revolving circular buffer for receivers */
	if(buffer->front == buffer->rear && buffer->size) {
		overwrite = 1;
		buffer->front++;
		// check to make sure front isn't past the end of the buffer
		if(buffer->front > buffer->buffer_end)
			buffer->front = buffer->buffer_start;
		buffer->size--;
	}
	
	// push data onto rear location and increment rear pointer
	*buffer->rear++ = data;
	// check to make sure rear isn't past the end of the buffer
	if(buffer->rear > buffer->buffer_end) buffer->rear = buffer->buffer_start;
	// Increment the size of the buffer
	buffer->size++;

	// If there is a callback, call it
	if(buffer->Callback) buffer->Callback(buffer);

	//ReEnable Interrupts
	RestoreInterrupts();

	if(overwrite && buffer->OverwriteCallback){
		buffer->OverwriteCallback();
	}
}

char Pop(buffer_t *buffer) {
	char data;
	//x in light of what was documented in the Push function in this file
	//x there is nothing in the Pop function which is not plain given the
	//x good use of variable names in this module
	BlockInterrupts();
	data = *buffer->front;
	if(buffer->size != 0) {
		buffer->front++;
		if(buffer->front > buffer->buffer_end) buffer->front = buffer->buffer_start;
		buffer->size--;
	}
	RestoreInterrupts();
	return data;
}

uint16_t GetSize(buffer_t *buffer) {
	return buffer->size;
}

void BufferInit(buffer_t *buffer, char *data_array, uint16_t max_size) {
	buffer->buffer_start = data_array;
	buffer->front = data_array;
	buffer->rear = data_array;
	buffer->buffer_end = data_array + max_size - 1;
	buffer->size = 0;
	buffer->Callback = 0;
}

void BufferSetCallback(buffer_t * buffer, void (*Callback)(buffer_t * buffer)) {
	buffer->Callback = (void (*)(void *))Callback; // cast callback to void pointer
	//x a void pointer was used in the structure for buffer_t becuase a buffer_t pointer
	//x couldn't be used since buffer_t hadn't been defined inside of the definition of
	//x buffer_t. See the .h file for more info.
}

void BufferClearCallback(buffer_t * buffer) { buffer->Callback = 0; }

char PushData(buffer_t * buffer, char * data, uint16_t length) {
	BlockInterrupts();
	// check if there is enough space in the buffer to load the data
	if(buffer->buffer_end - buffer->buffer_start + 1 - buffer->size >= length) {
		buffer->size += length;
		while(length) {
			// push data onto rear location and increment rear
			*buffer->rear++ = *data++;
			// check to make sure rear isn't past the end of the buffer
			if(buffer->rear > buffer->buffer_end) buffer->rear = buffer->buffer_start;
			length--;
		}
		if(buffer->Callback) buffer->Callback(buffer);

		RestoreInterrupts();

		return BUFFER_PUSH_SUCCEEDED;
	}

	RestoreInterrupts();

	return BUFFER_PUSH_FAILED;
}

