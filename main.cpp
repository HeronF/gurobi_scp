#include <iostream>
#include "Malha.h"
#include "gurobi_c++.h"
#include <math.h>
#include <time.h>


//#include "bpm.h"

using namespace std;

// --------------------- Inicio do main -----------------------------------

int main(int argc, char *argv[]) {

//clock_t inicio,fim;

//inicio = clock();

try{ 

GRBEnv env= GRBEnv ();
GRBModel model = GRBModel (env); //inciaindo o pacote

//malha atualizada com dimensoes do p1 em 20/09/2019 //
Malha Ma;
    Ma.set_malha(30,30,0,144,0,152); // (nx, ny, xi, xf, yi, yf)
	Ma.formar_p1_malha();
    Ma.Adicionar_roteador(2,7);
    Ma.Adicionar_roteador(3,13);   //  (custo e raio)
	//Ma.Adicionar_roteador(5,17);
	

GRBVar *x  = new GRBVar[Ma.coord[0].size()*Ma.custo_roteador.size()]; //criação da variável

	
for (int i=0;i<Ma.coord[0].size()*Ma.custo_roteador.size();i++){
	//int j=floor(i/Ma.coord[0].size()); //code garbage
		x[i] = model.addVar( 0.0 , 1.0 , 0.0, GRB_BINARY,"x[]"); //iniciando cada uma das variáveis como binárias

}
 
 // restrições
	for(int i = 0; i< Ma.coord[0].size();i++)
		{
		GRBLinExpr expr_restricao=0;
		
		for(int j = 0; j<Ma.coord[0].size();j++)
			{
				for(int k=0;k<Ma.custo_roteador.size();k++){
				//	cout << i<<" "<<j<<" "<<k<<" "<< k*Ma.coord[0].size()+j <<" "<<endl<<flush;
					expr_restricao += Ma.funcao_alcance(i,j,k)*x[k*Ma.coord[0].size()+j];  //equaçãio de restrição
				}
			}
		model.addConstr(expr_restricao,GRB_GREATER_EQUAL,1); //ambas restrições maior igual que 1 e menor igual que dois
		model.addConstr(expr_restricao,GRB_LESS_EQUAL,2);
	}


 // funcao objetivo

      GRBLinExpr  expr_objetivo = 0;
        for (int i = 0 ; i < Ma.coord[0].size()*Ma.custo_roteador.size() ; i++ )
        {
			int j= floor(i/Ma.coord[0].size());
			expr_objetivo += x[i]*Ma.custo_roteador[j];
        }

      model.setObjective (expr_objetivo, GRB_MINIMIZE);	  //minimizar custo
model.set("heuristics","1");  // habilitando e desabilitando
model.set("presolve","1");
model.set("cuts","1");
 model.optimize (); //iniciando otimização

//gerar grafico e log

int solucao[Ma.coord[0].size()*Ma.custo_roteador.size()];
 for ( int i = 0 ; i < Ma.coord[0].size()*Ma.custo_roteador.size() ; i++ ){
			solucao[i]=x[i].get(GRB_DoubleAttr_X); //Tomando a solução
	}
	
//fim = clock();

//cout << "tempo gasto" << ((double)(fim-inicio)/(double)(CLOCKS_PER_SEC)) << endl;

Ma.gerar_saida(solucao); // exportando solução

}


//acaba try

  catch ( GRBException e ) {
	cout << "Código do erro = " << e.getErrorCode () << endl;
	cout << e.getMessage() << endl;
  }
  catch (...) {
	cout << "Erro durante a otimização" << endl;
  }



return 0;
}
