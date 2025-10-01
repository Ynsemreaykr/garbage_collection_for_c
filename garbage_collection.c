#include <stdlib.h>

typedef struct s_mem_block
{
    void *ptr;
    size_t size;
    struct s_mem_block *next;
}   t_mem_block;

static t_mem_block **get_mem_list(void)
{
    static t_mem_block *mem_list = NULL;
    return (&mem_list);
}

void *ft_malloc(size_t size)
{
    void        *ptr;
    t_mem_block *block;

    ptr = malloc(size);
    if (!ptr)
        return (NULL);
    block = malloc(sizeof(t_mem_block));
    if (!block)
    {
        free(ptr);
        return (NULL);
    }
    block->ptr = ptr;
    block->size = size;
    block->next = *get_mem_list();
    *get_mem_list() = block;
    return (ptr);
}

static void free_mem_block(t_mem_block **mem_list,
    t_mem_block *current, t_mem_block *prev)
{
    if (prev)
        prev->next = current->next;
    else
        *mem_list = current->next;
    free(current);
}

void ft_free(void *ptr)
{
    t_mem_block **mem_list;
    t_mem_block *current;
    t_mem_block *prev;

    if (!ptr)
        return;
    mem_list = get_mem_list();
    current = *mem_list;
    prev = NULL;
    while (current)
    {
        if (current->ptr == ptr)
        {
            free_mem_block(mem_list, current, prev);
            free(ptr);
            return;
        }
        prev = current;
        current = current->next;
    }
    free(ptr); // listede bulunmasa da serbest bırak
}

void ft_mem_cleanup(void)
{
    t_mem_block **mem_list;
    t_mem_block *current;
    t_mem_block *next;

    mem_list = get_mem_list();
    if (!mem_list)
        return;
    current = *mem_list;
    while (current)
    {
        next = current->next;
        if (current->ptr)
            free(current->ptr);
        free(current);
        current = next;
    }
    *mem_list = NULL;
}
