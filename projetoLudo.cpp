//------------------------------ Bibliotecas Utilizadas ------------------------------//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include <numeric>

using namespace std;

//------------------------------ Estruturas e Variáveis Globais ------------------------------//

struct Usuario {

    string nome, senha, palavra_chave; 

};

struct Casa {
    char jogador; // 'A', 'B', 'C', 'D' para identificar o jogador, ' ' para vazio
    int peca; // Número da peça (1 a 4), -1 para casa vazia
};

struct Peca {
    int posicao;  // A posição da peça no tabuleiro
    bool ativa;   // Indica se a peça está ativa no jogo
    bool pontuada; // Indica se a peça já foi pontuada

};

static const vector<pair<int, int>> caminhoVermelho = {
    {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
    {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6},
    {1, 7}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, 
    {5, 8}, {6, 8}, {6, 9}, {6, 10},{6, 11}, 
    {6, 12}, {6, 13}, {7, 13}, {8, 13}, {8, 12}, 
    {8, 11}, {8, 10}, {8, 9}, {8, 8},{9, 8},
    {10, 8}, {11, 8}, {12, 8}, {13, 8},{13, 7}, 
    {13, 6}, {12, 6}, {11, 6}, {10, 6},{9, 6}, 
    {8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, 
    {8, 1}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, 
    {7, 5}, {7, 6}, {7, 7} // Final na base (7, 7)
};

static const vector<pair<int, int>> caminhoAzul = {
    {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, 
    {6, 9}, {6, 10},{6, 11}, {6, 12}, {6, 13}, 
    {7, 13}, {8, 13}, {8, 12}, {8, 11}, {8, 10}, 
    {8, 9}, {8, 8},{9, 8},{10, 8}, {11, 8}, 
    {12, 8}, {13, 8},{13, 7}, {13, 6}, {12, 6}, 
    {11, 6}, {10, 6},{9, 6}, {8, 6}, {8, 5}, 
    {8, 4}, {8, 3}, {8, 2}, {8, 1}, {7, 1}, 
    {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, 
    {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6},
    {1, 6}, {1, 7}, {2, 7}, {3, 7}, {4, 7},
    {5, 7}, {6, 7}, {7, 7} // Final na base (7, 7)
};

static const vector<pair<int, int>> caminhoAmarelo = {
    {8, 12}, {8, 11}, {8, 10}, {8, 9}, {8, 8},
    {9, 8},{10, 8}, {11, 8}, {12, 8}, {13, 8},
    {13, 7}, {13, 6}, {12, 6}, {11, 6}, {10, 6},
    {9, 6}, {8, 6}, {8, 5}, {8, 4}, {8, 3}, 
    {8, 2}, {8, 1}, {7, 1}, {6, 1}, {6, 2}, 
    {6, 3}, {6, 4}, {6, 5}, {6, 6}, {5, 6}, 
    {4, 6}, {3, 6}, {2, 6},{1, 6}, {1, 7},
    {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, 
    {6, 8}, {6, 9}, {6, 10},{6, 11}, {6, 12}, 
    {6, 13}, {7, 13}, {7, 12}, {7, 11}, {7, 10},
    {7, 9}, {7, 8}, {7, 7} // Final na base (7, 7)
};

static const vector<pair<int, int>> caminhoVerde = {
    {12, 6}, {11, 6}, {10, 6},{9, 6}, {8, 6}, 
    {8, 5}, {8, 4}, {8, 3}, {8, 2}, {8, 1},
    {7, 1}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, 
    {6, 5}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, 
    {2, 6}, {1, 6}, {1, 7},{1, 8}, {2, 8}, 
    {3, 8}, {4, 8}, {5, 8}, {6, 8}, {6, 9}, 
    {6, 10}, {6, 11}, {6, 12}, {6, 13}, {7, 13}, 
    {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9}, 
    {8, 8}, {9, 8},{10, 8}, {11, 8}, {12, 8}, 
    {13, 8}, {13, 7}, {12, 7}, {11 ,7}, {10, 7},
    {9, 7}, {8, 7}, {7, 7} // Final na base (7, 7)
};

static const vector<vector<pair<int, int>>> areasIniciaisVermelho = {
    {{1, 1}, {1, 2}, {2, 1}, {2, 2}},    // Jogador 1 (Vermelho)

};

static const vector<vector<pair<int, int>>> areasIniciaisAzul = {   
    {{1, 12}, {1, 13}, {2, 12}, {2, 13}}, // Jogador 2 (Azul)
    
};

static const vector<vector<pair<int, int>>> areasIniciaisVerde = { 
    {{12, 1}, {12, 2}, {13, 1}, {13, 2}}, // Jogador 3 (Verde)
    
};

static const vector<vector<pair<int, int>>> areasIniciaisAmarelo = { 
    {{12, 12}, {12, 13}, {13, 12}, {13, 13}} // Jogador 4 (Amarelo)
};

int numJogadores;

Peca pecas[4][4]; // 4 jogadores, 4 peças cada
string coresJogadores[4] = {"Amarelo", "Azul", "Verde", "Vermelho"};

// Códigos de cores ANSI
const string RESET = "\033[0m";
const string VERMELHO = "\033[31m";
const string VERDE = "\033[32m";
const string AMARELO = "\033[33m";
const string AZUL = "\033[34m";
const string CIANO = "\033[36m";
const string BRANCO = "\033[37m";

//------------------------------ Declaração das Funções do Código ------------------------------//

// Procedimentos
void tela_inicial();
void menu();
void cadastro();
void escolher_jogadores();
void regrasDoJogoLudo(vector<string>& nomesJogadores);
void mostrar_tabuleiro(const vector<vector<char>>& tabuleiro);
void moverPecaDaBase(vector<vector<Peca>>& pecas, vector<vector<char>>& tabuleiro, int jogadorAtual, int peca);
void atualizarTabuleiro(vector<vector<char>>& tabuleiro, const vector<vector<Peca>>& pecas, const vector<vector<char>>& tabuleiroOriginal);
void jogar(const vector<string>& nomesJogadores);
void ranking (const vector<string>& nomesJogadores, const vector<int>& pontosJogadores);
void editar();
void alterar_nome();
void alterar_senha();
void alterar_chave();
void recuperar_nome();
void recuperar_senha();
void recuperar_palavra_chave();
void sair();
void cls();
void pause();

// Boleanas
bool login();
bool moverPeca(vector<vector<Peca>>& pecas, vector<vector<char>>& tabuleiro, int jogadorAtual, int pecaMovida, int valorDado);
bool capturarPeca(vector<vector<Peca>>& pecas, int jogador, int peca);
bool verificarVitoria(const vector<vector<Peca>>& pecas, int jogador);
bool verificarPassarVez(vector<vector<Peca>>& pecas, int jogadorAtual, int valorDado);
bool jogoFoiJogado = false;  // Inicializa como false, indicando que nenhum jogo foi jogado ainda

// Vetores
vector<vector<char>> inicializarTabuleiroLudo();
vector<vector<Peca>> inicializarPecas();
vector<int> pontosJogadores(4, 0);  // Inicializa com zero para cada jogador
vector<string> nomesJogadores;

// Funções
int rolarDado();

//------------------------------ Funções Relacionada ao Usuário ------------------------------//

void tela_inicial () {

    int opcao;

    cls();
    
    cout << "========================================" << endl;
    cout << "          Bem-vindo ao Ludo PI       " << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "1. Cadastrar um novo usuario" << endl;
    cout << "2. Login" << endl;
    cout << "3. Sair" << endl;
    cout << "4. Recuperar nome" << endl;
    cout << "5. Recuperar senha" << endl;
    cout << "6. Recuperar palavra-chave" << endl;
    cout << "========================================" << endl;
    cout << "Escolha uma opcao: ";
    cin >> opcao;

        switch (opcao) {
            case 1:

             cadastro ();               
                break;
            case 2:

             login ();                
                break;
           
            case 3:
             
             sair ();
                break;

            case 4:
             
             recuperar_nome ();
                break;

            case 5:
             
             recuperar_senha ();
                break;

            case 6:
             
             recuperar_palavra_chave ();
                break;

            default:

             cout << "Opcao invalida!" << endl;
             pause();
             tela_inicial ();
                break;  
        }
}

void cadastro () {

    Usuario novoUsuario;

    cls();

    cin.ignore();
    cout << "Digite o nome: ";
    getline(cin, novoUsuario.nome);
    cout << "Digite a senha: ";
    getline(cin, novoUsuario.senha);
    cout << "Digite a palavra chave: ";
    getline(cin, novoUsuario.palavra_chave);

    ifstream arquivoUsuarios("usuarios.txt");
    string linha;
    bool usuarioExistente = false;

    if (arquivoUsuarios.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
         size_t primeiro_espaco = linha.find(" ");
         size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1); // Procura a partir da posição após o primeiro espaço
         string usuario = linha.substr(0, primeiro_espaco);
         string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
         string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == novoUsuario.nome && senhaSalva == novoUsuario.senha && chaveSalva == novoUsuario.palavra_chave ) {
                usuarioExistente = true;
                break;
            }
        }
        arquivoUsuarios.close();
    }

    if (usuarioExistente) {
        cout << "Erro: Usuario com este nome, senha e palavra chave ja existe." << endl;
        pause();
        tela_inicial();
    } else {
        ofstream arquivoUsuarios("usuarios.txt", ios::app);

        if (arquivoUsuarios.is_open()) {
            arquivoUsuarios << novoUsuario.nome << " " << novoUsuario.senha << " " << novoUsuario.palavra_chave << endl;
            arquivoUsuarios.close();
            cout << "Usuario cadastrado com sucesso!" << endl;
            pause();
            menu();
        } else {
            cout << "Erro ao cadastrar o arquivo." << endl;
            pause();
            tela_inicial();
        }
    }
}

bool login() {
    string nome, senha, chave, linha;

    cls();

    cout << "Digite o nome de usuario: ";
    cin >> nome;
    cout << "Digite a senha: ";
    cin >> senha;
    cout << "Digite a palavra chave: ";
    cin >> chave;
    

    ifstream arquivoUsuarios("usuarios.txt");

    if (arquivoUsuarios.is_open()) {

        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1); // Procura a partir da posição após o primeiro espaço
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == nome && senhaSalva == senha && chaveSalva == chave) {
                cout << "Login bem-sucedido!" << endl;
                pause();
                menu();
                return true;
                
            }
        }

        cout << "Nome de usuario, senha ou palavra incorretos." << endl;
        pause();
        tela_inicial();
        arquivoUsuarios.close();

    } else {

        cout << "Erro ao abrir o arquivo." << endl;
        pause();
        tela_inicial();
    }

    return false;
}

void menu() {

    cls();

    int opcao;

    cout << "========================================" << endl;
    cout << "                Menu                    " << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "1. Jogar" << endl;
    cout << "2. Ranking" << endl;
    cout << "3. Editar" << endl;
    cout << "4. Sair" << endl;
    cout << "========================================" << endl;
    cout << "Escolha uma opcao: ";
    cin >> opcao;

        switch (opcao) {
            case 1:

             escolher_jogadores();          
                break;
            case 2:

                // Opção para exibir ranking
                if (jogoFoiJogado) {
                    ranking(nomesJogadores, pontosJogadores);
                } else {
                    cout << "Ainda nao houve uma partida para definir o ranking.\n";
                    system("pause");
                    menu();
                }
                break;
            case 3:

             editar ();
                break;
            case 4:
             
             sair ();
                break;

            default:

             cout << "Opcao invalida!" << endl;
             pause();
             menu ();
                break;  
        }
}

void escolher_jogadores() {
    bool entradaValida = false;
    string cores[4] = {"Vermelho", "Azul", "Verde", "Amarelo"};

    cls();

    while (!entradaValida) {
        cout << "Escolha quantos jogadores: 2 a 4" << endl;
        cin >> numJogadores;

        if (cin.fail()) {
            cin.clear(); // Limpa o estado de erro do cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta a entrada até a próxima quebra de linha
            cout << "Entrada invalida! Por favor, insira um numero." << endl;
        } else if (numJogadores >= 2 && numJogadores <= 4) {
            cout << "Voce escolheu: " << numJogadores << " jogadores." << endl;
            entradaValida = true;

            nomesJogadores.resize(numJogadores);
            for (int i = 0; i < numJogadores; ++i) {
                cout << "Insira o nome do jogador " << i + 1 << ": ";
                cin >> nomesJogadores[i];
                cout << "Jogador " << nomesJogadores[i] << " sera a cor " << cores[i] << "." << endl;
            }

            pause();
            regrasDoJogoLudo(nomesJogadores);
        } else {
            cout << "Opcao invalida! Tente novamente." << endl;
        }
    }
}

void regrasDoJogoLudo(vector<string>& nomesJogadores) {

    cls();
    
    cout << "========================================" << endl;
    cout << "           Regras do Jogo Ludo          " << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "1. O jogo e para 2 a 4 jogadores." << endl;
    cout << "2. Cada jogador escolhe uma cor e pega os 4 peoes da cor escolhida." << endl;
    cout << "3. O objetivo do jogo e levar todos os seus peoes do ponto inicial ate a casa central do tabuleiro." << endl;
    cout << "4. Cada jogador joga um dado na sua vez e move um de seus peoes o numero de casas indicado pelo dado." << endl;
    cout << "5. Um jogador so pode mover um peao para fora de sua base se tirar um 6 no dado." << endl;
    cout << "6. Se um jogador tirar 6 no dado, ele pode jogar novamente." << endl;
    cout << "7. Se um peao de um jogador cair na mesma casa que um peao adversário, o peao adversário e enviado de volta para a base." << endl;
    cout << "8. As casas coloridas sao seguras, onde os peoes nao podem ser capturados." << endl;
    cout << "9. Um peao so pode entrar na casa final (central) se o valor do dado for exatamente igual ao numero de casas restantes para a chegada." << endl;
    cout << "10. O primeiro jogador a levar todos os seus peoes ate a casa central vence o jogo." << endl;
    cout << "11. Os jogadores devem seguir o percurso ao redor do tabuleiro." << endl;
    cout << endl;
    cout << "========================================" << endl;
    pause();
    cls();
    jogar(nomesJogadores);
}

void ranking (const vector<string>& nomesJogadores, const vector<int>& pontosJogadores) {

    cls();

    // Cria um vetor de índices para ordenar os jogadores pelos pontos
    vector<int> indices(4);
    iota(indices.begin(), indices.end(), 0);

    // Ordena os índices com base nos pontos
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return pontosJogadores[a] > pontosJogadores[b];
    });

    // Formatação do ranking
    cout << "\n+---------------------------+\n";
    cout << "|      Ranking Final        |\n";
    cout << "+---------------------------+\n";
    cout << "|  Posicao  |  Jogador  | Pontos |\n";
    cout << "+-----------+-----------+--------+\n";

    for (int i = 0; i < 4; ++i) {
        int jogadorIndex = indices[i];
        cout << "|    " << setw(3) << left << (i + 1) << "    | " 
             << setw(9) << left << nomesJogadores[jogadorIndex] << " | " 
             << setw(6) << right << pontosJogadores[jogadorIndex] << " |\n";
    }

    cout << "+---------------------------+\n";
    pause();
    menu();
}

void editar () {

    cls();

    int opcao;

   
        cout << "1. Alterar nome" << endl;
        cout << "2. Alterar senha" << endl;
        cout << "3. Alterar palavra chave" << endl;
        cout << "4. Voltar ao menu" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:

             alterar_nome();   
                break;

            case 2:

             alterar_senha();
                break;

            case 3:

             alterar_chave();
                break;

            case 4:

             menu();
                break;

            default:
                cout << "Opcao invalida!" << endl;
                pause();
                editar();
                break;
        }
}

void alterar_nome () {

    cls();

    string nomeAtual, novoNome, senha, chave, linha;
    cout << "Digite o nome atual de usuario: ";
    cin >> nomeAtual;
    cout << "Digite o novo nome de usuario: ";
    cin >> novoNome;
    cout << "Digite sua senha: ";
    cin >> senha;
    cout << "digite sua palavra chave: ";
    cin >> chave;

    ifstream arquivoUsuarios("usuarios.txt");
    ofstream arquivoTemp("temp.txt");

    bool encontrado = false;

    if (arquivoUsuarios.is_open() && arquivoTemp.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1); // Procura a partir da posição após o primeiro espaço
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == nomeAtual && senhaSalva == senha && chaveSalva == chave) {
                arquivoTemp << novoNome << " " << senhaSalva << " " << chaveSalva << endl;
                encontrado = true;
            } else {
                arquivoTemp << linha << endl;
            }
        }

        arquivoUsuarios.close();
        arquivoTemp.close();

        if (remove("usuarios.txt") != 0) {
            perror("Erro ao remover o arquivo original");
            editar();
        } else if (rename("temp.txt", "usuarios.txt") != 0) {
            perror("Erro ao renomear o arquivo temporario");
            editar();
        } else {
            if (encontrado) {
                cout << "Nome de usuario alterado com sucesso!" << endl;
                pause();
                editar();
            } else {
                cout << "Nome de usuario, senha ou palavra chave incorretos." << endl;
                pause();
                editar();
            }
        }
    } else {
        cout << "Erro ao abrir o arquivo." << endl;
        if (arquivoUsuarios.is_open()) arquivoUsuarios.close();
        if (arquivoTemp.is_open()) arquivoTemp.close();
        editar();
    }
    if (remove("temp.txt") != 0) {
        perror("Erro ao remover o arquivo temporario");
        editar();
    }
}

void alterar_senha () {

    cls();

    string nomeUsuario, senhaAtual, novaSenha, chave, linha;
    cout << "Digite o nome de usuario: ";
    cin >> nomeUsuario;
    cout << "Digite a senha atual: ";
    cin >> senhaAtual;
    cout << "Digite a nova senha: ";
    cin >> novaSenha;
    cout << "Digite sua palavra chave: ";
    cin >> chave;

    ifstream arquivoUsuarios("usuarios.txt");
    ofstream arquivoTemp("temp.txt");

    bool encontrado = false;

    if (arquivoUsuarios.is_open() && arquivoTemp.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1); // Procura a partir da posição após o primeiro espaço
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == nomeUsuario && senhaSalva == senhaAtual && chaveSalva == chave) {
                arquivoTemp << usuario << " " << novaSenha << " " << chaveSalva << endl;
                encontrado = true;
            } else {
                arquivoTemp << linha << endl;
            }
        }

        arquivoUsuarios.close();
        arquivoTemp.close();

        if (remove("usuarios.txt") != 0) {
            perror("Erro ao remover o arquivo original");
            editar();
        } else if (rename("temp.txt", "usuarios.txt") != 0) {
            perror("Erro ao renomear o arquivo temporario");
            editar();
        } else {
            if (encontrado) {
                cout << "Senha alterada com sucesso!" << endl;
                pause();
                editar();
            } else {
                cout << "Nome de usuario, senha ou palavra chave incorretos." << endl;
                pause();
                editar();
            }
        }
    } else {
        cout << "Erro ao abrir o arquivo." << endl;
        if (arquivoUsuarios.is_open()) arquivoUsuarios.close();
        if (arquivoTemp.is_open()) arquivoTemp.close();
        editar();
    }
    if (remove("temp.txt") != 0) {
        perror("Erro ao remover o arquivo temporario");
        editar();
    }
}

void alterar_chave () {

    cls();

    string nomeUsuario, senha, chave, novaChave, linha;
    cout << "Digite o nome de usuario: ";
    cin >> nomeUsuario;
    cout << "Digite a senha: ";
    cin >> senha;
    cout << "Digite sua palavra chave atual: ";
    cin >> chave;
    cout << "Digite sua nova palavra chave: ";
    cin >> novaChave;

    ifstream arquivoUsuarios("usuarios.txt");
    ofstream arquivoTemp("temp.txt");

    bool encontrado = false;

    if (arquivoUsuarios.is_open() && arquivoTemp.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1); // Procura a partir da posição após o primeiro espaço
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == nomeUsuario && senhaSalva == senha && chaveSalva == chave) {
                arquivoTemp << usuario << " " << senhaSalva << " " << novaChave << endl;
                encontrado = true;
            } else {
                arquivoTemp << linha << endl;
            }
        }

        arquivoUsuarios.close();
        arquivoTemp.close();

        if (remove("usuarios.txt") != 0) {
            perror("Erro ao remover o arquivo original");
            editar();
        } else if (rename("temp.txt", "usuarios.txt") != 0) {
            perror("Erro ao renomear o arquivo temporario");
            editar();
        } else {
            if (encontrado) {
                cout << "Palavra chave alterada com sucesso!" << endl;
                pause();
                editar();
            } else {
                cout << "Nome de usuario, senha ou palavra chave incorretos." << endl;
                pause();
                editar();
            }
        }
    } else {
        cout << "Erro ao abrir o arquivo." << endl;
        if (arquivoUsuarios.is_open()) arquivoUsuarios.close();
        if (arquivoTemp.is_open()) arquivoTemp.close();
        editar();
    }
    if (remove("temp.txt") != 0) {
        perror("Erro ao remover o arquivo temporario");
        editar();
    }
}

void recuperar_nome() {

    cls();

    string senha, chave, linha, novoNome;
    int opcao;
    
    cout << "Digite a senha: ";
    cin >> senha;
    cout << "Digite a palavra chave: ";
    cin >> chave;

    ifstream arquivoUsuarios("usuarios.txt");

    bool encontrado = false;

    if (arquivoUsuarios.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1);
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (senhaSalva == senha && chaveSalva == chave) {
                cout << "Nome de usuario: " << usuario << endl;
                encontrado = true;
                break; // Não é necessário continuar procurando
            }
        }
        arquivoUsuarios.close();

        if (encontrado) {
            cout << "Deseja alterar o nome? " << endl;
            cout << "1. Sim" << endl;
            cout << "2. Nao" << endl;
            cin >> opcao;

            switch (opcao) {
                case 1: {
                    cout << "Digite o novo nome de usuario: ";
                    cin >> novoNome;

                    ifstream arquivoUsuarios("usuarios.txt");
                    ofstream arquivoTemp("temp.txt");

                    bool encontrado = false;

                    if (arquivoUsuarios.is_open() && arquivoTemp.is_open()) {
                        while (getline(arquivoUsuarios, linha)) {
                            size_t primeiro_espaco = linha.find(" ");
                            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1);
                            string usuario = linha.substr(0, primeiro_espaco);
                            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
                            string chaveSalva = linha.substr(segundo_espaco + 1);

                            if (senhaSalva == senha && chaveSalva == chave) {
                                arquivoTemp << novoNome << " " << senhaSalva << " " << chaveSalva << endl;
                                encontrado = true;
                            } else {
                                arquivoTemp << linha << endl;
                            }
                        }

                        arquivoUsuarios.close();
                        arquivoTemp.close();

                        if (remove("usuarios.txt") != 0) {
                            perror("Erro ao remover o arquivo original");
                            tela_inicial();
                        } else if (rename("temp.txt", "usuarios.txt") != 0) {
                            perror("Erro ao renomear o arquivo temporario");
                            tela_inicial();
                        } else {
                            if (encontrado) {
                                cout << "Nome de usuario alterado com sucesso!" << endl;
                                pause();
                                tela_inicial();
                            } else {
                                cout << "Senha de usuario ou palavra chave incorretos." << endl;
                                pause();
                                tela_inicial();
                            }
                        }
                    } else {
                        cout << "Erro ao abrir o arquivo." << endl;
                        tela_inicial();
                    }
                    if (remove("temp.txt") != 0) {
                        perror("Erro ao remover o arquivo temporario");
                        tela_inicial();
                    }
                    break;
                }
                case 2:
                    tela_inicial();
                    break;
                default:
                    cout << "Opcao invalida!" << endl;
                    pause();
                    tela_inicial();
                    break;
            }
        } else {
            cout << "Senha ou palavra chave nao encontrada." << endl;
            pause();
            tela_inicial();
        }
    } else {
        cout << "Erro ao abrir o arquivo." << endl;
        tela_inicial();
    }
}

void recuperar_senha () {

    cls();

    string nome, chave, linha, novaSenha;
    int opcao;
    
    cout << "Digite o nome: ";
    cin >> nome;
    cout << "Digite a palavra chave: ";
    cin >> chave;

    ifstream arquivoUsuarios("usuarios.txt");

    bool encontrado = false;

    if (arquivoUsuarios.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1);
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == nome && chaveSalva == chave) {
                cout << "senha de usuario: " << senhaSalva << endl;
                encontrado = true;
                break; // Não é necessário continuar procurando
            }
        }
        arquivoUsuarios.close();

        if (encontrado) {
            cout << "Deseja alterar a senha? " << endl;
            cout << "1. Sim" << endl;
            cout << "2. Nao" << endl;
            cin >> opcao;

            switch (opcao) {
                case 1: {
                    cout << "Digite a nova senha de usuario: ";
                    cin >> novaSenha;

                    ifstream arquivoUsuarios("usuarios.txt");
                    ofstream arquivoTemp("temp.txt");

                    bool encontrado = false;

                    if (arquivoUsuarios.is_open() && arquivoTemp.is_open()) {
                        while (getline(arquivoUsuarios, linha)) {
                            size_t primeiro_espaco = linha.find(" ");
                            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1);
                            string usuario = linha.substr(0, primeiro_espaco);
                            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
                            string chaveSalva = linha.substr(segundo_espaco + 1);

                            if (usuario == nome && chaveSalva == chave) {
                                arquivoTemp << usuario << " " << novaSenha << " " << chaveSalva << endl;
                                encontrado = true;
                            } else {
                                arquivoTemp << linha << endl;
                            }
                        }

                        arquivoUsuarios.close();
                        arquivoTemp.close();

                        if (remove("usuarios.txt") != 0) {
                            perror("Erro ao remover o arquivo original");
                            tela_inicial();
                        } else if (rename("temp.txt", "usuarios.txt") != 0) {
                            perror("Erro ao renomear o arquivo temporario");
                            tela_inicial();
                        } else {
                            if (encontrado) {
                                cout << "Senha de usuario alterado com sucesso!" << endl;
                                pause();
                                tela_inicial();
                            } else {
                                cout << "Nome de usuario ou palavra chave incorretos." << endl;
                                pause();
                                tela_inicial();
                            }
                        }
                    } else {
                        cout << "Erro ao abrir o arquivo." << endl;
                        tela_inicial();
                    }
                    if (remove("temp.txt") != 0) {
                        perror("Erro ao remover o arquivo temporario");
                        tela_inicial();
                    }
                    break;
                }
                case 2:
                    tela_inicial();
                    break;
                default:
                    cout << "Opcao invalida!" << endl;
                    pause();
                    tela_inicial();
                    break;
            }
        } else {
            cout << "Nome ou palavra chave nao encontrada." << endl;
            pause();
            tela_inicial();
        }
    } else {
        cout << "Erro ao abrir o arquivo." << endl;
        tela_inicial();
    }
}

void recuperar_palavra_chave () {

    cls();

    string nome, senha, linha, novaChave;
    int opcao;
    
    cout << "Digite o nome: ";
    cin >> nome;
    cout << "Digite a senha: ";
    cin >> senha;

    ifstream arquivoUsuarios("usuarios.txt");

    bool encontrado = false;

    if (arquivoUsuarios.is_open()) {
        while (getline(arquivoUsuarios, linha)) {
            size_t primeiro_espaco = linha.find(" ");
            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1);
            string usuario = linha.substr(0, primeiro_espaco);
            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
            string chaveSalva = linha.substr(segundo_espaco + 1);

            if (usuario == nome && senhaSalva == senha) {
                cout << "Palavra chave de usuario: " << chaveSalva << endl;
                encontrado = true;
                break; // Não é necessário continuar procurando
            }
        }
        arquivoUsuarios.close();

        if (encontrado) {
            cout << "Deseja alterar a palavra chave? " << endl;
            cout << "1. Sim" << endl;
            cout << "2. Nao" << endl;
            cin >> opcao;

            switch (opcao) {
                case 1: {
                    cout << "Digite a nova palavra chave de usuario: ";
                    cin >> novaChave;

                    ifstream arquivoUsuarios("usuarios.txt");
                    ofstream arquivoTemp("temp.txt");

                    bool encontrado = false;

                    if (arquivoUsuarios.is_open() && arquivoTemp.is_open()) {
                        while (getline(arquivoUsuarios, linha)) {
                            size_t primeiro_espaco = linha.find(" ");
                            size_t segundo_espaco = linha.find(" ", primeiro_espaco + 1);
                            string usuario = linha.substr(0, primeiro_espaco);
                            string senhaSalva = linha.substr(primeiro_espaco + 1, segundo_espaco - primeiro_espaco - 1);
                            string chaveSalva = linha.substr(segundo_espaco + 1);

                            if (usuario == nome && senhaSalva == senha) {
                                arquivoTemp << usuario << " " << senhaSalva << " " << novaChave << endl;
                                encontrado = true;
                            } else {
                                arquivoTemp << linha << endl;
                            }
                        }

                        arquivoUsuarios.close();
                        arquivoTemp.close();

                        if (remove("usuarios.txt") != 0) {
                            perror("Erro ao remover o arquivo original");
                            tela_inicial();
                        } else if (rename("temp.txt", "usuarios.txt") != 0) {
                            perror("Erro ao renomear o arquivo temporario");
                            tela_inicial();
                        } else {
                            if (encontrado) {
                                cout << "Palavra chave alterado com sucesso!" << endl;
                                pause();
                                tela_inicial();
                            } else {
                                cout << "Nome de usuario ou senha incorretos." << endl;
                                pause();
                                tela_inicial();
                            }
                        }
                    } else {
                        cout << "Erro ao abrir o arquivo." << endl;
                        tela_inicial();
                    }
                    if (remove("temp.txt") != 0) {
                        perror("Erro ao remover o arquivo temporario");
                        tela_inicial();
                    }
                    break;
                }
                case 2:
                    tela_inicial();
                    break;
                default:
                    cout << "Opcao invalida!" << endl;
                    pause();
                    tela_inicial();
                    break;
            }
        } else {
            cout << "Nome ou senha nao encontrada." << endl;
            pause();
            tela_inicial();
        }
    } else {
        cout << "Erro ao abrir o arquivo." << endl;
        tela_inicial();
    }
}

void sair() {
    cout << "Saindo do jogo. Obrigado por jogar!" << endl;
    exit(0);
}

void cls() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    #if defined(_WIN32) || defined(_WIN64)
        system("pause");
    #else
        system("read -p 'Pressione Enter para continuar...'");
    #endif
}

//------------------------------ Funções Relacionadas ao Funcionamento do Jogo ------------------------------//

vector<vector<char>> inicializarTabuleiroLudo() {
    const int TAMANHO_TABULEIRO = 15;
    vector<vector<char>> tabuleiro(TAMANHO_TABULEIRO, vector<char>(TAMANHO_TABULEIRO, ' '));

    // Caminhos neutros verticais e horizontais
    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i) {
        if (i != 7) { // Evita o centro
            tabuleiro[7][i] = '-';
            tabuleiro[i][7] = '|';
        }
    }

    // Caminhos especiais para cada jogador
    for (int i = 0; i < 6; ++i) {
        tabuleiro[6 - i][6] = '.'; // Caminho do jogador 1
        tabuleiro[6][6 - i] = '.';
        tabuleiro[8 + i][8] = '.'; // Caminho do jogador 3
        tabuleiro[8][8 + i] = '.';
        tabuleiro[8 + i][6] = '.';
        tabuleiro[8][6 - i] = '.';
        tabuleiro[6 - i][8] = '.'; // Caminho do jogador 2
        tabuleiro[6][8 + i] = '.';
    }

    // Definindo o centro e áreas de início
    tabuleiro[7][7] = '*'; // Centro

    // Definindo as casas seguras
    vector<pair<int, int>> casasSeguras = { {6, 2}, {2, 8}, {8, 12}, {12, 6}, {7, 7} };
    for (const auto& casa : casasSeguras) {
        tabuleiro[casa.first][casa.second] = '*'; // Casa segura
    }

    return tabuleiro;
}

void mostrar_tabuleiro(const vector<vector<char>>& tabuleiro) {
    const int TAMANHO_TABULEIRO = 15;
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO; ++j) {
            char celula = tabuleiro[i][j];
            switch (celula) {
                case '1':
                case '2':
                case '3':
                case '4':
                    cout << VERMELHO << setw(2) << celula << RESET;
                    break;
                case '5':
                case '6':
                case '7':
                case '8':
                    cout << AZUL << setw(2) << celula << RESET;
                    break;
                case 'Q':
                case 'W':
                case 'E':
                case 'R':
                    cout << VERDE << setw(2) << celula << RESET;
                    break;
                case 'T':
                case 'Y':
                case 'U':
                case 'I':
                    cout << AMARELO << setw(2) << celula << RESET;
                    break;
                case '*':
                    cout << CIANO << setw(2) << celula << RESET;
                    break;
                case '.':
                    cout << BRANCO << setw(2) << celula << RESET;
                    break;
                default:
                    cout << setw(2) << celula;
                    break;
            }
        }
        cout << endl;
    }
}

int rolarDado() {
    return (rand() % 6) + 1;
}

vector<vector<Peca>> inicializarPecas() {
    vector<vector<Peca>> pecas(4, vector<Peca>(4, {-1, false, false}));
    return pecas;
}

bool moverPeca(vector<vector<Peca>>& pecas, vector<vector<char>>& tabuleiro, int jogadorAtual, int pecaMovida, int valorDado) {
    Peca& p = pecas[jogadorAtual][pecaMovida];

    // Verifica se a peça já está na casa final
    if (p.posicao == 52) {
        cout << "A peca chegou na casa final!" << endl;

        // Verifica se a peça já foi pontuada
        if (!p.pontuada) {
            p.pontuada = true; // Marca a peça como pontuada

            // Atualiza a pontuação do jogador
            pontosJogadores[jogadorAtual] += 25;
        }

        // Verifica se há outra peça que pode ser movida
        bool outraPecaPodeSerMovida = false;
        for (const Peca& outraPeca : pecas[jogadorAtual]) {
            if (outraPeca.posicao != 52 && (outraPeca.ativa || valorDado == 6)) {
                outraPecaPodeSerMovida = true;
                break;
            }
        }

        // Se não houver outra peça que possa ser movida, passa a vez
        if (!outraPecaPodeSerMovida) {
            cout << "Nenhuma outra peca pode ser movida. Passando a vez." << endl;
            return false;
        } else {
            return false;  // Permite ao jogador escolher outra peça
        }
    }

    if (p.ativa) {
        int novaPosicao = p.posicao + valorDado;

        // Verifica se o valor do dado excede o número de casas restantes até a posição final
        if (novaPosicao > 52) {
            cout << "O valor do dado excede o numero de casas restantes. Passando a vez." << endl;
            return false;
        }

        // Atualiza a posição da peça
        p.posicao = novaPosicao;

        // Verifica se a peça alcançou a posição final
        if (p.posicao == 52) {
            p.pontuada = true; // Marca a peça como pontuada
            pontosJogadores[jogadorAtual] += 25; // Atualiza a pontuação do jogador
        }

        // Verifica se há captura de peça
        if (capturarPeca(pecas, jogadorAtual, pecaMovida)) {
            cout << "Peca capturada!" << endl;
        }

        return true;
    } else if (valorDado == 6) {
        moverPecaDaBase(pecas, tabuleiro, jogadorAtual, pecaMovida);
        return true;
    }

    return false;
}

void moverPecaDaBase(vector<vector<Peca>>& pecas, vector<vector<char>>& tabuleiro, int jogadorAtual, int peca) {
    static const vector<pair<int, int>> posicoesEntrada = {
        {6, 2}, {2, 8}, {12, 6}, {8, 12}
    };

    const vector<vector<pair<int, int>>>* areasIniciais[] = {
        &areasIniciaisVermelho, &areasIniciaisAzul, &areasIniciaisVerde, &areasIniciaisAmarelo
    };

    const char simbolosJogadores[4][4] = {
        {'1', '2', '3', '4'},  // Jogador 1
        {'5', '6', '7', '8'},  // Jogador 2
        {'Q', 'W', 'E', 'R'},  // Jogador 3
        {'T', 'Y', 'U', 'I'}   // Jogador 4
    };

    if (pecas[jogadorAtual][peca].posicao == -1) {
        pecas[jogadorAtual][peca].posicao = 0;  // Posiciona no início do percurso correto
        pecas[jogadorAtual][peca].ativa = true;

        // Remover peça da área inicial
        int linhaInicial = (*areasIniciais[jogadorAtual])[0][peca].first;
        int colunaInicial = (*areasIniciais[jogadorAtual])[0][peca].second;
        tabuleiro[linhaInicial][colunaInicial] = ' '; // Aqui, limpamos a célula da área inicial

        // Colocar peça na posição de entrada do tabuleiro principal
        int entradaLinha = posicoesEntrada[jogadorAtual].first;
        int entradaColuna = posicoesEntrada[jogadorAtual].second;
        tabuleiro[entradaLinha][entradaColuna] = simbolosJogadores[jogadorAtual][peca]; // Ajustar o símbolo da peça
    } else {
        cout << "A peça já está em jogo!" << endl;
    }
}

bool capturarPeca(vector<vector<Peca>>& pecas, int jogador, int pecaMovida) {
    // Verifica se a peça movida está ativa e em uma posição válida
    if (!pecas[jogador][pecaMovida].ativa || pecas[jogador][pecaMovida].posicao < 0) {
        return false;
    }

    int posicaoAtual = pecas[jogador][pecaMovida].posicao;
    pair<int, int> posicaoTabuleiroAtual;

    // Determina a posição real no tabuleiro com base na cor do jogador
    switch (jogador) {
        case 0: posicaoTabuleiroAtual = caminhoVermelho[posicaoAtual]; break;
        case 1: posicaoTabuleiroAtual = caminhoAzul[posicaoAtual]; break;
        case 2: posicaoTabuleiroAtual = caminhoVerde[posicaoAtual]; break;
        case 3: posicaoTabuleiroAtual = caminhoAmarelo[posicaoAtual]; break;
    }

    // Verifica se a posição atual é uma casa segura
    vector<pair<int, int>> casasSeguras = { {6, 2}, {2, 8}, {8, 12}, {12, 6}, {7, 7} };
    if (find(casasSeguras.begin(), casasSeguras.end(), posicaoTabuleiroAtual) != casasSeguras.end()) {
        return false; // Peça não pode ser capturada em uma casa segura
    }

    for (int i = 0; i < 4; ++i) {
        if (i == jogador) continue; // Ignora peças do mesmo jogador

        for (int j = 0; j < 4; ++j) {
            if (pecas[i][j].ativa) {
                pair<int, int> posicaoTabuleiroOutraPeca;
                switch (i) {
                    case 0: posicaoTabuleiroOutraPeca = caminhoVermelho[pecas[i][j].posicao]; break;
                    case 1: posicaoTabuleiroOutraPeca = caminhoAzul[pecas[i][j].posicao]; break;
                    case 2: posicaoTabuleiroOutraPeca = caminhoVerde[pecas[i][j].posicao]; break;
                    case 3: posicaoTabuleiroOutraPeca = caminhoAmarelo[pecas[i][j].posicao]; break;
                }

                if (posicaoTabuleiroAtual == posicaoTabuleiroOutraPeca) {
                    // Captura a peça adversária
                    pecas[i][j].posicao = -1;
                    pecas[i][j].ativa = false;
                    return true; // Indica que uma peça foi capturada
                }
            }
        }
    }

    return false; // Nenhuma peça foi capturada
}

bool verificarPassarVez(vector<vector<Peca>>& pecas, int jogadorAtual, int valorDado) {
    int pecasNaCasaFinal = 0;
    int pecasEmJogo = 0;
    int pecasNaAreaInicial = 0;

    for (const Peca& p : pecas[jogadorAtual]) {
        if (p.posicao == 52) {
            pecasNaCasaFinal++;
        } else if (p.ativa) {
            pecasEmJogo++;
        } else if (p.posicao == -1) { // Supondo que -1 seja a posição da área inicial
            pecasNaAreaInicial++;
        }
    }

    // Se todas as peças estão na casa final, ou não há peças ativas ou na área inicial para mover e o valor do dado não é 6, retorna verdadeiro
    return (pecasNaCasaFinal == 4 || (pecasEmJogo == 0 && pecasNaAreaInicial == 0 && valorDado != 6));
}

bool verificarVitoria(const vector<vector<Peca>>& pecas, int jogador) {
    for (const Peca& p : pecas[jogador]) {
        if (p.posicao != 52) {  // Ajustado para o valor específico da posição final do percurso
            return false;
        }
    }
    return true;
}

void atualizarTabuleiro(vector<vector<char>>& tabuleiro, const vector<vector<Peca>>& pecas, const vector<vector<char>>& tabuleiroOriginal) {

    cls();

    // Limpar o tabuleiro restaurando os caracteres originais
    for (size_t i = 0; i < tabuleiro.size(); ++i) {
        for (size_t j = 0; j < tabuleiro[i].size(); ++j) {
            tabuleiro[i][j] = tabuleiroOriginal[i][j];
        }
    }

    // Reposicionar as áreas iniciais
    const vector<vector<pair<int, int>>>* areasIniciais[] = {
        &areasIniciaisVermelho, &areasIniciaisAzul, &areasIniciaisVerde, &areasIniciaisAmarelo
    };

    const char simbolosJogadores[4][4] = {
        {'1', '2', '3', '4'},  // Jogador 1
        {'5', '6', '7', '8'},  // Jogador 2
        {'Q', 'W', 'E', 'R'},  // Jogador 3
        {'T', 'Y', 'U', 'I'}   // Jogador 4
    };

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (pecas[i][j].posicao == -1) {
                int linhaInicial = (*areasIniciais[i])[0][j].first;
                int colunaInicial = (*areasIniciais[i])[0][j].second;
                tabuleiro[linhaInicial][colunaInicial] = simbolosJogadores[i][j];
            }
        }
    }

    // Reposicionar peças
    static const vector<pair<int, int>> posicoesEntrada = {
        {6, 2}, {2, 8}, {12, 6}, {8, 12}
    };

    for (size_t i = 0; i < pecas.size(); ++i) {
        for (size_t j = 0; j < pecas[i].size(); ++j) {
            if (pecas[i][j].ativa) {
                int posicao = pecas[i][j].posicao;
                int linha, coluna;

                if (posicao == 0) {
                    linha = posicoesEntrada[i].first;
                    coluna = posicoesEntrada[i].second;
                } else {
                    size_t offset = static_cast<size_t>(posicao);
                    switch (i) {
                        case 0: // Jogador 1 (Vermelho)
                            if (offset < caminhoVermelho.size()) {
                                linha = caminhoVermelho[offset].first;
                                coluna = caminhoVermelho[offset].second;
                            }
                            break;
                        case 1: // Jogador 2 (Azul)
                            if (offset < caminhoAzul.size()) {
                                linha = caminhoAzul[offset].first;
                                coluna = caminhoAzul[offset].second;
                            }
                            break;
                        case 2: // Jogador 3 (Verde)
                            if (offset < caminhoVerde.size()) {
                                linha = caminhoVerde[offset].first;
                                coluna = caminhoVerde[offset].second;
                            }
                            break;
                        case 3: // Jogador 4 (Amarelo)
                            if (offset < caminhoAmarelo.size()) {
                                linha = caminhoAmarelo[offset].first;
                                coluna = caminhoAmarelo[offset].second;
                            }
                            break;
                    }
                }
                tabuleiro[linha][coluna] = simbolosJogadores[i][j];
            }
        }
    }
}

void jogar(const vector<string>& nomesJogadores) {
    vector<vector<char>> tabuleiro = inicializarTabuleiroLudo();
    vector<vector<Peca>> pecas = inicializarPecas();
    bool vitoria = false;
    int jogadorAtual = 0;
    const int numJogadores = nomesJogadores.size(); // Use o tamanho do vetor nomesJogadores para definir o numero de jogadores
    const string coresJogadores[] = {"Vermelho", "Azul", "Verde", "Amarelo"};
    const vector<vector<char>> simbolosJogadores = {
        {'1', '2', '3', '4'},  // Jogador 1
        {'5', '6', '7', '8'},  // Jogador 2
        {'q', 'w', 'e', 'r'},  // Jogador 3
        {'t', 'y', 'u', 'i'}   // Jogador 4
    };
    const int totalCasas = 52;
    const int totalCasasJogador = 7;
    vector<vector<char>> tabuleiroOriginal = tabuleiro; // Copia do tabuleiro original

    auto mudarVez = [](int& jogadorAtual, int numJogadores) {
        jogadorAtual = (jogadorAtual + 1) % numJogadores;
    };

    auto exibirJogadorAtual = [](int jogadorAtual, const vector<string>& nomesJogadores) {
        cout << "Jogador " << nomesJogadores[jogadorAtual] << ", e sua vez de jogar." << endl;
    };

    srand(time(0)); // Inicializar o gerador de numeros aleatorios

    while (!vitoria) {
        bool jogouSeis;
        do {
            jogouSeis = false;
            int valorDado = rolarDado();
            char pecaMovidaChar;
            int pecaMovida = -1; // Para indicar que nao foi escolhida uma peca valida

            atualizarTabuleiro(tabuleiro, pecas, tabuleiroOriginal);
            mostrar_tabuleiro(tabuleiro);

            exibirJogadorAtual(jogadorAtual, nomesJogadores);
            cout << "Voce rolou um " << valorDado << ". Escolha uma peca para mover: ";

            // Calcular quantas peças estão na casa final
            int pecasNaCasaFinal = 0;
            for (const Peca& p : pecas[jogadorAtual]) {
                if (p.posicao == 52) {
                    pecasNaCasaFinal++;
                }
            }

            if (verificarPassarVez(pecas, jogadorAtual, valorDado)) {
                cout << "Nenhuma outra peca pode ser movida. Passando a vez." << endl;
                break;
            }

            bool temPecaEmJogo = false;
            for (const Peca& p : pecas[jogadorAtual]) {
                if (p.ativa) {
                    temPecaEmJogo = true;
                    break;
                }
            }

            // Permitir o jogador mover qualquer peça se rolou um 6, incluindo peças na área inicial
            if (valorDado == 6) {
                cout << "Voce rolou um 6! Escolha uma peca para mover, incluindo pecas na area inicial:\n";
                while (true) {
                    cin >> pecaMovidaChar;
                    auto it = find(simbolosJogadores[jogadorAtual].begin(), simbolosJogadores[jogadorAtual].end(), pecaMovidaChar);
                    if (it != simbolosJogadores[jogadorAtual].end()) {
                        pecaMovida = distance(simbolosJogadores[jogadorAtual].begin(), it);
                        if (pecas[jogadorAtual][pecaMovida].ativa || pecas[jogadorAtual][pecaMovida].posicao == -1) {
                            break; // Encontrou uma peça válida do jogador
                        }
                    }
                    cout << "Opcao invalida! Escolha uma peca ativa ou na area inicial para mover:\n";
                }
            } else if (temPecaEmJogo) {
                cout << "Voce deve mover uma peca que esta em jogo.\n";
                while (true) {
                    cin >> pecaMovidaChar;
                    auto it = find(simbolosJogadores[jogadorAtual].begin(), simbolosJogadores[jogadorAtual].end(), pecaMovidaChar);
                    if (it != simbolosJogadores[jogadorAtual].end()) {
                        pecaMovida = distance(simbolosJogadores[jogadorAtual].begin(), it);
                        if (pecas[jogadorAtual][pecaMovida].ativa && pecas[jogadorAtual][pecaMovida].posicao != totalCasas + totalCasasJogador) {
                            break; // Encontrou uma peca ativa que nao esta na casa final
                        }
                    }
                    cout << "Opcao invalida! Escolha uma peca que esta em jogo: ";
                }
            } else {
                while (true) {
                    cin >> pecaMovidaChar;
                    auto it = find(simbolosJogadores[jogadorAtual].begin(), simbolosJogadores[jogadorAtual].end(), pecaMovidaChar);
                    if (it != simbolosJogadores[jogadorAtual].end()) {
                        pecaMovida = distance(simbolosJogadores[jogadorAtual].begin(), it);
                        if (pecas[jogadorAtual][pecaMovida].posicao != totalCasas + totalCasasJogador) {
                            break; // Encontrou uma peca valida que nao esta na casa final
                        }
                    }
                    cout << "Opcao invalida! Tente novamente. Escolha uma peca para mover:\n";
                }
            }

            if (moverPeca(pecas, tabuleiro, jogadorAtual, pecaMovida, valorDado)) {
                if (capturarPeca(pecas, jogadorAtual, pecaMovida)) {
                    cout << "Peca capturada!" << endl;
                }
                jogouSeis = (valorDado == 6);
            } else {
                cout << "Movimento invalido!" << endl;
                continue;
            }

            vitoria = verificarVitoria(pecas, jogadorAtual);

        } while (jogouSeis && !vitoria);
        if (!vitoria) {
            mudarVez(jogadorAtual, numJogadores);
        }
    }

    cls();
    cout << "========================================" << endl;
    cout << "Jogador " << nomesJogadores[jogadorAtual] << " venceu!" << endl;
    cout << "========================================" << endl;
    jogoFoiJogado = true;  // Indica que pelo menos um jogo foi jogado
    pause();
    ranking(nomesJogadores, pontosJogadores);
    menu();
}

//------------------------------ Funções Principal ------------------------------//

int main () {
    tela_inicial ();
    return 0;
}