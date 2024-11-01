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

int MENU(){
  printf(" ");
  printf("--------------------BEM_VINDO AO MENU--------------------\n");
  printf("1.Cadastro de novo investidor\n");
  printf("2.Excluir investidor\n");
  printf("3.Cadastro de criptomoeda\n");
  printf("4.Excluir criptomoeda\n");
  printf("5.Consultar saldo de um investidor\n");
  printf("6.Consultar extrado de um investidor\n");
  printf("7.Atualizar cotação de criptomoedas\n");
};
int main(void) {
  struct ADM adm; 
  int opcao;

  strcpy(adm.CPF_ADM, "12345678910"); // CPF ADM
  adm.SENHA_ADM = 123456; // SENHA ADM
  login_ADM(adm);
  MENU();
  printf("Opçao: ");
  scanf("%d", &opcao);
  switch(opcao){
    case 1:
      printf("opção 1\n");
      break;
    case 2:
      printf("opção 2\n");
      break;
    case 3:
      printf("opção 3\n");
      break;
    case 4:
      printf("opção 4\n");
      break;
    case 5:
      printf("opção 5\n");
      break;
    case 6:
      printf("opção 6\n");
      break;
    case 7:
      printf("opção 7\n");
      break;
    default:
      printf("opcao invalida\n");
      break;
    }
  return 0;

}