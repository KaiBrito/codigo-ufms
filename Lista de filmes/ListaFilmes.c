#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LstMovie{
    char Name[30];
    int Duration;
    char Category[30];
    char Synopsis[80];
    struct LstMovie *Back;
    struct LstMovie *Next;
}LstMovie;

/* Prótotipos de funções */
void AddMovie(LstMovie **lst);
void InsertSorted(LstMovie **lst, LstMovie *NewMovie);
void RemoveMovie(LstMovie **lst);
void RemoveSearch(LstMovie **lst, char *Name);
void ToBrowse(LstMovie *lst);
void Search(LstMovie *lst);
void Change(LstMovie *Current);

int main(void)
{
    LstMovie *lst = NULL;
    int option;

    do
    {
        /* Traços apenas para melhorar a visulização do menu no terminal, aplicado em todos os menus do programa */
        printf("-------------------------------");
        printf("\n1- Adicionar filme\n2- Remover filme\n");
        printf("3- Navegar entre os filmes\n4- Pesquisar filme\n5- Sair do programa\n");
        printf("-------------------------------\n");
        printf("=> ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            AddMovie(&lst);
            break;
        case 2:
            RemoveMovie(&lst);
            break;
        case 3:
            ToBrowse(lst);
            break;
        case 4:
            Search(lst);
            break;
        }

    } while (option != 5);
    
    return 0;
}

/* Adiciona um filme na lista */
void AddMovie(LstMovie **lst)
{
    LstMovie *NewMovie;

    /* Alocação de memória */
    NewMovie = (LstMovie *) malloc(sizeof(LstMovie));
    /* Verificação para ver se a memória foi alocada corretamente */
    if (!NewMovie)
    {
        fprintf(stderr, "Opps, falha ao alocar memoria\n");
        return;
    }

    printf("Qual o nome do filme: ");
    scanf(" %[^\n]", NewMovie->Name);
    printf("Qual a duracao do filme: ");
    scanf("%d", &NewMovie->Duration);
    printf("Qual a categoria do filme: ");
    scanf(" %[^\n]", NewMovie->Category);
    printf("Qual a sinopse do filme: ");
    scanf(" %[^\n]", NewMovie->Synopsis);

    InsertSorted(&*lst, NewMovie);
}

/* Percorre a lista e insere o novo filme ordenado */
void InsertSorted(LstMovie **lst, LstMovie *NewMovie)
{
    LstMovie *Current, *Previous = NULL;

    Current = *lst;

    if (*lst == NULL)
    {
        *lst = NewMovie;
        NewMovie->Back = NewMovie;
        NewMovie->Next = NewMovie;
    }
    else
    {
        Previous = *lst;

        /* Incluindo o filme em seu lugar na lista */
        while (Current->Next != *lst)
        {
            Previous = Current;
            Current = Current->Next;
        }

        Previous = *lst;
        NewMovie->Back = Current;
        NewMovie->Next = Current->Next;
        Current->Next = NewMovie;
        Previous->Back = NewMovie;    
    } 
}

/* Remove um filme da lista*/
void RemoveMovie(LstMovie **lst)
{
    char Name[30];

    printf("Informe o filme que deseja excluir da sua lista: ");
    scanf(" %[^\n]", Name);

    RemoveSearch(&*lst, Name);
}

/* Busca e excluí o filme */
void RemoveSearch(LstMovie **lst, char *Name)
{
    LstMovie *Current;
    
    Current = *lst;

    /* Percorre a lista em busca do filme desejado para excluir */
    while (Current->Next != *lst && strcmp(Current->Name, Name) != 0)   
        Current = Current->Next;

    if (Current == *lst)
    {
        if (Current->Next == Current)   
            *lst = NULL; 
        else
        {
            *lst = Current->Next;
            Change(Current);
        }
    }
    else if (Current->Next == *lst)
    {
        Current->Back->Next = Current->Next;
        Current->Back->Back = Current->Back;
    }
    else if (Current->Next != *lst)
        Change(Current);
    
    free(Current);      
}

/* Navega entre os filmes existentes na lista */
void ToBrowse(LstMovie *lst)
{
    LstMovie *Pointer = lst;
    char option;

    do
    {
        printf("-------------------------------"); 
        printf("\n   \tW\n\tA  S  D\n");
        printf("A- Filme anterior\nD- Proximo filme\nS- Sair\n");
        printf("-------------------------------\n");
        printf("=> ");

        scanf(" %c", &option);

        if (Pointer != NULL)
        {
            if (option == 'A')
            {
                Pointer = Pointer->Back;
                printf("\nNome: %s - Duracao: %d\n\n", Pointer->Name, Pointer->Duration);
            }

            if (option == 'D')
            {
                Pointer = Pointer->Next;
                printf("\nNome: %s - Duracao: %d\n\n", Pointer->Name, Pointer->Duration);
            }      
        }
        
    } while (option != 'S');
}

/* Pesquisa o filme */
void Search(LstMovie *lst)
{
    LstMovie *Pointer = lst;
    char Name[30];

    printf("Informe o filme que deseja pesquisar: ");
    scanf(" %[^\n]", Name);

    /* Procura o filme */
    while (Pointer->Next != lst && strcmp(Pointer->Name, Name) != 0)   
        Pointer = Pointer->Next;

    if (Pointer != NULL && strcmp(Pointer->Name, Name) == 0)
    {
        printf("O filme %s, possui uma duracao de %d minutos\n", Pointer->Name, Pointer->Duration);
        printf("Categoria: %s\nSinopse: %s\n", Pointer->Category, Pointer->Synopsis);
    }
    else
        fprintf(stderr, "O filme mencionado nao existe!\n");   
}

/* Troca na lista */
void Change(LstMovie *Current)
{
    Current->Next->Back = Current->Back;
    Current->Back->Next = Current->Next;
}