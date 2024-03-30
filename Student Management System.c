/*>>>>>>>>>>>>>>>>>>>>>>>>>>>FINAL PROJECT IN DATA STRUCTURES & ALGORITHM<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    Leader: Guingab, Zarene Claire
                                    Members: Perez, Selwyn Audrey
                                            Dalumpines, Sophia
                                            Delos Reyes, Christine
                                            Flor, Lea

                                        BSCS 2ABM - (STEM)


        ---------------FILE COMPRESSION AND DECOMPRESSION USING HUFFMAN ALGORITHM---------------*/

//HEADER FILES
#include <stdio.h>      //header file for standard input and output.
#include <stdlib.h>     //functions involving memory allocation, process control, conversions and others.
#include <string.h>     //contains macro definitions, constants and declarations of functions.
#include <conio.h>      //built-in function of getch.
#include <windows.h>    //built-in function for gotoxy.
#define MAX 1000000     //macro related to the file's info

COORD coord={0,0};      //defines the coordinates of a character cell in a console screen buffer

//place cursor at desired location
void gotoxy(int x,int y) {
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

//structure to define node
typedef struct node_t {
	struct node_t *left, *right;
	int freq;
	char c;
} *node;

//global variables
int n_nodes = 0, pqend = 1; 		    //keep track of the number of nodes and end of the queue
struct node_t pool[256] = {{0}};        //pool of nodes
node qqq[255], *q = qqq-1;      	    //priority queque
char *code[128] = {0}, buf[1024];	    //string array of the codes for each letter
int input_data=0,output_data=0;         //input and output data
int index = -1;                         //index
char file_name[50];                     //file name
char encoded[MAX] = "";

//functions
node new_node(int freq, char c, node a, node b);
void pqinsert(node n);
node pqremove();
void huffmanTree(node n, char *s, int len);
void encode(FILE* fp_in, FILE* fp_out, unsigned int *freq );
void printTable(unsigned int *freq);
void decode(node root);
void menu(int ch);

//function to create a new node
node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq != 0)                      //if frequency is not equal to 0 proceed...
    {
		n->c = c;			            //assign 'c' to the character of the node
		n->freq = freq;			        //assign frequency
	}
	else
	{
		n->left = a, n->right = b;	    //if the invocation does not provide a frequency
		n->freq = a->freq + b->freq;	//the removed nodes at the queue's end will be added to left and right
	}
	return n;                           //return node
}

//function to insert a node into the priority queue
void pqinsert(node n)
{
	int j, i = pqend++;                 //variables, equate i to end of queue
	while ((j = i / 2))
    {
		if (q[j]->freq <= n->freq)      //arrange nodes base on frequncy
		{
            break;                      //break
		}
		q[i] = q[j], i = j;             //heapify array
	}
	q[i] = n;                           //equate to node
}

//function to remove a node into the priority queue
node pqremove()
{
	int i, l;                           //variables
	node n = q[i = 1];

	if (pqend < 2)                      //if leaf nofe, remove node
    {
        return 0;
    }
	pqend--;                            //decrement end of queue
	while ((l = i * 2) < pqend)         //checks for queue with highest priority
    {
		if (l + 1 < pqend && q[l + 1]->freq < q[l]->freq) l++;
        {
            q[i] = q[l], i = l;
        }
	}
	q[i] = q[pqend];                    //swap node to be deleted to the last node
	return n;	                        //return the node
}

//go through the huffman tree and  for each character assign the code
void huffmanTree(node n, char *s, int len)
{
	static char *out = buf;
	if (n->c)
    {
		s[len] = 0;		                //if the node is a leaf
		strcpy(out, s);		            //contains a character
		code[(int)n->c] = out;	        //copy the code into the relevant character
		out += len + 1;		            //increment out pointer
		return;
	}

	s[len] = '0'; huffmanTree(n->left,  s, len + 1);	//recurring is used to write out the code
	s[len] = '1'; huffmanTree(n->right, s, len + 1);	//add a 0 if right and add a 1 if right
}

//ENCODE
void encode(FILE* fp_in, FILE* fp_out, unsigned int *freq )
{
	char in,c,temp[20] = {0};           //variables
	int i,j=0,k=0,lim=0;                //variables
	rewind(fp_in);                      //sets the file position to the beginning of the file

	for(i=0; i<128; i++)
    {
		if(freq[i])	lim += (freq[i]*strlen(code[i]));   //calculates the length of code.
	}
	output_data = lim;			                //output data is equal to the limit
	for(i=0; i<lim; i++)
	{
		if(temp[j] == '\0')
		{
			in = fgetc(fp_in);                  //obtain input from the file one character at a time
			strcpy(temp,code[in]);              //copy string
			j = 0;                              //initialize j
            strcat(encoded, code[in]);          //concatenates the encode and code
		}
		if(temp[j] == '1')
		{
            c = c|(1<<(7-k));                   //shifts 1 to relevant position and OR with the temporary char
		}
        else if(temp[j] == '0')
        {
            c = c|(0<<(7-k));                   //shifts 0 to relevant position and OR with the temporary char
        }
        else
        {
            printf("\n\t\t\tERROR: WRONG INPUT!\n");    //prompt wrong input
        }
        k++;                            // k is used to divide the string into 8 bit chunks and save
		j++;                            // increment j
	}
	putchar('\n');                      //new line
    FILE *file;                         //open file
    file = fopen(file_name, "w");       //open file in write mode
    fprintf(file, "%s", encoded);       //write in file the encoded text
    fclose(file);                       //close file
}

void printTable(unsigned int *freq)     // function to print encoded data in a tabular form
{

    int i;                              // declare variable
	printf("\n\t\t\t\t\t---------CODE TABLE---------\n\t\t\t\t\t----------------------------\n\t\t\t\t\tCHAR  |   FREQ   |  CODE\n\t\t\t\t\t----------------------------\n"); // print on screen
	for(i=0; i<128; i++)                //executes until i is reaches 127
    {
		if(isprint((char)i)&&code[i]!=NULL&&i!=' ')
		{
			printf("\n\t\t\t\t\t%-4c | %-4d | %16s",i,freq[i],code[i]);   //print the frequency and the code of the character
		}
		else if(code[i]!=NULL)                                  //if code array is not null
        {
			switch(i)                                           //switch case for i
			{
				case '\n':                                      //new line
					printf("\n\t\t\t\t \n  ");                            //print on screen
					break;                                      //break
				case ' ':                                       //no character
					printf("\n\t\t\t\t '\' ");                            //print on screen
					break;                                      //break
				case '\t':                                      //tab
					printf("\\t  ");                            //print on screen
					break;                                      //break
				default:                                        //default option
					printf("%0X  ",(char)i);
					break;                                      //break
			}
			printf("\n\t\t\t\t\t | %-4d | %16s",freq[i],code[i]);        //print the frequency and the code of the character
		}
	}
	printf("\n\t\t\t\t\t----------------------------\n");                //print on screen
	printf("\t\t\tYour file is now compressed! Press any key to go back in the File Menu.\n");   //display message on successful file compression

}

void decode(node root)           // function for decoding
{
    if(root->c)                  //if node has character
    {
        printf("%c", root->c);   //print the node's character
        root = q[1];             //return to root
        decode(root);            //decode
    }

    index++;                     //iterate index

    if(encoded[index]=='0'){     //encoded character is 0
        decode(root->left);      //decode left root
    }
    if(encoded[index]=='1'){     //encoded character is 1
        decode(root->right);     //decode right root
    }
}

void menu(int ch)
{
	FILE *fp_in, *fp_out, *fp_huffman, *fp_table;        //file pointers
	char temp[50];				                         //for file name
	unsigned int freq[128] = {0};			             //for storing frequency of the letters
	char c,s[16]={0};		                             //declare temporary variables
	int i;                                               //declare integer variable

   	switch(ch)                                           //switch case
   	{
        case 1:                                          //first case, file compression
            system("COLOR E0");
            system("cls");                               //file compression
            printf("\a");
            printf("\n\n\t\t\t\t\t-------------------------");    //print on screen
            printf("\n\n\t\t\t\t\tF I L E   C O M P R E S S");    //print on screen
            printf("\n\n\t\t\t\t\t-------------------------");    //print on screen
            gotoxy(10, 7);                                        //go to cursor position
            printf("Please enter the file to be compressed (FORMAT: .txt/.c): "); //get name of file to be compressed
            //gotoxy(10, 9);
            scanf("%s",file_name);                                //scan file name
            system("color 0E");
            char x=177, y=219;                              // initialize char for printing
            printf("\n\n\n\n");printf("\n\n\n\n\t\t\t\t\t" "Compressing...\n\n");printf("\t\t\t\t\t");//print on screen
                for (int i = 0; i < 26; i++)               //execute until i reaches 25
                    printf("%c", x);
                    printf("\r");
                    printf("\t\t\t\t\t");                  //print multiple tabs
                for (int i = 0; i < 26; i++) {               //execute until i reaches 25
                    printf("%c", y);                          //print initial loading bar
                    Sleep(150);}
                    printf("\n");                            //print new line
                    system("cls");                           //clear screen

            if((fp_in = fopen(file_name,"r"))==NULL)         //if file not found
            {
                printf("\n\t\t\t\tERROR: No such file\n");   //print error message
                return 0;                                    //return false
            }
            while(!feof(fp_in))                              //while not end of file
            {
                c=fgetc(fp_in);                              //read file characters
                freq[(int)c]++;                             //iterate freq array
                putchar(c);                                 //put character
            }
            for (i = 0; i < 128; i++)                       //execute until i reaches 127
            {
                if (freq[i]) pqinsert(new_node(freq[i], i, 0, 0));//if frequency is detected, insert new nodes into queue
            }
            while (pqend > 2)                           //while pqend is greater than 2
            {
                pqinsert(new_node(0, 0, pqremove(), pqremove()));	//build the tree
            }
            huffmanTree(q[1], s, 0);		//build code for each characters
            printTable(freq);	//call and execute printTable function to print the code table
            strcat(file_name,".encoded");     //add '.encoded' to encoded file's filename
            fp_out = fopen(file_name,"w");    //open file in write mode
            encode(fp_in,fp_out,freq);        //encode
            fclose(fp_in);                    //close file
            fclose(fp_out);                    //close file
            getch();                           //hold output screen
            main();                            //call main function
            break;                             //break
        case 2:                                                   //second case, file decompression
            system("COLOR F0");
            system("cls");                                        //clear screen
            printf("\a");
            printf("\n\n\t\t\t\t\t-----------------------------");//print on screen
            printf("\n\n\t\t\t\t\tF I L E   D E C O M P R E S S");//print on screen
            printf("\n\n\t\t\t\t\t-----------------------------");//print on screen
            gotoxy(10, 7);                                        //go to cursor position
            printf("Enter filename (FORMAT: Fname.txt.encoded): ");                //get name of file to be decompressed
            scanf("%s",file_name);                                //scan file name
            system("color 0B");
            //char x=177, y=219;
            printf("\n\n\n\n");printf("\n\n\n\n\t\t\t\t" "Decompressing... Your file will be decoded in a text file!\n\n");printf("\t\t\t\t\t");//print on scren
                for (int i = 0; i < 26; i++)               //execute until i reaches 25
                    printf("%c", x);                       //print initial loading bar
                    printf("\r");                          //print on screen
                    printf("\t\t\t\t\t");                  //print multiple tabs
                for (int i = 0; i < 26; i++) {               //execute until i reaches 25
                    printf("%c", y);
                    Sleep(150);}
                    printf("\n");                            //print new line

            if((fp_in = fopen(file_name,"r"))==NULL)         //if file not found
            {
                printf("\n\t\t\t\tERROR: No such file\n");   //display error message
                return 0;                                    //return false
            }
            fscanf(fp_in, "%s", encoded);                    //scan encoded characters from file
            printf("\t\t\t\t\t----------------------------\n");        //print on screen
            printf("\n\t\t\tYour file is now decoded! Thank you for using this program!\n"); //display message for successful decoding
            printf("\n\t\t\t\tDECODED MESSAGE: \n");                 //display message for successful decoded file saving
            node n = q[1];
            decode(n);                                        //decode
            printf("\n\n");                                   //print two new lines
            exit(0);                                          //exit
        case 3:                                               //third case, program termination
            printf("\a");
            printf("\n\t\t\t\t\t»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»\n");  //print on screen
            printf("\t\t\t\t\t|   Thank you for using this   |\n");  //print on screen
            printf("\t\t\t\t\t|          Program!            |\n");  //print on screen
            printf("\t\t\t\t\t»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»\n");  //print on screen
            exit(0);                                               //exit
            break;                                                 //break
        default:                            //default option
        printf("\t\t\t\t\tInvalid choice 1-2 only");  //display invalid choice messade
            system("pause");                //hold screen
   	}

	return 0;
}

int main()                     //main function
{
    int choice;                //declare variable
    system("cls");             //clear screen
    system("COLOR B4");        // color of front and background
    gotoxy(13, 8);             //go to position
    printf("\n\n\n\t\t\t\t\tLoading Entry... Please Wait"); //Loading Bar
    printf("\n\n");            //print two new lines
    for (int i=0;i<120;i++)    //execute until i reaches 119
    {
        Sleep(30);              //sleep for a second
        printf("%c",222);       //print initial loading bar
    }
    while (1) //while true
    {
    system("cls");                                                                                   //clear screen
    printf("\n\t\t\t»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»");//print on screen
    printf("\n\t\t\t»»»»»»»»»»»»                                                     »»»»»»»»»»»»»");//print on screen
    printf("\n\t\t\t»»»»»»»»»»»»     Technological University of the Philippines     »»»»»»»»»»»»»");//print on screen
    printf("\n\t\t\t»»»»»»»»»»»»                   Manila Campus                     »»»»»»»»»»»»»");//print on screen
    printf("\n\t\t\t»»»»»»»»»»»»                                                     »»»»»»»»»»»»»");//print on screen
    printf("\n\t\t\t»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»");//print on screen
    printf("\n\t\t\t=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");//print on screen
    printf("\t\t\t   F I L E    C O M P R E S S I O N    A N D    D E C O M P R E S S I O N   \n");  //print on screen
    printf("\t\t\t=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");  //print on screen
        gotoxy(12,10);                           //go to cursor position
        printf("\n\t\t\t\t--------------------");//print on screen
        printf("\n\t\t\t\tF I L E   M E N U");   //print on screen
        printf("\n\t\t\t\t--------------------");//print on screen
        printf("\n\t\t\t\t1] Compress File\n");  //option 1, compress file
        printf("\n\t\t\t\t2] Decompress File\n");//option 2, decompress file
        printf("\n\t\t\t\t3] Exit\n\n");         //option 3, exit
        printf("\t\t\t\tEnter choice: ");        //get menu choice
        scanf("%d", &choice);                    //scan menu choice
        menu (choice);                           //call and execute menu function
    }
}

