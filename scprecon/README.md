# minishell

#### tputs:

In C programming, tputs() is a function that's commonly used in terminal manipulation, particularly for dealing with terminal capabilities such as moving the cursor, clearing the screen, changing text attributes, and more. It's part of the "terminfo" library which provides a database of terminal capabilities.

Here's the prototype of the tputs() function:

```c
int tputs(const char *str, int affcnt, int (*putc)(int));
```

    str: The string containing the terminal capabilities sequence to be sent to the terminal.
    affcnt: An integer representing the number of lines affected by the operation (not often used, can usually be set to 1).
    putc: A pointer to a function that will be used to output characters to the terminal.

Here's a simple example demonstrating the usage of tputs() to clear the screen:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    tputs("clear", 1, putchar); // Send the "clear" capability to clear the screen
    return 0;
}
```

In this example:

    setupterm() initializes the terminal. It takes three arguments: term (the terminal type, NULL uses the value of the TERM environment variable), filedes (the file descriptor for the terminal, fileno(stdout) gets the file descriptor for standard output), and errret (a pointer to an integer to receive error codes, set to (int *)0 to ignore errors).

    tputs("clear", 1, putchar) sends the capability string associated with the "clear" operation to the terminal. The second argument, 1, indicates that one line of the terminal is affected. putchar is a pointer to the putchar function from the standard library, which will be used to output characters to the terminal.

This example clears the screen by sending the appropriate terminal capability sequence, as defined in the terminfo database.
###

#### tgoto:

In C programming, tgoto() is a function that's often used in conjunction with tputs() for terminal manipulation. It is used to build control strings for cursor positioning and other cursor-related operations.

Here's the prototype of the tgoto() function:

```c

char *tgoto(const char *cap, int col, int row);
```
    cap: The name of the terminal capability being used.
    col: The column position to move the cursor to.
    row: The row position to move the cursor to.

The tgoto() function constructs a string containing the necessary terminal control sequences to position the cursor at the specified column and row.

Here's a simple example demonstrating the usage of tgoto() to generate a cursor positioning sequence:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    int col = 10; // Desired column
    int row = 5; // Desired row
    char *cursor_move_seq = tgoto(tgetstr("cm", NULL), col, row); // Get cursor movement sequence
    tputs(cursor_move_seq, 1, putchar); // Move cursor to (col, row)
    printf("Cursor moved to row %d, column %d\n", row, col);
    return 0;
}
```

In this example:

    setupterm() initializes the terminal, similar to the previous example.
    tgetstr("cm", NULL) retrieves the control sequence for cursor movement (cm) from the terminfo database. This returns a string like \033[%i%p1%d;%p2%dH, where %p1%d is replaced with the row and %p2%d is replaced with the column.
    tgoto(tgetstr("cm", NULL), col, row) generates the actual cursor movement sequence for the desired row and column.
    tputs(cursor_move_seq, 1, putchar) sends the cursor movement sequence to the terminal using tputs().
    The printf() statement simply prints a message indicating that the cursor has been moved to the specified row and column.

This example moves the cursor to the specified row (5) and column (10) on the terminal.
###
###
###
#### tgetstr:
###
The tgetstr() function is part of the ncurses library and is used to retrieve terminal capabilities from the terminfo database. It takes the name of a terminal capability as an argument and returns the corresponding string representation of that capability.

Here's the prototype of the tgetstr() function:

```c

char *tgetstr(const char *capname, char **area);
```
    capname: The name of the terminal capability you want to retrieve.
    area: A pointer to an area in memory where tgetstr() can store data. This is typically set to NULL, in which case tgetstr() will allocate memory for the capability string.

Here's a simple example demonstrating the usage of tgetstr() to retrieve the cm (cursor movement) capability:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    char *cursor_move_cap = tgetstr("cm", NULL); // Retrieve the cm capability
    printf("Cursor movement capability: %s\n", cursor_move_cap);
    return 0;
}
```
In this example:

    setupterm() initializes the terminal, similar to previous examples.
    tgetstr("cm", NULL) retrieves the control string associated with the cm capability (cursor movement) from the terminfo database.
    The retrieved control string is then printed using printf().

The output will be the actual control sequence associated with the cm capability, such as \033[%i%p1%d;%p2%dH, which is then used with tgoto() to position the cursor on the terminal screen.

For example, the cm capability string might look like \033[%i%p1%d;%p2%dH, where:

    \033 is the escape character.
    %i specifies the initialization string, which is often empty in modern terminals.
    %p1%d is a placeholder for the row position.
    %p2%d is a placeholder for the column position.
    H is the final character indicating the end of the control sequence.
###
###
###
###
#### tgetnum:

The tgetnum() function is part of the ncurses library and is used to retrieve numeric terminal capabilities from the terminfo database. It takes the name of a numeric capability as an argument and returns the corresponding value.

Here's the prototype of the tgetnum() function:

```c
int tgetnum(const char *capname);
```
    capname: The name of the numeric terminal capability you want to retrieve.

Here's a simple example demonstrating the usage of tgetnum() to retrieve the number of columns in the terminal:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    int num_columns = tgetnum("cols"); // Retrieve the number of columns
    printf("Number of columns: %d\n", num_columns);
    return 0;
}
```

In this example:

    setupterm() initializes the terminal, similar to previous examples.
    tgetnum("cols") retrieves the number of columns in the terminal from the terminfo database.
    The retrieved number of columns is then printed using printf().

The cols capability represents the number of columns (characters per line) supported by the terminal. The tgetnum() function retrieves this value and returns it as an integer, which can then be used in your program.

#### tgetflag:

The tgetflag() function is part of the ncurses library and is used to retrieve boolean terminal capabilities from the terminfo database. It takes the name of a boolean capability as an argument and returns a non-zero value if the capability is present, and 0 otherwise.

Here's the prototype of the tgetflag() function:

```c

int tgetflag(const char *capname);
```
    capname: The name of the boolean terminal capability you want to retrieve.

Here's a simple example demonstrating the usage of tgetflag() to check if the terminal supports color:

```c

#include <stdio.h>
#include <term.h>
#include <curses.h> // On some systems, term.h may be included via curses.h

int main() {
    setupterm(NULL, fileno(stdout), (int *)0); // Initialize terminal
    int has_color = tgetflag("colors"); // Check if terminal supports color
    if (has_color)
        printf("Terminal supports color.\n");
    else
        printf("Terminal does not support color.\n");
    return 0;
}
```
In this example:

    setupterm() initializes the terminal, similar to previous examples.
    tgetflag("colors") checks if the terminal supports color by querying the colors capability from the terminfo database.
    If the terminal supports color, tgetflag() returns a non-zero value, and the program prints "Terminal supports color.". Otherwise, it prints "Terminal does not support color.".

The colors capability represents whether the terminal supports colors. The tgetflag() function queries this capability and returns a non-zero value if it's present, indicating support for colors.

#### tgetent:
#### tcgetattr:
#### tcsetattr:
#### getenv:
#### ioctl:
#### ttyslot:
#### ttyname:
#### isatty:
#### perror:
#### strerror:
#### closedir && readdir && opendir:

In C, DIR *dir and struct dirent *entry are related to directory handling using the <dirent.h> header, commonly used for directory traversal and listing of directory contents.

`DIR *dir:`
DIR is a type representing a directory stream. It's an opaque structure that holds the state information needed to read the contents of a directory.
When you use opendir() to open a directory, it returns a pointer to a DIR structure (DIR *), which is then used for subsequent directory-related operations.
You use this DIR * pointer to perform operations like reading directory entries (readdir()), closing the directory stream (closedir()), etc.
Example:
```c
DIR *dir = opendir("/path/to/directory");
```
2 struct dirent *entry:
struct dirent is a structure representing a directory entry. It contains information about a single directory entry (file or subdirectory) within a directory.
When you use readdir() to read the next directory entry from a directory stream (DIR *), it returns a pointer to a struct dirent.
You use this struct dirent * pointer to access information about the directory entry, such as its name (d_name) and other attributes.
Example:
```c
Copy code
struct dirent *entry = readdir(dir);
if (entry != NULL) {
    printf("Directory entry name: %s\n", entry->d_name);
}
```
Here's a brief overview of struct dirent (from <dirent.h>):

```c
Copy code
struct dirent {
    ino_t d_ino;           // inode number
    off_t d_off;           // offset to the next dirent
    unsigned short d_reclen; // length of this record
    unsigned char d_type;  // type of file
    char d_name[256];      // filename
};
```
d_ino: The inode number of the file.
d_off: The offset to the next directory entry.
d_reclen: The length of this directory entry.
d_type: The type of the file (e.g., DT_REG for a regular file, DT_DIR for a directory).
d_name: The filename (null-terminated string).
When iterating over directory entries, you typically use a loop with readdir() to read each entry until readdir() returns NULL, indicating the end of the directory stream.

Here's an example of iterating through directory entries using readdir():

```c
Copy code
DIR *dir = opendir("/path/to/directory");
if (dir == NULL) {
    perror("opendir");
    return EXIT_FAILURE;
}

struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    printf("Directory entry name: %s\n", entry->d_name);
}

closedir(dir);
```
In this example:

We open the directory /path/to/directory using opendir().
We use a while loop to repeatedly call readdir() to read each directory entry (struct dirent) from the directory stream (DIR *dir).
For each directory entry, we print its name (entry->d_name).
We close the directory stream using closedir() after finishing the directory traversal.
This pattern of using DIR *dir and struct dirent *entry is fundamental for working with directory contents in C programs using standard POSIX APIs. Adjust and integrate these concepts into your code based on your specific directory handling requirements.

#### pipe:
#### dup2:
#### dup:
#### execve:
#### unlink:
#### fstat:
#### stat:
#### chdir:
#### getcwd:
#### sigaddset:
#### sigemptyset:
#### sigaction:
#### signal:
#### wait4:
#### wait3:
#### waitpid:
#### wait:
#### add_history:

*examples:*
```c
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    // Loop to repeatedly prompt the user for input
    while (1) {
        // Set a prompt
        const char *prompt = "Enter something (press Ctrl+D to exit): ";

        // Read a line of input
        input = readline(prompt);

        // If input is NULL, it means Ctrl+D was pressed (end of input)
        if (input == NULL) {
            printf("\nExiting...\n");
            break;
        }

        // Display the input
        printf("You entered: %s\n", input);

        // Add input to history
        if (*input)
            add_history(input);

        // Free the memory allocated by readline
        free(input);
    }

    return 0;
}
```

#### rl_redisplay:
#### rl_replace_line:


#### rl_on_new_line:

In C programming, specifically when using the GNU Readline library, the rl_on_new_line function is used to notify Readline that the cursor has moved to a new line. This function is typically used when implementing custom completion functions.

Here's the syntax of the rl_on_new_line function:

```c
void rl_on_new_line(void);
```

This function informs the Readline library that the cursor has moved to a new line. It's often used in conjunction with custom completion functions to ensure proper behavior when the user types input.

Here's a simple example demonstrating how rl_on_new_line can be used:

```c
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    // Set a prompt
    const char *prompt = "Enter something: ";

    // Read a line of input
    char *input = readline(prompt);

    // Add input to history
    if (input)
        add_history(input);

    // Inform Readline that the cursor has moved to a new line
    rl_on_new_line();

    // Free the memory allocated by readline
    free(input);

    return 0;
}
```

In this example, after reading input from the user and adding it to the history, rl_on_new_line() is called to notify Readline that the cursor has moved to a new line. This function can be helpful when writing custom completion functions or manipulating Readline's internal state in other ways.

Compile and run this program, and you'll see that it reads input from the user, adds it to the history, and notifies Readline that the cursor has moved to a new line.

#### rl_clear_history:

n C programming, specifically in programs using the GNU Readline library, the rl_clear_history function is used to clear the command history maintained by the Readline library. This function removes all entries from the history list.

Here's the syntax of the rl_clear_history function:

```c

void rl_clear_history(void);
```

And here's an example demonstrating how to use rl_clear_history:

```c

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    // Set a prompt
    const char *prompt = "Enter something: ";

    // Read a line of input
    char *input = readline(prompt);

    // Add input to history
    if (input)
        add_history(input);

    // Display the input
    printf("You entered: %s\n", input);

    // Clear the history
    rl_clear_history();

    // Free the memory allocated by readline
    free(input);

    return 0;
}
```

In this example, after reading input from the user and displaying it, the rl_clear_history() function is called to clear the command history. Note that after calling rl_clear_history(), any commands previously entered by the user will no longer be accessible via the Readline library's history navigation features.

Compile and run this program, and you'll notice that the command history is cleared after executing the program.

The function rl_clear_history() is responsible for removing all entries from the history list used by the Readline library, making the history empty. It does not free the memory associated with the individual history entries themselves. The memory allocated for individual history entries is typically managed by the Readline library and is not directly accessible for deallocation by user code.

So, in short, *rl_clear_history()* does not free memory for individual history entries; it simply clears the history list.

#### readline:

In C programming, the readline function is not a standard library function. However, it is commonly associated with the GNU Readline library, which provides line-editing and history capabilities for command-line interfaces.

To use readline in your C program, you need to include the <readline/readline.h> header file and link your program with the Readline library during compilation. The basic syntax of readline is as follows:

```c
char *readline(const char *prompt);
```

This function displays the specified prompt (if any) and waits for the user to input a line of text. It then reads that line of text from the standard input, allowing the user to edit it using various command-line editing capabilities provided by Readline. Finally, it returns a dynamically allocated string containing the input text.

Here's a simple example demonstrating the usage of readline:

```c

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    // Set a prompt
    const char *prompt = "Enter something: ";

    // Read a line of input
    input = readline(prompt);

    // Add input to history
    if (input)
        add_history(input);

    // Display the input
    printf("You entered: %s\n", input);

    // Free the memory allocated by readline
    free(input);

    return 0;
}
```

Remember to compile this program with the Readline library linked. For example:
```bash
gcc example.c -o example -lreadline
```
When you run this program, it will prompt you to enter something. After you enter some text and press Enter, it will display what you entered. The entered text will also be added to the command history, so you can use the arrow keys to navigate through your command history in subsequent runs of the program.

see example of add_histroy function.
