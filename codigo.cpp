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

using namespace std;

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

int main(int argc, char *argv[])
{
	//Captura o tempo do sistema, com hora e data
    char dateStr [9];
    char timeStr [9];
    _strdate(dateStr);
    _strtime(timeStr);
    int minutoanterior = -1;


    //Verifica o minuto atual o tranforma em um inteiro para poder fazer comparações
    char minutoatual[3];
    memcpy(minutoatual, &timeStr[3], 2);
    minutoatual[2] = '\0'; // adiciona o terminador de linha
    int intminutoatual = std::stoi(minutoatual);


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
				printf("%lf\n", operacao[i].getChuva());
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
			char horaatual[3];
			memcpy(horaatual, &timeStr[0], 2);
			horaatual[2] = '\0'; // adiciona o terminador de linha
			int inthoraatual = std::stoi(horaatual);

			int dia_treinamento = 0, mes_treinamento = 0, ano_treinamento = 0, hora_treinamento = 0;

			//A hora no treinamento deve ser a hora anterior
			int hora_treinamento = inthoraatual - 1;

			//Verifica o dia atual
			char diaatual[3];
			memcpy(diaatual, &dateStr[3], 2);
			diaatual[2] = '\0'; // adiciona o terminador de linha
			int intdiaatual = std::stoi(diaatual);

			//Verifica o mes atual
			char mesatual[3];
			memcpy(mesatual, &dateStr[0], 2);
			diaatual[2] = '\0'; // adiciona o terminador de linha
			int intmesatual = std::stoi(mesatual);

			//Se a hora atual for meia noite, o dado é do dia anterior
			if(inthoraatual == 0){
				if(intdiaatual == 1){
					if(intmesatual == 2 || 4 || 6 || 8|| 9 || 11){
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
			char anoatual[3];
			memcpy(anoatual, &dateStr[6], 2);
			anoatual[2] = '\0'; // adiciona o terminador de linha
			int intanoatual = std::stoi(anoatual) + 2000;

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
		}
		//Fecha o arquivo de dados
		fclose(dados);
    }

	    //Se o minuto atual for maior que o minuto anterior, ou seja, se mudou de minuto, faz a leitura dos sensores, grava no arquivo dados e verifica se há probabilidade de chover
		if(intminutoatual > minutoanterior){
			vector<Condicao> condicoes;
			int K = 13;

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
			 *
			 * LEITURA DOS SENSORES
			 *
			 *
			 *
			 * */

			double temperatura, umidade, chove;

			/*
			 *
			 *
			 *	FIM DA LEITURA DOS SENSORES
			 *
			 *
			 */

			//Iremos adicionar as informações lidas pelos sensores no arquivo de dados
			FILE *dados;
			dados = fopen("dados.txt","a");

			//Escrevendo os dados dos sensores no arquivo de dados
			fprintf(dados, "%d	%.2lf	%.2lf	%.0lf\n", intminutoatual, temperatura, umidade, chove);

			//Fechando o arquivo dados que estava no modo de anexação
			fclose(dados);

			//Numero de linhas dos arquivos de dados
			//Iremos abrir o nosso arquivo de dados para realizar a leitura
			dados = fopen("dados.txt","r");

			while(!feof(dados)){
				int chuva;
				double dia, mes, ano, hora, tmed, umed;

				fscanf(dados, "%lf %lf %lf %lf %lf %lf %i", &dia, &mes, &ano, &hora, &tmed, &umed, &chuva);

				Condicao cond(dia, mes, ano, hora, tmed, umed, chuva);

				double chuva_resultado = classificarAmostra(condicoes ,cond, K);
			}
			//fechar o arquivo
			fclose(dados);


			/*if(chuva_resultado == 1){
				 *
				 *
				 * PROGRAMAÇÃO EM CASO DE CHUVA
				 *
				 *
				 *
			}else{




			}*/

			//minutoanterior recebe o valor de intminutoatual para que não entre novamente no int
			minutoanterior = intminutoatual;
	    }
	return 0;
}
