// Classe Agencia para realizar o controle de clientes
#include "AgenciaBancariaManagement.h"

class Agencia
{
private:
	int nPosProximo;
	
	HASH vetHash[HASH_VALUE];
	CLIENTE dadosCliente;
	FILE *fdCadastro;

	/* Metodo para calcular o valor do hash */
	int CalcularHashCode(char *codigo)
	{
		int i,
			somatorio = 0,
			pesoInicial = PESO_INICIAL;

		for (i = 0; i < strlen(codigo); i++, pesoInicial--)
			somatorio += *(codigo + i) * pesoInicial;

		return somatorio % HASH_VALUE;
	}

	/* metodo que insere um cliente na lista ligada com seu hashcode */
	bool InserirNaListaDoHash(int hashcode, char *codigo)
	{
		if (vetHash[hashcode].primeiro == NULL)
			vetHash[hashcode].ultimo = vetHash[hashcode].primeiro = new CHAVE_BUSCA;
		else
			vetHash[hashcode].ultimo = vetHash[hashcode].ultimo->proximo = new CHAVE_BUSCA;

		if (vetHash[hashcode].ultimo == NULL)
			return false;

		vetHash[hashcode].ultimo->ativo = ATIVO;
		vetHash[hashcode].ultimo->proximo = NULL;
		strcpy_s(vetHash[hashcode].ultimo->codigo, codigo);
		vetHash[hashcode].ultimo->posicional = nPosProximo;

		nPosProximo++;
		return true;
	}

	bool RemoveDaListaDoHash(int hashcode, char *codigo)
	{
		CHAVE_BUSCA *handleAtual, *handle, *handleAnterior;
		bool naoPrimeiro;

		//nPosProximo--;

		for (handleAtual = vetHash[hashcode].ultimo, naoPrimeiro = false; handleAtual; handleAtual->proximo, naoPrimeiro = true)
		{
			if (strcmp(codigo, handleAtual->codigo) == 0)
			{
				if (!naoPrimeiro)
				{
					handle = handleAtual->proximo;
					delete handleAtual;
					vetHash[hashcode].primeiro = handle;

					return true;
				}
				else
				{
					handle = handleAtual->proximo;
					delete handleAtual;
					handleAnterior->proximo = handle;
				}
			}
			else
			{
				handleAnterior = handleAtual;
				continue;
			}
		}
	}

	/* Metodo que grava os dados do Cliente */
	bool GravaPosicionalCliente(CLIENTE *dadosCliente, int posicional)
	{
		if (fseek(fdCadastro, posicional * sizeof(CLIENTE), SEEK_SET) != 0)
			return false;

		if (fwrite(dadosCliente, sizeof(CLIENTE), 1, fdCadastro) == 0)
			return false;

		return true;
	}

	/* Metodo que faz a leitura do posicional do cliente */
	bool LerPosicionalCliente(CLIENTE *dadosCliente, int posicional)
	{
		if (fseek(fdCadastro, posicional * sizeof(CLIENTE), SEEK_SET) != 0)
			return false;

		if (fread_s(dadosCliente, sizeof(CLIENTE), sizeof(CLIENTE), 1, fdCadastro) == 0)
			return false;

		return true;
	}

	int RetornaPosicional(int hashcode, char *codigo)
	{
		int posicional;

		CHAVE_BUSCA *handleAtual;

		for (handleAtual = vetHash[hashcode].primeiro; handleAtual; handleAtual = handleAtual->proximo)
		{
			if (strcmp(handleAtual->codigo, codigo) == 0)
			{
				posicional = handleAtual->posicional;
				return posicional;
			}
			else
				continue;
		}
	}

public:
	Agencia(bool *status)
	{
		int i;
		nPosProximo = 0;

		/* Inicializa o hashtable */
		for (i = 0; i < HASH_VALUE; i++)
			vetHash[i].primeiro = NULL;

		/* Abertura do arquivo em modo leitura, gravacao e binario */
		if (fopen_s(&fdCadastro, NOME_ARQUIVO, "r+b") != 0)
		{
			if (fopen_s(&fdCadastro, NOME_ARQUIVO, "w+b") != 0)
			{
				*status = false;
				return;
			}

			for (i = 0; i < QTDE_MAXIMA_CLIENTES; i++)
			{
				dadosCliente.ativo = VAZIO;
				dadosCliente.codigo[0] = NULL;
				dadosCliente.nome[0] = NULL;
				dadosCliente.creditos =
					dadosCliente.debitos =
					dadosCliente.saldo = 0;
				dadosCliente.dataUltCredito.dia =
					dadosCliente.dataUltCredito.mes =
					dadosCliente.dataUltCredito.ano =
					dadosCliente.dataUltDebito.dia =
					dadosCliente.dataUltDebito.mes =
					dadosCliente.dataUltDebito.ano = 0;

				if (fwrite(&dadosCliente, sizeof(CLIENTE), 1, fdCadastro) == 0)
				{
					*status = false;
					return;
				}
			}

			fclose(fdCadastro);

			if (fopen_s(&fdCadastro, NOME_ARQUIVO, "r+b") != 0)
			{
				*status = false;
				return;
			}

			*status = true;
		}
		else
		{
			while (true)
			{
				if (fread_s(&dadosCliente, sizeof(CLIENTE), sizeof(CLIENTE), 1, fdCadastro) == 0)
					break;

				if (dadosCliente.ativo == VAZIO)
					break;

				if (dadosCliente.ativo == INATIVO)
				{
					nPosProximo++;
					continue;
				}

				/* 
					Feita a leitura do arquivo, e
					calculado o hashcode de cada codigo existente
					e inserido no vetor de hashes
				*/
				if (!InserirNaListaDoHash(CalcularHashCode(dadosCliente.codigo), dadosCliente.codigo))
				{
					*status = false;
					return;
				}
			}

			*status = true;
		}
	}

	/* Metodo para cadastrar um cliente no arquivo*/
	bool CadastrarCliente(CLIENTE *dadosCliente)
	{
		int hashcode = CalcularHashCode(dadosCliente->codigo);
		
		if (!GravaPosicionalCliente(dadosCliente, nPosProximo))
			return false;

		/* Associa um hashcode a um codigo do cliente */
		if (!InserirNaListaDoHash(hashcode, dadosCliente->codigo))
			return false;

		return true;
	}

	/* Metodo responsavel por excluir um cliente */
	bool ExcluirCliente(char *codigo, CLIENTE *dadosCliente)
	{
		// calcular o hashcode do codigo do cliente
		// obter o posicional
		// remover o cliente do hashtable
		// gravar os dados do cliente anativado no arquivo
		int hashcode = CalcularHashCode(codigo);

		int posicional = RetornaPosicional(hashcode, codigo);

		if (!RemoveDaListaDoHash(hashcode, dadosCliente->codigo))
			return false;

		dadosCliente->ativo = INATIVO;
		dadosCliente->codigo[0] = NULL;
		dadosCliente->nome[0] = NULL;
		dadosCliente->creditos =
			dadosCliente->debitos =
			dadosCliente->saldo = 0;
		dadosCliente->dataUltCredito.dia =
			dadosCliente->dataUltCredito.mes =
			dadosCliente->dataUltCredito.ano =
			dadosCliente->dataUltDebito.dia =
			dadosCliente->dataUltDebito.mes =
			dadosCliente->dataUltDebito.ano = 0;

		if (!GravaPosicionalCliente(dadosCliente, posicional))
			return false;

		return true;
	}

	/* Metodo que atualiza o saldo do cliente */
	bool AtualizarSaldo(char *codigo, CLIENTE *dadosCliente)
	{
		int hashcode = CalcularHashCode(codigo);
		int posicional = RetornaPosicional(hashcode, codigo);

		if (!GravaPosicionalCliente(dadosCliente, posicional))
			return false;

		return true;
	}

	/* Metodo que verifica se o cliente ja existe no cadastro*/
	bool VerificaSeExiste(char *codigo, CLIENTE *dadosCliente)
	{
		int hashcode = CalcularHashCode(codigo);

		CHAVE_BUSCA *handle;

		for (handle = vetHash[hashcode].primeiro; handle; handle = handle->proximo)
		{
			if (strcmp(codigo, handle->codigo) == 0)
			{
				if (handle->ativo == INATIVO)
					return false;

				if (!LerPosicionalCliente(dadosCliente, handle->posicional))
					return false;

				return true;
			}
		}

		return false;
	}


	/* Metodo para fechar o arquivo */
	void FecharArquivo()
	{
		fclose(fdCadastro);
	}
};