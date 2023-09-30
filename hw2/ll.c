#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct struct_address_t
{
    int octet[4];
    char alias[11];
    struct struct_address_t *next;
};


struct struct_address_t *prompt_address_add()
{
    //read a little bit further than 10 to know if user inputted too much
    char alias[20];
    char raw_addr[20];
    int octect[4];
    fprintf( stdout,"Enter alias: ")
    fgets(alias,20,stdin);
    fflush(stdin);
    //TODO validate
    fprintf( stdout,"Enter address: ")
    fgets(raw_addr,20,stdin);
    fflush(stdin);
    //taken from lecture notes
    sscanf(raw_addr, "%d.%d.%d.%d",octet,octet + 1,octet + 2,octet + 3);
    struct_address_t *new_entry=(*struct_address_t)malloc(sizeof(struct_address_t));
    if(new_entry==NULL)
   {
    fprintf("failed to allocate memory. qutting\n",stderr);
    return -1;
    }

    new_entry->alias=alias;
    new_entry->octet=octet;
    new_entry->next=NULL;
    return new_entry;
}

int print_iden(struct struct_address_t *node,char message[])
{
    printf("%s alias=%s\toctet=%i.%i.%i.%i\n",message,
                                            node->alias,
                                            node->octet[0],
                                            node->octet[1],
                                            node->octet[2],
                                            node->octet[3]);
}




struct struct_address_t *add_to_list(struct struct_address_t* new_node,                                    
                                     bool add_to_end,
                                     struct struct_address_t** head,
                                     struct struct_address_t** curr)
{
    if (NULL == *head)
    {
        *head=new_node;
        *curr=*head;
        printf("\n Node creation success! \n");
        return *head;
    }
    if (add_to_end)
    {
        (*curr)->next = new_node;
        *curr = new_node;
    }
    else
    {
        new_node->next = *head;
        *head = new_node;
    }
    return ptr;
}


//TODO implement octet seraching
struct struct_address_t *search_in_list(int octet[],
                                        char alias[],
                                     struct struct_address_t **prev,\
                                     struct struct_address_t **head,\
                                     struct struct_address_t **curr)
{
    struct struct_address_t *ptr = *head;
    struct struct_address_t *tmp = NULL;
    bool found = false;
    print_iden(octet,alias,"\n Searching the list for value [%d]");
    while (ptr != NULL)
    {
        if (strcmp(ptr->alias,alias)==0)
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

//todo implement for octet
int delete_from_list( int octet[],
                    char alias[],\
                    struct struct_address_t** head,\
                    struct struct_address_t** curr)
{
    struct struct_address_t *prev = NULL;
    struct struct_address_t *del = NULL;
    
    print_iden(octet,alias,"Deleting value from list");
    del = search_in_list(alias, &prev,head,curr);
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
    free(del);
    del = NULL;
    return 0;
}
void print_list(struct struct_address_t* head,\
                struct struct_address_t* curr)
{
    struct struct_address_t *ptr = head;
    printf("\n -------Printing list Start------- \n");
    while (ptr != NULL)
    {
        // printf("\n [%d] \n", ptr->val);
        print_iden(ptr->octet,ptr->alias,"\n");
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");
    return;
}
int main(void)
{
    struct struct_address_t *ptr = NULL;
    ptr=prompt_address_add();
    print_iden(ptr);
    return 0;
    // int i = 0, ret = 0;
    // struct struct_address_t *ptr = NULL;
    // struct struct_address_t *head = NULL;
    // struct struct_address_t *curr = NULL;
    // tmp_oct=[0,0,0,0]
    // person="sean"
    // print_list(head,curr);
    // print_list(head,curr);
    // for (i = 5; i < 10; i++)
        // add_to_list(tmp_oct,person, true,&head,&curr);
    // print_list(head,curr);
    // for (i = 4; i > 0; i--)
        // add_to_list(tmp_oct,person, false,&head,&curr);
    // print_list(head,curr);
    // return 0;
    // for (i = 1; i < 10; i += 4)
    // {
        // ptr = search_in_list(i, NULL,&head,&curr);
        // if (NULL == ptr)
        // {
            // printf("\n Search [val = %d] failed, no such element found\n", i);
        // }
        // else
        // {
            // printf("\n Search passed [val = %d]\n", ptr->val);
        // }
        // print_list(head,curr);
        // ret = delete_from_list(i,&head,&curr);
        // if (ret != 0)
        // {
            // printf("\n delete [val = %d] failed, no such element found\n", i);
        // }
        // else
        // {
            // printf("\n delete [val = %d] passed \n", i);
        // }
        // print_list(head,curr);
    // }
    // return 0;
}