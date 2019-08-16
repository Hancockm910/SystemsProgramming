/* 
Mark Hancock 19307363
*/

#include	<stdio.h>
#include 	<stdlib.h>
#include	"time.h"
#include	<unistd.h>
#include	<string.h>

#define LINE_LENGTH 120// Macro for default line length

void parent_code(int childpid);
void child1();
void child2(char input[]);
void child3(char input[]);
  
 
int main(int argc, char *argv[]) 
{ 

	int i;
	int newpid; // int set for forking
	for ( i = 0; i < 3; i++)
	{
		if ( (newpid = fork()) == -1 ) // Forks are created here.
			perror("fork");
		if(i == 0 && newpid == 0) // child 1
		{
			printf("child 1  pid = %d & ppid = %d\n", getpid(), getppid()); 
			child1();
			exit(0);
		}
		else if (i == 1 && newpid == 0) // child 2
		{
			printf("child 2  pid = %d & ppid = %d\n", getpid(), getppid());
			child2(argv[1]);// first string from command line is passed
			exit(0);
		}
		else if (i == 2 && newpid == 0) // child 3
		{
			printf("child 3 pid = %d & ppid = %d\n", getpid(), getppid()); 
			child3(argv[2]);// second string from command line passed
			exit(0);
		}
		else
			parent_code(newpid);
	}
  
    return 0; 
} 

void child1()
{
	printf("Enter student's marks\n");
	int i;
	int highest;
	int input;
	highest = 0;
	for (i = 0; i < 10; i++)// runs 10 times to accept 10 inputs
	{
		scanf("%d", &input);
		if (input > highest)// only updates if new number is the largest
			highest = input;
	}
	printf("Highest score = %d\n", highest);
}

void child2(char input[])
{
	char* Counting[]=
	{
		"wc","-w", input,NULL // wc is set to count words only
	};
	execv("/usr/bin/wc", Counting);// command line is passed to wc program
}

void child3(char input[])
{				
	char ReadFile[32];
	strcpy(ReadFile, input);// command line string passed through as file name
	char WriteFile[32] = "tempfile.tmp";// temporary file for editing
	char TextFind[80] = "execute";// original text
	char TextReplace[80] = "run";// replacement text
	char SecFind[80] = "study";// original text
	char SecReplace[80] = "examine ";// replacement text
	char Buffer[LINE_LENGTH+2];
	char *Buffered, *Finder;
	FILE *OrgFile, *TempFile;
	size_t StrLength = strlen(TextFind);
	OrgFile = fopen(ReadFile,"r");// Original file to read only
	TempFile = fopen(WriteFile,"w");// Temporary file to write only
	fprintf(TempFile, "This is the updated version.\n");// written at start of temp file
	while(fgets(Buffer,LINE_LENGTH+2,OrgFile))
	{
		Buffered = Buffer;// passes line from original text file
		while ((Finder = strstr(Buffered,TextFind)))// searching for first word
		{
			while(Buffered < Finder)
				fputc((int)*Buffered++,TempFile);
			fputs(TextReplace,TempFile);// replaces word
			Buffered += StrLength;
		}
		while ((Finder = strstr(Buffered,SecFind)))// searching for second word
		{
			while(Buffered < Finder)
				fputc((int)*Buffered++,TempFile);
			fputs(SecReplace,TempFile);// replaced word
			Buffered += StrLength;
		}
		fputs(Buffered,TempFile);// line placed into temp file
	}
	// files are closed
	fclose(TempFile);
	fclose(OrgFile);
	
	remove(ReadFile);// original file deleted
	
	rename("tempfile.tmp", ReadFile);// temp file renamed to original's name
	printf("Text has been altered. \n");
}

void parent_code(int childpid)
{
	int wait_rv;		/* return value from wait() */
	int child_status;
	int high_8, low_7, bit_7;

	wait_rv = wait(&child_status);// waits until child has finished executing
	printf("done waiting for %d. Wait returned: %d\n", childpid, wait_rv);

	high_8 = child_status >> 8;     /* 1111 1111 0000 0000 */
	low_7  = child_status & 0x7F;   /* 0000 0000 0111 1111 */
	bit_7  = child_status & 0x80;   /* 0000 0000 1000 0000 */
	printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);
}



