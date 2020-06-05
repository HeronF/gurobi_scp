#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Malha{

public:
	
vector<float> coord[2];
vector<float> Grid[2];

vector<float> custo_roteador;
vector<float> raio;

//vector<vector<vector<bool>>> conjunto_adjacencia;

int NX, NY;
float XI, XF, YI, YF;
vector<float> RXI, RXF, RYI , RYF;

void set_malha(int NX, int NY, float Xi, float Xf, float Yi, float Yf); //---cria uma região retangular entre x inicial - x final e y inicial - y final
//void printar_pontos_na_tela();  //------uma função que printa na tela os pontos de uma lista k
void retirar_retangulo(float Rxi,float Rxf,float Ryi,float Ryf); //-------retira um retangulo entre os pontos Rxi - Rxf, Ryi - Ryf
//void retirar_N_retangulos(int n);
//void atualizar_matrizes_de_adjacencia();//-----atualiza lista de pontos que distam do centro com raio R
void gerar_saida(int x[]);
bool fora_da_malha(float x,float y);
void formar_p1_malha(); //
void Adicionar_roteador(float custo,float raio);
int Funcao_de_apoio1(int x[],int j);   //porblema com variavél não estática sendo usada na hora de definir.
int funcao_alcance(int i, int j, int k);
};

int Malha::funcao_alcance(int i, int j, int k)
{
    if((coord[0][i]-coord[0][j])*(coord[0][i]-coord[0][j])+(coord[1][i]-coord[1][j])*(coord[1][i]-coord[1][j])<raio[k]*raio[k]) return(1);
    else return(0);

}

void Malha::set_malha(int Nx, int Ny, float Xi, float Xf, float Yi, float Yf){ //cria uma região retangular entre x inicial - x final e y inicial - y final

	XI = Xi;
	XF = Xf;
	YI = Yi;
	YF = Yf;

	NX=Nx;
	NY=Ny;
    float dx,dy,vx,vy;   //declaração de variavel delta
    int j,i;
     //variavel ponto que será usada no pushback
     //variavel de saida

			vx = (Xf - Xi);
			vy = (Yf - Yi);
			dx = vx/NX; //Delta x e Delta y.
			dy = vy/NY;

            for (i=0; i<=NX;i++){
                for (j=0; j<=NY; j++){
                    float atualx;
                    float atualy;  //declaração de variaveis que usaremos.

                    atualx=Xi+(i*dx);  //atual ponto X
                    atualy=Yi+(j*dy);  //atual ponto Y


					coord[0].push_back(atualx);
                    coord[1].push_back(atualy);
                    Grid[0].push_back(atualx);
                    Grid[1].push_back(atualy);
                }
            }
 return;
}

/*void Malha::printar_pontos_na_tela(){
	
           int i;

            cout << "\n"           << endl;
			cout << "   PONTOS   " << endl;
			cout << "[ X ] , " << "[ Y ]" << "\n" << endl;

            for(i=0; i<coord[1].size();i++){
                cout << "[" << coord[0][i] << "]" << ", [" << coord[1][i] << "]" << endl;

        //system("pause");
		//system("cls");
            }
return;	
}*/

void Malha::retirar_retangulo(float Rxi, float Rxf, float Ryi, float Ryf){
	
	RXI.push_back(Rxi);
	RXF.push_back(Rxf);
	RYI.push_back(Ryi);
	RYF.push_back(Ryf);
	
	vector<float> apoio[2];
	
	for(int i=0; i < coord[0].size(); i++){

		if((coord[0][i] < Rxi)||(coord[0][i] > Rxf)||(coord[1][i] < Ryi)||(coord[1][i] > Ryf))
		{			
			apoio[0].push_back(coord[0][i]);
			apoio[1].push_back(coord[1][i]);
		}
	}
	
	coord[0].swap(apoio[0]);
	coord[1].swap(apoio[1]);	
return;
}

/*void Malha::atualizar_matrizes_de_adjacencia(){

vector<bool> apoio;

bool bin;

for (int i=0;i<coord[0].size();i++){
	for (int j=0;j<coord[0].size();j++){
		if((coord[0][i]-coord[0][j])*(coord[0][i]-coord[0][j])+(coord[1][i]-coord[1][j])*(coord[1][i]-coord[1][j])<R*R){  //checa se a distância euclidiana é maior que R
			
			bin=1;
			apoio.push_back(bin);
			
		}
		else
		{
			bin=0;
			apoio.push_back(bin);
			
		}
    }
    lista.push_back(apoio);
	apoio.clear();
  //  cout << endl;
}
return;	
}*/

void Malha::gerar_saida(int x[]){
	int b=0;
	int j=0;
	
	ofstream imagem ("grid.ppm");
	ofstream texto ("log.txt");
	
	
	
	if(texto.is_open()){
		texto << NX+1 << "\n" << NY+1 << endl;
		texto <<  XI << "\n" << XF << "\n" << YI << "\n" << YF << endl;
		texto << RXI.size() << endl;
		for(int i=0;i<RXI.size();i++) texto << RXI[i] << "\n" << RXF[i] << "\n" << RYI[i] << "\n" << RYF[i] << endl;
		texto << custo_roteador.size() << endl;
		for(int k=0;k<custo_roteador.size();k++) texto << custo_roteador[k] << "\n" << raio[k] << endl;
		for(int k=0;k<custo_roteador.size();k++){
			for(int i =0;i<coord[0].size();i++){
				if(x[k*coord[0].size()+i]==1){
					texto << k << "\n" << coord[0][i] << "\n" << coord[1][i] << endl;
				}
			}
		}
		texto.close();
		}
	else cout << "não foi possivel abrir log.txt" << endl;
	
	if (imagem.is_open()){
		imagem << "P3\n" << NX+1 << " " << NY+1 << " 255"<< endl; 
		//cout << "até aqui chegou" << endl << flush;
		while(b<Grid[0].size()){
				if(0==fora_da_malha(Grid[0][b],Grid[1][b])){
					int n=Funcao_de_apoio1(x,j);
					if(n!=-1){
						if(n==0) imagem << "0 255 0"<< endl;
					    else if(n==1) imagem << "255 0 0"<<endl;
					    else imagem << "0 0 255"<<endl;
						b++;
						j++;
						}
					else{
						imagem << "255 255 255" << endl;
						b++;
						j++;
						}
				}
				else{
					imagem << "122 122 122" << endl;
					b++;
				}
				}
			imagem.close();
			}
	else cout << "arquivo 'grid' não pôde ser aberto" << endl;
	

return;		
}

/*void Malha::retirar_N_retangulos(int n){
	for(int j=0;j<n;j++){
		
		float Rxi,Rxf,Ryi,Ryf;
		
		cout << "entre com Rxi \n" << endl;
		cin >> Rxi;
		cout << "\n entre com Rxf \n" << endl;
		cin >> Rxf;
		cout << "\n entre com Ryi \n" << endl;
		cin >> Ryi;
		cout << "\n entre com Ryf \n" << endl;
		cin >> Ryf;
		
		retirar_retangulo(Rxi,Rxf,Ryi,Ryf);
	}
	return;
	}*/
	
void Malha::formar_p1_malha(){    //lembrando que as entradas da malha devem ser tamanhos x144 e y152
	retirar_retangulo(0,30,0,14); //entrar com cada um dos retângulos retirados aqui exemplo retirar_retangulo(a,b,c,d);.... e assim por diante. todo resto do código esta coeso com essa entrada.
	retirar_retangulo(0,14,14,23);
	retirar_retangulo(0,4,23,37);
	retirar_retangulo(0,14,37,67);
	retirar_retangulo(0,14,83,115);
	retirar_retangulo(0,4,115,129);
	retirar_retangulo(0,14,129,138);
	retirar_retangulo(0,30,138,152);
	retirar_retangulo(30,44,0,4);
	retirar_retangulo(44,59,0,14);
	retirar_retangulo(75,90,0,14);
	retirar_retangulo(90,104,0,4);
	retirar_retangulo(104,145,0,14);
	retirar_retangulo(30,44,148,152);
	retirar_retangulo(44,59,138,152);
	retirar_retangulo(75,90,138,152);
	retirar_retangulo(90,104,148,152);
	retirar_retangulo(104,145,138,152);
	retirar_retangulo(120,145,14,23);
	retirar_retangulo(120,145,129,138);
	retirar_retangulo(120,145,46,106);
	retirar_retangulo(26,108,26,126);
	}

bool Malha::fora_da_malha(float x,float y){ //retorno 1 indica que esta fora da malha, retorno de 0 indica que está dentro.
	
	int j = RXI.size();
	bool teste[j];
	
	for (int i=0;i<j;i++){
		if((x < RXI[i])||(x > RXF[i])||(y < RYI[i])||(y > RYF[i])) teste[i] = 1;
		else teste[i]=0;
		}
		
	for(int i=0;i<j;i++){
		if(teste[i]==1) continue;
		else return(1);
	}
	return(0);
	
	}

void Malha::Adicionar_roteador(float custo, float R){
	
	custo_roteador.push_back(custo);
	raio.push_back(R);
	
	/*vector<bool> vetor_adjacencia;
	vector<vector<bool>> matriz_adjacencia;

	bool bin;

for (int i=0;i<coord[0].size();i++){
	for (int j=0;j<coord[0].size();j++){
		if((coord[0][i]-coord[0][j])*(coord[0][i]-coord[0][j])+(coord[1][i]-coord[1][j])*(coord[1][i]-coord[1][j])<R*R){  //checa se a distância euclidiana é maior que R
			
			bin=1;
			vetor_adjacencia.push_back(bin);
			
		}
		else
		{
			bin=0;
			vetor_adjacencia.push_back(bin);
			
		}
    }
    matriz_adjacencia.push_back(vetor_adjacencia);
	vetor_adjacencia.clear();
	
}
conjunto_adjacencia.push_back(matriz_adjacencia);
matriz_adjacencia.clear();*/
return;	
}

int Malha::Funcao_de_apoio1(int x[],int j){ //retorno i indica que existe roteador na coordenada i, retorno -1 indica que não existe em nenhuma coordenada
	for(int i=0;i<custo_roteador.size();i++){
		if(x[i*coord[0].size()+j]==1) return(i);		
		}
	return(-1);		
	}
