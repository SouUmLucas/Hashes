#include <ctime>
#include <iostream>
#include <Windows.h>

using namespace std;

#define PAUSE					system("pause")
#define CLEAR_SCREEN			system("cls")
#define QTDE_MAXIMA_CLIENTES	3
#define HASH_VALUE				300
#define PESO_INICIAL			51
#define ATIVO					'A'
#define INATIVO					'I'
#define VAZIO					'F'
#define NOME_ARQUIVO			"..\\Clients.cad"

// Menu do sistem
#define CADASTRAR_CLIENTE			'C'
#define EXCLUIR_CLIENTE				'E'
#define BUSCAR_CLIENTE				'M'
#define DEBITAR_CHEQUE				'D'
#define DEBITAR_DEPOSITO			'P'
#define LISTAR_CLIENTES_MESMO_HASH	'L'
#define SAIR_DO_PROGRAMA			'S'

/* Struct para conter a data */
typedef struct tagDATA
{
	int dia, mes, ano;
} DATA;

/* Struct para conter dados do cliente */
typedef struct tagCLIENTE
{
	char codigo[13],
		ativo,
		nome[41];
	
	double saldo,
		debitos,
		creditos;

	DATA dataUltDebito,
		dataUltCredito;
} CLIENTE;

typedef struct tagCHAVE_BUSCA
{
	char codigo[13],
		ativo;

	int posicional;
	
	tagCHAVE_BUSCA *proximo; // Ponteiro para a proxima chave com o mesmo hash
} CHAVE_BUSCA;

typedef struct tagHASH
{
	CHAVE_BUSCA *primeiro, *ultimo;
} HASH;