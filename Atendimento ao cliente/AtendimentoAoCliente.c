#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Registro do cadastro */
typedef struct RegCidadao{
    int Codigo;
    char Nome[31];
    int Idade;
    char Estado[3];  
	struct RegCidadao *prox;
}RegCidadao;

/* Registro da ficha */
typedef struct RegSenha{
    int Codigo;
    char Nome[31];
    char Tipo[15];
	int Idade;
    int Senha;
    int Preferencial;
	struct RegSenha *prox;     
}RegSenha;

/* Registro do atendimento */
typedef struct RegAtendido{
	char Tipo[15];
	char Nome[31];
	int Idade;
	int Senha;
	char Servidor[31];
    int Mesa;
	struct RegAtendido *prox;
}RegAtendido;
 
/* Protótipos das funções MAIN */
void CIDADAO(RegCidadao **lstCidadao);
void GERAR_SENHA(RegSenha **iP, RegSenha **fP, RegSenha **iNP, RegSenha **fNP, RegCidadao *lstCidadao, int *Senha);
void ATENDIMENTO(RegSenha **iP, RegSenha **fP, RegSenha **iNP, RegSenha **fNP, RegAtendido **lstAtendido);
void RELATORIOS(RegCidadao *lstCidadao, RegSenha *iP, RegSenha *iNP, RegAtendido **lstAtendido);

/* Protótipos das funções da função CIDADÃO */
void CADASTRAR(RegCidadao **lstCidadao);
void PESQUISAR(RegCidadao *lstCidadao);
void EXCLUIR(RegCidadao **lstCidadao);
void ATUALIZAR(RegCidadao *lstCidadao);

/* Protótipos das funções da função GERAR SENHA */
void ENFILEIRA(RegSenha **i, RegSenha **f, RegCidadao *cadastro, char *tipo, int *Senha);

/* Protótipos das funções da função ATENDIMENTO AO CIDADÃO */
void ATENDE(RegSenha **i, RegSenha **f, RegAtendido **lstAtendido);
RegSenha *DESENFILEIRA(RegSenha **i, RegSenha **f);

/* Protótipos das funções da função RELATÓRIOS */
void CADASTROS(RegCidadao *lstCidadao);
void NAO_ATENDIDOS(RegSenha *iP, RegSenha *iNP);
void ATENDS_REALIZADOS(RegAtendido **lstAtendido);

/* Protótipos de Outras funções */
RegCidadao *PesquisaCidadao(RegCidadao *lstCidadao, int codigo);
void InsereOrdenado(RegAtendido *nova, RegAtendido **lstAtendido);
void OrdenarNaoAtendido(RegSenha *p, RegSenha **OrdenaNaoAtendido);
void GravarDados(RegCidadao *lstCidadao);
void LerArquivo(RegAtendido **lstArquivo, char *tipo);
void SalvarArquivo(RegAtendido **lstArquivo, char *tipo);

int main(void)
{
	RegCidadao *lstCadastro = NULL;
	RegSenha *iP = NULL, *fP = NULL; 
	RegSenha *iNP = NULL, *fNP = NULL;
	RegAtendido *lstAtendido = NULL;
	char opcao;
	int Senha = 0;

	do
	{
		printf("------------------------------");
		printf("\n1 CIDADÃO\n");
		printf("2 GERAR SENHA\n");
		printf("3 ATENDIMENTO AO CIDADÃO\n");
		printf("4 RELATÓRIOS\n");
		printf("S SAIR\n");
		printf("-------------------------------\n");
		
		scanf(" %c", &opcao);
		
		switch(opcao)
		{
			case '1':
				CIDADAO(&lstCadastro);
				break;
			case '2':
				GERAR_SENHA(&iP, &fP, &iNP, &fNP, lstCadastro, &Senha);
				break;		
			case '3':
				ATENDIMENTO(&iP, &fP, &iNP, &fNP, &lstAtendido);
				break;
			case '4':
				RELATORIOS(lstCadastro, iP, iNP, &lstAtendido);
				break;		
			case 'S':
				/* Verifica se há pessoas na fila, caso não tenha grava os dados da seção em um arquivo de persisência */
                if (iP != NULL || iNP != NULL)
					fprintf(stderr, "Ainda há pessoas na fila!\n");
				else
				{
					GravarDados(lstCadastro);
					return 0;
				}			
				break;	
		}

	}while(1);
}

/* FUNÇÕES DO MAIN */

/* Função CIDADÃO */
void CIDADAO(RegCidadao **lstCidadao)
{
	int opcao;
	
	do
	{
		printf("-------------------------------");
		printf("\n1 CADASTRAR\n");
		printf("2 PESQUISAR POR CÓDIGO\n");
		printf("3 ATUALIZAR CADASTRO\n");
		printf("4 EXCLUIR CADASTRO\n");
		printf("0 VOLTAR\n");
		printf("-------------------------------\n");
		
		scanf("%d", &opcao);
		
		switch(opcao)
		{
			case 1:
				CADASTRAR(&*lstCidadao);
				break;			
			case 2:
				PESQUISAR(*lstCidadao);
				break;		
			case 3:
				ATUALIZAR(*lstCidadao);
				break;			
			case 4:
				EXCLUIR(&*lstCidadao);
				break;
		}		
		
	}while(opcao != 0);
	
}

/* Função GERAR SENHA */
void GERAR_SENHA(RegSenha **iP, RegSenha **fP, RegSenha **iNP, RegSenha **fNP, RegCidadao *lstCidadao, int *Senha)
{
	RegCidadao *existe;
	char tipo[15];
	int codigo;
	
	printf("Informe o codigo: ");
	scanf("%d", &codigo);

	existe = PesquisaCidadao(lstCidadao, codigo);

	if (existe != NULL)
	{
		printf("Informe o tipo de atendimento: ");
		printf("DOCUMENTOS, TRANSPORTES ou MORADIA\n");
		scanf(" %s", tipo);

		/* Verifica se o tipo atendimento informado pelo o usúario é válido */
		while (strcmp("DOCUMENTOS", tipo) != 0 && strcmp("TRANSPORTES", tipo) != 0 && strcmp("MORADIA", tipo) != 0)
		{
			printf("Por favor preste atenção no que se pede,\nInforme o tipo de atendimento: ");
			printf("DOCUMENTOS, TRANSPORTES ou MORADIA\n");
			scanf(" %s", tipo);
		}

		/* Verifica se a pessoa é preferencial ou não para alocar na fila */
		if (existe->Idade >= 65)
		{
			ENFILEIRA(&*iP, &*fP, existe, tipo, &*Senha);
		}
		else
		{
			ENFILEIRA(&*iNP, &*fNP, existe, tipo, &*Senha);
		}	
	}	
	else
		fprintf(stderr, "Código não encontrado!\n");
		
}

/* Função ATENDIMENTO AO CIDADÃO */
void ATENDIMENTO(RegSenha **iP, RegSenha **fP, RegSenha **iNP, RegSenha **fNP, RegAtendido **lstAtendido)
{
	/* Verificação se existem pessoas na fila preferencial ou não */
	if (*iP != NULL)	
		ATENDE(&*iP, &*fP, &*lstAtendido);
	else if (*iNP != NULL)
		ATENDE(&*iNP, &*fNP, &*lstAtendido);
	else
		fprintf(stderr, "Não há pessoas na fila!\n");

	return;	  
}

/* Função RELATÓRIOS */
void RELATORIOS(RegCidadao *lstCidadao, RegSenha *iP, RegSenha *iNP, RegAtendido **lstAtendido)
{
	char opcao;

	do
	{
		printf("-------------------------------");
		printf("\n1 CIDADÃOS CADASTRADOS\n");
		printf("2 SENHAS NÃO ATENDIDAS\n");
		printf("3 ATENDIMENTOS REALIZADOS\n");
		printf("0 VOLTAR\n");
		printf("-------------------------------\n");

		scanf(" %c", &opcao);

		switch (opcao)
		{
			case '1':
				CADASTROS(lstCidadao);
				break;
			case '2':
				NAO_ATENDIDOS(iP, iNP);
				break;
			case '3':
				ATENDS_REALIZADOS(&*lstAtendido);
				break;
		}

	} while (opcao != '0');	
	
}

/* FUNÇÕES DA FUNÇÃO CIDADÃO */

/* Função CADASTRAR */
void CADASTRAR(RegCidadao **lstCidadao)
{
	RegCidadao *nova, *existe, *ant, *atual;
	int codigo;

	ant = NULL; 
	atual = *lstCidadao;
	
	printf("Informe seu codigo: ");
	scanf("%d", &codigo);
	
	/* Verifica se o código já existe, e solicita outro até que seja válido */
	existe = PesquisaCidadao(*lstCidadao, codigo);

	while (existe != NULL)
	{
		printf("Esse código já existe, por favor tente outro\nInforme outro codigo: ");
		scanf("%d", &codigo);

		existe = PesquisaCidadao(*lstCidadao, codigo);
	}

	/* Alocação de memoria e verificação da mesma */
	nova = (RegCidadao *) malloc(sizeof(RegCidadao));
	if (nova == NULL)
	{
		fprintf(stderr, "Opps, falha na alocação de memoria!\n");
		return;
	}

	/* Verificação para saber se é o primeiro cadastro da lista ou se já tem outros */
	while (atual != NULL && codigo > atual->Codigo)
	{
		ant = atual;
		atual = atual->prox;
	}
	
	if (ant == NULL)
		*lstCidadao= nova;
	else
		ant->prox = nova;

	nova->prox = atual;

	/* Preenche os dados restantes do cadastro */
	nova->Codigo = codigo;

	printf("Informe seu nome: ");
	scanf(" %[^\n]", nova->Nome);
	
	printf("Informe seu idade: ");
	scanf("%d", &nova->Idade);
	
	printf("Informe sua UF: ");
	scanf(" %s", nova->Estado);
	
}

/* Função PESQUISAR POR CÓDIGO */
void PESQUISAR(RegCidadao *lstCidadao)
{
	RegCidadao *existe;
	int codigo;
	
	printf("Informe o codigo: ");
	scanf("%d", &codigo);
	
    /* Verifica se o código é válido, se for válido exibe as informações
	do cidadão, senão exibe uma mensagem de erro */
	existe = PesquisaCidadao(lstCidadao, codigo);

	if (existe != NULL)
	{
		printf("\nNome: %s  ", existe->Nome);
		printf("Idade: %d  ", existe->Idade);
		printf("UF: %s\n", existe->Estado);
	}
	else
		fprintf(stderr, "Código não encontrado!\n");
	
}

/* Função ATUALIZAR CADASTRO */
void ATUALIZAR(RegCidadao *lstCidadao)
{
	RegCidadao *existe;
	int codigo;
	
	printf("Informe o codigo: ");
	scanf("%d", &codigo);

     /* Verifica se o código é válido, se for válido disponibiliza uma atualização
	 dos dados do cidadão, senão exibe uma mensagem de erro */
	existe = PesquisaCidadao(lstCidadao, codigo);
	
	/* Mostra os dados atuais para o cidadão relembrar */
	printf("Caso você tenha esquecido, seus dados atuais são esses: \n");
	printf("Nome: %s  Idade: %d  UF: %s\n", existe->Nome, existe->Idade, existe->Estado);

	if (existe != NULL)
	{
		printf("Informe seu nome: ");
		scanf(" %[^\n]", existe->Nome);
	
		printf("Informe seu idade: ");
		scanf("%d", &existe->Idade);
			
		printf("Informe sua UF: ");
		scanf(" %s", existe->Estado);

		fprintf(stderr, "Cadastro foi atualizado com sucesso!\n");
	}
	else
		fprintf(stderr, "Código não encontrado!\n");
	
    return;
}

/* Função EXCLUIR CADASTRO */
void EXCLUIR(RegCidadao **lstCidadao)
{
	RegCidadao *ant, *atual; 
	int codigo;

	ant = NULL;
	atual = *lstCidadao;
	
	printf("Informe o codigo: ");
	scanf("%d", &codigo);
	
	while (atual != NULL && atual->Codigo != codigo)
	{
		ant = atual;
		atual = atual->prox;
	}
	
	/* Verifica se a pessoa existe na lista de cadastros e faz a operação ou mostrar uma mensagem de erro */
	if (atual == NULL && ant->Codigo != codigo)
	{
		fprintf(stderr, "Código não encontrado!\n");
	}
	else
	{
		if (ant == NULL)
			*lstCidadao = atual->prox;
		else
			ant->prox = atual->prox;
		
		free(atual);
		fprintf(stderr, "Cadastro foi excluido com sucesso!\n");			
	}	
		
}

/* FUNÇÕES DA FUNÇÃO GERAR SENHA */

/* Função ENFILEIRA */
void ENFILEIRA(RegSenha **i, RegSenha **f, RegCidadao *cadastro, char *tipo, int *Senha)
{
	RegSenha *nova;
	
	/* Alocação de memoria e verificação da mesma */
	nova = (RegSenha *) malloc(sizeof(RegSenha));
	if (nova == NULL)
	{
		fprintf(stderr, "Opps, falha na alocação de memoria!\n");
		return;
	}

	/* Alocando o elemento na fila */
	if (*i == NULL)
		*i = nova;
	else
		(*f)->prox = nova;
	
	*f = nova;
	nova->prox = NULL;
	
	/* Preenche os dados restantes da ficha */
	nova->Codigo = cadastro->Codigo;
	strcpy(nova->Tipo, tipo);
	strcpy(nova->Nome, cadastro->Nome);
	nova->Idade = cadastro->Idade;

	/* Verifica se o cidadão é preferencial */	
	if (cadastro->Idade >= 65)
		nova->Preferencial = 1;
	else
		nova->Preferencial = 0;
	
	/* somando +1 na senha */
	*Senha = *Senha + 1;
	nova->Senha = *Senha;
}

/* FUNÇÕES DA FUNÇÃO ATENDIMENTO AO CIDADÃO */

/* Função ATENDE */
void ATENDE(RegSenha **i, RegSenha **f, RegAtendido **lstAtendido)
{
	RegSenha *atendido;
	RegAtendido *nova;
	char servidor[31];
	int mesa;
	
	printf("Informe o nome do servidor: ");
	scanf(" %[^\n]", servidor);
	
	printf("Informe o numero da mesa: ");
	scanf("%d", &mesa);

	/* Seleciona a proxima pessoas a ser atendida */
	atendido = DESENFILEIRA(&*i, &*f);

	/* Alocação de memoria e verificação da mesma */
	nova = (RegAtendido *) malloc(sizeof(RegAtendido));
	if (nova == NULL)
	{
		fprintf(stderr, "Opps, falha na alocação de memoria!\n");
		return;
	}

	/* Preenchendo as informações na fila de atendidos*/
	strcpy(nova->Nome, atendido->Nome);
	strcpy(nova->Servidor, servidor);
	strcpy(nova->Tipo, atendido->Tipo);
	nova->Idade = atendido->Idade;
	nova->Senha = atendido->Senha;
	nova->Mesa = mesa;

	/* Chamando função para alocar ordenado por senha */
	InsereOrdenado(nova, &*lstAtendido);
	
	/* Liberando a alocação de memoria da pessoa que foi atendida */
	free(atendido);

	printf("\nProxima senha: %d  ", nova->Senha);
	printf("Tipo: %s  ", nova->Tipo);
	printf("Mesa: %d\n", nova->Mesa);
}

/* Função DESENFILEIRAR */
RegSenha *DESENFILEIRA(RegSenha **i, RegSenha **f)
{
	RegSenha *p;

	p = *i;

	if (p != NULL)
	{
		*i = p->prox;
		if (*i == NULL)
			*f = *i;	
	}
	else
		fprintf(stderr, "A fila está vazia!\n");
	
	return p;
}

/* FUNÇÕES DA FUNÇÃO RELATÓRIOS */

/* Função CIDADÃOS CADASTRADOS */
void CADASTROS(RegCidadao *lstCidadao)
{
	RegCidadao *p;
	FILE *cidadaos;

	p = lstCidadao;

	/* Abertura do arquivo para a leitura */
	cidadaos = fopen("cidadaos.csv", "w");
	
	/* Verificação se o arquivo foi aberto */
	if (cidadaos == NULL)
	{
		printf("Infelizmente seu arquivo cidadaos.csv não foi aberto!\n");
		return;
	}

	/* Salvando os dados no arquivo */
	while (p != NULL)
	{
		fprintf(cidadaos, "%d;%s;%d;%s\n", p->Codigo, p->Nome, p->Idade, p->Estado);
		p = p->prox;
	}

	fclose(cidadaos);
	
	/* Mensagem de sucesso para o usuário */
	printf("Os dados foram atualizados com sucesso!\n");
}

/* Função SENHAS NÃO ATENDIDAS */
void NAO_ATENDIDOS(RegSenha *iP, RegSenha *iNP)
{
	RegSenha *OrdenaNaoAtendido, *p;
	FILE *proximos;

	OrdenaNaoAtendido = NULL;

	/* Abertura do arquivo e verificação para analisar se foi aberto com sucesso */
	proximos = fopen("proximos.csv", "w");

	if (proximos == NULL)
	{
		printf("Infelizmente seu arquivo proximos.csv não foi aberto!\n");
		return;
	}

	/* Salvando as pessoas da duas filas em lista ordenada */
	p = iP;
	while (p != NULL)
	{
		OrdenarNaoAtendido(p, &OrdenaNaoAtendido);
		p = p->prox;
	}

	p = iNP;
	while (p != NULL)
	{
		OrdenarNaoAtendido(p, &OrdenaNaoAtendido);
		p = p->prox;
	}

	/* Salva dados no arquivo e já libera a memoria alocada anteriormente */
	while (OrdenaNaoAtendido != NULL)
	{
		p = OrdenaNaoAtendido;
		fprintf(proximos, "%d;%s;%s;%d\n", p->Senha, p->Tipo, p->Nome, p->Preferencial);
		OrdenaNaoAtendido = OrdenaNaoAtendido->prox;
		free(p);
	}
		
	/* Fechamento do arquivo */
	fclose(proximos);

	/* Mensagem de sucesso para o usuário */
	printf("Os dados foram atualizados com sucesso!\n");
}

/* Função ATENDIMENTOS REALIZADOS */
void ATENDS_REALIZADOS(RegAtendido **lstAtendido)
{
	RegAtendido *ant, *atual, *lstDocumentos, *lstMoradia, *lstTransportes;

	lstDocumentos = NULL;
	lstMoradia = NULL;
	lstTransportes = NULL;
	ant = NULL;
	atual = *lstAtendido;

	/* Lendo dados do arquivo */
	LerArquivo(&lstDocumentos, "documentos.csv");
	LerArquivo(&lstTransportes, "transportes.csv");
	LerArquivo(&lstMoradia, "moradia.csv");

	/* Laço para salvar os atendimentos no arquivo */
	while (atual != NULL)
	{
		ant = atual;
		atual = atual->prox;

		if (strcmp(ant->Tipo, "DOCUMENTOS") == 0)
			InsereOrdenado(ant, &lstDocumentos);

		if (strcmp(ant->Tipo, "MORADIA") == 0)
			InsereOrdenado(ant, &lstMoradia);

		if (strcmp(ant->Tipo, "TRANSPORTES") == 0)
			InsereOrdenado(ant, &lstTransportes);
	}

	/* Resetando a lista de atendido */
	*lstAtendido = NULL;

	/* Salva no arquivo */
	SalvarArquivo(&lstDocumentos, "documentos.csv");
	SalvarArquivo(&lstTransportes, "transportes.csv");
	SalvarArquivo(&lstMoradia, "moradia.csv");

	/* Mensagem de sucesso para o usuário */
	printf("Os dados foram atualizados com sucesso!\n");
}

/* FUNÇÕES DA FUNÇÃO ATENDS REALIZADOS DO RELATORIOS */

/* Função da para ler o arquivo e gravar em uma lista */
void LerArquivo(RegAtendido **lstArquivo, char *tipo)
{
	RegAtendido *nova;
	FILE *arquivo;

	arquivo = fopen(tipo, "r");

	if (arquivo == NULL)
	{
		fprintf(stderr, "Infelizmente seu arquivo %s não foi aberto!\n", tipo);
		return;
	}

	/* Alocação de memoria e verificação da mesma */
	nova = (RegAtendido *) malloc(sizeof(RegAtendido));
	if (nova == NULL)
	{
		fprintf(stderr, "Opps, falha na alocação de memoria!\n");
		return;
	}

	fscanf(arquivo, "%d;%[^;];%d;%[^;];%d", &nova->Senha, nova->Nome, &nova->Idade, nova->Servidor, &nova->Mesa);

	while (!feof(arquivo))
	{
		InsereOrdenado(nova, &*lstArquivo);

		/* Alocação de memoria e verificação da mesma */
		nova = (RegAtendido *) malloc(sizeof(RegAtendido));
		if (nova == NULL)
		{
			fprintf(stderr, "Opps, falha na alocação de memoria!\n");
			return;
		}

		fscanf(arquivo, "%d;%[^;];%d;%[^;];%d", &nova->Senha, nova->Nome, &nova->Idade, nova->Servidor, &nova->Mesa);
	}

	/* Fechamento do arquivo */
	fclose(arquivo);
}

/* funcoes atendimentos realizados */
void SalvarArquivo(RegAtendido **lstArquivo, char *tipo)
{
	RegAtendido *lixo;
	FILE *arquivo;

	arquivo = fopen(tipo, "w");

	if (arquivo == NULL)
	{
		fprintf(stderr, "Infelizmente seu arquivo %s não foi aberto!\n", tipo);
		return;
	}

	/* Salvando os dados no arquivo e libernado a memoria alocada anteriormente */
	while (*lstArquivo != NULL)
	{
		fprintf(arquivo, "%d;%s;%d;%s;%d\n", (*lstArquivo)->Senha, (*lstArquivo)->Nome, (*lstArquivo)->Idade, (*lstArquivo)->Servidor, (*lstArquivo)->Mesa);
		lixo = *lstArquivo;
		*lstArquivo = (*lstArquivo)->prox;
		free(lixo);
	}

	/* REsentando a lista */
	*lstArquivo = NULL;

	/* Fechamento do arquivo */
	fclose(arquivo);
}

/* FUNÇÕES DA FUNÇÃO REUTILIZAVÉIS */

/* Função verifica se o código informado existe e retorna um ponteiro para ele ou retornar NULL*/
RegCidadao *PesquisaCidadao(RegCidadao *lstCidadao, int codigo)
{
	RegCidadao *p;

	/* Verifica se o codigo já existe na lista */
	for (p = lstCidadao; p != NULL; p = p->prox)
		if (p->Codigo == codigo)
			return p;	

	return NULL;
}

/* Função para inserir ordenado por senha */
void OrdenarNaoAtendido(RegSenha *p, RegSenha **OrdenaNaoAtendido)
{
	RegSenha *ant, *atual, *nova;

	ant = NULL;
	atual = *OrdenaNaoAtendido;

	/* Alocação de memoria e verificação da mesma */
	nova = (RegSenha *) malloc(sizeof(RegSenha));
	if (nova == NULL)
	{
		fprintf(stderr, "Opps, falha na alocação de memoria!\n");
		return;
	}

	/* Preenchendo a nova celula com as informações do que foi passado por parametro */
	nova->Senha = p->Senha;
	nova->Preferencial = p->Preferencial;
	strcpy(nova->Nome, p->Nome);
	strcpy(nova->Tipo, p->Tipo);

	while (atual != NULL && nova->Senha > atual->Senha)
	{
		ant = atual;
		atual = atual->prox;
	}
	
	/* Verifica se é no inicio ou não e insere onde foi selecionado */
	if (ant == NULL)
		*OrdenaNaoAtendido = nova;
	else
		ant->prox = nova;
	
	nova->prox = atual;
}

/* Função para inserir ordenado por senha */
void InsereOrdenado(RegAtendido *nova, RegAtendido **lstAtendido)
{
	RegAtendido *ant, *atual;

	ant = NULL;
	atual = *lstAtendido;

	while (atual != NULL && nova->Senha > atual->Senha)
	{
		ant = atual;
		atual = atual->prox;
	}
	
	if (ant == NULL)
		*lstAtendido = nova;
	else
		ant->prox = nova;

	nova->prox = atual;
}

void GravarDados(RegCidadao *lstCidadao)
{
	RegCidadao *p;
	FILE *cidadaos;

	p = lstCidadao;

	/* Abertura do arquivo para a leitura */
	cidadaos = fopen("persistencia.csv", "a+");
	
	/* Verificação se o arquivo foi aberto */
	if (cidadaos == NULL)
	{
		printf("Infelizmente seu arquivo persistencia.csv não foi aberto!\n");
		return;
	}

	/* Salvando os dados no arquivo */
	while (p != NULL)
	{
		fprintf(cidadaos, "%d;%s;%d;%s\n", p->Codigo, p->Nome, p->Idade, p->Estado);
		p = p->prox;
	}
}