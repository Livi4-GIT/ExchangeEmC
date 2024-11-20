#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int conferirCPF_adm(long long cpf){
    int linha_busca = 0;
    long long cpf_busca;    

    FILE *fp = fopen("adm.txt", "r");

    if(fp == NULL){
            printf("erro na abertura do arquivo usuarios.txt para leitura.\n");
            return 1;
    };
    
    while (fscanf(fp, "%lld", &cpf_busca) != EOF){
        linha_busca++;
        if (linha_busca == 1)
            break;
        
    };

    if (cpf == cpf_busca)
        return 1;

    fclose(fp);
    return 0;
};

int conferir_senha_adm(int senha){
    int senha_busca;
    int linha_busca = 0;

    FILE *fp = fopen ("adm.txt", "r");
    if(fp == NULL){
        printf("erro na abertura do arquivo para leitura (conferir senha).\n");
        return 1;
    };

    while (fscanf(fp, "%d", &senha_busca) != EOF){
        linha_busca++;
        if (linha_busca == 2)
            break;
    }
    if (senha == senha_busca)
        return 1;
    
    fclose(fp);
    return 0;
};

void menu_adm(){
    printf("\n--------------------------\n");
    printf("1. Cadastro de novo investidor\n");
    printf("2. Excluir investidor\n");
    printf("3. Cadastro de criptomoeda\n");
    printf("4. Excluir criptomoeda\n");
    printf("5. Consultar saldo de um investidor\n");
    printf("6. Consultar extrato de um investidor\n");
    printf("7. Atualizar cotacoes\n");
    printf("8. Menu\n");
    printf("0. Sair\n");
    printf("--------------------------\n");
};

void contar_criptomoedas_adm(int *qtde_linhas){ //contar qtde de criptomoedas
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

void quantidade_investidores(int *quantidade_cpfs){
    int maximo_cpfs = 20;
    long long cpfs[maximo_cpfs];

    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para cadastro (usuarios.txt)\n");
        return;
    }

    while (fscanf(arquivo, "%lld", &cpfs[*quantidade_cpfs]) != EOF && *quantidade_cpfs < maximo_cpfs) {
        (*quantidade_cpfs)++;
    }

    fclose(arquivo);
}

int novo_investidor(){//FUNCAO PARA INCLUIR NOVO INVESTIDOR
    int SENHA_N_INVEST;
    char NOME[200];
    long long CPF_N_INVEST;

    //contador cpfs e nova linha de cpf ao usuarios.txt
    int maximo_cpfs = 20;
    long long cpfs[maximo_cpfs];

    FILE *arquivo = fopen("usuarios.txt", "a+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para cadastro (usuarios.txt)\n");
        return 1;
    };

    int quantidade_cpfs = 0;
    int i = 0;
    
    while (fscanf(arquivo, "%lld", &cpfs[quantidade_cpfs]) != EOF && quantidade_cpfs < maximo_cpfs) {
        quantidade_cpfs++;
    }

    while(1){
        printf("Nome do investidor: ");
        scanf("%s", NOME);  
        printf("CPF do investidor (11 digitos): ");
        scanf("%lld", &CPF_N_INVEST);  
        printf("Senha do investidor (6 digitos): ");
        scanf("%d", &SENHA_N_INVEST);

        for (int j = 0; j < quantidade_cpfs; j++){
            if(CPF_N_INVEST == cpfs[j]){
                printf("CPF de investidor ja cadastrado.\nCadastro cancelado.\n");
                return 0;}
        }if (CPF_N_INVEST < 10000000000 || CPF_N_INVEST > 99999999999 || SENHA_N_INVEST < 100000 || SENHA_N_INVEST > 999999){
            printf("CPF ou SENHA invalidos! Tente novamente!\n");
            continue;
        }else
            break;
    }

    fprintf(arquivo, "%lld\n", CPF_N_INVEST); // adiciona uma nova linha com o novo cpf no usuarios.txt
    fclose(arquivo);

    char nome[20];
    sprintf(nome, "user%d.txt", quantidade_cpfs + 1);
    FILE *arquivo_investidor = fopen (nome, "w");
    if(arquivo_investidor == NULL){
        printf("erro na criacao do txt de novo investidor\n");
        return 1;
    };
    fprintf(arquivo_investidor, "%lld\n",CPF_N_INVEST); //escrevendo informacoes no user%d.txt
    fprintf(arquivo_investidor, "%d\n", SENHA_N_INVEST);
    fprintf(arquivo_investidor,"%s\n", NOME);
    int quantidade_criptomoedas;
    float nova_linha = 0.0;
    contar_criptomoedas_adm(&quantidade_criptomoedas);    
    for(int i = 0; i < quantidade_criptomoedas+1; i++) //criptomoedas + R$
        fprintf(arquivo_investidor, "%f\n", nova_linha);
    fclose(arquivo_investidor);
    // printf("quantidade criptomoedas: %d", quantidade_criptomoedas/5);

    //novo arquivo user%dextrato.txt, com extrato vazio.
    char nome2[20];
    sprintf(nome2, "user%dextrato.txt", quantidade_cpfs + 1);
    FILE *fp = fopen (nome2, "w");
    if(fp == NULL){
        printf("erro na criacao do txt de extrato de novo investidor\n");
        return 1;
    };

    printf("Cadastro efetuado!\n");
}

void Cadastro_Cript(){
    float tax_C, tax_V, Cot_In;
    char NOME[200], sigla[4];

    printf("Nome da moeda: ");
    scanf("%s", NOME);
    printf("Sigla da moeda (3 caracteres): ");
    scanf("%s", sigla);
    printf("Cotacao base: ");
    scanf("%f", &Cot_In);
    printf("Taxa de compra (%%): ");
    scanf("%f", &tax_C);
    printf("Taxa de venda (%%): ");
    scanf("%f", &tax_V);

    FILE *arquivo_Cripto  = fopen("criptomoedas.txt", "a");//CRIANDO ARQUIVO DAS CRIPTOMOEDAS
    if (arquivo_Cripto  == NULL) {
        printf("Erro ao cadastrar nova criptomoeda!\n");
        return;
    }
    //ESCREVENDO AS INFORMAÇOES NOS ARQUIVOS
    fprintf(arquivo_Cripto, "%s\n", sigla);
    fprintf(arquivo_Cripto, "%s\n", NOME);
    fprintf(arquivo_Cripto,"%f\n", Cot_In);
    fprintf(arquivo_Cripto, "%f\n", tax_C);
    fprintf(arquivo_Cripto, "%f\n", tax_V);
    fclose(arquivo_Cripto);
        
    printf("Moeda %s (%s) cadastrada com sucesso!\n", NOME, sigla);

    //adicionando nova linha a todos os usuarios, respectivo a nova moeda adicionada.
    int quantidade_cpfs = 0;
    quantidade_investidores(&quantidade_cpfs);

    for (int i = 0; i < quantidade_cpfs; i++){
        char nome[20];
        sprintf(nome, "user%d.txt", i+1);
        FILE *fp = fopen(nome, "a");
        if (fp == NULL) {
            printf("erro ao abrir o arquivo para escrita (cadastro criptomoedas, user%d.txt).\n", i+1);
            return;}
        float nova_linha = 0.0;
        fprintf(fp, "%f\n", nova_linha);
        fclose(fp);  
    }

    //adicionando uma nova linha a cotacoes usando o valor base da nova criptomoeda.
    FILE *fp2 = fopen("cotacao_criptomoedas.txt", "a");

    if (fp2 == NULL){
        printf("erro\n");
        return;
    }

    fprintf(fp2, "%.2f\n", Cot_In);
    fclose(fp2);
}

void ler_dados_criptomoedas_adm(char sigla[][4], char nome[][20], float *cotacao, float *taxavenda, float *taxacompra, int quantidade_criptomoedas){
    FILE *fp = fopen("criptomoedas.txt", "r");

    if (fp == NULL){
        printf("erro\n");
        return;
    }
    
    for (int i = 0; i < quantidade_criptomoedas; i++){
        fscanf(fp, "%s", sigla[i]);
        fscanf(fp, "%s", nome[i]);
        fscanf(fp, "%f", &cotacao[i]);
        fscanf(fp, "%f", &taxacompra[i]);
        fscanf(fp, "%f", &taxavenda[i]);
    };

    fclose(fp);
}


void apagar_dados_arquivo(const char *nome){
    FILE *fp = fopen (nome, "w");
    if (fp == NULL){
        printf("erro ao abrir o arquivo 'cotacao_criptomoedas.txt'");
        return;}
    fclose(fp);
}

void excluir_criptomoeda(){
    //a funcao de saldo conta quatnas criptomoedas tem entao nao precisa mudar isso. so usar o ler dados de criptmoedas e reescrever igual no excluir investidores.
    //printar dados da criptomoeda, confirmar exclusao, apagar a linha respectiva da criptomoeda em cada user.txt
    int quantidade_criptomoedas;
    contar_criptomoedas_adm(&quantidade_criptomoedas);
    int total_moedas = quantidade_criptomoedas + 1;
    char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
    char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
    float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
    float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
    float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));
    ler_dados_criptomoedas_adm(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

    char nome_criptomoeda_exclusao[200];
    int contador = -1;
    while(1){
        // int contador = -1;
        printf("Nome da moeda para exclusao: ");
        scanf("%s", nome_criptomoeda_exclusao);

        for (int i = 0; i < quantidade_criptomoedas; i++){            
            if (strcasecmp(nome_criptomoeda_exclusao, nome_cripto[i]) == 0){ //compara a moeda pra exclusao e as existentes no criptomoedas.txt; strcasecmp ignora o case das letras
                printf("Moeda '%s' selecionada.\n", nome_cripto[i]);
                contador = i;
                }
            }
        if (strcasecmp(nome_criptomoeda_exclusao, nome_cripto[contador]) == 0){ //compara a moeda pra exclusao e as existentes no criptomoedas.txt; strcasecmp ignora o case das letras
            printf("Dados da criptomoeda: \n");
            printf("Nome: %s\n", nome_cripto[contador]);
            printf("Sigla: %s\n", sigla_cripto[contador]);
            printf("Cotacao: %.2f\n", cotacao[contador]);
            printf("Taxa de compra: %.0f%%\n", taxacompra[contador]);
            printf("Taxa de venda: %.0f%%\n", taxavenda[contador]);
            break;
        }
        printf("Criptomoeda nao encontrada.\nLista de criptomoedas:\n");
        for (int i = 0; i < quantidade_criptomoedas; i++)
            printf("- %s\n", nome_cripto[i]);
        }

    while(1){
        int confirmar;
        printf("Confirme a exclusao: 1. Confirmar | 2. Cancelar\n");
        scanf("%d", &confirmar);
        if (confirmar == 2){
            printf("Operacao cancelada.\n");
            return;
        }else if(confirmar == 1){
            printf("Exclusao confirmada.\n");
            break;
        }else{
            printf("Digito invalido.\n");
            continue;
        }
    }

    int quantidade_cpfs = 0;
    quantidade_investidores(&quantidade_cpfs);

    for (int i = 0; i < quantidade_cpfs; i++){ //excluir a moeda da conta de todos os investidores
        char nome_investidor[200];
        long long cpf;
        int senha;
        float *saldos = malloc(total_moedas * sizeof(*saldos));
        char nome[10];
        sprintf(nome, "user%d.txt", i+1);
        FILE *fp = fopen (nome, "r");
        if (fp == NULL){
            printf("erro na leitura do usuarios.txt\n");
            return;
        }
        fscanf(fp, "%lld", &cpf);   // cpf (descartavel, usando apenas pra pular pra próxima linha)
        fscanf(fp, "%d", &senha);   // senha (descartavel, igual o cpf)
        fscanf(fp, "%s", nome_investidor);   // nome (descartavel, igual o cpf)
        for (int k = 0; k < total_moedas; k++) // reais, btc, eth, xrp, etc
            fscanf(fp, "%f", &saldos[k]);
        fclose(fp);

        char nome3[10];
        sprintf(nome3, "user%d.txt", i+1); //users começa no user1.txt
        apagar_dados_arquivo(nome3);

        char nome2[10];
        sprintf(nome2, "user%d.txt", i+1);
        FILE *fp2 = fopen (nome2, "w");
        if (fp2 == NULL){
            printf("erro na escrita do user.txt\n");
            return;
        }
        fprintf(fp2, "%lld\n", cpf);
        fprintf(fp2, "%d\n", senha);
        fprintf(fp2, "%s\n", nome_investidor);
        for (int i = 0; i < total_moedas; i++) {
            if (i != contador + 1) // +1 pra ignorar o saldo em reais
                fprintf(fp2, "%f\n", saldos[i]);
        }
        fclose(fp2);
    }

    // //excluir dados da criptomoeda no criptomoedas.txt
    apagar_dados_arquivo("criptomoedas.txt");
    FILE *arquivo_criptomoedas = fopen ("criptomoedas.txt", "w");
        if (arquivo_criptomoedas == NULL){
            printf("erro na escrita do criptomoedas.txt\n");
            return;
        }
    for (int i = 0; i < quantidade_criptomoedas; i++){
        if (i != contador){
            fprintf(arquivo_criptomoedas, "%s\n", sigla_cripto[i]);
            fprintf(arquivo_criptomoedas, "%s\n", nome_cripto[i]);
            fprintf(arquivo_criptomoedas, "%f\n", cotacao[i]);
            fprintf(arquivo_criptomoedas, "%f\n", taxacompra[i]);
            fprintf(arquivo_criptomoedas, "%f\n", taxavenda[i]);
        }
    }
    fclose(arquivo_criptomoedas);

    apagar_dados_arquivo("cotacao_criptomoedas.txt");

    FILE *arquivo_cotacao = fopen ("cotacao_criptomoedas.txt", "w");
    if (arquivo_cotacao == NULL){
            printf("erro na escrita do arquivo_cotacao.txt\n");
            return;
        }
    for (int i = 0; i < quantidade_criptomoedas; i++){
        if (i != contador){
            fprintf(arquivo_cotacao, "%f\n", cotacao[i]);
        }
    }
    fclose(arquivo_criptomoedas);
    free(sigla_cripto);
    free(nome_cripto);
    free(cotacao);
    free(taxavenda);
    free(taxacompra);
    }

void consultar_saldo_adm(int linha_usuario){
    char nome[10];
    sprintf(nome, "user%d.txt", linha_usuario);
    FILE *fp = fopen (nome, "r");
    
    if(fp == NULL)
        printf("erro na abertura do arquivo para leitura (conferir saldo).\n");

    long long cpf;
    int senha;
    char nome_investidor[200];
    int quantidade_criptomoedas = 0;
    contar_criptomoedas_adm(&quantidade_criptomoedas); // contar qtde criptomoedas
    
    float *moedas = malloc(quantidade_criptomoedas * sizeof(*moedas));
    fscanf(fp, "%lld", &cpf);   // cpf (descartavel, usando apenas pra pular pra próxima linha)
    fscanf(fp, "%d", &senha);   // senha (descartavel, igual o cpf)
    fscanf(fp, "%s", nome_investidor);   // nome (descartavel, igual o cpf)
    for (int k = 0; k < quantidade_criptomoedas + 1; k++) // saldo: reais, btc, eth, xrp (ficam da 3 a 6 linha)
        fscanf(fp, "%f", &moedas[k]);

    //adquirir nomes e siglas das criptomoedas
    char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
    char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
    float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
    float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
    float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));
    ler_dados_criptomoedas_adm(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

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

int buscar_linha_investidor(long long cpf, int *linha_usuario){
    long long cpf_busca;
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
        }else
            (*linha_usuario)++;
    };
    fclose(fp);
    return 0;
}

void remover_CPF_do_arquivo(int linha_usuario) {
    //salva todos os cpfs em um array, escreve por cima todos menos o cpf excluido
    int quantidade_cpfs = 0;
    quantidade_investidores(&quantidade_cpfs);
    
    long long cpfs[quantidade_cpfs];
    FILE *fp = fopen("usuarios.txt", "r");
    if (fp == NULL){
        printf("erro na leitura do usuarios.txt\n");
        return;
    }
    for (int i = 0; i < linha_usuario; i++)
        fscanf(fp, "%lld", &cpfs[i]);
    fclose(fp);


    FILE *fp2 = fopen("usuarios.txt", "w");
    if (fp2 == NULL){
        printf("erro na escrita do usuarios.txt\n");
        return;
    }

    for (int i = 0; i < quantidade_cpfs; i++) {
        if (i != linha_usuario - 1)
            fprintf(fp2, "%lld\n", cpfs[i]);
        // else 
        //     fprintf(fp, "0.0\n");
    }
    
    fclose(fp2);
    printf("Conta de investidor (CPF: %lld) excluida!\n", cpfs[linha_usuario - 1]);
}

void excluir_investidor(){
    printf("Insira o CPF de investidor para exclusao.\n");
    long long cpf_investidor;
    int linha_usuario;
    while(1){
        printf("CPF: ");
        scanf("%lld", &cpf_investidor);
        if (buscar_linha_investidor(cpf_investidor, &linha_usuario))
            break;
        else{
            printf("Insira um CPF valido.\n");
            continue;}
    }

    consultar_saldo_adm(linha_usuario);//printa saldo do investidor da exclusao

    int senha;
    printf("Para confirmar a exclusao, insira novamente a senha de administador.\n");
    while(1){
        printf("Senha: ");
        scanf("%d", &senha);
        conferir_senha_adm(senha);
        if (conferir_senha_adm(senha)){
            break;
        }else{
            printf("Senha incorreta. Insira sua senha novamente.\n");
            continue;
        }
    }
    remover_CPF_do_arquivo(linha_usuario);
    
    char nome[20];
    sprintf(nome, "user%d.txt", linha_usuario);
    remove(nome);
    char nome2[30];
    sprintf(nome2, "user%dextrato.txt", linha_usuario); // só exclui caso o arquivo tenha algo já escrito (pelo menos no vscode)
    remove(nome2);

}


void consultar_extrato_adm(int linha_usuario){
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

void printar_criptomoedas_adm(int quantidade_cripto, char nome[][20]){
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

void atualizar_criptomoedas_adm(float valor_base){
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

int main(void) {
    long cpf_login;
    int senha_login;
    printf("Boas vindas ao programa administrador da Exchange de Criptomoedas.\nPor favor, insira o CPF e senha de administrador.\n");

    while(1){
        printf("CPF: "); //cpf valido
        scanf("%ld", &cpf_login);

        if (conferirCPF_adm(cpf_login)) 
            break;                  
        else{
            printf("Insira um CPF valido.\n");
            continue;}
    }

    while(1){ //senha valida
        printf("Senha: ");
        scanf("%d", &senha_login);


        if (conferir_senha_adm(senha_login)){
            printf("Login efetuado com sucesso!\n");
            break;
        }else{
            printf("Senha incorreta. Insira sua senha novamente.\n");
            continue;
        }
    }

    menu_adm();
    int opcao;
    long long cpf_investidor;
    int linha_usuario;
    do{
        printf("\nSelecione a operacao desejada ou digite 8 para ver o menu novamente: ");
        scanf("%d", &opcao);

        switch(opcao){
        case 1:
            printf("\n1. Cadastro de novo investidor:\n");
            novo_investidor();
            break;
        case 2:
            printf("\n2. Excluir investidor:\n");
            excluir_investidor();
            break;
        case 3:
            printf("\n3. Cadastro de criptomoeda:\n");
            Cadastro_Cript();
            break;
        case 4:
            printf("\n4. Excluir criptomoeda:\n");
            excluir_criptomoeda();
            break;
        case 5:
            printf("\n5. Consultar saldo de um investidor:\n");
            while(1){
                printf("Digite o CPF de investidor: ");
                scanf("%lld", &cpf_investidor);
                if (buscar_linha_investidor(cpf_investidor, &linha_usuario))
                    break;
                else{
                    printf("Insira um CPF valido.\n");
                    continue;}
            }
            consultar_saldo_adm(linha_usuario);
            break;

        case 6:
            printf("\n6. Consultar extrato de um investidor:\n");
            while(1){
                printf("Digite o CPF de investidor: ");
                scanf("%lld", &cpf_investidor);
                if (buscar_linha_investidor(cpf_investidor, &linha_usuario))
                    break;
                else{
                    printf("Insira um CPF valido.\n");
                    continue;}
            }
            consultar_extrato_adm(linha_usuario);
            break;

        case 7:
            printf("\n7. Atualizar cotacoes:\n");
            int quantidade_criptomoedas;
            contar_criptomoedas_adm(&quantidade_criptomoedas);
            apagar_dados_arquivo("cotacao_criptomoedas.txt");

            char (*sigla_cripto)[4] = malloc(quantidade_criptomoedas * sizeof(*sigla_cripto));
            char (*nome_cripto)[20] = malloc(quantidade_criptomoedas * sizeof(*nome_cripto));
            float *cotacao = malloc(quantidade_criptomoedas * sizeof(*cotacao));
            float *taxavenda = malloc(quantidade_criptomoedas * sizeof(*taxavenda));
            float *taxacompra = malloc(quantidade_criptomoedas * sizeof(*taxacompra));

            ler_dados_criptomoedas_adm(sigla_cripto, nome_cripto, cotacao, taxavenda, taxacompra, quantidade_criptomoedas);

            for (int i = 0; i < quantidade_criptomoedas; i++)
                atualizar_criptomoedas_adm(cotacao[i]);

            printar_criptomoedas_adm(quantidade_criptomoedas, nome_cripto);
            free(sigla_cripto);
            free(nome_cripto);
            free(cotacao);
            free(taxavenda);
            free(taxacompra);
            break;

        case 8:
            printf("\n8. Menu:\n");
            menu_adm();
            break;

        case 0:
            printf("\nVoce saiu do sistema. Ate logo!\n");
            break;
        default:
            printf("\nOpcao invalida.\n");
            break;
        }
    } while (opcao != 0);
return 0;
}