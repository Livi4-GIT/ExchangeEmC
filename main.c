#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int conferir_senha(int senha, int linha_usuario){
    int senha_busca;
    int linha_busca = 0;

    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL){
        printf("erro na abertura do arquivo para leitura (conferir senha).\n");
        return 1;
    };

    while (fscanf(fp, "%d", &senha_busca) != EOF){
        linha_busca++;        // armazena a senha correta, já que a 2ª linha
        if (linha_busca == 2) // no user[].txt, contem a senha do usuario.
            break;
    }

    if (senha == senha_busca)
        return 1;
    
    fclose(fp);
    return 0;
};

void menu(){
    printf("\n---------------\n");
    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar fundos\n");
    printf("4. Sacar fundos\n");
    printf("5. Compra de criptomoedas\n");
    printf("6. Venda de criptomoedas\n");
    printf("7. Atualizar cotacoes\n");
    printf("8. Menu\n");
    printf("0. Sair\n");
    printf("---------------\n");
};

void consultar_saldo(int linha_usuario, long *cpf, int *senha, float *moedas){
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL)
        printf("erro na abertura do arquivo para leitura (conferir saldo).\n");

    fscanf(fp, "%ld", cpf);   // cpf (descartavel, usando apenas pra pular pra próxima linha)
    fscanf(fp, "%d", senha);   // senha (descartavel, igual o cpf)
    for (int k = 0; k < 4; k++) // saldo: reais, btc, eth, xrp (ficam da 3 a 6 linha)
        fscanf(fp, "%f", &moedas[k]);

    printf("Saldo atual: R$ %.2f\n", moedas[0]);
    printf("Bitcoin: %.6f BTC\n", moedas[1]);
    printf("Ethereum: %.4f ETH\n", moedas[2]);
    printf("Ripple: %.2f XRP\n", moedas[3]);

    fclose(fp);
}

void adicionar_linha_extrato(int linha_usuario, char operacao, float valor, char *tipo, float cotacao, float taxa, float *moedas){
    char nome[20];
    sprintf(nome, "user%dextrato.txt", linha_usuario);
    FILE *fp = fopen (nome, "a");
    if(fp == NULL){
        printf("Erro na abertura do arquivo user%dextrato.txt.\n", linha_usuario);
        return;}

    time_t tempo_atual;
    char tempo[80];
    time(&tempo_atual);
    struct tm *info_tempo = localtime(&tempo_atual);
    strftime(tempo, sizeof(tempo), "%d/%m/%Y %H:%M:%S", info_tempo);

    fprintf(fp,"%s %c %16.8f %s CT: %10.2f TX: %3.2f R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %7.2f\n", tempo, operacao, valor, tipo, cotacao, taxa, moedas[0], moedas[1], moedas[2], moedas[3]);
    printf("Nova linha adicionada ao extrato:\n");
    printf("%s %c %16.8f %s CT: %10.2f TX: %3.2f R$: %9.2f BTC: %9.6f ETH: %8.4f XRP: %7.2f\n", tempo, operacao, valor, tipo, cotacao, taxa, moedas[0], moedas[1], moedas[2], moedas[3]);

    fclose(fp);
}

void sobrescrever_dados(int linha_usuario, long cpf, int senha, float moedas[]){
    char nome[20];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "w");

    if (fp == NULL) {
        printf("erro ao abrir o arquivo para escrita (sobrescrever dados).\n");
        return;}
    fprintf(fp, "%ld\n", cpf);
    fprintf(fp, "%d\n", senha);
    for (int k = 0; k < 4; k++)
        fprintf(fp, "%.6f\n", moedas[k]);
    fclose(fp);
}

void depositar_fundos(int linha_usuario){
    long cpf;
    int senha;
    float moedas[4] = {0.0f};
    consultar_saldo(linha_usuario, &cpf, &senha, moedas); //usando a função consultar_saldo pra pegar o saldo atual

    float valor_deposito;
    while(1){
        printf("Digite o valor do deposito em reais: ");
        scanf("%f", &valor_deposito);
        if (valor_deposito < 0)
            printf("Digite um valor valido. Caso deseje cancelar o deposito, digite 0.\n");
        else if (valor_deposito == 0){
            printf("Deposito cancelado.\n");
            return;}
        else{
            printf("R$ %.2f adicionado a conta.\n", valor_deposito);
            break;}
    }

    moedas[0] += valor_deposito;

    sobrescrever_dados(linha_usuario, cpf, senha, moedas);

    char operacao = '+'; //chamar funcao de extrato
    char tipo[4] = " R$";
    float taxa = 0.0;
    float cotacao = 0.0;
    adicionar_linha_extrato(linha_usuario, operacao, valor_deposito, tipo, cotacao, taxa, moedas);
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

}

void sacar_fundos(int linha_usuario){
    long cpf;
    int senha;
    float moedas[4] = {0.0f};
    consultar_saldo(linha_usuario, &cpf, &senha, moedas); //usando a função consultar_saldo pra pegar o saldo atual

    float valor_saque;
    while(1){
        printf("Digite o valor do saque em reais: ");
        scanf("%f", &valor_saque);
        float saldo_atualizado = moedas[0] - valor_saque;
        if (saldo_atualizado < 0){
            printf("O saldo nao pode ficar negativo. Insira um valor ate %.2f ou digite 0 para sair.\n", moedas[0]);
            continue;
        }else if(valor_saque == 0){
            printf("Saque cancelado.\n");
            return;
            break;
        }else{
            printf("R$ %.2f removido da conta.\n", valor_saque);
            break;
        }
    }

    moedas[0] -= valor_saque;

    sobrescrever_dados(linha_usuario, cpf, senha, moedas);

    char operacao = '-'; //chamar funcao de extrato
    char tipo[4] = " R$";
    float taxa = 0.0;
    float cotacao = 0.0;
    adicionar_linha_extrato(linha_usuario, operacao, valor_saque, tipo, cotacao, taxa, moedas);
}

void apagar_criptomoedas(){
    FILE *fp = fopen("cotacao_criptomoedas.txt", "w");
    if (fp == NULL){
        printf("erro ao abrir o arquivo 'cotacao_criptomoedas.txt'");
        return;}
    fclose(fp);
}

void atualizar_criptomoedas(float valor_base){
    FILE *fp = fopen("cotacao_criptomoedas.txt", "a");
    if (fp == NULL){
        printf("erro ao abrir o arquivo 'cotacao_criptomoedas.txt'");
        return;}

    int subtracao_adicao = (rand() % 2); // random entre 0 e 1. se 0 = subtrai 0-5% ao valor base. se 1, adiciona
    float percentual = (rand() % 6) / 100.0; // valor entre 0 e 5%:

    if(subtracao_adicao == 0)
        valor_base -= valor_base * percentual;
    else
        valor_base += valor_base * percentual;
    
    fprintf(fp, "%.2f\n", valor_base);

    fclose(fp);
}

void printar_criptomoedas(float *cotacoes){
    FILE *fp = fopen("cotacao_criptomoedas.txt", "r");
    if (fp == NULL){
        printf("erro ao abrir o arquivo 'cotacao_criptomoedas.txt'");
        return;}

    // float btc, eth, xrp;
    // float *cotacoes[] = {&btc, &eth, &xrp};
    char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};

    int qtde_criptomoedas = 3;
    for (int i = 0; i < qtde_criptomoedas; i++){
        fscanf(fp, "%f", &cotacoes[i]);
    }
    printf("Cotacao atualizada:\n");
    for (int j = 0; j < qtde_criptomoedas; j++)
        printf("%s: %.2f\n", nomes[j], cotacoes[j]);

    fclose(fp);
}

void comprar_criptomoedas(int linha_usuario){
    long cpf;
    int senha;
    float moedas[4] = {0.0f};
    consultar_saldo(linha_usuario, &cpf, &senha, moedas); //usando a função consultar_saldo pra pegar o saldo atual

    int qtde_criptomoedas = 3;
    char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};
    char *siglas[] = {"BTC", "ETH", "XRP"};
    float taxa_compra[4] = {2, 1, 1};
    int escolha;

    while(1){
        for(int i = 0; i < qtde_criptomoedas; i++)
            printf("%d. %s\n", i+1, nomes[i]);
        printf("Escolha a criptomoeda desejada: ");
        scanf("%d", &escolha);
        if (escolha >= 1 && escolha <= qtde_criptomoedas){
            printf("%s escolhida.\n", nomes[escolha-1]);
            printf("Taxa de compra: %.0f%%\n", taxa_compra[escolha-1]);
            break;}
        else{
            printf("Insira um valor valido.");
            continue;}
    }

    float cotacoes[3] = {0.0f};
    printar_criptomoedas(cotacoes); //usando a função pra pegar cotacoes atuais

    float valor_compra;
    while(1){
        printf("Digite o valor da compra em reais: ");
        scanf("%f", &valor_compra);
        if (valor_compra > moedas[0]-moedas[0] * taxa_compra[escolha-1]/100){
            printf("O saldo em reais nao pode ficar negativo.\nInsira um valor ate %.2f (taxa de compra: %.0f%%)\n", moedas[0]-moedas[0]*taxa_compra[escolha-1]/100, taxa_compra[escolha-1]);
            continue;}
        else if (valor_compra == 0){
            printf("Compra cancelada.\n");
            return;
        }
    int digito;
    float saldo_convertido_em_cripto; // valor de compra convertido em cripto
    saldo_convertido_em_cripto = (valor_compra - (valor_compra*(taxa_compra[escolha-1]/100))) / cotacoes[escolha-1];

    printf("R$ %.2f equivale a %.6f %s. Deseja confirmar a compra?\n1. Confirmar | 0. Cancelar a compra: ", valor_compra, saldo_convertido_em_cripto, siglas[escolha-1]);
    scanf("%d", &digito);
    if (digito == 1) {
        printf("Compra confirmada.\n");
        moedas[escolha] += saldo_convertido_em_cripto;
        break;
    } else if (digito == 0) {
        printf("Compra cancelada.\n");
        return;
    }else {
        printf("Insira um digito valido.\n");
        continue;}
    }

    moedas[0] -= moedas[0]*taxa_compra[escolha-1]/100;
    moedas[0] -= valor_compra;
    sobrescrever_dados(linha_usuario, cpf, senha, moedas);

    char operacao = '+'; //chamar funcao de extrato
    adicionar_linha_extrato(linha_usuario, operacao, (valor_compra - valor_compra * taxa_compra[escolha-1]/100)/cotacoes[escolha-1], siglas[escolha-1], cotacoes[escolha-1], taxa_compra[escolha-1], moedas);
}

void venda_criptomoedas(int linha_usuario){
    long cpf;
    int senha;
    float moedas[4] = {0.0f};
    consultar_saldo(linha_usuario, &cpf, &senha, moedas); //usando a função consultar_saldo pra pegar o saldo atual

    int qtde_criptomoedas = 3;
    char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};
    char *siglas[] = {"BTC", "ETH", "XRP"};
    float taxa_venda[4] = {3, 2, 1};
    int escolha;

    while(1){
        for(int i = 0; i < qtde_criptomoedas; i++)
            printf("%d. %s\n", i+1, nomes[i]);
        printf("Escolha a criptomoeda desejada: ");
        scanf("%d", &escolha);
        if (escolha >= 1 && escolha <= qtde_criptomoedas){
            printf("%s escolhida.\n", nomes[escolha-1]);
            printf("Taxa de venda: %.0f%%\n", taxa_venda[escolha-1]);
            break;}
        else{
            printf("Insira um valor valido.");
            continue;}
    }

    float cotacoes[3] = {0.0f};
    printar_criptomoedas(cotacoes); //usando a função pra pegar cotacoes atuais

    float valor_venda;
    while(1){
        printf("Digite o valor da venda em reais: ");
        scanf("%f", &valor_venda);
        printf("moedas[escolha] * cotacoes[escolha-1]: %f\n", moedas[escolha] * cotacoes[escolha-1]);
        printf("taxa_venda[escolha-1]/100) %f\n", moedas[escolha] * cotacoes[escolha-1] * taxa_venda[escolha-1]/100);
            //se valor de venda > qtde btc * cotacao btc * taxa venda
        if (valor_venda > moedas[escolha] * cotacoes[escolha-1] - moedas[escolha] * cotacoes[escolha-1] * taxa_venda[escolha-1]/100){
        // if (valor_venda < moedas[0]-moedas[0]*taxa_venda[escolha-1]/100){
            printf("O saldo em criptomoedas nao pode ficar negativo.\nInsira um valor ate %.2f (taxa de venda: %.0f%%)\n", moedas[escolha] * cotacoes[escolha-1] - moedas[escolha] * cotacoes[escolha-1] * taxa_venda[escolha-1]/100, taxa_venda[escolha-1]);
            continue;}
        else if (valor_venda == 0){
            printf("Compra cancelada.\n");
            return;
        }
        else if (valor_venda < 0){
            printf("O valor de venda nao pode ser negativo. Insira um valor valido.");
            continue;}
    int digito;
    float saldo_convertido_em_cripto; // valor de venda convertido em cripto
    saldo_convertido_em_cripto = (valor_venda - (valor_venda*(taxa_venda[escolha-1]/100))) / cotacoes[escolha-1];

    printf("R$ %.2f equivale a %.6f %s. Deseja confirmar a venda?\n1. Confirmar | 0. Cancelar a venda: ", valor_venda, saldo_convertido_em_cripto, siglas[escolha-1]);
    scanf("%d", &digito);
    if (digito == 1) {
        printf("Venda confirmada.\n");
        moedas[escolha] -= saldo_convertido_em_cripto;
        break;
    } else if (digito == 0) {
        printf("Venda cancelada.\n");
        return;
    }else {
        printf("Insira um digito valido.\n");
        continue;}
    }

    moedas[0] += valor_venda - valor_venda * taxa_venda[escolha-1]/100;

    sobrescrever_dados(linha_usuario, cpf, senha, moedas);

    char operacao = '-'; //chamar funcao de extrato
    adicionar_linha_extrato(linha_usuario, operacao, (valor_venda - valor_venda * taxa_venda[escolha-1]/100)/cotacoes[escolha-1], siglas[escolha-1], cotacoes[escolha-1], taxa_venda[escolha-1], moedas);
}

int main(void){
    long cpf_login;
    int senha_login;
    int linha_usuario; //responsavel por localizar o numero de usuario. se linha_usuario = 2, usa o user2.txt

    printf("Boas vindas ao Exchange de Criptomoedas.\nPor favor, insira seu CPF e senha.\n");
    
    while(1){
        printf("CPF: "); //cpf valido
        scanf("%ld", &cpf_login);

        if (conferirCPF(cpf_login, &linha_usuario)) // ponteiro dentro de uma função (conferirCPF) permite que
            break;                                  // a variável dentro de outra função (main) seja modificada
        else{
            printf("Insira um CPF valido.\n");
            continue;}
    }

    while(1){ //senha valida
        printf("Senha: ");
        scanf("%d", &senha_login);


        if (conferir_senha(senha_login, linha_usuario)){
            printf("Login efetuado com sucesso!\n");
            break;
        }else{
            printf("Senha incorreta. Insira sua senha novamente.\n");
            continue;
        }
    }

    menu(); //printa o menu
    float moedas[4] = {0.0f};
    long cpf;
    int senha;
    int senha_novamente;

    int i = 1;
    while(i >= 1 && i <= 8) { //switch responsavel por controlar o codigo inteiro (apos login)
        printf("\nSelecione a operacao desejada ou digite 8 para ver o menu novamente: ");
        scanf("%d", &i);
            
        switch(i) {
            case 1:
                printf("\n1. Consultar saldo:\n");
                consultar_saldo(linha_usuario, &cpf, &senha, moedas);
                break;
            case 2:
                printf("\n2. Consultar extrato:\n");
                consultar_extrato(linha_usuario);
                break;
            case 3:
                printf("\n3. Depositar fundos:\n");
                depositar_fundos(linha_usuario);
                break;
            case 4:
                printf("\n4. Sacar fundos:\n");

                while(1){
                printf("Insira sua senha novamente: ");
                scanf("%d", &senha_novamente);
                if (conferir_senha(senha_novamente, linha_usuario)){
                    sacar_fundos(linha_usuario);
                    break;}
                else{
                    printf("Senha incorreta!\n");}
                    continue;}

                break;
            case 5:
                printf("\n5. Compra de criptomoedas\n");

                while(1){
                printf("Insira sua senha novamente: ");
                scanf("%d", &senha_novamente);
                if (conferir_senha(senha_novamente, linha_usuario)){
                    comprar_criptomoedas(linha_usuario);
                    break;}
                else{
                    printf("Senha incorreta!\n");}
                    continue;}

                break;
            case 6:
                printf("\n6. Venda de criptomoedas\n");

                while(1){
                printf("Insira sua senha novamente: ");
                scanf("%d", &senha_novamente);
                if (conferir_senha(senha_novamente, linha_usuario)){
                    venda_criptomoedas(linha_usuario);
                    break;}
                else{
                    printf("Senha incorreta!\n");}
                    continue;}

                break;
            case 7:
                printf("\n7. Atualizar cotacao de criptomoedas\n");
                apagar_criptomoedas();
                atualizar_criptomoedas(352980.21f);
                atualizar_criptomoedas(14250.63f);
                atualizar_criptomoedas(3.81f);
                float cotacoes[3] = {0.0f};
                printar_criptomoedas(cotacoes);
                break;
            case 8:
                menu();
                break;
            default: //0 ou qualquer outro numero diferente de 1 a 8 finaliza o switch
                printf("Saindo...\n");
                break;
        }
    }
};