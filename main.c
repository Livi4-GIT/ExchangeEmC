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

void contar_criptomoedas(int *qtde_linhas){ //contar qtde de criptomoedas
    FILE *fp = fopen("criptomoedas.txt", "r");
    if(fp == NULL){
        printf("erro na abertura do arquivo para leitura (criptomoedas.txt).\n");
        return;
    };

    char c;
    *qtde_linhas = 1;
    while((c = fgetc(fp)) != EOF){
        if (c == '\n')
            (*qtde_linhas)++;
    };
    *qtde_linhas = *qtde_linhas/5;

    fclose(fp);
}

void ler_dados_criptomoedas(char sigla[][4], char nome[][20], float *cotacao, float *taxavenda, float *taxacompra, int quantidade_criptomoedas){
    FILE *fp = fopen("criptomoedas.txt", "r");
    if (fp == NULL){
        printf("erro\n");
        return;
    }for (int i = 0; i < quantidade_criptomoedas; i++){
        fscanf(fp, "%s", sigla[i]);
        fscanf(fp, "%s", nome[i]);
        fscanf(fp, "%f", &cotacao[i]);
        fscanf(fp, "%f", &taxacompra[i]);
        fscanf(fp, "%f", &taxavenda[i]);
    };
    fclose(fp);
}

void consultar_saldo(int linha_usuario, long *cpf, int *senha, char nome_investidor[], float *moedas){
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL)
        printf("erro na abertura do arquivo para leitura (conferir saldo).\n");

    int quantidade_criptomoedas = 0;
    contar_criptomoedas(&quantidade_criptomoedas); // contar qtde criptomoedas

    fscanf(fp, "%ld", cpf);   // cpf (descartavel, usando apenas pra pular pra próxima linha)
    fscanf(fp, "%d", senha);   // senha (descartavel, igual o cpf)
    fscanf(fp, "%s", nome_investidor);
    for (int k = 0; k < quantidade_criptomoedas + 1; k++) // saldo: reais, btc, eth, xrp (ficam da 3 a 6 linha)
        fscanf(fp, "%f", &moedas[k]);

    //adquirir nomes e siglas das criptomoedas
    char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
    char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
    float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
    float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
    float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));
    ler_dados_criptomoedas(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

    printf("Saldo atual: R$ %.2f\n", moedas[0]);
    for (int j = 0; j < quantidade_criptomoedas; j++)
        printf("%s: %.6f %s\n", nome_cripto[j], moedas[j+1], sigla_cripto[j]);

    fclose(fp);
    free(sigla_cripto);
    free(nome_cripto);
    free(cotacao);
    free(taxavenda);
    free(taxacompra);
}

void adicionar_linha_extrato(int linha_usuario, char operacao, float valor, int escolha, float taxa){
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


    int quantidade_criptomoedas = 0;
    contar_criptomoedas(&quantidade_criptomoedas); // contar qtde criptomoedas

    char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
    char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
    float *cotacoes = malloc(quantidade_criptomoedas * sizeof(*cotacoes));
    float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
    float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));
    ler_dados_criptomoedas(sigla_cripto, nome_cripto, cotacoes, taxavenda, taxacompra, quantidade_criptomoedas);

    long cpf;
    int senha;
    char nome_investidor[200];
    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    consultar_saldo(linha_usuario, &cpf, &senha, nome_investidor, moedas);

    if (escolha >= 0){
        printf("Nova linha adicionada ao extrato:\n");
        printf("%s %c %16.8f %s CT: %10.2f TX: %3.2f  R$: %9.2f ", tempo, operacao, valor, sigla_cripto[escolha - 1], cotacoes[escolha - 1], taxa, moedas[0]);
        fprintf(fp, "%s %c %16.8f %s CT: %10.2f TX: %3.2f  R$: %9.2f ", tempo, operacao, valor, sigla_cripto[escolha - 1], cotacoes[escolha - 1], taxa, moedas[0]);
        for (int i = 0; i < quantidade_criptomoedas; i++){
            printf("%s %9.6f ", sigla_cripto[i], moedas[i+1]);
            fprintf(fp, "%s %9.6f ", sigla_cripto[i], moedas[i+1]);
    }
    }else{
        printf("Nova linha adicionada ao extrato:\n");
        printf("%s %c %16.8f R$ CT: 0.0 TX: 0.0  R$: %9.2f ", tempo, operacao, valor, moedas[0]);
        fprintf(fp, "%s %c %16.8f R$ CT: 0.0 TX: 0.0  R$: %9.2f ", tempo, operacao, valor, moedas[0]);
        for (int i = 0; i < quantidade_criptomoedas; i++){
            printf("%s %9.6f ", sigla_cripto[i], moedas[i+1]);             
            fprintf(fp, "%s %9.6f ", sigla_cripto[i], moedas[i+1]);
        }        
    }printf("\n");
    fprintf(fp, "\n");

    fclose(fp);
}

void sobrescrever_dados(int linha_usuario, long cpf, int senha, char nome_investidor[], float moedas[], int quantidade_criptomoedas){
    char nome[20];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "w");
    int total_moedas = quantidade_criptomoedas + 1;

    if (fp == NULL) {
        printf("erro ao abrir o arquivo para escrita (sobrescrever dados).\n");
        return;}
    fprintf(fp, "%ld\n", cpf);
    fprintf(fp, "%d\n", senha);
    fprintf(fp, "%s\n", nome_investidor);
    for (int k = 0; k < total_moedas; k++)
        fprintf(fp, "%.6f\n", moedas[k]);
    fclose(fp);
}

void depositar_fundos(int linha_usuario){
    long cpf;
    int senha;
    int quantidade_criptomoedas = 0;
    contar_criptomoedas(&quantidade_criptomoedas); // contar qtde criptomoedas

    char nome_investidor[200];
    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    consultar_saldo(linha_usuario, &cpf, &senha, nome_investidor, moedas);

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

    sobrescrever_dados(linha_usuario, cpf, senha, nome_investidor, moedas, quantidade_criptomoedas);

    char operacao = '+'; //chamar funcao de extrato
    float taxa = 0.0;
    int escolha = -1;
    
    adicionar_linha_extrato(linha_usuario, operacao, valor_deposito, escolha, taxa);
}

void consultar_extrato(int linha_usuario){
    char nome[20];
    sprintf(nome, "user%dextrato.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    if(fp == NULL){
        printf("Erro na abertura do arquivo para leitura (extrato).\n");
        return;
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
        for (int j = 0; j < i; j++)
            printf("%s", extrato[j]);
    }
}

void sacar_fundos(int linha_usuario){
    long cpf;
    int senha;
    char nome_investidor[200];
    int quantidade_criptomoedas;
    contar_criptomoedas(&quantidade_criptomoedas);

    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    consultar_saldo(linha_usuario, &cpf, &senha, nome_investidor, moedas);
    
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

    sobrescrever_dados(linha_usuario, cpf, senha, nome_investidor, moedas, quantidade_criptomoedas);

    char operacao = '-'; //chamar funcao de extrato
    float taxa = 0.0;
    int escolha = -1;

    adicionar_linha_extrato(linha_usuario, operacao, valor_saque, escolha, taxa);
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

void printar_criptomoedas(int quantidade_cripto, char nome[][20]){
    FILE *fp = fopen("cotacao_criptomoedas.txt", "r");
    if (fp == NULL){
        printf("erro ao ler o arquivo 'cotacao_criptomoedas.txt'");
        return;}

    float *cotacao = malloc(quantidade_cripto * sizeof(*cotacao));
    for (int i = 0; i < quantidade_cripto; i++)
        fscanf(fp, "%f", &cotacao[i]);

    printf("Cotacao atualizada:\n");
    for (int j = 0; j < quantidade_cripto; j++)
        printf("%s: %.2f\n", nome[j], cotacao[j]);

    fclose(fp);
    free(cotacao);
}

void comprar_criptomoedas(int linha_usuario){
    long cpf;
    int senha;
    int quantidade_criptomoedas;
    char nome_investidor[200];
    contar_criptomoedas(&quantidade_criptomoedas);
    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    consultar_saldo(linha_usuario, &cpf, &senha, nome_investidor, moedas);

    char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
    char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
    float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
    float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
    float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));
    ler_dados_criptomoedas(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

    int escolha;
    while(1){
        for(int i = 0; i < quantidade_criptomoedas; i++)
            if (i == quantidade_criptomoedas - 1)
                printf("%d. %s\n", i+1, nome_cripto[i]);
            else 
                printf("%d. %s | ", i+1, nome_cripto[i]);
        printf("Digito da criptomoeda desejada: ");
        scanf("%d", &escolha);
        if (escolha >= 1 && escolha <= quantidade_criptomoedas){
            printf("%s escolhida.\n", nome_cripto[escolha-1]);
            printf("Taxa de compra (%s): %.0f%%\n", sigla_cripto[escolha-1], taxacompra[escolha-1]);
            break;}
        else{
            printf("Insira um valor valido.");
            continue;}
    }

    printar_criptomoedas(quantidade_criptomoedas, nome_cripto); //usando a função pra pegar cotacoes atuais

    float saldo_convertido_em_cripto; // valor de compra convertido em cripto
    float valor_compra;
    while(1){
        printf("Digite o valor da compra em reais: ");
        scanf("%f", &valor_compra);
        if (valor_compra > moedas[0]-moedas[0] * taxacompra[escolha-1]/100){
            printf("O saldo em reais nao pode ficar negativo.\nInsira um valor ate %.2f [taxa de compra: %.0f%%]\n", moedas[0]-moedas[0]*taxacompra[escolha-1]/100, taxacompra[escolha-1]);
            continue;}
        else if (valor_compra == 0){
            printf("Compra cancelada.\n");
            return;
        }
    int digito;

    saldo_convertido_em_cripto = (valor_compra - (valor_compra*(taxacompra[escolha-1]/100))) / cotacao[escolha-1];

    printf("R$ %.2f equivale a %.6f %s [taxa de compra: %0.f%%]. Deseja confirmar a compra?\n1. Confirmar | 0. Cancelar a compra: ", valor_compra, saldo_convertido_em_cripto, sigla_cripto[escolha-1], taxacompra[escolha-1]);
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

    moedas[0] -= moedas[0]*taxacompra[escolha-1]/100;
    moedas[0] -= valor_compra;
    sobrescrever_dados(linha_usuario, cpf, senha, nome_investidor, moedas, quantidade_criptomoedas);


    char operacao = '+'; //chamar funcao de extrato
    float taxa = taxacompra[escolha-1];


    adicionar_linha_extrato(linha_usuario, operacao, saldo_convertido_em_cripto, escolha, taxa);
}

void venda_criptomoedas(int linha_usuario){
    long cpf;
    int senha;
    int quantidade_criptomoedas;
    char nome_investidor[200];
    contar_criptomoedas(&quantidade_criptomoedas);
    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    consultar_saldo(linha_usuario, &cpf, &senha, nome_investidor, moedas);

    char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
    char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
    float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
    float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
    float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));
    ler_dados_criptomoedas(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

    int escolha;

    while(1){
        for(int i = 0; i < quantidade_criptomoedas; i++)
            if (i == quantidade_criptomoedas - 1)
                printf("%d. %s\n", i+1, nome_cripto[i]);
            else 
                printf("%d. %s | ", i+1, nome_cripto[i]);
        printf("Escolha a criptomoeda desejada: ");
        scanf("%d", &escolha);
        if (escolha >= 1 && escolha <= quantidade_criptomoedas){
            printf("%s escolhida.\n", nome_cripto[escolha-1]);
            printf("Taxa de venda: %.0f%%\n", taxavenda[escolha-1]);
            break;}
        else{
            printf("Insira um valor valido.");
            continue;}
    }

    printar_criptomoedas(quantidade_criptomoedas, nome_cripto); //usando a função pra pegar cotacoes atuais

    float valor_venda;
    float saldo_convertido_em_cripto; // valor de venda convertido em cripto
    while(1){
        printf("Digite o valor da venda em reais: ");
        scanf("%f", &valor_venda);
            //se valor de venda > qtde btc * cotacao btc * taxa venda
        if (valor_venda > moedas[escolha] * cotacao[escolha-1] - moedas[escolha] * cotacao[escolha-1] * taxavenda[escolha-1]/100){
        // if (valor_venda < moedas[0]-moedas[0]*taxa_venda[escolha-1]/100){
            printf("O saldo em criptomoedas nao pode ficar negativo.\nInsira um valor ate %.2f [taxa de venda: %.0f%%]\n", moedas[escolha] * cotacao[escolha-1] - moedas[escolha] * cotacao[escolha-1] * taxavenda[escolha-1]/100, taxavenda[escolha-1]);
            continue;}
        else if (valor_venda == 0){
            printf("Compra cancelada.\n");
            return;
        }
        else if (valor_venda < 0){
            printf("O valor de venda nao pode ser negativo. Insira um valor valido.");
            continue;}
    int digito;
    saldo_convertido_em_cripto = (valor_venda - (valor_venda*(taxavenda[escolha-1]/100))) / cotacao[escolha-1];

    printf("R$ %.2f equivale a %.6f %s [taxa de venda: %.0f%%]. Deseja confirmar a venda?\n1. Confirmar | 0. Cancelar a venda: ", valor_venda, saldo_convertido_em_cripto, sigla_cripto[escolha-1], taxavenda[escolha - 1]);
    scanf("%d", &digito);
    if (digito == 1) {
        printf("Venda confirmada.\n");
        printf("moedas[escolha] = %f", moedas[escolha]);
        moedas[escolha] -= saldo_convertido_em_cripto;
        printf("moedas[escolha] = %f", moedas[escolha]);
        break;
    } else if (digito == 0) {
        printf("Venda cancelada.\n");
        return;
    }else {
        printf("Insira um digito valido.\n");
        continue;}
    }

    moedas[0] += valor_venda - valor_venda * taxavenda[escolha-1]/100;

    sobrescrever_dados(linha_usuario, cpf, senha, nome_investidor, moedas, quantidade_criptomoedas);

    
    char operacao = '-'; //chamar funcao de extrato
    float taxa = taxacompra[escolha-1];

    adicionar_linha_extrato(linha_usuario, operacao, saldo_convertido_em_cripto, escolha, taxa);
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
    
    int quantidade_criptomoedas = 0;
    contar_criptomoedas(&quantidade_criptomoedas); // contar qtde criptomoedas

    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    char nome_investidor[200];
    long cpf;
    int senha;
    int senha_novamente;

    int i = 1;
    // while(i >= 1 && i <= 8) { //switch responsavel por controlar o codigo inteiro (apos login)
    do{
        printf("\nSelecione a operacao desejada ou digite 8 para ver o menu novamente: ");
        scanf("%d", &i);
            
        switch(i) {
            case 1:
                printf("\n1. Consultar saldo:\n");
                consultar_saldo(linha_usuario, &cpf, &senha, nome_investidor, moedas);
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
                            
                int quantidade_criptomoedas;
                contar_criptomoedas(&quantidade_criptomoedas);
                apagar_criptomoedas();

                char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
                char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
                float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
                float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
                float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));

                ler_dados_criptomoedas(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

                for (int i = 0; i < quantidade_criptomoedas; i++)
                    atualizar_criptomoedas(cotacao[i]);

                printar_criptomoedas(quantidade_criptomoedas, nome_cripto);
                free(sigla_cripto);
                free(nome_cripto);
                free(cotacao);
                free(taxavenda);
                free(taxacompra);
                break;
            case 8:
                printf("\n8. Menu:\n");
                menu();
                break;
        case 0:
                printf("\nVoce saiu do sistema. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida.\n");
                break;
        }
    } while(i != 0);
};