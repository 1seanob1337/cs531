// insert_record -> create new node and push to stack

// get_alias -> address of node where alias is or -1 if not exists

// find_2address-> return node** based on  two addres bytes
// find_4address-> return node* based on 4 address bytes
// delete node -> possibly do a memcopy from next to cur???

int delete_from_list(int val)
{
    struct test_struct *prev = NULL;
    struct test_struct *del = NULL;
    printf("\n Deleting value [%d] from list\n", val);
    del = search_in_list(val, &prev);
    if (del == NULL)
    {
        return -1;
    }
    else
    {
        if (prev != NULL)
            prev->next = del->next;
        if (del == curr)
        {
            curr = prev;
        }
        else if (del == head)
        {
            head = del->next;
        }
    }
    free(del);
    del = NULL;
    return 0;
}
// update record

int main()
{
    // read in file
    // insert entries into structure

    // do while input not 8
    // print menu
    // do selection
}