    #include<stdio.h>

/*Função que calcula e imprime o troco em moedas*/
void calculaTroco(int cent)
{
    /* Array com os valores das moedas em cêntimos*/
    int valores[]= {200, 100, 50, 20, 10, 5, 2, 1};

    /*Array com as descrições das moedas*/
    char *descricoes[]=
    {
        "2 euros",
        "1 euro",
        "50 centimos",
        "20 centimos",
        "10 centimos",
        "5 centimos",
        "2 centimos",
        "1 centimo",
    };

    int i, qtd;
    int num_moedas=8; /*Total do tipo de moedas*/

    /*Percorre cada tipo de moeda*/
    for(i=0; i< num_moedas; i++){
        qtd = cent / valores[i];

        /*Apenas imprime se houver uma do tipo entre as 8*/
        if(qtd>0)
        {
            printf("%s: %d\n", descricoes[i], qtd);
            cent = cent % valores [i]; // atualiza resto
        }
    }
}

int main()
{
    double montante;
    int cent;

    //Lê o montante em euros
    scanf("%lf", &montante);

    //Converte para cêntimos (multiplicando por 100)
    cent = (int)(montante*100 + 0.5); /*0.5 para arredondamento*/

    //Calcula e Imprime o troco
    calculaTroco(cent);
    return 0;
}