#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* buffer;
	size_t buffer_length;
	 ssize_t input_length; //capable of storing -1 for error indication
} InputBuffer;

typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
	StatementType type;
} Statement;

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

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
	if (strcmp(input_buffer->buffer, ".exit") == 0) {
		close_input_buffer(input_buffer);
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}


//this will be our "SQL compiler" right now it only understands to words
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
	if (strncmp(input_buffer->buffer, "insert", 6) == 0) { //will be followed by data
		//we set what type is so the in the excution we know of what type it is
		statement->type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}
	if (strcmp(input_buffer->buffer, "select") == 0) { // will not be followed by data
		statement->type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}

	return PREPARE_UNRECOGNIZED_STATEMENT;
}


// this will be our virtual machine. The one that executes the code
void execute_statement(Statement* statement){
	switch(statement->type) {
		case (STATEMENT_INSERT):
			printf("This is where we would do an insert. \n");
			break;
		case (STATEMENT_SELECT):
			printf("This is where we would do a select. \n");
			break;
	}
}



int main(int argc, char* argv[]) {
	//create a pointer with the structure InputBuffer using the function
	InputBuffer* input_buffer = new_input_buffer();

	while (true) {
		print_prompt();
		read_input(input_buffer);

		if (input_buffer->buffer[0] == '.') {
			switch (do_meta_command(input_buffer)) {
				// sucess case will be implemented in the future need to add because it is in th enum even though the function doesnt return it
				case (META_COMMAND_SUCCESS):
					continue;
				case (META_COMMAND_UNRECOGNIZED_COMMAND):
					printf("Unrecognized command '%s'\n", input_buffer->buffer);
					continue;
			}
		}
	

		Statement statement;
		switch (prepare_statement(input_buffer, &statement)) {
			case (PREPARE_SUCCESS):
				break;
			case (PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keyword at start of '%s'.\n",
				input_buffer->buffer);
				continue;
		}
		execute_statement(&statement);
		printf("Executed.\n");
	}

}
