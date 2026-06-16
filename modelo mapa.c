/*bibliotecas utilizadas*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Declare as constantes */
#define TAMANHO_ACERVO  20

/*Estrutura de dados */
/*Define a estrtura de um livro com seus atributos*/
struct livro
{
    int codigo;
    char titulo[50];
    char autor[30];
    char area[30];
    char editora[30];
    int ano;
};

/*função para limpar o buffer de entrada*/
void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
/*função para ler strings com segurança*/
void read_str(char *buf, int size)
{
    if (fgets(buf, size, stdin) != NULL) {
        size_t l = strlen(buf);
        if (l > 0 && buf[l-1] == '\n') buf[l-1] = '\0';
        else clear_input();
    } else {
        buf[0] = '\0';
        clear_input();
    }
}

/* =========================
   Funções de persistência
   ========================= */

/* salva todo o array de livros em arquivo binário */
void save_acervo(const char *nome, struct livro *ficha, int n)
{
    FILE *f = fopen(nome, "wb");
    if (!f) {
        perror("Erro ao abrir arquivo para salvar");
        return;
    }
    if (fwrite(ficha, sizeof(struct livro), n, f) != (size_t)n) {
        /* se ocorrer erro, avisamos mas não abortamos */
        perror("Erro ao gravar acervo");
    }
    fclose(f);
}

/* carrega o acervo do arquivo binário; se não existir, inicializa vazio */
void load_acervo(const char *nome, struct livro *ficha, int n)
{
    FILE *f = fopen(nome, "rb");
    if (!f) {
        /* arquivo não existe: inicializa com posições vazias */
        for (int i = 0; i < n; i++) ficha[i].codigo = 0;
        return;
    }
    size_t lidos = fread(ficha, sizeof(struct livro), n, f);
    /* se leu menos, marca o restante como vazio */
    for (size_t k = lidos; k < (size_t)n; k++) ficha[k].codigo = 0;
    fclose(f);
}

/*função principal corpo do codigo*/

int main(void)

{   /*declaracao de variaveis */
    struct livro ficha[TAMANHO_ACERVO];
    struct livro troca;
    int busca, i, j, acha, op, pos;

    /* carrega acervo salvo (se existir) */
    load_acervo("acervo.bin", ficha, TAMANHO_ACERVO);

    op = 0;

    /*Menu de opções*/
    while (op != 5)
    {   
        printf("\n-----Menu de opcoes-----:\n");
        printf("1. Cadastrar livro\n");
        printf("2. Listar livros\n");
        printf("3. Pesquisar livro\n");
        printf("4. Ordenar livros\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");

        /* lê a opção do usuário */
        if (scanf("%d", &op) != 1) { clear_input(); op = 0; }
        clear_input();/*limpa o buffer de entrada*/
        /* executa a ação correspondente à opção escolhida */

        switch (op)
        {
           /*cadastrar livro*/
            case 1:
                system("cls");/*limpa a tela*/
                 /*procura a primeira posicao  no array, e loop*/
                pos = -1;
                for (i = 0; i < TAMANHO_ACERVO; i++) {
                    if (ficha[i].codigo == 0) { pos = i; break; }
                }
                if (pos == -1) {
                    printf("------Acervo cheio. Remova um livro antes de cadastrar outro-----.\n");
                    break;
                }

                 /*Caso queira cancelar o cadastro digite 0*/

                printf("\n----Cadastrando livro na posicao----- %d\n", pos + 1);
                printf("Digite o codigo do Livro (0 para cancelar): ");
                if (scanf("%d", &ficha[pos].codigo) != 1) ficha[pos].codigo = 0;
                clear_input();

                if (ficha[pos].codigo == 0) {
                    printf("Cadastro cancelado. Voltando ao menu.\n");
                    break;
                }
                
                /*leitura dos dados do livro com o read_str*/
                printf("Digite o titulo do Livro: ");
                read_str(ficha[pos].titulo, sizeof ficha[pos].titulo);

                printf("Digite o autor do Livro: ");
                read_str(ficha[pos].autor, sizeof ficha[pos].autor);

                printf("Digite a area do Livro: ");
                read_str(ficha[pos].area, sizeof ficha[pos].area);

                printf("Digite o ano: ");
                if (scanf("%d", &ficha[pos].ano) != 1) ficha[pos].ano = 0;
                clear_input();

                printf("Digite a editora do Livro: ");
                read_str(ficha[pos].editora, sizeof ficha[pos].editora);

                /* Exibe confirmação do livro cadastrado com sucesso */

                printf("\n-----Livro cadastrado com sucesso-----:\n");
                printf("Posicao: %d\n", pos + 1);
                printf("Codigo: %d\n", ficha[pos].codigo);
                printf("Titulo: %s\n", ficha[pos].titulo);
                printf("Autor: %s\n", ficha[pos].autor);
                printf("Area: %s\n", ficha[pos].area);
                printf("Ano: %d\n", ficha[pos].ano);
                printf("Editora: %s\n", ficha[pos].editora);

                /* salva o acervo no arquivo após cadastro */
                save_acervo("acervo.bin", ficha, TAMANHO_ACERVO);
                break;

                /*listar todos os livros cadastrados*/
            case 2:
                system("cls");
                for (i = 0; i < TAMANHO_ACERVO; i++)
                /* exibe todos os livros cadastrados */
                {
                    if (ficha[i].codigo == 0) continue; 
                    printf("\nCodigo: %d\n", ficha[i].codigo);
                    printf("Titulo: %s\n", ficha[i].titulo);
                    printf("Autor: %s\n", ficha[i].autor);
                    printf("Area: %s\n", ficha[i].area);
                    printf("Ano: %d\n", ficha[i].ano);
                    printf("Editora: %s\n", ficha[i].editora);
                }
                break;  

                    /* pesquisa livro por codigo */
            case 3:
                system("cls");
                printf("Digite o codigo do livro que deseja buscar: ");
                if (scanf("%d", &busca) != 1) busca = -1;
                clear_input();
                acha = 0;

                /* Percorre o array procurando pelo código */
                for (i = 0; i < TAMANHO_ACERVO; i++)
                {
                    if (ficha[i].codigo == busca)
                    {
                        /* livro encontrado */
                        acha = 1;
                        printf("\n-----Livro encontrado----:\n");
                        printf("Codigo: %d\n", ficha[i].codigo);
                        printf("Titulo: %s\n", ficha[i].titulo);
                        printf("Autor: %s\n", ficha[i].autor);
                        printf("Area: %s\n", ficha[i].area);
                        printf("Ano: %d\n", ficha[i].ano);
                        printf("Editora: %s\n", ficha[i].editora);
                        break;
                    }
                }
                /* verifica se o livro foi encontrado */

                if (!acha) printf("Livro nao encontrado!\n");
                break;
                /*opcao 4  ordena os livros por codigo em ordem crescente */
            case 4:
                system("cls");/*limpa a tela*/

                /* Implementa Bubble Sort para ordenar por código */
                for (i = 0; i < TAMANHO_ACERVO - 1; i++)
                {
                    for (j = i + 1; j < TAMANHO_ACERVO; j++)
                    {
                        if (ficha[i].codigo > ficha[j].codigo)
                        {
                            troca = ficha[i];
                            ficha[i] = ficha[j];
                            ficha[j] = troca;
                        }
                    }
                }

                  /* Exibe os livros após ordenação em ordem crescente */
                printf("Livros ordenados por codigo:\n");
                for (i = 0; i < TAMANHO_ACERVO; i++)
                {
                    if (ficha[i].codigo == 0) continue;
                    printf("Codigo: %d  Titulo: %s  Ano: %d\n", ficha[i].codigo, ficha[i].titulo, ficha[i].ano);
                }
                break;
                /* sair do programa */
            case 5:
                /* salva antes de sair */
                save_acervo("acervo.bin", ficha, TAMANHO_ACERVO);
                printf("Saindo...\n");
                break;
                /* opção inválida */
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
 
    return 0;
}