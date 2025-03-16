#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* buffer;
	size_t buffer_length;
	 ssize_t input_length; //capable of storing -1 for error indication
} InputBuffer;


//this function will create a pointer with the strucutre InputBuffer
InputBuffer* new_input_buffer() {
	 //Creates a pointer and alocates the same memory size the strucutre has
	InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
	//initialize each variable of the pointer
	input_buffer->buffer = NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length = 0;

	return input_buffer;
}

void print_prompt() {
	printf("db > ");
}

void read_input(InputBuffer* input_buffer){
	//the function getline(char **lineptr, size_t *n, FILE *stream); stores the read line in the buffer and the size in buffer_length
	ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	if (bytes_read <=0) {
		 printf("Error reading input\n");
		exit(EXIT_FAILURE);
	}

	// Ignore trailing newline "\n"
	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
	free(input_buffer->buffer); //getline allocates memory for input_buffer->buffer in read_input
	free(input_buffer);
}

int main(int argc, char* argv[]) {
	// ceate a pointer with the structure InputBuffer using the function
	InputBuffer* input_buffer = new_input_buffer();

	while (true) {
		print_prompt();
		read_input(input_buffer);

		if (strcmp(input_buffer->buffer, ".exit") == 0) {
			close_input_buffer(input_buffer);
			exit(EXIT_SUCCESS);
		}
		else {
			printf("Unrecognized command '%s'.\n", input_buffer->buffer);
		}
	}
}
