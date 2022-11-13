#include <cekeikon.h>

Mat_<COR> transformaCinza(Mat_<COR> Ent){
	Mat_<COR> Entcinza(Ent.size(),0);
	COR vermelho(10,10,160);
	for (int l = 0; l < Entcinza.rows; l++)
		for(int c = 0; c < Entcinza.cols; c++)
			if (distancia(vermelho, Ent(l,c)) < 100)
				Entcinza(l,c) = COR(255,255,255);
	return Entcinza;
}

Mat_<FLT> encontrarPlacaCC(Mat_<COR> Ent, Mat_<COR> Temp){
	Mat_<FLT> Tempcinza; converte(Temp, Tempcinza);
	Mat_<FLT> Ent2; converte(Ent, Ent2);
	Mat_<FLT> Tempcinza2 = somaAbsDois(dcReject(Tempcinza));
	Mat_<FLT> TempResized; 

	Mat_<FLT> maior(Ent.size(),0);
	Mat_<FLT> arg(Ent.size(),0);
	Mat_<FLT> Ci(Ent.rows,Ent.cols);	
	for(int i = 0; i < Ent.rows; i++){
		for(int j = 0; j < Ent.cols; j++){
			Ci(i,j)=0;
			maior(i,j)=0;
		}
	}
	
	int lins = Temp.rows*pow(1.05,5);
	int cols = Temp.cols*pow(1.05,5);
	for (int k = 0; k <=30; k++){
		int x = lins*pow(0.95,k); int y = cols*pow(0.95,k);
		resize(Tempcinza2, TempResized, Size(x,y));
		Ci = matchTemplateSame(Ent2, TempResized, CV_TM_CCORR);
		for(int i = 0; i < Ent.rows; i++){
			for(int j = 0; j < Ent.cols; j++){
				if (Ci(i,j) >= maior(i,j)){
					maior(i,j) = Ci(i,j);
					arg(i,j) = k;
				}
			}
		}
	}	
	return maior;
}

Mat_<FLT> encontrarPlacaNCC(Mat_<COR> Ent, Mat_<COR> Temp, Mat_<FLT> arg){
	Mat_<FLT> Tempcinza; converte(Temp, Tempcinza);
	Mat_<FLT> Ent2; converte(Ent, Ent2);
	Mat_<FLT> TempResized; 

	Mat_<FLT> maior(Ent.size(),0);
	Mat_<FLT> Ci(Ent.rows,Ent.cols);	
	for(int i = 0; i < Ent.rows; i++){
		for(int j = 0; j < Ent.cols; j++){
			Ci(i,j)=0;
			maior(i,j)=0;
		}
	}
	
	for (int k = 0; k <= 30; k++){
		int lins = Temp.rows*pow(1.05,5);
		int cols = Temp.cols*pow(1.05,5);
		int x = lins*pow(0.95,k); int y = cols*pow(0.95,k);
		resize(Tempcinza, TempResized, Size(x,y));
		Ci = matchTemplateSame(Ent2, TempResized, CV_TM_CCOEFF_NORMED);
		for(int i = 0; i < Ent.rows; i++){
			for(int j = 0; j < Ent.cols; j++){
				if (Ci(i,j) >= maior(i,j)){
					maior(i,j) = Ci(i,j);
					arg(i,j) = k;
				}
			}
		}
	}	
	return maior;
}

Mat_<COR> desenharPlaca(Mat_<COR> Ent, Mat_<FLT> Resultado, Mat_<COR> Temp, Mat_<FLT> arg){
	float maior = 0;
	int l,c;
	Mat_<COR> Saida; Saida = Ent.clone();
	for(int i = 0; i < Saida.rows; i++){
		for(int j = 0; j < Saida.cols; j++){
			if (Resultado(i,j) >= maior) {
				maior = Resultado(i,j);
				l=i;
				c=j;
			}

		}
	}
	
	for (int i2=-3; i2<=3; i2++)
		for (int j2=-3; j2<=3; j2++){
			if (0<=l+i2 && 0<=c+j2 && l+i2<Saida.rows && c+j2<Saida.cols) {
				Saida(l+i2,c+j2)[0]=0;
				Saida(l+i2,c+j2)[1]=255;
				Saida(l+i2,c+j2)[2]=0;
			}		
		}
	int lins = Temp.rows*pow(1.05,5);
	int cols = Temp.cols*pow(1.05,5);
	int x = lins*pow(0.95,arg(l,c)); int y = cols*pow(0.95,arg(l,c));
	for (int i = -x/2; i <= x/2; i++){
		for( int j = -y/2; j <= y/2; j++){
			if ( i<=(-x/2+3) || i >=(x/2 - 3) || j<=(-y/2+3) || j >=(y/2 -3)){
				Saida(l+i,c+j)[0]=0;
				Saida(l+i,c+j)[1]=255;
				Saida(l+i,c+j)[2]=0;		
			}			
		}
	}
	
	return Saida;
	
}

int main(int argc, char** argv){

if (argc!=3) erro(" Ep1: Detecta placa 'proibido virar' \n Sintaxe: ep1 ent.ext sai.ext \n Erro: Numero de argumentos invalido \n");
	Mat_<COR> Ent; le(Ent,argv[1]);
	Mat_<COR> Temp1; le(Temp1,"Temp1cinza1.jpg");
	Mat_<FLT> arg(Ent.size(),0);
	
	Mat_<COR> Entcinza = transformaCinza(Ent);

	Mat_<FLT> Resultado = encontrarPlacaNCC(Entcinza, Temp1, arg);
	
	Mat_<COR> Saida = desenharPlaca(Ent, Resultado, Temp1, arg);

	imp(Saida, argv[2]);

	return 0;
}