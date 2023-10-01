#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define FILENAME "testing"
#define OCTET_LEN 4
#define MAX_OCTET 255
#define MIN_OCTET 0
#define USE_ALIAS 1
#define USE_ADDR 2
#define USE_HALF_ADDR 3
#define ADD_END true
#define ADD_FRONT false
#define EXPECT_MISSING true
#define EXPECT_PRESENT false

struct struct_address_t
{
    int *octet;
    char *alias;
    struct struct_address_t *next;
};

struct struct_address_t* search_in_list(void*,
                                        int,
                                        struct struct_address_t**,
                                        struct struct_address_t**,
                                        struct struct_address_t**);
char print_menu();
struct struct_address_t* create_node(char*, int*);
char* octet_to_string(int* octet);
int validate_addr(int*,int,struct struct_address_t**,struct struct_address_t** );
bool validate_alias(char* alias, bool,struct struct_address_t**, struct struct_address_t** );
int update_address(struct struct_address_t**, struct struct_address_t** );
struct struct_address_t* prompt_address_add(struct struct_address_t**,
                                             struct struct_address_t** );
void print_iden(struct struct_address_t*, char[]);
struct struct_address_t* add_to_list(struct struct_address_t*,
                                     bool,
                                     struct struct_address_t**,
                                    struct struct_address_t** );
int alias_compare(struct struct_address_t* , void*);
int ip_compare(struct struct_address_t* , void*);
int delete_from_list(void*,
                     bool,
                     struct struct_address_t**,
                     struct struct_address_t**);
void print_list(struct struct_address_t*, struct struct_address_t* ,FILE* );
int look_up_address(struct struct_address_t**, struct struct_address_t**);
struct struct_address_t*  parse_line(char[]);
int read_file(struct struct_address_t**, struct struct_address_t** );
void my_flush(FILE*);
int delete_user_address(struct struct_address_t**, struct struct_address_t** );
int display_alias_for_location(struct struct_address_t**, struct struct_address_t** );
int validate_oct(int,int);
int save_file(struct struct_address_t* ,struct struct_address_t* );

int save_file(struct struct_address_t* head,struct struct_address_t* curr )
{
    FILE* fp;
    fp=fopen(FILENAME,"w");
    if(fp==NULL)
    {
        perror("Unable to write to file");
        return -2;
    }
    else
    {
        print_list(head,curr,fp);
        return 0;
    }

}
char * node_string(struct struct_address_t* node)
{
    static char node_s[26];
    sprintf(node_s,"%s %s\n",octet_to_string(node->octet),node->alias);
    return node_s;
}



int validate_oct(int octet,int matched)
{
    return((matched==1) && octet >=MIN_OCTET && octet<=MAX_OCTET );
}



int display_alias_for_location(struct struct_address_t** head, struct struct_address_t** curr)
{
    //ensure last two octets are set 
    //although shouldn't matter
    int half_addr[4]={0,0,0,0};
    char input[5];
    char* tmp;
    struct struct_address_t* node=*head;
    int matched;
    for(int i=0; i<2; i++)
    {
        do{
            fprintf(stdout,"Enter Address %i: ",i+1);
            memset(input,'\0',5*sizeof(char));
            fgets(input,5,stdin);
            (tmp=strrchr(input, '\n')) ? *tmp='\0': my_flush(stdin);
            matched=sscanf(input, "%i", &half_addr[i]);
        }while(!validate_oct(half_addr[i],matched));
    }
    while((node=search_in_list(&half_addr[0],USE_HALF_ADDR,NULL,&node,curr))!=NULL )
    {
        print_iden(node,"");
        node=node->next;
    }
    return 0;
}


int delete_user_address(struct struct_address_t** head, struct struct_address_t** curr)
{
    char* tmp;
    char tmp_buf[20];
    char input[5];
    int ret;
    struct struct_address_t* node;
    fprintf(stdout, "Enter alias: ");
    fgets(tmp_buf, 20, stdin);
    (tmp=strrchr(tmp_buf, '\n')) ? *tmp='\0': my_flush(stdin);
    if(!validate_alias(tmp_buf,EXPECT_PRESENT,head,curr))
    {
        fprintf(stderr,"Invalid alias to delete\n");
        print_list(*head,*curr,stdout);
    } 
    node=search_in_list(tmp_buf,USE_ALIAS,NULL,head,curr);
    print_iden(node,"delete: ");
    printf("(y/n):");
    fgets(input,5, stdin);
    (tmp=strrchr(input, '\n')) ? *tmp='\0': my_flush(stdin);
    if(strlen(input)<0)
    {
        perror("Invalid option\n");
        return 0;
    }
    if(input[0]=='y') 
    { 
        ret=delete_from_list(tmp_buf,USE_ALIAS,head,curr);
        if(ret==-1)
        {
            perror("Failed to delete node: reason unknown\n");
            return ret;
        }
        else return ret;
    }
    else if(input[0]=='n')
    {
         return 0;
    }
    else {
        perror("Invalid option\n");
        return -3;
    }
    


}


void my_flush(FILE* fp)
{
    char c;
    while((c=getc(fp))!='\n' && c!= EOF);
}
/**
 * @brief Prints out menu and validates menu selection 
 * 
 * @return char - Menu selection '1'-'8'
 */
char print_menu()
{
    bool valid=true;
    char input;
    char temp_buf[10];
    char* tmp;
    char c;
    do
    {
        fprintf(stdout, "1) Add address\n\
2) Look up address\n\
3) Update address\n\
4) Delete address\n\
5) Display list\n\
6) Display aliases for location\n\
7) Save to file\n\
8) Quit\n");
        fgets(temp_buf, 10, stdin);
        (tmp=strrchr(temp_buf, '\n')) ? *tmp='\0': my_flush(stdin);
        if (strlen(temp_buf) > 2) 
        {
            perror("Too much input, please enter [1-8]\n");
            valid = false;
        }
        else if(temp_buf[0]<'1' || temp_buf[0] >'8')
        {
            fprintf(stderr,"You entered %c which is invlaid,\
                            please enter [1-8]\n",temp_buf[0]);
        } 
        else valid=true;
    }while(!valid);
    return temp_buf[0];
}
/**
 * @brief Create a node object
 * Of special note for this function, all nodes live
 * entirely on the heap including their contents.
 * having one function that does all memory allocation
 * world be better but, I couldn't find a way to do that
 * without a memcpy
 * 
 * @param alias char* to already allocated alias 
 * @param octet int* to already allocated memory containing ints 
 * @return struct struct_address_t* 
 */
struct struct_address_t *create_node(char *alias, int *octet)
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

/**
 * @brief  helper function to go backwards from octect to
 *  
 * 
 * @param octet pointer to octet
 * @return char* pointer to static stack space holding string
 * representation of the octet 
 */
char *octet_to_string(int *octet)
{
    static char oct_buf[20];
    sprintf(oct_buf, "%d.%d.%d.%d", *octet, *(octet + 1), *(octet + 2), *(octet + 3));
    return oct_buf;
}
/**
 * @brief determines if octet is valid
 * 
 * @param octet 
 * @return int 1 on valid octet 0 otherwise 
 */
int validate_addr(int *octet,int matched, struct struct_address_t **head,struct struct_address_t **curr)
{
    if(matched!=4)
    {
        perror("error unable to parse input\n");
        return 0;
    }
    for (int i = 0; i < OCTET_LEN; i++)
    {
        if (*(octet + i) > MAX_OCTET || *(octet + i) < MIN_OCTET)
        {
            fprintf(stderr, "error: %s is an illegal address - please reenter:\n", octet_to_string(octet));
            return 0;
        }
    }
    if(search_in_list(octet,USE_ADDR,NULL,head,curr)!=NULL)
    {
        fprintf(stderr, "error: %s is already taken \n", octet_to_string(octet));
        return 0;
    }
    return 1;
}


//assumes new line stripped
bool validate_alias(char* alias,
                     bool expect_missing,
                     struct struct_address_t **head,
                    struct struct_address_t **curr
                    )
{
    char c;
    if(strlen(alias)<1 || strlen(alias)>10) return false;
        //converting alias to lowercase
    for(int i=0; *(alias+i)!='\0'; i++)
    {
        *(alias+i)=tolower(*(alias+i));
    }

    if(!(search_in_list(alias,USE_ALIAS,NULL,head,curr)!=NULL ^ expect_missing))
    {
        if(expect_missing) fprintf(stderr,"Invalid %s is in the list\n",alias);
        else  fprintf(stderr,"Invalid %s is not in the list\n",alias);
        return false;
    }
    else
    {
        return true;
    } 
}

int update_address(struct struct_address_t **head,
                    struct struct_address_t **curr)
{
    struct struct_address_t *node;
    char buf[20];
    int octet=-1;
    int valid;
    char *tmp;
    fprintf(stdout, "Enter alias: ");
    fgets(buf, 20, stdin);

    (tmp=strrchr(buf, '\n')) ? *tmp='\0': (void)0;
    if(tmp==NULL) my_flush(stdin);

    if(!validate_alias(buf,false,head,curr))
    {
        fprintf(stderr,"Invalid entry for alias\n");
        return 0;
    }
    //garunteed to succeed since we already searched the list
    //inefficent but I am not refactoring at this point
    node=search_in_list(buf,USE_ALIAS,NULL,head,curr);
    print_iden(node,"Update address for: ");
    for (int i=1; i<5; i++)
    {
        do{
            fprintf(stdout, "Enter location value #%i (0-255):",i);
            fgets(buf, 20, stdin);
            (tmp=strrchr(buf, '\n')) ? *tmp='\0': (void)0;
            if(tmp==NULL) my_flush(stdin);
            my_flush(stdin);
            if((valid=sscanf(buf,"%i",&octet))==1)
            {
                perror("problem scanning input. Please enter input (0-255)");
            }
            if(valid=(valid && (octet>0 && octet < 255)))
            {
                fprintf(stderr,"error %i is an illegal entry - please reenter:\n",octet);
            }
            if(valid)
            {
                *(node->octet +i-1)=octet;
            }
        }
        while(!valid);
    }
}




/**
 * @brief prompt user for alias and address
 *  allocates memory for user input 
 *  calls create_node with input
 * @return struct struct_address_t* 
 */
struct struct_address_t *prompt_address_add(struct struct_address_t **head,
                                            struct struct_address_t **curr
                                            )
{
    char *alias = malloc(20 * sizeof(char));
    char raw_addr[20];
    int *octet = malloc(4 * sizeof(int));
    char * tmp;
    int matched;
    struct struct_address_t *node;
    fprintf(stdout, "Enter alias: ");
    fgets(alias, 20, stdin);
    (tmp=strrchr(alias, '\n')) ? *tmp='\0': (void)0;
    if(tmp==NULL) my_flush(stdin);
    if(!validate_alias(alias,true,head,curr)) return NULL;

    //memset is to clear prior input from buffer
    memset(raw_addr,'\0',20*sizeof(char));
    fprintf(stdout, "Enter address: ");
    fgets(raw_addr, 20, stdin);
    (tmp=strrchr(raw_addr, '\n')) ? *tmp='\0': (void)0;
    if(tmp==NULL) my_flush(stdin);
    // taken from lecture notes
    matched=sscanf(raw_addr, "%d.%d.%d.%d",
                     octet, octet + 1, octet + 2, octet + 3);
    if(!validate_addr(octet,matched,head,curr)) return NULL;
    node= create_node(alias, octet);
    if(node==NULL)
    {
        perror("error: malloc() failed to create node.\n\
Not exiting but you should save your work\n");
    }
    return node;
}


/**
 * @brief helper to print representation of a node 
 * this is opposite pattern of the octet_to_string
 * 
 * @param node 
 * @param message 
 * @return void 
 */
void print_iden(struct struct_address_t *node, char message[])
{
    printf("%s alias=%s\toctet=%i.%i.%i.%i\n", message,
           node->alias,
           node->octet[0],
           node->octet[1],
           node->octet[2],
           node->octet[3]);
}

/**
 * @brief adds a node to the linked list
 * 
 * @param new_node pointer to new node
 * @param add_to_end true if adding to end O(n)
 *  false if adding to beggining O(1) 
 * @param head pointer to head pointer
 * @param curr pointer to curr pointer
 * @return struct struct_address_t* 
 */
struct struct_address_t *add_to_list(struct struct_address_t *new_node,
                                     bool add_to_end,
                                     struct struct_address_t **head,
                                     struct struct_address_t **curr)
{
    if (NULL == *head)
    {
        *head = new_node;
        *curr = *head;
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
    return *head;
}
/**
 * @brief comparator function, wrapper for strcmp
 * 
 * @param ptr 
 * @param v_alias 
 * @return int 
 */
int alias_compare(struct struct_address_t *ptr, void *v_alias)
{
    char *alias = (char *)v_alias;
    return (strcmp(ptr->alias, alias));
}
/**
 * @brief comparator function,  
 * 
 * @param ptr 
 * @param v_octet 
 * @return int numerical difference of first non matching octet 
 */
int ip_compare(struct struct_address_t *ptr, void *v_octet)
{

    int *octet = (int *)v_octet;
    // returns the difference between the first non matching int
    for (int i = 0; i < OCTET_LEN; i++)
    {
        if (*(ptr->octet + i) != *(octet+i))
        {
            return *(ptr->octet + i) - *(octet+i);
        }
    }
    return 0;
}
/**
 * @brief comparator function, same as ip but only checks first two
 * octets  
 * 
 * @param ptr 
 * @param v_octet 
 * @return int numerical difference of first non matching octet 
 */
int half_compare(struct struct_address_t *ptr, void *v_octet)
{

    int *octet = (int *)v_octet;

    // returns the difference between the first non matching int
    for (int i = 0; i < 2; i++)
    {
        if (*(ptr->octet + i) != *(octet+i))
        {
            return *(ptr->octet + i) - *(octet+i);
        }
    }
    return 0;
}

/**
 * @brief Searches in list itertively for input
 * 
 * @param input void * to either a char or int or known size
 * @param use_alias true if void* is a pointer to char
 *                  false if void* is a pointer to int 
 * @param prev used by the section of function from lecture notes
 * @param head 
 * @param curr 
 * @return struct struct_address_t* pointer to matching node if found
 * NULL otherwise 
 */
struct struct_address_t *search_in_list(void *input,
                                        int input_type,
                                        struct struct_address_t **prev,
                                        struct struct_address_t **head,
                                        struct struct_address_t **curr)
{
    struct struct_address_t *ptr = *head;
    struct struct_address_t *tmp = NULL;
    int (*compare)(struct struct_address_t *, void *);
    
    bool found = false;
    if (input_type==USE_ALIAS)  compare = alias_compare;
    else if(input_type==USE_ADDR) compare = ip_compare; 
    else compare=half_compare;
    while (ptr != NULL)
    {
        if (compare(ptr, input) == 0)
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

/**
 * @brief deletes nodes from linked list if found and frees memory
 * 
 * @param input 
 * @param use_alias 
 * @param head 
 * @param curr 
 * @return int 0 on succesful delete
 * -1 otherwise
 */
int delete_from_list(void *input,
                     bool use_alias,
                     struct struct_address_t **head,
                     struct struct_address_t **curr)
{
    struct struct_address_t *prev = NULL;
    struct struct_address_t *del = NULL;

    // printf("Deleting value from list %s \n", alias);
    del = search_in_list(input, use_alias, &prev, head, curr);
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
    // TODO implement node free function
    print_iden(del,"deleted");
    free(del);
    del = NULL;
    return 0;
}

/**
 * @brief traverse list and prints out nodes in order
 * 
 * @param head 
 * @param curr 
 */
void print_list(struct struct_address_t *head,
                struct struct_address_t *curr,
                FILE* fp)
{
    int i =0;
    struct struct_address_t *ptr = head;
    printf("\n -------Printing list Start------- \n");
    while (ptr != NULL)
    {
        fprintf(fp,"%s",node_string(ptr));
        i+=1;
        ptr = ptr->next;
    }
    if(fp==stdout)
    {
        fprintf(fp, "Total node count: %i\n",i);
    } 
    // else
    // {
    //  remove trailing newline
        // fseek(fp,-1,SEEK_END);
        // fprintf(fp,"%c",' ');   
    // }
    
    printf("\n -------Printing list End------- \n");
    return;
}
/**
 * @brief prompt user for alias and searches list to find the matching address
 *  
 * @param head 
 * @param curr 
 * @return int 
 */
int look_up_address(struct struct_address_t **head,
                    struct struct_address_t **curr)
{

    static char alias[20];
    char * tmp;
    fprintf(stdout, "Enter alias: ");
    fgets(alias, 20, stdin);
    (tmp=strrchr(alias, '\n')) ? *tmp='\0': my_flush(stdin);
    if(strlen(alias)>10) return -1;


    struct struct_address_t *ptr;
    ptr = search_in_list(alias, true, NULL, head, curr);
    if (ptr == NULL)
    {
        fprintf(stderr, "error %s does not exist\n", alias);
    }
    else
    {
        fprintf(stdout, "Address for %s: %s\n", alias, octet_to_string(ptr->octet));
    }
    return 0;
}

/**
 * @brief create a node from a single line of input file
 * 
 * @param line 
 * @return struct struct_address_t* 
 */
struct struct_address_t *parse_line(char line[])
{
// Assume the file is well formatted
    char *alias = malloc(11 * sizeof(char));
    int *octet = malloc(4 * sizeof(int));
    if(alias ==NULL || octet==NULL) return NULL;
    sscanf(line, "%d.%d.%d.%d %s\n", octet, octet + 1, octet + 2, octet + 3, alias);
    struct struct_address_t * node;
    node= create_node(alias, octet);
    return node;
}


/**
 * @brief read input file and add nodes to linked list 
 * one line at a time
 * 
 * @param head 
 * @param curr 
 * @return int 0 on success,
 *          -2 on fp error,
 *          -1 on memory allocation error
 */
int read_file(struct struct_address_t **head,
              struct struct_address_t **curr)
{
    FILE *fp;
    struct struct_address_t *node;
    fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        return -2;
    }
    char line[50];
    //not doing flush because assuming file is safe
    while (fgets(line, 50, fp) != NULL)
    {
        node = parse_line(line);
        if (node == NULL) return -1;
        add_to_list(node, true, head, curr);
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
    //TODO check if initial print is required
    // print_list(head, curr,stdout);
    char selection;
    while (1)
    {
        selection = print_menu();
        if (selection == '1') (ptr=prompt_address_add(&head,&curr)) == NULL ?
                                        (void)0:
                                        add_to_list(ptr,ADD_FRONT,&head,&curr);
        else if (selection == '2')  look_up_address(&head, &curr);
        else if (selection == '3')  update_address(&head,&curr);
        else if (selection == '4')  delete_user_address(&head,&curr);
        else if (selection == '5')  print_list(head,curr,stdout);
        else if (selection == '6')  display_alias_for_location(&head,&curr);
        else if (selection == '7')  save_file(head,curr);
        else if (selection == '8')  return 0;
        else (void)0;
    }




    // int test[] = {1, 2, 3, 4};
    // add_to_list(ptr, false, &head, &curr);
    // delete_from_list("jet", true, &head, &curr);
    // print_list(head, curr);
    // search_in_list("jet", true, &prev, &head, &curr);
    // print_iden(search_in_list("baker", true, &prev, &head, &curr), "found in search");
    // print_iden(ptr, "");
    // return 0;

    // int i = 0, ret = 0;
}