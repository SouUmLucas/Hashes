#include"Agencia.h"

void main()
{
	bool status;
	char opcao;
	CLIENTE cliente;
	Agencia agencia(&status);

	// para conter os dados da data atual do sistema
	time_t t = time(NULL);
	tm* ptrTime = localtime(&t);

	setlocale(LC_ALL, "portuguese_brazil");

	if (!status)
	{
		cout << "Houve um erro na instanciamento do objeto Agencia..." << endl;
		return;
	}

	while (true)
	{
		CLEAR_SCREEN;

		cout << "\n\tAGENCIA BANCÁRIA" << endl;
		cout << CADASTRAR_CLIENTE << " - Cadastrar cliente" << endl;
		cout << EXCLUIR_CLIENTE   << " - Excluir cliente" << endl;
		cout << BUSCAR_CLIENTE    << " - Buscar cliente" << endl;
		cout << DEBITAR_CHEQUE    << " - Debitar cheque do cliente" << endl;
		cout << DEBITAR_DEPOSITO  << " - Debitar depósito" << endl;
		cout << LISTAR_CLIENTES_MESMO_HASH << " - Listar cliente do mesmo Hash" << endl;
		cout << SAIR_DO_PROGRAMA  << " - Sair do programa" << endl;
		cout << ">> ";
		cin >> opcao;
		opcao = toupper(opcao);

		switch (opcao)
		{
			case CADASTRAR_CLIENTE:
				CLEAR_SCREEN;

				cout << "Digite o código do cliente: ";
				cin >> cliente.codigo;

				if (agencia.VerificaSeExiste(cliente.codigo, &cliente))
				{
					cout << "Cliente já existente..." << endl;
					cout << "Nome: " << cliente.nome << endl;

					PAUSE;
					break;
				}

				cin.ignore(1, EOF);
				cout << "Nome do cliente: ";
				cin.getline(cliente.nome, sizeof(cliente.nome) - 1, '\n');
				cout << "Saldo: ";
				cin >> cliente.saldo;
				
				cliente.ativo = ATIVO;

				cliente.creditos =
					cliente.debitos = 0.0;
				cliente.dataUltCredito.dia =
					cliente.dataUltCredito.mes =
					cliente.dataUltCredito.ano = 0;

				cliente.dataUltDebito.dia =
					cliente.dataUltDebito.mes =
					cliente.dataUltDebito.ano = 0;

				if (!agencia.CadastrarCliente(&cliente))
				{
					cout << "Houve um erro ao realizar o cadastro...";
					PAUSE;
				}

				break;
			case EXCLUIR_CLIENTE:
			
				// Pedir codigo
				// Verificar se existe
				// Chamar metodo ExcluirCliente
				
				CLEAR_SCREEN;
				
				cout << "Digite o código do cliente: ";
				cin >> cliente.codigo;

				if (!agencia.VerificaSeExiste(cliente.codigo, &cliente))
				{
					cout << "Cliente não existe..." << endl;
					PAUSE;
					break;
				}
				else
				{
					printf("Confirmar exclusão de %s(s/n)?\n>> ", cliente.nome);
					cin >> opcao;
					opcao = toupper(opcao);

					if (opcao == 'S')
					{
						if (!agencia.ExcluirCliente(cliente.codigo, &cliente))
						{
							cout << "Ocorreu um erro ao excluir o cliente" << endl;
							PAUSE;
						}
					}
				}

				break;

			case BUSCAR_CLIENTE:

				CLEAR_SCREEN;

				cout << "\n\tBuscar por cliente" << endl;
				cout << "Digite o código do cliente: ";
				cin >> cliente.codigo;

				if (agencia.VerificaSeExiste(cliente.codigo, &cliente))
				{
					cout << "Nome do cliente: " << cliente.nome << endl;
					cout << "Saldo: " << cliente.saldo << endl;
					printf("Data do último crédito: %u/%u/%u\n", cliente.dataUltCredito.dia, cliente.dataUltCredito.mes, cliente.dataUltCredito.ano);
					printf("Data do último débito: %u/%u/%u\n", cliente.dataUltDebito.dia, cliente.dataUltCredito.mes, cliente.dataUltDebito.ano);
				}
				else
				{
					cout << "Cliente inexistente..." << endl;
				}

				PAUSE;
				break;

			case DEBITAR_CHEQUE:
				// Completar...
				CLEAR_SCREEN;

				cout << "\n\tDebitar cheque" << endl;
				cout << "Digite o código do cliente: ";
				cin >> cliente.codigo;

				if (agencia.VerificaSeExiste(cliente.codigo, &cliente))
				{
					double valorDebito;

					cout << "Nome do cliente: " << cliente.nome << endl;
					cout << "Saldo: " << cliente.saldo << endl;
					cout << "Digite o valor a ser debitado: ";
					cin >> valorDebito;

					cliente.dataUltDebito.dia = ptrTime->tm_mday;
					cliente.dataUltDebito.mes = ptrTime->tm_mon + 1;
					cliente.dataUltDebito.ano = ptrTime->tm_year + 1900;

					cliente.saldo -= valorDebito;

					if (!agencia.AtualizarSaldo(cliente.codigo, &cliente))
					{
						cout << "Houve um erro ao realizar o débito..." << endl;
						PAUSE;
					}
				}
				break;

			case DEBITAR_DEPOSITO:
				// Completar...
				CLEAR_SCREEN;

				cout << "\n\tDebitar depósito" << endl;
				cout << "Digite o código do cliente: ";
				cin >> cliente.codigo;

				if (agencia.VerificaSeExiste(cliente.codigo, &cliente))
				{
					double valorDeposito;

					cout << "Nome do cliente: " << cliente.nome << endl;
					cout << "Saldo: " << cliente.saldo << endl;
					cout << "Digite o valor a ser depositado: ";
					cin >> valorDeposito;

					cliente.saldo += valorDeposito;
					cliente.dataUltCredito.dia = ptrTime->tm_mday;
					cliente.dataUltCredito.mes = ptrTime->tm_mon + 1;
					cliente.dataUltCredito.ano = ptrTime->tm_year + 1900;

					if (!agencia.AtualizarSaldo(cliente.codigo, &cliente))
					{
						cout << "Houve um erro ao realizar o depósito..." << endl;
						PAUSE;
					}
				}
				break;

			case SAIR_DO_PROGRAMA:
				cout << "Deseja realmente sair(s/n)? >> ";
				cin >> opcao;
				opcao = toupper(opcao);
				
				if (opcao == 'S')
				{
					agencia.FecharArquivo();
					return;
				}

				break;

			case LISTAR_CLIENTES_MESMO_HASH:
				// completar

				break;

			default:
				cout << "Opcão inválida!" << endl;
				PAUSE;
				break;
		}
	}
}