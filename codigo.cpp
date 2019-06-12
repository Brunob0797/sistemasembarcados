/*
 * knn.cpp
 *
 *  Created on: 26 de abr de 2019
 *      Author: Bruno Barcarolo
 */
// Implementação do k-NN (k-nearest neighbors algorithm)

#include <iostream>
#include <vector>
#include <math.h>
#include <set>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<unistd.h>
#include<signal.h>

//Bibliiotecas dos sensores
#include "bbb_dht_read.h"
#include "I2C.h"
#include "SSD1306_OLED.h"
#include "v2_0/BlackGPIO.h"
extern I2C_DeviceT I2C_DEV_2;




volatile unsigned char flag = 0;

using namespace std;
using namespace BlackLib;


class Condicao{
private:
	int chuva;
	double dia, mes, ano, hora, tmed, umed;

public:
	Condicao(double dia, double mes, double ano, double hora, double tmed, double umed, int chuva){
		this->dia = dia;
		this->mes = mes;
		this->ano = ano;
		this->hora = hora;
		this->tmed = tmed;
		this->umed = umed;
		this->chuva = chuva;
	}

	double getDia()
	{
		return dia;
	}
	double getMes()
	{
		return mes;
	}
	double getAno()
	{
		return ano;
	}
	double getHora()
	{
		return hora;
	}
	double getTmed()
	{
		return tmed;
	}
	double getUmed()
	{
		return umed;
	}
	double getChuva()
	{
		return chuva;
	}
};

class Operacoes{
private:
	double minuto, temperatura, umidade, chuva;

public:
	Operacoes(double minuto, double temperatura, double umidade, double chuva){
		this->minuto = minuto;
		this->temperatura = temperatura;
		this->umidade = umidade;
		this->chuva = chuva;
	}

	double getTemp()
	{
		return temperatura;
	}
	double getUmidade()
	{
		return umidade;
	}
	double getChuva()
	{
		return chuva;
	}
};

double obterDistEuclidiana(Condicao cond1, Condicao cond2)
{
	double soma = pow((cond1.getDia()-cond2.getDia()), 2) +
			      pow((cond1.getMes()-cond2.getMes()), 2) +
				  pow((cond1.getAno()-cond2.getAno()), 2) +
				  pow((cond1.getHora()-cond2.getHora()), 2)+
				  pow((cond1.getTmed()-cond2.getTmed()), 2)+
				  pow((cond1.getUmed()-cond2.getUmed()), 2);
	return sqrt(soma);
}

double classificarAmostra(vector<Condicao>& condicoes,
							Condicao novo_exemplo, int K)
{
	//se o K for par, decrementa
	if(K%2 == 0){
		K--;
		if(K<=0)
			K=1;
	}

	int tam_vet=condicoes.size();

	set<pair<double, int>> dist_condicoes;

	for(int i = 0; i < tam_vet; i++)
	{
		double dist = obterDistEuclidiana(condicoes[i], novo_exemplo);
		dist_condicoes.insert(make_pair(dist, i));
	}
	set<pair<double,int>>::iterator it;

	vector<int> cont_chuva(2);

	int contK = 0;

	for(it = dist_condicoes.begin(); it != dist_condicoes.end(); it++)
	{
		if(contK == K) break;

		int chuva = condicoes[it->second].getChuva();

		if(chuva == 0)
			cont_chuva[0]++;
		else if(chuva == 1)
			cont_chuva[1]++;
		contK++;
	}

	double chover;

	if(cont_chuva[0]>cont_chuva[1]){
		chover = 0;
	}
	else if(cont_chuva[1]>cont_chuva[0]){
		chover = 1;
	}

	return chover;
}

/* Para o funcionamento do OLED */
void ALARMhandler(int sig)
{
    /* Set flag */
    flag = 5;
}


/* Function Declarations */
void display_texts();

/* Bit Map - Taken from Adafruit SSD1306 OLED Library  */
static const unsigned char logo16_glcd_bmp[] =
{
 0b00000000, 0b11000000,
 0b00000001, 0b11000000,
 0b00000001, 0b11000000,
 0b00000011, 0b11100000,
 0b11110011, 0b11100000,
 0b11111110, 0b11111000,
 0b01111110, 0b11111111,
 0b00110011, 0b10011111,
 0b00011111, 0b11111100,
 0b00001101, 0b01110000,
 0b00011011, 0b10100000,
 0b00111111, 0b11100000,
 0b00111111, 0b11110000,
 0b01111100, 0b11110000,
 0b01110000, 0b01110000,
 0b00000000, 0b00110000
};

/* Display Texts */
void display_texts(char temp[256], char umi[256], char solo[256])
{
    setTextSize(1);
    setTextColor(WHITE);
    print_str(reinterpret_cast<const unsigned char *>("Temperatura: "));
    print_str(reinterpret_cast<const unsigned char *>(temp));
    print_str(reinterpret_cast<const unsigned char *>(" C"));
    println();
    println();
    print_str(reinterpret_cast<const unsigned char *>("Umidade: "));
    print_str(reinterpret_cast<const unsigned char *>(umi));
    print_str(reinterpret_cast<const unsigned char *>("%"));
    println();
    println();
    print_str(reinterpret_cast<const unsigned char *>("Solo: "));
    print_str(reinterpret_cast<const unsigned char *>(solo));
    print_str(reinterpret_cast<const unsigned char *>("%"));
}

int main(int argc, char *argv[])
{
	 //Iniciando a conexão I2C e conectando o OLED 
    if(init_i2c_dev2(SSD1306_OLED_ADDR) == 0)
    {
        printf("(Main)i2c-2: Bus Connected to SSD1306\r\n");
    }
    else
    {
        printf("(Main)i2c-2: OOPS! Something Went Wrong\r\n");
        exit(1);
    }
    signal(SIGALRM, ALARMhandler);

    // Roda a sequencia de inicialização do Display 
    display_Init_seq();
    
    //Seta o pino para o rele
    BlackLib::BlackGPIO	rele(BlackLib::GPIO_66,BlackLib::output, BlackLib::FastMode);
    rele.setValue(BlackLib::low);
    
    int minutoanterior = -1;
    double chuva_resultado = 1;
    
    double dia_dados, mes_dados, ano_dados, hora_dados, tmed_dados, umed_dados, chuva_dados;  
    
	while(1){
		
		char cptime[50]; //char ptr
		
		//Captura o tempo do sistema, com hora e data
		time_t T= time(NULL);
    	struct  tm tm = *localtime(&T);
    	

		//Verifica o minuto atual o tranforma em um inteiro para poder fazer comparações
		int intminutoatual = tm.tm_min;

		//Caso o minuto atual seja 0, é necessário verificar se o arquivo de dados está vazio ou preenchido
		if(intminutoatual == 0){
			//Abre o arquivo de dados para verificar se está preenchido
			FILE *dados;
			dados = fopen("dados.txt","r");

			//Cria as variáveis necessárias
			double tmed = 0, umed = 0, chuva = 0;

			//Verifica se o arquivo de texto está preenchido, se estiver vazio ele só passa pelo if
			fseek (dados, 0, SEEK_END);
			if(ftell (dados) != 0){
				//Se o arquivo estiver vazio, é necessário fazer as operações para encontrar a temperatura média, umidade média e se choveu ou não
				//Após isso gravaremos os dados no arquivo de treinamento
				//Cria um vetor para salvar os dados
				vector<Operacoes> operacao;

				//Volta o indicador de posição para o primeiro elemento
				fseek(dados,0, SEEK_SET);

				//Captura todos os dados do arquivo dados e adiciona a um vetor operações
				while(!feof(dados)){
					double minuto, temperatura, umidade, chuva;
					fscanf(dados, "%lf %lf %lf %lf", &minuto, &temperatura, &umidade, &chuva);
					operacao.push_back(Operacoes(minuto, temperatura, umidade, chuva));
				}

				//Verifica a quantidade de minutos que tem salvo
				int n = operacao.size();

				//Calcula a temperatura média
				for(int i=0; i<n; i++){
					tmed =tmed+operacao[i].getTemp();
				}
				tmed = tmed/n;

				//Calcula a umidade média
				for(int i=0; i<n; i++){
					umed =umed+operacao[i].getUmidade();
				}
				umed = umed/n;

				//Verifica se choveu no período, se sim, o valor final de chuva é 1
				for(int i=0; i<n; i++){
					chuva=chuva+operacao[i].getChuva();
				}
				if(chuva >= 1){
					chuva = 1;
				}
				//Iremos adicionar as informações calculadass no arquivo de treinamento
				FILE *treinamento;
				treinamento = fopen("treinamento.txt","a");

				//Definindo a hora, dia, mes e ano
				//Verifica a hora atual
				int inthoraatual = tm.tm_hour;
				hora_dados = inthoraatual;

				int dia_treinamento = 0, mes_treinamento = 0, ano_treinamento = 0, hora_treinamento = 0;

				//A hora no treinamento deve ser a hora anterior
				hora_treinamento = inthoraatual - 1;

				//Verifica o dia atual
				int intdiaatual = tm.tm_mday;
				dia_dados = intdiaatual;

				//Verifica o mes atual
				int intmesatual = tm.tm_mon+1;
				mes_dados = intmesatual;

				//Se a hora atual for meia noite, o dado é do dia anterior
				if(inthoraatual == 0){
					hora_treinamento = 23;
					if(intdiaatual == 1){
						if(intmesatual == 2 || intmesatual == 4 || intmesatual == 6 || intmesatual == 8|| intmesatual == 9 || intmesatual == 11){
							dia_treinamento = 31;
						}else if(intmesatual == 3){
							dia_treinamento = 28;
						}
						else{
							dia_treinamento = 30;
						}
					}
					else{
						dia_treinamento = intdiaatual - 1;
					}
				}else{
					dia_treinamento = intdiaatual;
				}

				if(intmesatual == 1 && intdiaatual == 1 && inthoraatual == 0){
					mes_treinamento = 12;
				}
				else{
					mes_treinamento = intmesatual - 1;
				}

				//Verifica o ano atual
				int intanoatual = tm.tm_year+1900;
				ano_dados = intanoatual;

				if(intmesatual == 1 && intdiaatual == 1 && inthoraatual == 0){
					ano_treinamento = intanoatual - 1;
				}else{
					ano_treinamento = intanoatual;
				}

				//Escrevendo os dados no arquivo de treinamento
				fprintf(treinamento, "%d	%d	%d	%d	%.2lf	%.2lf	%.0lf\n", dia_treinamento, mes_treinamento, ano_treinamento, hora_treinamento, tmed, umed, chuva);

				//Fechando o arquivo treinamento que estava no modo de anexação
				fclose(treinamento);

				//Reseta o minuto anterior
				minutoanterior = -1;

				//Apaga os dados do arquivo de dados
				freopen("dados.txt","w", dados);
			}

			//Fecha o arquivo de dados
			fclose(dados);

		}

		//Se o minuto atual for maior que o minuto anterior, ou seja, se mudou de minuto, faz a leitura dos sensores, grava no arquivo dados e verifica se há probabilidade de chover
		if(intminutoatual > minutoanterior){
			vector<Condicao> condicoes;
			int K = 13;
			
			//Desliga o rele
			rele.setValue(BlackLib::low);

			//Iremos abrir o nosso arquivo de treinamento e de dados
			FILE *treinamento;
			treinamento = fopen("treinamento.txt","r");
			

			//Lê os dados do arquivo treinamento e treina a IA
			while(!feof(treinamento)){
				int chuva;
				double dia, mes, ano, hora, tmed, umed;

				fscanf(treinamento, "%lf %lf %lf %lf %lf %lf %i", &dia, &mes, &ano, &hora, &tmed, &umed, &chuva);
				condicoes.push_back(Condicao(dia, mes, ano, hora, tmed, umed, chuva));
			}
			//fechar o arquivo
			fclose(treinamento);

			/*
			 * LEITURA DOS SENSORES
			*/

			float temperatura, umidade;
			double chove;
			
			// Lê o sensor de temperatura e umidade do ar. PINO P8_11
    		int sensor=22, base = 1, number= 13;
			int result = bbb_dht_read(sensor, base, number, &umidade, &temperatura);

			//Lê o sensor de umidade do solo
			char value_str[7];
		    long int value_int = 0;
		
		    FILE* f0 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw", "r");
		    fread(&value_str, 6, 6, f0);
		    value_int = strtol(value_str,NULL,0); //Valor retornado
		    fflush(stdout);
		    rewind(f0);
		    
		    //Convertendo os valores para string para mostrar no display OLED
		    char temp_string[10];
		    char umi_string[10];
		    char solo_string[10];
		    
		    sprintf(temp_string, "%.2f", temperatura);
		    sprintf(umi_string, "%.2f", umidade);
		    
			float porc_solo = (100 - (value_int-2100)/17);
		    sprintf(solo_string, "%.2f", porc_solo);

			/*
			 *	FIM DA LEITURA DOS SENSORES
			 */

			//Iremos adicionar as informações lidas pelos sensores no arquivo de dados
			FILE *dados;
			dados = fopen("dados.txt","a");

			//Escrevendo os dados dos sensores no arquivo de dados
			fprintf(dados, "%d	%.2f	%.2f	%.0lf\n", intminutoatual, temperatura, umidade, 0);

			//Fechando o arquivo dados que estava no modo de anexação
			fclose(dados);

			//Numero de linhas dos arquivos de dados
			//Iremos abrir o nosso arquivo de dados para realizar a leitura
			dados = fopen("dados.txt","r");

			while(!feof(dados)){
				int chuva, minuto;
				double tmed, umed;
				fscanf(dados, "%lf %lf %lf %i", &minuto, &tmed, &umed, &chuva);
				
				tmed_dados = tmed;
				umed_dados = umed;
				chuva_dados = chuva;
				
			}
			
			//Treina a IA com o ultimo dado
			Condicao cond(dia_dados, mes_dados, ano_dados, hora_dados, tmed_dados, umed_dados, chuva_dados);
			chuva_resultado = classificarAmostra(condicoes ,cond, K);
			
			
			//fechar o arquivo
			fclose(dados);

			//Define o minimo e o máximo lido pelo sensor de umidade do solo
			int minimo = 2100;
			int maximo = 3800;
			
			// Limpa o Display 
    		clearDisplay();
    		setCursor(0,0);
    		usleep(500000);
    
    		// Display Texts and Numbers
    		display_texts(temp_string, umi_string, solo_string);
    		Display();
			
			//Verifica se esta dentro do intervalo para ligar a irrigação
			if(porc_solo < 13.5){
				if(chuva_resultado == 0 || porc_solo < 8){
						//Liga o Rele para acionar as válvulas solenoides
    					rele.setValue(BlackLib::high);
				}else{}

			}else{}

			//minutoanterior recebe o valor de intminutoatual para que não entre novamente no int
			minutoanterior = intminutoatual;
		}
	}
	return 0;
}