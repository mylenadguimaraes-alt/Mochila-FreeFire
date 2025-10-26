#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdbool.h> 
// Código da Ilha – Edição Free Fire 
// Nível: Mestre // Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha. 
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

//Quantidade maxima de itens na mochila 
#define Tam_Mochila 10

void LimparBuffer () { 
    int c;
    while((c=getchar()) != '\n' && c != EOF); 
}


// Enum CriterioOrdenacao: 
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    ORDENACAO_NENHUMA = 0,
    ORDENACAO_NOME = 1,
    ORDENACAO_TIPO = 2,
    ORDENACAO_PRIORIDADE = 3
} CriterioOrdenacao;


// Struct Item: 
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5). 
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct { 
    char nome [30]; 
    char tipo [20]; 
    int quantidade;
    int prioridade; 
}Item;


// Vetor mochila: 
// Armazena até 10 itens coletados. 
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
typedef struct { 
    Item itens[Tam_Mochila]; 
    int numItens; 
    int comparacoes; 
    int ordenadaPorNome; 
} Mochila;

char ordem [100];


void exibirMenu( Mochila *mochila,int * opcao, int tam_armazenamento); 
void inseriritem(Mochila *mochila, int Espaco); 
void removeritem(Mochila * mochila, int Espaco); 
void listaritens(Mochila * mochila, int Espaco);
void menuDeOrdenacao(Mochila *mochila, CriterioOrdenacao *ordenacao);
void insertionSort(Mochila *mochila, CriterioOrdenacao ordenacao);
void buscaBinariaPorNome(Mochila * mochila, int Espaco);

int main() { 
    int opcao = 0;
    Mochila mochila; 
    mochila.numItens = 0; // inicia vazia 
    mochila.comparacoes = 0;
	mochila.ordenadaPorNome = 0; 
    strcpy(ordem, "NAO ORDENADO");
    int ordempor = 0;
    int status = 0;
    
     CriterioOrdenacao criterio = ORDENACAO_NENHUMA;
    // Menu principal com opções: 
    // 1. Adicionar um item 
    // 2. Remover um item 
    // 3. Listar todos os itens 
    // 4. Ordenar os itens por critério (nome, tipo, prioridade) 
    // 5. Realizar busca binária por nome 
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    do 
    { 
        exibirMenu(&mochila,&opcao, Tam_Mochila);
        switch (opcao) 
        { 
            case 1: 
            {
            	
            	inseriritem(&mochila, Tam_Mochila);
    			// Se j� houver um crit�rio de ordena��o ativo, reordena automaticamente
    			if (criterio != ORDENACAO_NENHUMA)
    			{
        			insertionSort(&mochila, criterio);
       	 			strcpy(ordem, "ORDENADO");
    			}
    			else
    			{
        			strcpy(ordem, "NAO ORDENADO");
        			mochila.ordenadaPorNome = 0;
    			}

    			listaritens(&mochila, Tam_Mochila);
    		
            break;
			}
            case 2: 
                removeritem(&mochila, Tam_Mochila); 
            break;

            case 3: 
                listaritens(&mochila, Tam_Mochila); 
            break;
            case 4:
            {
			
            	//CriterioOrdenacao criterio = ORDENACAO_NENHUMA;

    			menuDeOrdenacao(&mochila, &criterio);

			    if (criterio == ORDENACAO_NOME)
    			{
        			insertionSort(&mochila, criterio);
        			listaritens(&mochila, Tam_Mochila);

        
        			strcpy(ordem, "ORDENADO");
    			}
    			else if (criterio == ORDENACAO_TIPO || criterio == ORDENACAO_PRIORIDADE)
    			{
        			insertionSort(&mochila, criterio);
        			listaritens(&mochila, Tam_Mochila);
        			strcpy(ordem, "NAO ORDENADO");
    			}
            break;
        	}

            case 0: 
                printf("\n\nVoce esta saindo do programa\n");
                printf("Digite ENTER para continuar..."); 
                getchar();
            break;

            default: 
                printf("\n\nValor invalido! Voce sera mandado de volta ao menu principal\n");
                printf("Digite ENTER para continuar..."); 
                getchar();
            break;
        }
    } while(opcao != 0);

    return 0;
}


// limparTela(): 
// Simula a limpeza da tela imprimindo várias linhas em branco.
    
// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu(Mochila * mochila, int * opcao, int tam_armazenamento) 
{
    printf("\n==================================================\n"); 
    printf(" PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("==================================================\n");
    printf("Itens na Mochila: %d/%d \n", mochila->numItens, tam_armazenamento);
	printf("Status da Ordenacao por Nome: %s \n\n", ordem);

    printf("1. Adicionar Componente\n"); 
    printf("2. Descartar Componente\n"); 
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n"); 
    printf("5. Buscar Binaria por Componente-Chave(por nome)\n");
    printf("0. Ativar TORRE DE FUGA (Sair)\n"); 
    printf("---------------------------------------------------\n"); 
    printf("Escolha uma opcao: "); 
    
    scanf("%d", opcao); 
    LimparBuffer();
}

// inserirItem(): 
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade. 
// Após inserir, marca a mochila como "não ordenada por nome".
void inseriritem(Mochila *mochila, int Espaco) 
{
    if(mochila->numItens >= Espaco) 
    {
        printf("\n\nMochila cheia!\n"); 
        printf("Se quiser inserir um componente, remova um deles!\n"); 
        getchar(); 
        return;
    }
    
    int idx = mochila->numItens; 
    printf("\n---------------Coletando Novo Componente---------------");
    
    printf("\n\nNome: "); 
    fgets(mochila->itens[idx].nome, sizeof(mochila->itens[idx].nome), stdin);
    mochila->itens[idx].nome[strcspn(mochila->itens[idx].nome, "\n")] = '\0';
    
    int tipojafoi = -1;
    do
	{
    	
    	if (tipojafoi == 1)
    	{
    		printf("\n\nAlerta: Valor incompativel, tente novamente!!\n\n");
		}
		
    	printf("\nTipo (Estrutural, Eletrico, Energia): "); 
    	fgets(mochila->itens[idx].tipo, sizeof(mochila->itens[idx].tipo), stdin);
    	mochila->itens[idx].tipo[strcspn(mochila->itens[idx].tipo, "\n")] = '\0';
    	
		tipojafoi = 1;
	}while( (strcmp(mochila->itens[idx].tipo, "Estrutural") != 0) && (strcmp(mochila->itens[idx].tipo, "Eletrico") != 0) && (strcmp(mochila->itens[idx].tipo, "Energia") != 0));
	
    printf("\nQuantidade: "); 
    scanf("%d", &mochila->itens[idx].quantidade); 
    LimparBuffer();

	int prijafoi = -1;
	do{
		if (prijafoi == 1)
    	{
    		printf("\n\nAlerta: Valor incompativel, tente novamente!!\n\n");
		}
		
    	printf("\nPririodade de Montagem(1-5): "); 
    	scanf("%d", &mochila->itens[idx].prioridade); 
    	LimparBuffer();
    	prijafoi = 1;
    	
	}while((mochila->itens[idx].prioridade<1) || (mochila->itens[idx].prioridade>5));
    
	mochila->numItens += 1; 
    printf("\n\nComponente %s adicionado!\n", mochila->itens[idx].nome);


}

// removerItem(): 
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna. 
void removeritem(Mochila *mochila, int Espaco)
{
    char ItemRemover [30]; 
    char achado = 0; 
    int pos;
    if (mochila->numItens == 0) 
    { 
        printf("\n\nNao ha nenhum item na mochila\n"); 
        printf("Digite ENTER para continuar..."); 
        getchar(); 
        return; 
    }

    printf("\n---------------REMOVER ITEM---------------\n\n"); 
    
    printf("Digite o nome do item a ser removido: "); 
    fgets(ItemRemover, sizeof(ItemRemover), stdin);

    ItemRemover[strcspn(ItemRemover, "\n")] = '\0';

	int i;
    for (i = 0; i < mochila->numItens; i++) {
        if(strcmp(mochila->itens[i].nome, ItemRemover) == 0) 
        { 
            pos = i; 
            achado = 1; 
            break; 
        }
    }

    if(achado == 0) 
    { 
        printf("\nItem %s nao achado!\n", ItemRemover); 
        printf("Digite ENTER para continuar..."); 
        getchar(); 
        return; 
    }
    else if (achado == 1) 
    { 
    	int i;
        for (i = pos; i< mochila->numItens - 1; i++){ 
            mochila->itens[i] = mochila->itens[i + 1];
        }
        mochila->numItens--;
        printf("\nItem %s removido com sucesso!\n", ItemRemover); 
        printf("Digite ENTER para continuar..."); 
        getchar(); 
    }

}

// listarItens(): 
// Exibe uma tabela formatada com todos os componentes presentes na mochila. 
void listaritens(Mochila *mochila, int Espaco)
{ 
    if (mochila->numItens == 0) 
    {
        printf("\n\nNao ha nenhum item na mochila\n"); 
        printf("Digite ENTER para continuar..."); 
        getchar(); 
        return;
    }
	
   	printf("\n----INVENTARIO ATUAL (%d/%d)----\n", mochila->numItens, Espaco); 
   	printf("-----------------------------------------------------------------\n"); 
    
   	printf("%-20s|%-15s|%-15s|%-15s \n", "NOME", "TIPO", "QUANTIDADE","PRIORIDADE"); 
   	printf("-----------------------------------------------------------------\n");
   	int i;
	for(i = 0; i < mochila->numItens; i++)     
	{
        printf("%-20s|", mochila->itens[i].nome); 
   		printf("%-15s|", mochila->itens[i].tipo); 
     	printf("%-15d|", mochila->itens[i].quantidade); 
   	    printf("%-15d \n", mochila->itens[i].prioridade); 
   	}
   	printf("-----------------------------------------------------------------\n\n"); 
    	
	
	printf("Digite ENTER para continuar..."); 
    getchar();
}

// menuDeOrdenacao(): 
// Permite ao jogador escolher como deseja ordenar os itens. 
// Utiliza a função insertionSort() com o critério selecionado. 
// Exibe a quantidade de comparações feitas (análise de desempenho).
void menuDeOrdenacao(Mochila *mochila, CriterioOrdenacao * ordenacao)
{
	if (mochila->numItens == 0)
    {
        printf("\n\nNao ha nenhum item na mochila\n"); 
        printf("Digite ENTER para continuar..."); 
        getchar(); 
        *ordenacao = ORDENACAO_NENHUMA;
        return;
    }
    int foi = -1;
    int escolha = 0;
    do {
        if (foi == 1) {
            printf("\n\nAlerta: Valor invalido! Tente novamente!\n\n");
        }
        printf("\n----Estrategia de Organizacao----\n");
        printf("Como deseja ordenar os componentes?\n");
        printf("1. Por Nome (Ordem Alfabetica)\n");
        printf("2. Por Tipo\n");
        printf("3. Por Prioridade de Montagem\n");
        printf("0. Cancelar\n");
        printf("Escolha o criterio: ");
        if (scanf("%d", &escolha) != 1) {
            LimparBuffer();
            escolha = -1;
        } else {
            LimparBuffer();
        }
        foi = 1;
	}while((*ordenacao<0) || (*ordenacao>3));
	switch (escolha) {
        case 1: *ordenacao = ORDENACAO_NOME; break;
        case 2: *ordenacao = ORDENACAO_TIPO; break;
        case 3: *ordenacao = ORDENACAO_PRIORIDADE; break;
        default: *ordenacao = ORDENACAO_NENHUMA; break;
    }

}



// insertionSort(): 
// Implementação do algoritmo de ordenação por inserção. 
// Funciona com diferentes critérios de ordenação: 
// - Por nome (ordem alfabética) 
// - Por tipo (ordem alfabética) 
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(Mochila *mochila, CriterioOrdenacao ordenacao)
{
	int n = mochila->numItens;
    mochila->comparacoes = 0; 
    

	const char *Ordempor = "NENHUMA";
    if (ordenacao == 1) Ordempor = "NOME";
    else if (ordenacao == 2) Ordempor = "TIPO";
    else if (ordenacao == 3) Ordempor = "PRIORIDADE";
    
	int i;
    for (i = 1; i < n; i++) {
        Item key = mochila->itens[i];
        int j = i - 1;
        bool cond = false;

        while (j >= 0) {
            mochila->comparacoes++;
            bool cond = false;

            switch (ordenacao) {
                case ORDENACAO_NOME:
                    cond = strcmp(mochila->itens[j].nome, key.nome) > 0;
                    break;
                case ORDENACAO_TIPO:
                    cond = strcmp(mochila->itens[j].tipo, key.tipo) > 0;
                    break;
                case ORDENACAO_PRIORIDADE:
                    cond = mochila->itens[j].prioridade < key.prioridade;
                    break;
                default:
                    cond = false;
            }

            if (!cond) break;

            mochila->itens[j + 1] = mochila->itens[j];
            j = j - 1;
        }
        mochila->itens[j + 1] = key;
    }

    mochila->ordenadaPorNome = (ordenacao == ORDENACAO_NOME);
    
    printf("\n\nMochila organizada por %s", Ordempor);
	printf("\nAnalise de Desempenho: Foram necessarias %d comparacoes.\n\n", mochila->comparacoes);
    

}




// buscaBinariaPorNome(): 
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome. 
// Se encontrar, exibe os dados do item buscado. 
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome(Mochila * mochila, int Espaco)
{
    char ItemBuscar [30]; 
    int achado = -1;
    int numero;
    if (mochila->numItens == 0) 
    {
        printf("\n\nNao ha nenhum item na mochila\n"); 
        printf("Digite ENTER para continuar..."); 
        getchar(); 
        return;
    }
    printf("\n---------------BUSCAR ITEM NA MOCHILA---------------\n\n");
    printf("Digite o nome do item que deseja buscar: \n");
    fgets(ItemBuscar, sizeof(ItemBuscar),stdin);
    ItemBuscar[strcspn(ItemBuscar, "\n")] = '\0';

    int i;
    for (i = 0; i<mochila->numItens; i++)
    {
        if (strcmp(mochila->itens[i].nome, ItemBuscar) == 0)
        {
            achado = 1;
            numero = i;
            break;
        }
    }
    if (achado == -1)
    {
        printf("\n Resultado: Item '%s' NAO foi encontrado na mochila!\n\n", ItemBuscar);
    }
    else
    {
        printf("\n-------Item Encontrado!-------\n\n");
        printf("Nome: %s\n", mochila->itens[numero].nome);
        printf("Tipo: %s\n", mochila->itens[numero].tipo);
        printf("Quantidade: %d\n\n", mochila->itens[numero].quantidade);
        printf("------------------------------\n\n");
    }
    printf("Digite ENTER para continuar..."); 
    getchar();
}







