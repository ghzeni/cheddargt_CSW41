#include "snake_list.h"
#include <time.h>  

/* */
Node* snake_create (void) 
{
   return NULL;
}

/* Adiciona uma nova posição na cobra, sempre adiciona e retorna a cabeça*/
Node* snake_add (Node* head, int x, int y, int direction)
{
   Node* new_node =(Node*)malloc(sizeof(Node));
   new_node->x = x;
   new_node->y = y;
   new_node->direction = direction;
   new_node->next = head;
   return new_node;
}


void snake_print (Node* head) 
{
    Node* v; 
    for (v = head; v != NULL; v = v->next) 
        printf("VX: %d, Y: %d\n", v->x, v->y);
} 


void snake_free (Node* head) 
{
    while (head != NULL) {
        Node* aux = head->next; /*guarda ref. p/ prox.*/
        free (head); /*libera a memória.*/
        head = aux; /*faz node apontar p/ o prox. elem.*/
    }
}

int snake_size (Node* head)
{
    int n = 0;
    Node* p;
    for (p = head; p!=NULL; p = p->next)
        n++;

    return n;
}

Node* snake_search (Node* head , int x, int y)
{
    Node* i;
    for (i = head; i != NULL; i = i->next)
    {
        if (i->x == x)
        {
            if (i->y == y)
                return i;
        }
    }
    return NULL;
}

void snake_update (Node* head, int direction)
{
    Node* i;
    int last_dir = head->direction, aux; // Salva a direção atual da cabeça
    head->direction = direction;    // Atualiza a cabeça com a nova direção
    
    for (i = head; i != NULL; i = i->next)
    {
        if (i != head)
        {
            aux = i->direction;
            i->direction = last_dir;
            last_dir = aux;
        }
        switch (i->direction)
        {
            case 1:
                (i->x)++;
                break;
            case 2:
                (i->x)--;
                break;
            case 3:
                (i->y)--;
                break;
            case 4:
                (i->y)++;
                break;
        }
    }
}

Node* new_food(Node* head)
{
    Node *food =(Node*)malloc(sizeof(Node));
    int good_food = 0;
    
    while (!good_food)
    {
        srand(time(NULL));
        (food->x) = rand() % 14 + 1;
        (food->y) = rand() % 14 + 1;
        
        struct Node *colision = snake_search(head , food->x, food->y);
        if (colision == NULL)
            good_food = 1;
    }
    food->direction = 0;
    food->next = NULL;
    return food;
}

int snake_ate(Node* head, Node* food, int direction)
{
    int x = head->x;
    int y = head->y;
    switch (direction)
    {
        case 1:
            x++;
            break;
        case 2:
            x--;
            break;
        case 3:
            y--;
            break;
        case 4:
            y++;
            break;
    }
    
    if  ((x == (food->x)) && (y == (food->y))) // Comeu a comida
        return 1;
    else 
        return 0;
}

int snake_collision(Node *head)
{
    Node *collision = snake_search(head->next, head->x, head->y);
    
    if (collision != NULL)  // Colisão com o corpo
        return 1;
    else
        return 0;   // Sem Colisão
}
