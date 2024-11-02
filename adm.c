#include <stdio.h>
#include <string.h>

struct ADM {
    char CPF_ADM[12];
    int SENHA_ADM;
};

int login_ADM(struct ADM adm) { 
    char cpf_escrito[12]; // Array para armazenar o CPF digitado
    int senha_escrita;

    printf("--------------------EFETUE O LOGIN--------------------\n");
    printf("CPF: ");
    scanf("%s", cpf_escrito); // Limita a entrada a 11 caracteres
    printf("SENHA: ");
    scanf("%d", &senha_escrita); // Adicionando o operador &

    while (strcmp(cpf_escrito, adm.CPF_ADM) != 0 || senha_escrita != adm.SENHA_ADM) { // Compara CPF e senha
        printf("CPF ou Senha incorreto(os)! Tente novamente\n");
        printf("--------------------EFETUE O LOGIN--------------------\n");
        printf("CPF: ");
        scanf("%s", cpf_escrito); // Limita a entrada a 11 caracteres
        printf("SENHA: ");
        scanf("%d", &senha_escrita); // Adicionando o operador &
    }
    printf("Login Efetuado com sucesso!\n"); // Mensagem de sucesso
    return 0;

};

int menu(){
  printf("\n--------BEM-VINDO AO MENU--------\n");
  printf("1. Cadastro de novo investidor\n");
  printf("2. Excluir investidor\n");
  printf("3. Cadastro de criptomoeda\n");
  printf("4. Excluir criptomoeda\n");
  printf("5. Consultar saldo de um investidor\n");
  printf("6. Consultar extrato de um investidor\n");
  printf("7. Atualizar cotacao de criptomoedas\n");
  printf("8. Menu\n");
  printf("0. Sair\n");
  printf("---------------------------------\n");
};

void printar_usuarios(){
  FILE *fp = fopen("usuarios.txt", "r");

  if(fp == NULL){
    printf("erro na abertura do arquivo usuarios.txt para leitura.\n");
    return 1;
    };

    char users[10];
    while (fgets(users, sizeof(users), fp) != NULL) {
        printf("%s", users);
    }

    fclose(fp);
}

int conferirCPF(long long cpf, int *linha_usuario){// ponteiro dentro de uma função permite que a
    long long cpf_busca;                           // variável dentro de outra função seja modificada
    *linha_usuario = 1;

    FILE *fp = fopen("usuarios.txt", "r");

    if(fp == NULL){
            printf("erro na abertura do arquivo usuarios.txt para leitura.\n");
            return 1;
    };
    
    while (fscanf(fp, "%lld", &cpf_busca) != EOF){
        if (cpf_busca == cpf){
            return 1; // cpf valido
            break;
        }else{
            (*linha_usuario)++;
}
    };

    fclose(fp);
}

void consultar_extrato(int linha_usuario){
    char nome[20];
    sprintf(nome, "user%dextrato.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    if(fp == NULL){
        printf("Erro na abertura do arquivo para leitura (extrato).\n");
        return 1;
    };

    int linhas_maximo = 40;
    int tamanho_linha = 200;
    char extrato[linhas_maximo][tamanho_linha];
    int i = 0;

    while (fgets(extrato[i], tamanho_linha, fp) != NULL){
        i++;
    }
    if (i == 0)
        printf("Extrato vazio.\n");
    else{
        printf("Extrato:\n");
        for (int j; j < i; j++)
            printf("%s", extrato[j]);
    }
    fclose(fp);

}

int main(void) {
  struct ADM adm; 
  int opcao;
  int linha_usuario;
  strcpy(adm.CPF_ADM, "111"); // CPF ADM
  adm.SENHA_ADM = 111; // SENHA ADM
  login_ADM(adm);
  MENU();
  printf("Opçao: ");
  scanf("%d", &opcao);
  switch(opcao){
    case 1:
      printf("1. Cadastro de novo investidor: \n");
      break;
    case 2:
      printf("2. Excluir investidor: \n");
      break;
    case 3:
      printf("3. Cadastro de criptomoeda:\n");
      break;
    case 4:
      printf("4. Excluir criptomoeda: \n");
      break;
    case 5:
      printf("5. Consultar saldo de um investidor: \n");
      break;
    case 6:
      printf("6. Consultar extrato de um investidor: \n");

      printar_usuarios();
      long cpf_login;
      while(1){
        printf("CPF: "); //cpf valido
        scanf("%ld", &cpf_login);

        if (conferirCPF(cpf_login, &linha_usuario)) // ponteiro dentro de uma função (conferirCPF) permite que
            break;                                  // a variável dentro de outra função (main) seja modificada
        else{
            printf("Insira um CPF valido.\n");
            continue;}
    }
      consultar_extrato(linha_usuario);
      break;


    case 7:
      printf("7. Atualizar cotacao de criptomoedas: \n");
      break;
    case 8:
      menu();
      break;
    default:
      printf("Saindo...\n");
      break;
    }
}