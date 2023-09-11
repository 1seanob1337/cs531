#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int validate(char **all_strings, int cur);
int has_illegal(char *mystr);
int st_length(char *mystr);
int check_dupes(char **all_strings, int cur);
void bubble_sort(char **all_strings, int n, int(*f)(char*,char* ));
int ascending(char* first, char* second);
int descending(char* first, char* second);

int ascending(char* first, char* second)
{
    //String comparator function for ascending order
    return (strcmp(first,second)>0);
}
int descending(char* first, char* second)
{
    return (strcmp(first,second)<0);
}


void bubble_sort(char **list, int n, int (*f)(char*,char*))
{
    //Bubble sort function taken from lecture notes, with change of using
    //comparator function *f
    long c, d;
    char* t;
    for (c = 0; c < (n - 1); c++)
    {
        for (d = 0; d < n - c - 1; d++)
        {
            if ( (*f)(*(list+d), *(list+d + 1))  )
            {
                /* Swapping */
                t = *(list+d);
                *(list+d) = *(list +d + 1);
                *(list+d+1) = t;
            }
        }
    }
}

int check_dupes(char **all_strings, int cur)
{
    //iterate over the array of strings and return 0 if *(all_stings+cur)
    //has any duplicates as determined by strcmp
    //returns 0 if any dupes exist, 1 otherwise
    for (int i = 0; i < cur; i++)
    {
        if (strcmp(*(all_strings+i), *(all_strings+cur)) == 0)
        {
            printf("Error: %s is a duplicate string\n", all_strings[cur]);
            return 0;
        }
    }
    return 1;
}

int st_length(char *mystr)
{
    //check,length of a string against validation rules,
    // and returning the actual length if it passes validation. 
    //returns 0 for invalid string, 1 otherwise

    int l = strlen(mystr);
    if (l > 25)
    {
        printf("Error: Entered String is of length %i\
                only 25 characters is accepted.\n",
               l);
        return 0;
    }
    else if (l == 0)
    {
        printf("Error: Entered String is of length %i\
       empty string is not accepted.\n",
               l);
        return 0;
    }
    else
    {
        return 1;
    }
}

int has_illegal(char *mystr)
{
    //iterate over a list of illegal char for chars in mystr
    //returns 0 if illegal chars are found, 1 otherwise
    int is_illegal = 1;
    int len=strlen(mystr);
    char illegal[10] = {'+', '*', '#', '$', '%', '^', '(', ')','\0'};
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mystr[i] == illegal[j])
            {
                is_illegal = is_illegal & 0;
                printf("Error: %c is an illegal character - please re-enter\n", illegal[j]);
                illegal[j] = '\0';
            }
        }
    }
    return is_illegal;
}

int validate(char **all_strings, int cur)
{
    //validation wrapper
    //runs all validation checks and ANDS results together
    //if any validation is 0 then validation fails
    char *mystr = all_strings[cur];
    int is_allowed = 1;
    int l = st_length(mystr);
    is_allowed = is_allowed & l;
    is_allowed = is_allowed & has_illegal(mystr);
    is_allowed = is_allowed & check_dupes(all_strings, cur);
    return is_allowed;
}

int main()
{
    //------User string input-----------//
    //Prompt user for input string until 10 valid strings
    //are read from stdin.
    char **str_list = malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        *(str_list+i) = malloc(50);
        int validated = 0;
        while (!validated)
        {
            printf("Enter string %i ->", i);
            fgets(*(str_list+i), 50, stdin);
            fflush(stdin);
            char *pos;
            //Assuming the \n will be a trailing \n with no other newline sper input
            pos=strrchr(*(str_list+i),'\n');
            if(pos!=NULL)
            {
                *pos='\0';
            }
            validated = validate(str_list, i);
        }

    }
    //-------Sort Selection-------//
    //similar to user input section. Read from standard input until
    //vaid input is found 'A'|'B'
    printf("Print character strings in (A)scending or (D)escending order:");
    char order = 0;
    char temp_buf[50];
    while (!order)
    {
        order = getchar();
        fflush(stdin);
        if (order != 'A' & order != 'D')
        {
            printf("%c is not valid, enter A for\
            (A)scending or D for (D)escending: ",
                   order);
            order = 0;
        }
    }
    //--------List Sort-----------//
    //depending on user input for sort type,
    // pass different comparator into our sort funciton
    if(order=='A')
    {
        bubble_sort(str_list,10,ascending);
    }
    else
    {
        bubble_sort(str_list,10,descending); 
    }
    //-----------Output -------------//
    //loop through list and print values in order
    //print lowest and highest ascii string
    printf("\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%s\n", *(str_list+i));
    }
    printf("\n");
    if(order=='A')
    {
        printf("String with lowest ascii value: %s\n",*(str_list));
        printf("String with highest ascii value: %s\n",*(str_list+9));

    }
    else
    {
        printf("String with lowest ascii value: %s\n",*(str_list+9));
        printf("String with highest ascii value: %s\n",*(str_list));
    }
    //cleanup
    for(int i=0; i<10;i++)
    {
        free(*(str_list+i));
    }
    free(str_list);


    return 0;
}
