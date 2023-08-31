#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int validate(char ** all_strings, int cur);
int has_illegal(char* mystr,int len);
int st_length(char* mystr);
int check_dupes(char** all_strings, int cur);
char** bubble_sort(char** all_strings, )

int check_dupes(char** all_strings, int cur)
{
    for(int i=0; i< cur; i++)
    {
        if(strcmp(all_strings[i],all_strings[cur]) ==0 )
        {
            printf("Error: %s is a duplicate string\n", all_strings[cur]);
            return 0;
        }
    }
    return 1;

}




int st_length(char* mystr)
{
   int l=strlen(mystr);
    if(l>25) 
    {
        printf("Error: Entered String is of length %i\
                only 25 characters is accepted.\n",l);
        return 0;
    }
    else if(l==0)
    {
        printf("Error: Entered String is of length %i\
        only 25 characters is accepted.\n",l);
        return 0;
    }
    else
    {
        return l;
    }
}


int has_illegal(char* mystr,int len)
{
    int is_illegal=1;
    char illegal[10]={'+', '*', '#', '$', '%', '^', '(', ')'};
    for(int i=0; i<len; i++)
    {
        for(int j=0;j<8;j++)
        {
            if(mystr[i]==illegal[j])
            {
                is_illegal=is_illegal*0;
                printf("Error: Illegal character entered %c\n",illegal[j]);
                illegal[j]='\0';

            }
        }
    }
    return is_illegal;
//sdadasd
}


int validate(char ** all_strings, int cur) 
{
    char* mystr=all_strings[cur];
    int is_allowed=1;
    int l=st_length(mystr);
    is_allowed=is_allowed*l;
    is_allowed=is_allowed*has_illegal(mystr,l);
    is_allowed=is_allowed*check_dupes(all_strings, cur);
}


int main()
{
    char** str_list= malloc(10*sizeof(char*));
    for(int i =0; i<10;i++)
    {
        str_list[i]=malloc(50);
        int validated=0;
        while(!validated)
        {
            printf("Enter string %i ->",i);
            fgets(str_list[i],50,stdin);
            validated=validate(str_list,i) ;    
        }
    }
    printf("Print character strings in (A)scending or (D)escending order:");
    // char temp_buf[50];
    char order=0;
    char temp_buf[50];
    while(!order)
    {
        order= fgets(temp_buf,50,stdin)[0];
        if(order !='A' & order !='D')
        {
            printf("%c is not valid, enter A for\
            (A)scending or D for (D)escending: ",order);
            order=0;
        }
    }

    for(int i =0; i<10;i++)
    {
        printf("Your string is %s",str_list[i]);
    }
  return 0;

}