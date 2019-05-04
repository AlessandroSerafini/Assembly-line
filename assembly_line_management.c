/**********************************************************************************************************************
*     DESCRIPTION:
*     Write an ANSI C program that works as follows:
*        1) It gets a text file whose format entails a given number of rows, each one containing:
*           product identification code (4 characters),
*           name of the piece in assembly (text string),
*           piece identification code (4 characters),
*           time of entry in line (hours:minutes:seconds),
*           time of exit in line (hours:minutes:seconds). The fields are assumed to be separated by tabs or by space.
*           For instance:
*           H235 Sportello_dx N246 15:20:43 15:27:55
*           K542 Sportello_sx N247 10:03:10 10:15:30
*           A356 Maniglia_fronte G102 18:40:11 18:52:23
*        2) It loads data into a suitable tree data structure.
*        3) It allows the user to choose and perform the following operations:
*            (a) insertion of new input from keyboard;
*            (b) deletion of existing data, selected from keyboard;
*            (c) print on display of the sorted list of data, sorted by identification code of the finished product,
*                or by processing time in seconds (obtained from the times of entry and exit from the line),
*                chosen by the user.
*
*     AUTHOR: Alessandro Serafini <a.serafini21@campus.uniurb.it>
*
**********************************************************************************************************************/


/* Definition of constants */
#define INPUT_FILE "input.txt"

/* Binary tree types */
#define TYPE_PRODUCT_ID 0
#define TYPE_PROCESS_TIME 1

#define ID_LENGTH 4
#define _GNU_SOURCE
#define __USE_XOPEN


/* Including standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Structures declaration */

/* Article structure */
struct article
{
    char  *product_id;
    char  *name;
    char  *piece_id;
    char  *time_entry;
    char  *time_exit;
    float process_time;
};

/* List element structure */
typedef struct list_article
{
    struct article      *article;
    struct list_article *succ_p;
} list_article_t;


/* Node of binary tree structure */
struct node
{
    struct article *item;
    struct node    *left, *right;
};

/* Node of list structure */
struct list_node
{
    struct article   *item;
    struct list_node *next;
};

/* Declaration of functions */

/* Article functions */
struct article *new_article(char *product_id,
                            char *name,
                            char *piece_id,
                            char *time_entry,
                            char *time_exit,
                            float process_time);

void print_article(struct article *item);

/* Binary tree functions */
struct node *load_data(const char file[],
                       int type);

struct node *new_node(struct article *item);

struct node *insert(struct node *node,
                    struct article *item,
                    int type);

struct node *min_value_node(struct node *node);

struct node *remove_product(struct node *root,
                            struct article *item,
                            int type);

struct node *search_id(struct node *root,
                       char *product_id);

void print_tree(struct node *root);


/* List functions */
struct list_node *insert_in_list(struct list_node *head_ref,
                                 struct list_node *new_list_node,
                                 int type);

struct list_node *create_list_node(struct article *item);

void print_list(struct list_node *head);

struct list_node *load_data_list(const char file[],
                                 int type);

void remove_list_item(struct list_node *head,
                      struct list_node *node_to_remove);

struct list_node *search_in_list(struct list_node *head,
                                 char *product_id);


/* Time functions */
void get_valid_time(char *when,
                    char *bigger_then);

double get_prod_process_time(char *time_entry,
                             char *time_exit);

/* General functions */
void print_data(struct node *root);

void clear_buffer();

int get_valid_int(char *field_name);


/* Main function */
int main()
{
    printf("\n*************************\nAssembly line management\n*************************\n");
    
    /* Initialization of 2 binary trees, one for each type of data (product id and processing time) */
    struct node *root_product_id   = load_data(INPUT_FILE,
                                               TYPE_PRODUCT_ID);
    struct node *root_process_time = load_data(INPUT_FILE,
                                               TYPE_PROCESS_TIME);
    
    /* Initialization of 2 lists, one for each type of data (product id and processing time) */
    struct list_node *head_product_id   = load_data_list(INPUT_FILE,
                                                         TYPE_PRODUCT_ID);
    struct list_node *head_process_time = load_data_list(INPUT_FILE,
                                                         TYPE_PROCESS_TIME);
    
    
    /* Check for errors during the loading of data */
    if (root_product_id == NULL || root_process_time == NULL)
    {
        printf("Opening file error\n");
    }
    else
    {
        int choice;
        
        /* Menu options */
        do
        {
            printf("\n");
            printf("Choose an option:\n");
            printf("1) Display items\n");
            printf("2) Insert item\n");
            printf("3) Remove item\n");
            printf("0) Exit\n\n");
            printf("Choice: ");
            choice = get_valid_int("Choice"); /* Acquiring a valid integer using get_valid_int() function */
            printf("\n");
            
            switch (choice)
            {
                
                case 1:
                    /* Before displaying data, user must select a sort key,
                     * but if the binary tree is empty, this step can be skipped */
                    if (root_product_id == NULL)
                    {
                        printf("--------------------------------------------------------------\n");
                        printf("Data set is empty\n");
                        printf("--------------------------------------------------------------\n");
                    }
                    else
                    {
                        printf("Display items, ");
                        printf("please choose a sort key\n");
                        printf("0) Product identification code\n");
                        printf("1) Processing time\n");
                        
                        /* Acquiring sort key and validating it */
                        int sort_key;
                        printf("Sort key: ");
                        do
                        {
                            sort_key = get_valid_int("Sort key");
                            if (sort_key < 0 || sort_key > 1)
                            {
                                printf("Sort key %d) does not exists, try again: ",
                                       sort_key);
                            }
                        }
                        while (sort_key < 0 || sort_key > 1);
                        
                        /* Display data from the correct tree based on user's selected sort key */
                        printf("\nData sorted by field: ");
                        
                        /* Elaboration time for tree display */
                        clock_t start_print = clock();
                        switch (sort_key)
                        {
                            case TYPE_PRODUCT_ID:
                                printf("Product id");
                                print_data(root_product_id);
                                break;
                            
                            case TYPE_PROCESS_TIME:
                                printf("Processing time");
                                print_data(root_process_time);
                                break;
                            
                            default:
                                break;
                        }
                        
                        clock_t end_print        = clock();
                        double  time_spent_print = (double) (end_print - start_print) / CLOCKS_PER_SEC;
                        printf("\nTime taken for binary tree: %f milliseconds\n\n",
                               time_spent_print * 1000);
                        
                        
                        /* Display data from the correct list based on user's selected sort key */
                        printf("\nList sorted by field: ");
                        
                        /* Elaboration time for list display */
                        start_print = clock();
                        switch (sort_key)
                        {
                            case TYPE_PRODUCT_ID:
                                printf("Product id\n");
                                print_list(head_product_id);
                                break;
                            
                            case TYPE_PROCESS_TIME:
                                printf("Processing time\n");
                                print_list(head_process_time);
                                break;
                            
                            default:
                                break;
                        }
                        
                        end_print        = clock();
                        time_spent_print = (double) (end_print - start_print) / CLOCKS_PER_SEC;
                        printf("\n\nTime taken for list: %f milliseconds\n\n",
                               time_spent_print * 1000);
                        
                        
                    }
                    
                    break;
                
                case 2:
                    printf("Insert item");
                    printf("\n-------------------------------\n");
                    
                    char product_id[64], name[64], piece_id[64], time_entry[64], time_exit[64];
                    
                    /* Acquision of the new record to insert in binary tree and in list,
                     * product id is unique value so it can't be duplicated and it has a specific length,
                     * that's the reason of the following checks */
                    printf("Product id: ");
                    int id_exists = 1;
                    do
                    {
                        scanf("%s",
                              product_id);
                        if (search_id(root_product_id,
                                      product_id) != NULL)
                        {
                            clear_buffer();
                            printf("A record with product id: %s already exists, try again: ",
                                   product_id);
                        }
                        else if (strlen(product_id) > ID_LENGTH || strlen(product_id) < ID_LENGTH)
                        {
                            clear_buffer();
                            printf("Product id length must be of %d characters: ",
                                   ID_LENGTH);
                        }
                        else
                        {
                            id_exists = 0;
                        }
                    }
                    while (id_exists);
                    
                    
                    printf("Name (Use undescores instead spaces): ");
                    scanf("%s",
                          name);
                    
                    
                    /* Piece id has a specific length, that's the reason of the following check */
                    printf("Piece id: ");
                    int is_piece_id_valid = 0;
                    do
                    {
                        scanf("%s",
                              piece_id);
                        if (strlen(piece_id) > ID_LENGTH || strlen(piece_id) < ID_LENGTH)
                        {
                            clear_buffer();
                            printf("Piece id length must be of %d characters: ",
                                   ID_LENGTH);
                        }
                        else
                        {
                            is_piece_id_valid = 1;
                        }
                    }
                    while (!is_piece_id_valid);
                    
                    
                    /* Acquision of the time entry and time exit values. Both values ​​must have a specific format,
                     * for this reason I verify the formats through the function get_valid_time */
                    printf("Time entry (HH:MM:SS format): ");
                    get_valid_time(time_entry,
                                   time_entry);
                    
                    printf("Time exit (HH:MM:SS format): ");
                    get_valid_time(time_exit,
                                   time_entry);
                    
                    /* Creating new article and inserting it every binary tree */
                    struct article *item = new_article(product_id,
                                                       name,
                                                       piece_id,
                                                       time_entry,
                                                       time_exit,
                                                       get_prod_process_time(time_entry,
                                                                             time_exit));
                    
                    
                    if (item != NULL)
                    {
                        /* Elaboration time for tree insert */
                        clock_t start_insert = clock();
                        
                        root_product_id   = insert(root_product_id,
                                                   item,
                                                   TYPE_PRODUCT_ID);
                        root_process_time = insert(root_process_time,
                                                   item,
                                                   TYPE_PROCESS_TIME);
                        
                        clock_t end_insert        = clock();
                        double  time_spent_insert = (double) (end_insert - start_insert) / CLOCKS_PER_SEC;
                        
                        printf("\n\nTime taken for binary tree: %f milliseconds",
                               time_spent_insert * 1000);
                        
                        /* Inserting article every list */
                        
                        /* Start with the empty list */
                        struct list_node *new_list_node = NULL;
                        
                        /* Elaboration time for list insert */
                        start_insert = clock();
                        
                        new_list_node   = create_list_node(item);
                        head_product_id = insert_in_list(head_product_id,
                                                         new_list_node,
                                                         TYPE_PRODUCT_ID);
                        
                        new_list_node     = create_list_node(item);
                        head_process_time = insert_in_list(head_process_time,
                                                           new_list_node,
                                                           TYPE_PROCESS_TIME);
                        
                        printf("\n\nUpdated Linked List:\n");
                        print_list(head_product_id);
                        
                        end_insert        = clock();
                        time_spent_insert = (double) (end_insert - start_insert) / CLOCKS_PER_SEC;
                        
                        printf("\n\nTime taken for list: %f milliseconds\n\n",
                               time_spent_insert * 1000);
                        
                        
                        printf("\nRecord inserted successfully\n\n");
                        
                    }
                    else
                    {
                        choice = 0; /* Memory allocation error, exit the program setting the choice = 0 */
                    }
                    
                    break;
                
                case 3:
                    printf("Remove item");
                    print_data(root_product_id); /* Displaying data to ease the user to select the product id */
                    
                    /* Checking if the selected product id exists */
                    char id_to_remove[64];
                    printf("\nProduct id to remove: ");
                    int id_not_exists = 1;
                    while (id_not_exists)
                    {
                        scanf("%s",
                              id_to_remove);
                        if (search_id(root_product_id,
                                      id_to_remove) == NULL)
                        {
                            clear_buffer();
                            printf("Product id does not exist, try again: ");
                        }
                        else
                        {
                            id_not_exists = 0;
                        }
                    }
                    
                    /* Searching the node to remove by the selected product id and deleting that node from every binary tree */
                    
                    /* Elaboration time for tree remove */
                    clock_t start_remove = clock();
                    
                    struct node *node_to_remove = search_id(root_product_id,
                                                            id_to_remove);
                    root_product_id = remove_product(root_product_id,
                                                     node_to_remove->item,
                                                     TYPE_PRODUCT_ID);
                    
                    node_to_remove    = search_id(root_process_time,
                                                  id_to_remove);
                    root_process_time = remove_product(root_process_time,
                                                       node_to_remove->item,
                                                       TYPE_PROCESS_TIME);
                    
                    
                    clock_t end_remove        = clock();
                    double  time_spent_remove = (double) (end_remove - start_remove) / CLOCKS_PER_SEC;
                    printf("\n\nTime taken for binary tree: %f milliseconds",
                           time_spent_remove * 1000);
                    
                    
                    /* Searching the node to remove by the selected product id and deleting that node from every list */
                    
                    /* Elaboration time for list remove */
                    start_remove = clock();
                    
                    struct list_node *list_node_to_remove = search_in_list(head_product_id,
                                                                           id_to_remove);
                    remove_list_item(head_product_id,
                                     list_node_to_remove);
                    
                    list_node_to_remove = search_in_list(head_process_time,
                                                         id_to_remove);
                    remove_list_item(head_process_time,
                                     list_node_to_remove);
                    
                    
                    end_remove        = clock();
                    time_spent_remove = (double) (end_remove - start_remove) / CLOCKS_PER_SEC;
                    
                    printf("\nTime taken for list: %f milliseconds\n\n",
                           time_spent_remove * 1000);
                    
                    printf("\n\nRecord removed successfully\n");
                    
                    
                    break;
                
                default:
                    if (choice != 0)
                    {
                        printf("\nOption: %d) does not exist\n",
                               choice);
                    }
                    break;
                
            }
            
        }
        while (choice != 0);
    }
    
    /* Memory de-allocation */
    free(root_product_id);
    free(root_process_time);
    
    return 0;
}

/* Implementation of functions */

/* Article functions */

/* The function acquires the data (product_id, name, piece_id, time_entry, time_exit, process_time)
 * and creates a new article, returning it.*/
struct article *new_article(char *product_id,
                            char *name,
                            char *piece_id,
                            char *time_entry,
                            char *time_exit,
                            float process_time)
{
    /* Allocating memory for the new item */
    struct article *item = (struct article *) malloc(sizeof(struct article));
    
    /* Checking for memory allocation errors */
    if (item != NULL)
    {
        item->product_id = malloc(strlen(product_id) + 1);
        if (item->product_id == NULL)
        {
            item = NULL;
            printf("\n[ERROR] Memory allocation failed, try to re-run the program\n");
        }
        else
        {
            strcpy(item->product_id,
                   product_id);
            item->name = malloc(strlen(name) + 1);
            if (item->name == NULL)
            {
                item = NULL;
                printf("\n[ERROR] Memory allocation failed, try to re-run the program\n");
            }
            else
            {
                strcpy(item->name,
                       name);
                item->piece_id = malloc(strlen(piece_id) + 1);
                if (item->name == NULL)
                {
                    item = NULL;
                    printf("\n[ERROR] Memory allocation failed, try to re-run the program\n");
                }
                else
                {
                    strcpy(item->piece_id,
                           piece_id);
                    item->time_entry = malloc(strlen(time_entry) + 1);
                    if (item->name == NULL)
                    {
                        item = NULL;
                        printf("\n[ERROR] Memory allocation failed, try to re-run the program\n");
                    }
                    else
                    {
                        strcpy(item->time_entry,
                               time_entry);
                        item->time_exit = malloc(strlen(time_exit) + 1);
                        if (item->name == NULL)
                        {
                            item = NULL;
                            printf("\n[ERROR] Memory allocation failed, try to re-run the program\n");
                        }
                        else
                        {
                            strcpy(item->time_exit,
                                   time_exit);
                            item->process_time = process_time;
                        }
                    }
                }
            }
        }
    }
    else
    {
        printf("\n[ERROR] Memory allocation failed, try to re-run the program");
    }
    
    return item;
}

/* The function acquires the item and print its data in a formatted way */
void print_article(struct article *item)
{
    printf("%-15s%-20s%-15s%-20s%-20s\n",
           item->product_id,
           item->name,
           item->piece_id,
           item->time_entry,
           item->time_exit);
}

/* Binary tree functions */

/* The function acquires the input file where the data is stored and the binary tree type to insert that data */
struct node *load_data(const char file[],
                       int type)
{
    /* Initializing tree */
    struct node *root = NULL;
    
    /* Opening input file */
    FILE *f = fopen(file,
                    "r");
    
    /* Opening file error */
    if (f != NULL)
    {
        char product_id[64], name[64], piece_id[64], time_entry[64], time_exit[64];
        
        /* Loading data from file */
        while (fscanf(f,
                      "%s %s %s %s %s",
                      product_id,
                      name,
                      piece_id,
                      time_entry,
                      time_exit) != EOF)
        {
            /* Creating new article and inserting it in binary tree */
            struct article *item = new_article(product_id,
                                               name,
                                               piece_id,
                                               time_entry,
                                               time_exit,
                                               get_prod_process_time(time_entry,
                                                                     time_exit));
            if (item != NULL)
            {
                root = insert(root,
                              item,
                              type);
            }
            else
            {
                root = NULL;
            }
        }
        fclose(f);
    }
    
    return root;
}

/* The function acquires the item and allocates a new node with the given data.
It also initialize the node left and right pointers as NULL. Then, the node is returned. */
struct node *new_node(struct article *item)
{
    struct node *temp = (struct node *) malloc(sizeof(struct node)); /* Allocate memory for new node */
    
    if (temp != NULL)
    {
        temp->item = item; /* Storing the item in the node */
        temp->left = temp->right = NULL; /* Initialize left and right child as NULL */
    }
    else
    {
        printf("\n[ERROR] Memory allocation failed, try to re-run the program\n");
    }
    
    return temp;
}

/* The function acquires a node, an item and the type of data to insert.
   Then it insert the item into the correct node, and return that node. */
struct node *insert(struct node *node,
                    struct article *item,
                    int type)
{
    /* If the node is NULL, it is the node where to insert the item */
    if (node == NULL)
    {
        node = new_node(item);
    }
    else
    {
        /* If the tree is not empty, recur down it to find the correct node to insert the given item.
           For data comparison between strings, strcmp() is used to get the smaller value */
        
        /* The switch statement is used to compare the correct item value */
        switch (type)
        {
            /* The logic behind the insert function is quite simple:
            if the value to insert is smaller than the current node value then use recursion to insert the value in his left child.
            Otherwise insert it in his right child */
            
            /* Only product id is unique value, so < and > are enough for values comparyson, because duplicate values are not allowed. */
            case TYPE_PRODUCT_ID:
                if (strcmp(item->product_id,
                           node->item->product_id) < 0)
                {
                    node->left = insert(node->left,
                                        item,
                                        type);
                }
                else if (strcmp(item->product_id,
                                node->item->product_id) > 0)
                {
                    node->right = insert(node->right,
                                         item,
                                         type);
                }
                break;
                
                /* process time allow duplicate values, so <= and > are required for values comparyson. */
            case TYPE_PROCESS_TIME:
                if (item->process_time <= node->item->process_time)
                {
                    node->left = insert(node->left,
                                        item,
                                        type);
                }
                else if (item->process_time > node->item->process_time)
                {
                    node->right = insert(node->right,
                                         item,
                                         type);
                }
                break;
            
            default:
                break;
        }
    }
    
    return node;
}

/* The function acquires a node and return his smallest child */
struct node *min_value_node(struct node *node)
{
    struct node *temp = node;
    
    /* Loop down the node until the leftmost child is found*/
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    
    return temp;
}

/* The function acquires a node, an item and the type of data to remove.
   Then it remove the item from the correct tree, and return that tree. */
struct node *remove_product(struct node *root,
                            struct article *item,
                            int type)
{
    if (root != NULL)
    {
        /* The switch statement is used to compare the correct item value */
        switch (type)
        {
            case TYPE_PRODUCT_ID:
                /* If the key to be removed is smaller than the root's key,
                 * then it lies in left subtree */
                if (strcmp(item->product_id,
                           root->item->product_id) < 0)
                {
                    root->left = remove_product(root->left,
                                                item,
                                                type);
                }
                    /* If the key to be removed is greater than the root's key,
                     * then it lies in right subtree */
                else if (strcmp(item->product_id,
                                root->item->product_id) > 0)
                {
                    root->right = remove_product(root->right,
                                                 item,
                                                 type);
                }
                    /* If key is same as root's key, then this is the node to be removed */
                else
                {
                    /* Node with only one child or no child */
                    if (root->left == NULL)
                    {
                        struct node *temp = root->right;
                        free(root);
                        return temp;
                    }
                    else if (root->right == NULL)
                    {
                        struct node *temp = root->left;
                        free(root);
                        return temp;
                    }
                    
                    /* Node with two children: get the smallest child the right subtree */
                    struct node *temp = min_value_node(root->right);
                    
                    /* Copy the found node item to this node */
                    root->item = temp->item;
                    
                    /* Deleting the found node */
                    root->right = remove_product(root->right,
                                                 temp->item,
                                                 type);
                }
                break;
            case TYPE_PROCESS_TIME:
                if (item->process_time < root->item->process_time)
                {
                    root->left = remove_product(root->left,
                                                item,
                                                type);
                }
                else if (item->process_time > root->item->process_time)
                {
                    root->right = remove_product(root->right,
                                                 item,
                                                 type);
                }
                else
                {
                    /* Since process time allows duplicate values, an ID comparyson must be done to remove the right item */
                    if (strcmp(item->product_id,
                               root->item->product_id) == 0)
                    {
                        if (root->left == NULL)
                        {
                            struct node *temp = root->right;
                            free(root);
                            return temp;
                        }
                        else if (root->right == NULL)
                        {
                            struct node *temp = root->left;
                            free(root);
                            return temp;
                        }
                        
                        struct node *temp = min_value_node(root->right);
                        root->item  = temp->item;
                        root->right = remove_product(root->right,
                                                     temp->item,
                                                     type);
                    }
                    else
                    {
                        /* If ID is not the same, then remove the left child of that node (since insert on left child is conditioned by <= ) */
                        root->left = remove_product(root->left,
                                                    item,
                                                    type);
                    }
                }
                break;
        }
    }
    
    return root;
}

/* The function acquires the root node and the product_id of the searched element,
   then search a node that has the given id. It returns the node if the element exists, NULL otherwise */


struct node *search_id(struct node *root,
                       char *id)
{
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        if (strcmp(root->item->product_id,
                   id) == 0)
        {
            return root;
        }
        struct node *tmp;
        tmp = search_id(root->left,
                        id);
        if (tmp)
        {
            return tmp;
        }
        tmp = search_id(root->right,
                        id);
        if (tmp)
        {
            return tmp;
        }
    }
    /* search didn't find anything */
    return NULL;
}

/* The function acquires the root and print its data in order */
void print_tree(struct node *root)
{
    if (root != NULL)
    {
        /* Use recursion to elaborate the left child */
        print_tree(root->left);
        /* Elaborate node item */
        print_article(root->item);
        /* Use recursion to elaborate the right child */
        print_tree(root->right);
    }
}



/* List functions */

/* function to insert a new node in a list. */

struct list_node *insert_in_list(struct list_node *head_ref,
                                 struct list_node *new_list_node,
                                 int type)
{
    struct list_node *current;
    /* The switch statement is used to compare the correct item value */
    switch (type)
    {
        case TYPE_PRODUCT_ID:
            /* Special case for the head end */
            if (head_ref == NULL || strcmp((head_ref)->item->product_id,
                                           new_list_node->item->product_id) > 0)
            {
                new_list_node->next = head_ref;
                head_ref = new_list_node;
            }
            else
            {
                /* Locate the node before the point of insertion */
                current = head_ref;
                while (current->next != NULL &&
                       strcmp(current->next->item->product_id,
                              new_list_node->item->product_id) < 0)
                {
                    current = current->next;
                }
                new_list_node->next = current->next;
                current->next       = new_list_node;
            }
            
            break;
        
        case TYPE_PROCESS_TIME:
            /* Special case for the head end */
            if (head_ref == NULL || (head_ref)->item->process_time >= new_list_node->item->process_time)
            {
                new_list_node->next = head_ref;
                head_ref = new_list_node;
            }
            else
            {
                /* Locate the node before the point of insertion */
                current = head_ref;
                while (current->next != NULL &&
                       current->next->item->process_time < new_list_node->item->process_time)
                {
                    current = current->next;
                }
                new_list_node->next = current->next;
                current->next       = new_list_node;
            }
            break;
        
        default:
            break;
    }
    return (head_ref);
}

/* A utility function to create a new list node */
struct list_node *create_list_node(struct article *item)
{
    /* allocate list node */
    struct list_node *new_list_node = (struct list_node *) malloc(sizeof(struct list_node));
    
    /* put in the data */
    new_list_node->item = item;
    new_list_node->next = NULL;
    
    return new_list_node;
}

/* Function to print a list */
void print_list(struct list_node *head)
{
    struct list_node *temp = head;
    while (temp != NULL)
    {
        printf("%s, ",
               temp->item->product_id);
        temp = temp->next;
    }
}

/* The function acquires the input file where the data is stored and the list type to insert that data */
struct list_node *load_data_list(const char file[],
                                 int type)
{
    /* Initializing list */
    struct list_node *head     = NULL;
    struct list_node *new_node = NULL;
    
    
    /* Opening input file */
    FILE *f = fopen(file,
                    "r");
    
    /* Opening file error */
    if (f != NULL)
    {
        char product_id[64], name[64], piece_id[64], time_entry[64], time_exit[64];
        
        /* Loading data from file */
        while (fscanf(f,
                      "%s %s %s %s %s",
                      product_id,
                      name,
                      piece_id,
                      time_entry,
                      time_exit) != EOF)
        {
            
            /* Creating new article and inserting it in list */
            struct article *item = new_article(product_id,
                                               name,
                                               piece_id,
                                               time_entry,
                                               time_exit,
                                               get_prod_process_time(time_entry,
                                                                     time_exit));
            if (item != NULL)
            {
                new_node = create_list_node(item);
                head     = insert_in_list(head,
                                          new_node,
                                          type
                );
            }
            else
            {
                new_node = NULL;
            }
        }
        fclose(f);
    }
    
    
    return head;
}

void remove_list_item(struct list_node *head,
                      struct list_node *node_to_remove)
{
    if (head->next == NULL)
    {
        printf("There is only one node. The list can't be made empty ");
    }
    else
    {
        /* When node to be deleted is head node */
        if (head == node_to_remove)
        {
            /* Copy the item of next node to head */
            head->item = head->next->item;
            
            /* store address of next node */
            node_to_remove = head->next;
            
            /* Remove the link of next node */
            head->next = head->next->next;
            
            /* free memory */
            free(node_to_remove);
            
            return;
        }
        
        /* When not first node, follow the normal deletion process */
        
        /* find the previous node */
        struct list_node *prev = head;
        while (prev->next != NULL && prev->next != node_to_remove)
            prev = prev->next;
        
        /* Check if node really exists in Linked List */
        if (prev->next == NULL)
        {
            printf("\n Given node is not present in Linked List");
            return;
        }
        
        /* Remove node from Linked List */
        prev->next = prev->next->next;
        
        /* Free memory */
        free(node_to_remove);
    }
    return;
}

/* Checks whether the value is present in list */
struct list_node *search_in_list(struct list_node *head,
                                 char *product_id)
{
    /* Initialize current */
    struct list_node *current = head;
    struct list_node *result  = NULL;
    while (current != NULL)
    {
        if (strcmp(current->item->product_id,
                   product_id) == 0)
        {
            result = current;
        }
        current = current->next;
    }
    return result;
}


/* Time functions */

/* This function acquires a string and validates it in the format %H:%M:%S.
 * In case the format has not been respected, it requires the string reinsertion.
 * Checks, if necessary, that the time is after a certain time, this is because
 * the exit time can not be earlier than entry time. */
void get_valid_time(char *when,
                    char *bigger_then)
{
    struct tm tm_when;
    int       is_valid = 0;
    char      *end;
    do
    {
        scanf("%s",
              when);
        end = strptime(when,
                       "%H:%M:%S",
                       &tm_when);
        if (end == NULL || *end != '\0')
        {
            clear_buffer();
            printf("Time must be expressed as hh:mm:ss format. Please re-insert it: \n");
        }
        else
        {
            /* Check that the time is after a certain time, this is because
            * the exit time can not be earlier than entry time. */
            if (strlen(bigger_then) > 0)
            {
                if (get_prod_process_time(bigger_then,
                                          when) >= 0)
                {
                    is_valid = 1;
                }
                else
                {
                    clear_buffer();
                    printf("Time bust be bigger then %s. Please re-insert it: \n",
                           bigger_then);
                }
            }
            else
            {
                is_valid = 1;
            }
        }
    }
    while (!is_valid);
}

/* This function calculates and returns the processing time in seconds,
 * obtained from the entry time and the exit time, both passed as arguments. */
double get_prod_process_time(char *time_entry,
                             char *time_exit)
{
    double    diff_t;
    struct tm tm_entry, tm_exit, *tm_today;
    time_t    t_entry, t_exit, rawtime;
    
    strptime(time_entry,
             "%H:%M:%S",
             &tm_entry);
    strptime(time_exit,
             "%H:%M:%S",
             &tm_exit);
    
    time(&rawtime);
    tm_today = localtime(&rawtime);
    
    /* Set on entry time and exit time, day, month and current year, in order to make a correct evaluation. */
    tm_entry.tm_mday = tm_exit.tm_mday = tm_today->tm_mday;
    tm_entry.tm_mon  = tm_exit.tm_mon  = tm_today->tm_mon;
    tm_entry.tm_year = tm_exit.tm_year = tm_today->tm_year;
    
    /* Convert the entry time and the exit time from the struct tm format to the suitable type
     * for storing the calendar format (time_t) in order to calculate the process time. */
    t_entry = mktime(&tm_entry);
    t_exit  = mktime(&tm_exit);
    
    /* Calculate de time difference in seconds between time exit and time entry. */
    diff_t = difftime(t_exit,
                      t_entry);
    
    return (diff_t);
}


/* General functions */

/* The function acquires the root and print its data in a formatted way */
void print_data(struct node *root)
{
    printf("\n-------------------------------------------------------------------------------\n");
    printf("%-15s%-20s%-15s%-20s%-20s\n",
           "Product id",
           "Name",
           "Piece id",
           "Time entry",
           "Time exit");
    printf("-------------------------------------------------------------------------------\n");
    print_tree(root);
    printf("-------------------------------------------------------------------------------\n");
}

/* Clear buffer function to avoid scanf errors */
void clear_buffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(char *field_name)
{
    int  is_valid = 0;
    int  value;
    char term;
    while (!is_valid)
    {
        if (scanf("%d%c",
                  &value,
                  &term) != 2 || term != '\n')
        {
            clear_buffer();
            printf("%s must be of type integer, try again: ",
                   field_name);
        }
        else
        {
            is_valid = 1;
        }
    }
    
    return value;
}
