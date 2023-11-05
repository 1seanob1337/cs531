
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "abc.txt"
// "CS531_Inet.txt"
#define DEBUG 0
#define EXPECT_MISSING true
#define EXPECT_PRESENT false
#define OCTET_LEN 4
#define MAX_OCTET 255
#define MIN_OCTET 0
struct address_t {
    int octet[4];
    char alias[11];
    struct address_t *leftChild, *rightChild, *parent;
    int height, depth;
};

// union to suppress compilation warnings about void*
union Accumulator {
    void *p;
    int i;
};

/**
 * @brief Flushes the input buffer.
 *
 * @param fp A file pointer to the input stream.
 */
void my_flush(FILE *fp);

/**
 * @brief Compares half of an address against a node.
 *
 * @param key Pointer to the key to compare.
 * @param node Pointer to the node for comparison.
 * @return int Numerical difference between the halves.
 */
int compare_half(void *key, struct address_t *node);

/**
 * @brief Saves the current list of addresses to a file.
 *
 * @param head Pointer to the head of the address list.
 * @return int 0 on success, error code otherwise.
 */
int save_file(struct address_t *head);

/**
 * @brief Converts an address octet to a string.
 *
 * @param octet Pointer to the array of octets.
 * @return char* String representation of the address.
 */
char *octet_to_string(int *octet);

/**
 * @brief Creates a new node with given alias and octet.
 *
 * @param alias String alias for the new node.
 * @param octet Integer array representing the address octet.
 * @return struct address_t* Pointer to the new node.
 */
struct address_t *create_node(char *alias, int *octet);

/**
 * @brief Reads addresses from a file and builds a tree.
 *
 * @param head Pointer to the head of the address tree.
 * @return struct address_t* Pointer to the head of the tree.
 */
struct address_t *read_file(struct address_t *head);

/**
 * @brief Performs an inorder traversal of the address tree and applies a
 * function.
 *
 * @param head Pointer to the current node.
 * @param print_function Function to apply to each node.
 * @param fp File pointer for output stream.
 */
void inorder(struct address_t *head,
             void (*print_function)(struct address_t *, char[], FILE *),
             FILE *fp);

/**
 * @brief Inserts a node into the address tree.
 *
 * @param node Pointer to the node to insert.
 * @param head Pointer to the head of the tree.
 * @return struct address_t* Pointer to the new head of the tree.
 */
struct address_t *insert(struct address_t *node, struct address_t *head);

/**
 * @brief Deletes a node with the given alias from the tree.
 *
 * @param root Pointer to the root of the tree.
 * @param alias Alias string of the node to delete.
 * @return struct address_t* Pointer to the new root of the tree.
 */
struct address_t *deleteNode(struct address_t *root, char *alias);

/**
 * @brief Searches the tree for a node with a given alias.
 *
 * @param key Pointer to the alias to search for.
 * @param head Pointer to the head of the tree.
 * @param compare Function used to compare nodes.
 * @return struct address_t* Pointer to the found node or NULL.
 */
struct address_t *search_in_list(void *key, struct address_t *head,
                                 int (*compare)(void *,
                                                struct address_t *));

/**
 * @brief Validates an alias according to specific rules.
 *
 * @param alias Alias to validate.
 * @param expect_missing Flag to indicate if the alias is expected to be
 * missing or present in the tree.
 * @param head Pointer to the head of the tree.
 * @return bool True if alias is valid, false otherwise.
 */
bool validate_alias(char *alias, bool expect_missing,
                    struct address_t *head);

/**
 * @brief Validates a given address octet array.
 *
 * @param octet Array of octets to validate.
 * @param matched Number of octets read.
 * @param head Pointer to the head of the tree for additional validation.
 * @return int 1 if address is valid, 0 otherwise.
 */
int validate_addr(int *octet, int matched, struct address_t *head);

/**
 * @brief Displays a user menu and validates the input.
 *
 * @return char User's menu selection.
 */
char print_menu();

/**
 * @brief Prints a node's data with an identifying message to a file.
 *
 * @param node Pointer to the node to print.
 * @param message Message to prepend to the node's data.
 * @param fp File pointer to the output stream.
 */
void print_iden(struct address_t *node, char message[], FILE *fp);

/**
 * @brief Prints a node's data in file format to a file.
 *
 * @param node Pointer to the node to print.
 * @param message Message to prepend to the node's data (unused).
 * @param fp File pointer to the output stream.
 */
void print_file_format(struct address_t *node, char message[], FILE *fp);

/**
 * @brief Prompts the user for address information and adds it to the tree.
 *
 * @param head Pointer to the head of the address tree.
 * @return struct address_t* Pointer to the newly created node.
 */
struct address_t *prompt_address_add(struct address_t *head);

/**
 * @brief Compares the alias of a node to a given key.
 *
 * @param key Pointer to the key for comparison.
 * @param node Pointer to the node for comparison.
 * @return int Result of strcmp between key and node's alias.
 */
int compare_alias(void *key, struct address_t *node);

/**
 * @brief Validates an address based on specific octet rules.
 *
 * @param octet Octet to validate.
 * @param matched Indicator of whether an octet was successfully matched.
 * @return int 1 if octet is valid, 0 otherwise.
 */
int validate_oct(int octet, int matched);

/**
 * @brief Prompts the user for an alias and displays the corresponding
 * address.
 *
 * @param head Pointer to the head of the address tree.
 * @return int 0 on success, -1 if alias is too long.
 */
int look_up_address(struct address_t *head);

/**
 * @brief Displays all aliases corresponding to a specific location.
 *
 * @param head Pointer to the head of the address tree.
 * @return int 0 on success, error code otherwise.
 */
int display_alias_for_location(struct address_t *head);

/**
 * @brief Deletes a user's address from the tree after confirmation.
 *
 * @param head Pointer to the head of the address tree.
 * @return struct address_t* Pointer to the new head of the tree.
 */
struct address_t *delete_user_address(struct address_t *head);

/**
 * @brief Applies a function to each node in the address tree while a
 * condition is true.
 *
 * @param key Pointer to the key used in the comparison function.
 * @param head Pointer to the current node of the tree.
 * @param compare Function pointer to the comparison function.
 * @param f Function pointer to the function to apply to each node.
 * @param acc Accumulator to hold the result of the applied function.
 * @return void* Accumulator after folding the tree.
 */
void *foldWhile(void *key, struct address_t *head,
                int (*compare)(void *, struct address_t *),
                void *(*f)(struct address_t *, void *), void *acc);

/**
 * @brief Searches the address tree in order and returns the first node
 * that matches the given key.
 *
 * @param key Pointer to the key used in the comparison function.
 * @param head Pointer to the head of the address tree.
 * @param compare Function pointer to the comparison function.
 * @return struct address_t* Pointer to the found node or NULL if no match
 * is found.
 */
struct address_t *searchInOrder(void *key, struct address_t *head,
                                int (*compare)(void *,
                                               struct address_t *));

/**
 * @brief Compares the full address against a node.
 *
 * @param key Pointer to the key to compare.
 * @param ptr Pointer to the node for comparison.
 * @return int Numerical difference of first non-matching octet.
 */
int compare_addr(void *key, struct address_t *ptr);

/**
 * @brief Updates an address associated with an alias.
 *
 * @param head Pointer to the head of the address tree.
 * @return struct address_t* Pointer to the updated head of the tree.
 */
struct address_t *update_address(struct address_t *head);

/**
 * @brief Calculates the maximum depth of the left or right subtree.
 *
 * @param left Pointer to the left child node.
 * @param right Pointer to the right child node.
 * @return int Maximum depth of the subtree.
 */
int node_max_depth(struct address_t *left, struct address_t *right);

/**
 * @brief Calculates the maximum height of the left or right subtree.
 *
 * @param left Pointer to the left child node.
 * @param right Pointer to the right child node.
 * @return int Maximum height of the subtree.
 */
int node_max_height(struct address_t *left, struct address_t *right);

/**
 * @brief Converts a node's address and alias into a string.
 *
 * @param node Pointer to the node to convert.
 * @return char* String representation of the node.
 */
char *node_string(struct address_t *node);

/**
 * @brief Recalculates the subtree's height and depth starting from a given
 * root.
 *
 * @param root Pointer to the root node where recalculation begins.
 */
void recalc_subtree(struct address_t *root);

/**
 * @brief Recalculates the height and depth of a single node.
 *
 * @param node Pointer to the node for recalculation.
 */
void recalc_node(struct address_t *node);

/**
 * @brief Finds the node with the minimum value in a subtree.
 *
 * @param node Pointer to the root node of the subtree.
 * @return struct address_t* Pointer to the node with the minimum value.
 */
struct address_t *minValueNode(struct address_t *node);

/**
 * @brief Parses a line of input to create an address node.
 *
 * @param line String containing one line of input.
 * @return struct address_t* Pointer to the created node.
 */
struct address_t *parse_line(char line[]);

/**
 * @brief Prints aliases for each node in the address tree that matches a
 * certain condition.
 *
 * @param node Pointer to the current node.
 * @param acc Accumulator to keep count.
 * @return void* Updated count of printed aliases.
 */
void *print_alias(struct address_t *node, void *acc);

int max(int x, int y) { return x > y ? x : y; }

int node_max_depth(struct address_t *left, struct address_t *right) {
    int l_depth, r_depth;
    l_depth = left ? left->depth : 0;
    r_depth = right ? right->depth : 0;
    return max(l_depth, r_depth);
}

int node_max_height(struct address_t *left, struct address_t *right) {
    int l_height, r_height;
    l_height = left ? left->height : -1;
    r_height = right ? right->height : -1;
    return max(l_height, r_height);
}
char *node_string(struct address_t *node) {
    static char node_s[26];
    sprintf(node_s, "%s %s\n", octet_to_string(node->octet), node->alias);
    return node_s;
}

int save_file(struct address_t *head) {
    FILE *fp;
    printf("Enter filename:");
    char fname[FILENAME_MAX];
    char *tmp;
    fgets(fname, FILENAME_MAX, stdin);
    (tmp = strrchr(fname, '\n')) ? *tmp = '\0' : my_flush(stdin);
    fp = fopen(fname, "w");
    if (fp == NULL) {
        fprintf(stderr, "Unable to write to file");
        return -2;
    } else {
        if (DEBUG)
            printf(" attempting to write file from save_file\n");
        inorder(head, print_file_format, fp);
    }
    fclose(fp);
    return 0;
}
/**
 * @brief create a node from a single line of input file
 *
 * @param line
 * @return struct address_t*
 */
struct address_t *parse_line(char line[]) {
    // Assume the file is well formatted
    // char *alias = malloc(11 * sizeof(char));
    // int *octet = malloc(4 * sizeof(int));
    char alias[11];
    int octet[4];
    if (alias == NULL || octet == NULL)
        return NULL;
    sscanf(line, "%d.%d.%d.%d %s\n", octet, octet + 1, octet + 2,
           octet + 3, alias);
    struct address_t *node;
    node = create_node(alias, octet);
    return node;
}

char *octet_to_string(int *octet) {
    static char oct_buf[20];
    sprintf(oct_buf, "%d.%d.%d.%d", *octet, *(octet + 1), *(octet + 2),
            *(octet + 3));
    return oct_buf;
}

struct address_t *create_node(char *alias, int *octet) {
    struct address_t *new_entry =
        (struct address_t *)malloc(sizeof(struct address_t));
    if (new_entry == NULL) {
        fprintf(stderr, "failed to allocate memory. qutting\n");
        return NULL;
    }

    strcpy(new_entry->alias, alias);
    memcpy(new_entry->octet, octet, sizeof(int) * 4);
    new_entry->leftChild = NULL;
    new_entry->rightChild = NULL;
    new_entry->parent = NULL;
    // when node is new it is floating and has height/depth of zero
    new_entry->height = 0;
    new_entry->depth = 0;
    return new_entry;
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
struct address_t *read_file(struct address_t *head) {
    FILE *fp;
    struct address_t *node;
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file\n");
        return NULL;
    }
    char line[50];
    // not doing flush because assuming file is safe
    while (fgets(line, 50, fp) != NULL) {
        node = parse_line(line);
        if (node == NULL)
            return NULL;
        // printf("before insert of %s\t
        // head=%s\n",node->alias,head->alias);
        head = insert(node, head);
    }
    fclose(fp);
    return (head);
}

void inorder(struct address_t *head,
             void (*print_function)(struct address_t *, char message[],
                                    FILE *fp),
             FILE *fp) {
    if (head != NULL) {

        inorder(head->leftChild, print_function, fp);
        print_function(head, " ", fp);
        inorder(head->rightChild, print_function, fp);
    }
}
/*
Function that applies function f to
all elements that satisfy function compare
*/

void *foldWhile(void *key, struct address_t *head,
                int (*compare)(void *, struct address_t *),
                void *(*f)(struct address_t *, void *acc), void *acc) {
    if (head) {
        if (compare(key, head) == 0)
            acc = f(head, acc);
        acc = foldWhile(key, head->leftChild, compare, f, acc);
        acc = foldWhile(key, head->rightChild, compare, f, acc);
    }
    // if(DEBUG) printf("acc = %d\n",(int)acc);
    return acc;
}

// function to simply return the first inorder match of the comparator
struct address_t *searchInOrder(void *key, struct address_t *head,
                                int (*compare)(void *,
                                               struct address_t *)) {
    struct address_t *temp;
    if (head != NULL) {
        if (compare(key, head) == 0)
            return head;
        else {
            if (DEBUG)
                printf("recur left inorder\n");
            if ((temp = searchInOrder(key, head->leftChild, compare)))
                return temp;
            if (DEBUG)
                printf("recur right inorder\n");
            if ((temp = searchInOrder(key, head->rightChild, compare)))
                return temp;
        }
    }
    // if the whole stack collapses without a match return NULL
    return NULL;
}

struct address_t *insert(struct address_t *node, struct address_t *head) {
    /* If the tree is empty, return a new node */
    if (head == NULL) {
        if (DEBUG)
            printf("head empty returning node %s\n", node->alias);
        // since tree is empty head has depth=0, height=0
        return node;
    }
    /* Otherwise, recur down the tree */
    // TODO consider wrapping whole thing in else
    // to be explicit that this is the recursive case
    // and we know head is non null
    node->depth += 1;
    node->parent = head;
    if (strcmp(node->alias, head->alias) < 0) {
        if (DEBUG)
            ("recur left\n");
        // this will be child of head therefore depth of this node needs
        // incrementing
        head->leftChild = insert(node, head->leftChild);
        head->height =
            1 + node_max_height(head->leftChild, head->rightChild);
    } else {
        if (DEBUG)
            printf("recur right\n");
        head->rightChild = insert(node, head->rightChild);
        head->height =
            1 + node_max_height(head->leftChild, head->rightChild);
    }
    /* return the (unchanged) node pointer */
    if (DEBUG)
        print_iden(head, "finished inserting", stdout);
    return head;
}

/* Given a non-empty binary search tree, return the node with the minimum
key value found in that tree. */
struct address_t *minValueNode(struct address_t *node) {
    struct address_t *current = node;
    /* loop down to find the leftmost leaf */
    while (current && current->leftChild != NULL)
        current = current->leftChild;
    return current;
}

void recalc_subtree(struct address_t *root) {
    if (!root)
        return;
    if (DEBUG)
        printf("recalc subtree for %s\n", root->alias);
    // first need to calc children
    root->depth = root->parent ? root->parent->depth + 1 : 0;
    recalc_subtree(root->leftChild);
    recalc_subtree(root->rightChild);
    root->height = 1 + node_max_height(root->leftChild, root->rightChild);
    // if I have a parent I am one deeper, otherwise I am root
    return;
}

void recalc_node(struct address_t *root) {
    if (!root)
        return;
    root->height = 1 + node_max_height(root->leftChild, root->rightChild);
    // if I have a parent I am one deeper, otherwise I am root
    root->depth = root->parent ? root->parent->depth + 1 : 0;
    return;
}

struct address_t *deleteNode(struct address_t *root, char *alias) {
    // my helper method makes inserts linear time : (
    if (root == NULL)
        return root;
    // no need to recalc subtree recursively in the recursive case but i
    // has to be done for current node after recursing
    if (strcmp(alias, root->alias) < 0) {
        root->leftChild = deleteNode(root->leftChild, alias);
        recalc_node(root);
    } else if (strcmp(alias, root->alias) > 0) {
        root->rightChild = deleteNode(root->rightChild, alias);
        recalc_node(root);
    }
    // if alias is same as root's alias, then this is the node
    // to be deleted
    else {
        struct address_t *grandParent = root->parent;
        int t_depth = root->depth - 1;
        // node with only one child or no child
        // the only child will have height of zero but will be a depth of
        // parent-1 it will also inherent parent's parent if (root-)
        if (root->leftChild == NULL && root->rightChild == NULL) {
            if (DEBUG)
                printf("no children");
            free(root);
            return NULL;
        }
        if (root->leftChild == NULL) {
            struct address_t *temp = root->rightChild;
            free(root);
            temp->parent = grandParent;
            recalc_subtree(temp);
            return temp;
        } else if (root->rightChild == NULL) {
            struct address_t *temp = root->leftChild;
            free(root);
            temp->parent = grandParent;
            recalc_subtree(temp);
            return temp;
        }
        // TODO consider doing this without mutating memory
        //  node with two children: Get the inorder successor (smallest
        //  in the rightChild subtree)
        struct address_t *temp = minValueNode(root->rightChild);
        // Copy the inorder successor's content to this node
        strcpy(root->alias, temp->alias);
        // Delete the inorder successor
        root->rightChild = deleteNode(root->rightChild, temp->alias);
        recalc_subtree(root);
        // need to recalculate height, depth should be the same
    }
    return root;
}

struct address_t *search_in_list(void *key, struct address_t *head,
                                 int (*compare)(void *,
                                                struct address_t *)) {
    if (head == NULL)
        return NULL;

    // if (strcmp(key, head->alias) == 0) return head;
    if (compare(key, head) == 0)
        return head;
    if (compare(key, head) < 0) {
        if (DEBUG)
            printf("recur left\n");
        search_in_list(key, head->leftChild, compare);
    } else {
        if (DEBUG)
            printf("recur right\n");
        search_in_list(key, head->rightChild, compare);
    }
    /* return the (unchanged) node pointer */
}

// assumes new line stripped
bool validate_alias(char *alias, bool expect_missing,
                    struct address_t *head) {
    char c;
    if (strlen(alias) < 1 || strlen(alias) > 10)
        return false;
    // converting alias to lowercase
    for (int i = 0; *(alias + i) != '\0'; i++) {
        *(alias + i) = tolower(*(alias + i));
    }
    if (!(search_in_list(alias, head, &compare_alias) != NULL ^
          expect_missing)) {
        if (expect_missing)
            fprintf(stderr, "Invalid %s is in the list\n", alias);
        else
            fprintf(stderr, "Invalid %s is not in the list\n", alias);
        return false;
    } else {
        return true;
    }
}

void my_flush(FILE *fp) {
    char c;
    while ((c = getc(fp)) != '\n' && c != EOF)
        ;
}
/**
 * @brief prompt user for alias and address
 *  allocates memory for user input
 *  calls create_node with input
 * @return struct struct_address_t*
 */
struct address_t *prompt_address_add(struct address_t *head) {
    char alias[12];
    char raw_addr[20];
    int octet[4];
    char *tmp;
    int matched;
    struct address_t *node;
    bool valid;
    // TODO change to check strlen for reprompt

    do {
        // memset is to clear prior input from buffer
        memset(raw_addr, '\0', 20 * sizeof(char));
        fprintf(stdout, "Enter address: ");
        fgets(raw_addr, 20, stdin);
        (tmp = strrchr(raw_addr, '\n')) ? *tmp = '\0' : (void)0;
        if (tmp == NULL)
            my_flush(stdin);
        // taken from lecture notes
        matched = sscanf(raw_addr, "%d.%d.%d.%d", octet, octet + 1,
                         octet + 2, octet + 3);
    } while (!validate_addr(octet, matched, head));

    do {
        fprintf(stdout, "Enter alias: ");
        fgets(alias, 12, stdin);
        (tmp = strrchr(alias, '\n')) ? *tmp = '\0' : my_flush(stdin);
        valid = (strlen(alias) <= 10);
        if (!valid)
            fprintf(stderr,
                    "Error: enter alias with length 10 or less.\n");
    } while (!valid);

    if (!validate_alias(alias, EXPECT_MISSING, head))
        return NULL;

    node = create_node(alias, octet);
    if (node == NULL) {
        perror("error: malloc() failed to create node.\n\
Not exiting but you should save your work\n");
        return NULL;
    }
    if (DEBUG)
        print_iden(node, "returning from prompt_add: ", stdout);
    return node;
}

int compare_alias(void *key, struct address_t *node) {
    return strcmp((char *)key, node->alias);
}

/**
 * @brief comparator function, same as ip but only checks first two
 * octets
 *
 * @param ptr
 * @param v_octet
 * @return int numerical difference of first non matching octet
 */
int compare_half(void *key, struct address_t *node) {
    int *octet = (int *)key;
    // returns the difference between the first non matching int
    for (int i = 0; i < 2; i++) {
        if (*(node->octet + i) != *(octet + i)) {
            return *(node->octet + i) - *(octet + i);
        }
    }
    return 0;
}
int compare_addr(void *key, struct address_t *ptr) {
    int *octet = (int *)key;
    // returns the difference between the first non matching int
    for (int i = 0; i < OCTET_LEN; i++) {
        if (*(ptr->octet + i) != *(octet + i)) {
            return *(ptr->octet + i) - *(octet + i);
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

int validate_addr(int *octet, int matched, struct address_t *head) {
    if (matched != 4) {
        perror("error unable to parse input\n");
        return 0;
    }
    for (int i = 0; i < OCTET_LEN; i++) {
        if (*(octet + i) > MAX_OCTET || *(octet + i) < MIN_OCTET) {
            fprintf(stderr,
                    "error: %s is an illegal address - please reenter:\n",
                    octet_to_string(octet));
            return 0;
        }
    }
    // TODO consider replacing searchInOrder with foldWhile which is more
    // generic possibly build a linked list with the matches? something
    // like foldWhile(octet,head,&compare_addr,insert_ll, Null head )
    if (searchInOrder(octet, head, &compare_addr) != NULL) {
        fprintf(stderr, "error: %s is already taken \n",
                octet_to_string(octet));
        return 0;
    }
    return 1;
}

/**
 * @brief Prints out menu and validates menu selection
 *
 * @return char - Menu selection '1'-'8'
 */
char print_menu() {
    bool valid = true;
    char input;
    char temp_buf[10];
    char *tmp;
    char c;
    do {
        fprintf(stdout, "1) Add address\n\
2) Look up address\n\
3) Update address\n\
4) Delete address\n\
5) Display list\n\
6) Display aliases for location\n\
7) Save to file\n\
8) Quit\n");
        fgets(temp_buf, 10, stdin);
        (tmp = strrchr(temp_buf, '\n')) ? *tmp = '\0' : my_flush(stdin);
        if (strlen(temp_buf) > 2) {
            perror("Too much input, please enter [1-8]\n");
            valid = false;
        } else if (temp_buf[0] < '1' || temp_buf[0] > '8') {
            fprintf(stderr, "You entered %c which is invlaid,\
                            please enter [1-8]\n",
                    temp_buf[0]);
        } else
            valid = true;
    } while (!valid);
    return temp_buf[0];
}

void print_iden(struct address_t *node, char message[], FILE *fp) {

    const char *p_alias = node->parent ? node->parent->alias : "NONE";
    fprintf(fp, "%s%s\t%i.%i.%i.%i\theight:%i\tdepth:%i\tparent:%s\n",
            message, node->alias, node->octet[0], node->octet[1],
            node->octet[2], node->octet[3], node->height, node->depth,
            p_alias);
}

void print_file_format(struct address_t *node, char message[], FILE *fp) {
    // different print function that prints node in format
    // neded for writing to file. takes message for compatibility
    // but it is thrown away
    fprintf(fp, "%i.%i.%i.%i %s\n", node->octet[0], node->octet[1],
            node->octet[2], node->octet[3], node->alias);
}

// new approach needed for update since we now care about order.
//  1) search for existing node
//  2) delete existing node
//  3) create node from validated address and existing alias.
//  4) insert new node
struct address_t *update_address(struct address_t *head) {
    struct address_t *node;
    char buf[20];
    int octet = -1;
    static int tmp_addr[4];
    int valid;
    char *tmp;
    // primpt user alias and validate
    fprintf(stdout, "Enter alias: ");
    fgets(buf, 20, stdin);

    (tmp = strrchr(buf, '\n')) ? *tmp = '\0' : my_flush(stdin);
    if (tmp == NULL)
        my_flush(stdin);

    if (!validate_alias(buf, EXPECT_PRESENT, head)) {
        fprintf(stderr, "Invalid entry for alias\n");
        return head;
    }
    // garunteed to succeed since we already searched the list
    // inefficent but I am not refactoring at this point

    node = search_in_list(buf, head, compare_alias);
    // buf holds alias
    print_iden(node, "Update address for: ", stdout);
    for (int i = 1; i < 5; i++) {
        do {
            fprintf(stdout, "Enter location value #%i (0-255):", i);
            fgets(buf, 20, stdin);
            (tmp = strrchr(buf, '\n')) ? *tmp = '\0' : my_flush(stdin);
            if ((valid = sscanf(buf, "%i", &octet)) != 1) {
                fprintf(
                    stderr, "%s",
                    "problem scanning input. Please enter input (0-255)");
            } else if (!(valid = (valid && ((octet >= MIN_OCTET) &
                                            (octet <= MAX_OCTET))))) {
                fprintf(stderr,
                        "error %i is an illegal entry - please reenter:\n",
                        octet);
            } else {
                tmp_addr[i - 1] = octet;
            }
        } while (!valid);
    }
    if (validate_addr(tmp_addr, 4, head)) {
        // now ready to do update.
        /**
         * todo I am searching the list four times
         * 1 validate the alias being given to me
         * 2 validate the address being given to me
         * 3 find the node the alias belongs to just to print it out
         * 4 running the deleteNode function
         * 5 running the insert function
         *
         * because alias and addr and interactively validated, I am not
         * sure I can get away from those checks. but steps 3 and 4 can
         * probably be combined
         */
        head = deleteNode(head, buf);
        node = create_node(buf, tmp_addr);
        head = insert(node, head);
    }
    return head;
}

/**
 * @brief prompt user for alias and searches list to find the matching
 * address
 *
 * @param head
 * @return int
 */
int look_up_address(struct address_t *head) {
    static char alias[20];
    char *tmp;
    fprintf(stdout, "Enter alias: ");
    fgets(alias, 20, stdin);
    (tmp = strrchr(alias, '\n')) ? *tmp = '\0' : my_flush(stdin);
    if (strlen(alias) > 10)
        return -1;

    struct address_t *ptr;
    ptr = search_in_list(alias, head, &compare_alias);
    if (ptr == NULL) {
        fprintf(stderr, "error %s does not exist\n", alias);
    } else {
        fprintf(stdout, "Address for %s: %s\n", alias,
                octet_to_string(ptr->octet));
    }
    return 0;
}

int validate_oct(int octet, int matched) {
    return ((matched == 1) && octet >= MIN_OCTET && octet <= MAX_OCTET);
}

void *print_alias(struct address_t *node, void *acc) {
    const char *s_alias = node ? node->alias : "NONE";
    printf("%s\n", s_alias);
    // if(DEBUG) printf("acc = %d\n",(int)acc);
    return acc + 1;
}

int display_alias_for_location(struct address_t *head) {
    // ensure last two octets are set
    // although shouldn't matter
    int half_addr[4] = {0, 0, 0, 0};
    char input[5];
    char *tmp;
    struct address_t *node = head;
    int matched;
    // TODO error msg when none found
    int n_matched = 0;
    for (int i = 0; i < 2; i++) {
        do {
            fprintf(stdout, "Enter Address %i: ", i + 1);
            memset(input, '\0', 5 * sizeof(char));
            fgets(input, 5, stdin);
            (tmp = strrchr(input, '\n')) ? *tmp = '\0' : my_flush(stdin);
            matched = sscanf(input, "%i", &half_addr[i]);
        } while (!validate_oct(half_addr[i], matched));
    }
    // we have to check node if it is a match
    //  and then recursively check if left and right are matches.
    printf("Location: %d:%d", half_addr[0], half_addr[1]);
    union Accumulator acc;
    acc.p =
        foldWhile(half_addr, node, compare_half, print_alias, (void *)0);
    n_matched = acc.i;
    if (DEBUG)
        printf("n_matched=%i\n", n_matched);
    if (!n_matched)
        fprintf(stderr, "error location does not exst within the list\n");
    return 0;
}
//
// struct address_t* next(struct address_t* node)
// {
// if(node!=NULL){
// inorder(node->leftChild,fp);
// return(head, "",fp);
// inorder(node->rightChild,fp);
// }
// }
//

struct address_t *delete_user_address(struct address_t *head) {
    // TODO save file formatting
    // TODO remove redundant returnInOrder function,
    // TODO testing data
    char *tmp;
    char tmp_buf[20];
    char input[5];
    struct address_t *ret;
    struct address_t *node;
    // get and validate alias
    fprintf(stdout, "Enter alias: ");
    fgets(tmp_buf, 20, stdin);
    (tmp = strrchr(tmp_buf, '\n')) ? *tmp = '\0' : my_flush(stdin);
    if (!validate_alias(tmp_buf, EXPECT_PRESENT, head)) {
        fprintf(stderr, "Invalid alias to delete\n");
        inorder(head, print_iden, stdout);
        return 0;
    }
    // prompt delete
    node = search_in_list(tmp_buf, head, compare_alias);
    print_iden(node, "delete: ", stdout);
    printf("(y/n):");
    fgets(input, 5, stdin);
    (tmp = strrchr(input, '\n')) ? *tmp = '\0' : my_flush(stdin);
    if (strlen(input) < 0) {
        perror("Invalid option\n");
        return head;
    }
    if (input[0] == 'y') {
        // delete node
        ret = deleteNode(head, tmp_buf);
        if (ret == NULL) {
            // TODO
            perror(
                "Failed to delete node: reason unknown OR EMPTY TREE\n");
            return head;
        } else
            return ret;
    } else if (input[0] == 'n') {
        return head;
    } else {
        perror("Invalid option\n");
        return head;
    }
}

int main(void) {
    struct address_t *head = (struct address_t *)NULL;
    head = read_file(head);
    struct address_t *ptr = NULL;
    // print_list(head, curr,stdout);
    char selection;
    while (1) {
        selection = print_menu();
        if (selection == '1') {
            if ((ptr = prompt_address_add(head)) != NULL) {
                head = insert(ptr, head);
            }
        } else if (selection == '2') {
            look_up_address(head);
        } else if (selection == '3') {
            head = update_address(head);
        } else if (selection == '4') {
            head = delete_user_address(head);
        } else if (selection == '5') {
            inorder(head, print_iden, stdout);
        } else if (selection == '6') {
            display_alias_for_location(head);
        } else if (selection == '7') {
            save_file(head);
        } else if (selection == '8') {
            return 0;
        } else {
            (void)0;
        }
    }

    // read file void -> char*  OR char** ?
    //    reduce(treeify[node,node ->  node],map(nodeify[char* -> node],
    //    peoples))
    return 0;
}