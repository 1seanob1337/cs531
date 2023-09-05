#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int validate(char **all_strings, int cur);
int has_illegal(char *mystr, int len);
int st_length(char *mystr);
int check_dupes(char **all_strings, int cur);
void bubble_sort(char **all_strings, int n, int(*f)(char*,char* ));
int ascending(char* first, char* second);
int descending(char* first, char* second);

int ascending(char* first, char* second)
{
    return (strcmp(first,second)>0);
}
int descending(char* first, char* second)
{
    return (strcmp(first,second)<0);
}


void bubble_sort(char **list, int n, int (*f)(char*,char*))
{
    long c, d;
    char* t;
    for (c = 0; c < (n - 1); c++)
    {
        for (d = 0; d < n - c - 1; d++)
        {
            //todo use comparator function, passed into sort
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
    for (int i = 0; i < cur; i++)
    {
        if (strcmp(all_strings[i], all_strings[cur]) == 0)
        {
            printf("Error: %s is a duplicate string\n", all_strings[cur]);
            return 0;
        }
    }
    return 1;
}

int st_length(char *mystr)
{
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
        only 25 characters is accepted.\n",
               l);
        return 0;
    }
    else
    {
        return l;
    }
}

int has_illegal(char *mystr, int len)
{
    int is_illegal = 1;
    char illegal[10] = {'+', '*', '#', '$', '%', '^', '(', ')'};
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mystr[i] == illegal[j])
            {
                is_illegal = is_illegal * 0;
                printf("Error: %c is an illegal character - please re-enter \n", illegal[j]);
                illegal[j] = '\0';
            }
        }
    }
    return is_illegal;
    // sdadasd
}

int validate(char **all_strings, int cur)
{
    char *mystr = all_strings[cur];
    int is_allowed = 1;
    int l = st_length(mystr);
    is_allowed = is_allowed * l;
    is_allowed = is_allowed * has_illegal(mystr, l);
    is_allowed = is_allowed * check_dupes(all_strings, cur);
    return is_allowed;
}

int main()
{
    char **str_list = malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        *(str_list+i) = malloc(50);
        int validated = 0;
        while (!validated)
        {
            printf("Enter string %i ->", i);
            fgets(str_list[i], 50, stdin);
            validated = validate(str_list, i);
        }
    }
    printf("Print character strings in (A)scending or (D)escending order:");
    // char temp_buf[50];
    char order = 0;
    char temp_buf[50];
    while (!order)
    {
        order = fgets(temp_buf, 50, stdin)[0];
        if (order != 'A' & order != 'D')
        {
            printf("%c is not valid, enter A for\
            (A)scending or D for (D)escending: ",
                   order);
            order = 0;
        }
    }
    if(order=='A')
    {
        bubble_sort(str_list,10,ascending);
    }
    else
    {
        bubble_sort(str_list,10,descending); 
    }
    printf("\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%s", *(str_list+i));
    }
    printf("\n");
    if(order=='A')
    {
        printf("String with lowest ascii value: %s",*(str_list));
        printf("String with highest ascii value: %s",*(str_list+9));

    }
    else
    {
        printf("String with lowest ascii value: %s",*(str_list+9));
        printf("String with highest ascii value: %s",*(str_list));

    }

    return 0;
}
