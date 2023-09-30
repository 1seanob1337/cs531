#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define FILENAME  "foobar"
#define OCTET_LEN 4
#define MAX_OCTET 255
#define MIN_OCTET 0
struct struct_address_t
{
    int *octet;
    char *alias;
    struct struct_address_t *next;
};




struct struct_address_t* create_node(char *alias, int *octet)
{
    struct struct_address_t *new_entry = (struct struct_address_t *)malloc(sizeof(struct struct_address_t));
    if (new_entry == NULL)
    {
        fprintf(stderr, "failed to allocate memory. qutting\n");
        return NULL;
    }

    new_entry->alias = alias;
    new_entry->octet = octet;
    new_entry->next = NULL;
    return new_entry;

}

char* octet_to_string(int* octet)
{
    static char oct_buf[20];
    sprintf(oct_buf,"%d.%d.%d.%d",*octet, *(octet + 1), *(octet + 2), *(octet + 3));  
    return oct_buf;    

}

int validate_addr(int* octet)
{
    for(int i=0; i<OCTET_LEN; i++)
    {
        if(*(octet+i) >MAX_OCTET  | *(octet+i)< MIN_OCTET )
        {
            fprintf(stderr,"error: %s is an illegal address - please reenter:\n",octet_to_string(octet));
            return 0;
        }
    }
    return 1;

}

struct struct_address_t *prompt_address_add()
{
    //use heap for node allocation, since we don't have any idea
    //how many inputs will occur
    // read a little bit further than 10 to know if user inputted too much
    char *alias=malloc(20*sizeof(char));
    char *raw_addr=malloc(20*sizeof(char));
    int *octet=malloc(4*sizeof(int)); 
    fprintf(stdout, "Enter alias: ");
    fgets(alias, 20, stdin);
    *(strrchr(alias,'\n'))='\0';
    fflush(stdin);
    // TODO validate

    do
    {
        fprintf(stdout, "Enter address: ");
        fgets(raw_addr, 20, stdin);
        *(strrchr(raw_addr,'\n'))='\0';
        fflush(stdin);
    // taken from lecture notes
        sscanf(raw_addr, "%d.%d.%d.%d", octet, octet + 1, octet + 2, octet + 3);
    }while(!validate_addr(octet));

    return create_node(alias,octet);
}

int print_iden(struct struct_address_t *node, char message[])
{
    printf("%s alias=%s\toctet=%i.%i.%i.%i\n", message,
           node->alias,
           node->octet[0],
           node->octet[1],
           node->octet[2],
           node->octet[3]);
}

struct struct_address_t *add_to_list(struct struct_address_t *new_node,
                                     bool add_to_end,
                                     struct struct_address_t **head,
                                     struct struct_address_t **curr)
{
    if (NULL == *head)
    {
        // printf("debug:  head null make new list\n");
        *head = new_node;
        *curr = *head;
        printf("\n Node creation success! \n");
        // print_iden(*curr,"debug create node curr");
        // print_iden(new_node,"debug create node newnode");
        return *head;
    }
    if (add_to_end)
    {
        (*curr)->next = new_node;
        *curr = new_node;
        // print_iden(*curr,"debug curr node");
    }
    else
    {
        new_node->next = *head;
        *head = new_node;
    }
    return *head;
}


int alias_compare(struct struct_address_t *ptr, char* alias)
{
    return(strcmp(ptr->alias,alias));
}
//TODO integer representation of octet
//256^3*oct[0]+256^2*oct[1]+256*oct[2]+oct[3]
int ip_compare(struct struct_address_t *ptr, int* octet)
{
    //returns the difference between the first non matching int 
    for(int i=0; i< OCTET_LEN; i++)
    {
        if(*(ptr->octet+i) != *octet)
        {
            return *(ptr->octet+i)-*octet;
        }
    }
    return 0;
}

// TODO implement octet seraching
struct struct_address_t *search_in_list(int* octet,
                                        char* alias,
                                        struct struct_address_t **prev,
                                        struct struct_address_t **head,
                                        struct struct_address_t **curr)
{
    struct struct_address_t *ptr = *head;
    struct struct_address_t *tmp = NULL;
    void (*compare)(struct struct_address_t*, void*);
    bool found = false;
    //assume either alias or octet is set
    if(alias==NULL)
    {
        compare=alias_compare;
    }
    else
    {
        compare=ip_compare;
    }
    // print_iden(octet,alias,"\n Searching the list for value [%s]");
    printf("\n Searching the list for value [%s]", alias);
    while (ptr != NULL)
    {
        if (strcmp(ptr->alias, alias) == 0)
        {
            found = true;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }
    if (true == found)
    {
        if (prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}

// todo implement for octet
int delete_from_list(int octet[],
                     char alias[],
                     struct struct_address_t **head,
                     struct struct_address_t **curr)
{
    struct struct_address_t *prev = NULL;
    struct struct_address_t *del = NULL;

    printf("Deleting value from list %s \n", alias);
    del = search_in_list(octet, alias, &prev, head, curr);
    if (del == NULL)
    {
        return -1;
    }
    else
    {
        if (prev != NULL)
            prev->next = del->next;
        if (del == *curr)
        {
            *curr = prev;
        }
        else if (del == *head)
        {
            *head = del->next;
        }
    }
    //TODO implement node free function
    free(del);
    del = NULL;
    return 0;
}
void print_list(struct struct_address_t *head,
                struct struct_address_t *curr)
{
    struct struct_address_t *ptr = head;
    printf("\n -------Printing list Start------- \n");
    while (ptr != NULL)
    {
        // printf("\n [%d] \n", ptr->val);
        print_iden(ptr, "");
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");
    return;
}
int look_up_address(struct struct_address_t **head,
                struct struct_address_t **curr)
{
    //TODO consider making this a function
    static char alias[20];
    fprintf(stdout, "Enter alias: ");
    fgets(alias, 20, stdin);
    //TODO check strlen
    *(strrchr(alias,'\n'))='\0';
    fflush(stdin);
    struct struct_address_t *ptr;
    ptr=search_in_list(NULL,alias,NULL,head,curr);
    if(ptr==NULL)
    {
        fprintf(stderr,"error %s does not exist",alias); 
    }
    else
    {
        fprintf(stdout,"Address for %s: %s\n",alias,octet_to_string(ptr->octet));
    }
    return 0;

}


struct struct_address_t* parse_line(char line[])
{
    int line_pos=0;
    int buf_pos=0;
    char c;
    char* buf;
    char *alias=malloc(20*sizeof(char));
    char *raw_octet=malloc(20*sizeof(char));
    int *octet=malloc(4*sizeof(int)); 
    buf=raw_octet;
    while((c=*(line+line_pos))!='\n')
    {
        // fprintf(stdout,"c=%c\n",c);
        if(c==' ' )
        {
            buf=&alias[0];
            buf_pos=0;
        }
        else
        {
            *(buf+buf_pos)=c;
            buf_pos+=1;
        }
        line_pos+=1;
    }
    sscanf(raw_octet, "%d.%d.%d.%d", octet, octet + 1, octet + 2, octet + 3);
    return create_node(alias,octet);

}


int read_file(struct struct_address_t **head,
              struct struct_address_t **curr)
{
    FILE *fp;
    struct struct_address_t* node;
    fp=fopen(FILENAME,"r");
    if(fp==NULL)
    {
        fprintf(stderr,"Error opening file\n");
        return -2;
    }
    char line[50];
    while(fgets(line,50,fp)!=NULL)
    {
        fprintf(stdout,"line=%s\n",line);
        node=parse_line(line);
        if(node==NULL)
        {
            return -1;
        }
        // print_iden(node,"debug node");
        add_to_list(node,true,head,curr);
    }
    fclose(fp);
    return 0;
}

int main(void)
{
    struct struct_address_t *ptr = NULL;
    struct struct_address_t *prev = NULL;
    struct struct_address_t *head = NULL;
    struct struct_address_t *curr = NULL;
    // read file into LL
    int ret;
    ret = read_file(&head, &curr);
    if (ret != 0)
    {
        return ret;
    }

    print_list(head,curr);
    char selection;
    while(1)
    {
        selection=print_menu();
        if(selection=='1')
        {
            ptr=prompt_address_add();
            if(ptr==NULL);
            return -1;
            add_to_list(ptr,true,&head,&curr);
        }
        else if (selection=='2')
        {
            look_up_address(&head,&curr);
        }
        else if (selection=='3')
        {
            /* code */
        }
        else if (selection=='4')
        {
            /* code */
        }
        else if (selection=='5')
        {
            /* code */
        }              

        else if (selection=='6')
        {
            /* code */
        }              
        else if (selection=='7')
        {
            /* code */
        }              
        else if (selection=='8')
        {
            /* code */
        }              
    }



    ptr=prompt_address_add();
    if(ptr==NULL)
    {
        return -1;
    }
    int test[]={1,2,3,4};
    add_to_list(ptr,false,&head,&curr);
    delete_from_list(test,"jet",&head,&curr);
    print_list(head,curr);
    search_in_list(test,"jet",&prev,&head,&curr);
    print_iden(search_in_list(test,"baker",&prev,&head,&curr),"found in search");
    // print_iden(ptr, "");
    return 0;
    // int i = 0, ret = 0;
}