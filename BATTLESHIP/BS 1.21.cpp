#include<iostream>
#include<fstream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<iomanip>
#include<math.h>
#include<string>
#define N 8
#define M 8

#include "getSystem.cpp"

using namespace std;

int cur_x,cur_y;
int pos_r,pos_c;	//"位置"
int numOfBombs;             // 数量 
int playcount;
char key0;

void UI();
void showBombs();
void MapGun(); 
void HideCursor(); 
void ShowCursor();
void init();
void judge();
void ASCII_pic(FILE *fp);
void loadinganime();
void color(const unsigned short _color);
void gotoxy(int x , int y){
	COORD c;
	c.X = x -1;
	c.Y = y -1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

class Ship{
public:
	Ship(int a,int b):length(a),width(b){hp = length*width;state = 0;}

	int getlength(){return length;}
	int getwidth(){return width;}
	
	void setStartX(int xx){sx=xx;}
	void setStartY(int yy){sy=yy;}
	int getStartX(){return sx;}
	int getStartY(){return sy;}
	
	void setEndX(int xx){ex=xx;}
	void setEndY(int yy){ey=yy;}
	int getEndX(){return ex;}
	int getEndY(){return ey;}
	int hp,state;                  //  state: 0 safe   1 damaged    2 destroyed
private:    
	int length,width;
	int sx,sy;
	int ex,ey;
};
Ship SS(2,1),DD(3,1),CL(4,1),BB(4,2);

class Map{
public:
	int map[N][M] ;	//N 行数   ;   M 列数   -4 surivalship;-3 无（损）；-2 沉；-1损；0无；2 SS；3 DD；4 CL；5 BB 
	int	score;
	int highestscore;
	string topplayer,curplayer;
public:
	void init();
	void show();
	void show10();
	void survivalShip();
	void setscore(int s){score = s;}
	int getscore(){return score;}
	void calcscore();
	void showscore();
	void sethighestscore(int s){highestscore = s;}
	int gethighestscore(){return highestscore;}
	
	void settopplayer(string name){topplayer=name;}
	string gettopplayer(){return topplayer;}
	void setcurplayer(string name){curplayer=name;}
	string getcurplayer(){return curplayer;}
	
};
void Map::init(){
	score = 0;
	for(int i = 0 ; i< N ; i++)
		for(int j = 0 ; j < M ; j++)
		{
			map[i][j] = 0;
		}
}

void Map::show(){
	printf ("\t┏");
	for(int i = 0 ; i< M-1;i++){
		printf ("━━┳");
	}
	printf ("━━┓\n");
	
	for(int j = 0; j < N-1;j++){
		
		printf ("\t┃");
		for(int i = 0 ; i < M-1; i++){
			printf ("    ┃");
		}
		printf("    ┃\n");
		
		printf("\t┣");
		for(int i = 0 ; i < M-1; i++){
			printf ("━━╋");
		}
		printf("━━┫\n");
	}
	
	printf ("\t┃");
		for(int i = 0 ; i < M-1 ; i++){
			printf ("    ┃");
		}
		printf("    ┃\n");
	
	printf("\t┗");
	for(int i = 0 ; i < M-1 ; i++){
		printf ("━━┻");
	}
	printf("━━┛\n");
	
}

void Map::show10(){
	printf ("\t┏");
	for(int i = 0 ; i< M-1;i++){
		printf ("━━━━━┳");
	}
	printf ("━━━━━┓\n");
	
	for(int j = 0; j < N-1;j++){
		
		printf ("\t┃");
		for(int i = 0 ; i < M-1; i++){
			printf ("     ┃");
		}
		printf("     ┃\n");
		
		printf("\t┣");
		for(int i = 0 ; i < M-1; i++){
			printf ("━━━━━╋");
		}
		printf("━━━━━┫\n");
	}
	
	printf ("\t┃");
		for(int i = 0 ; i < M-1 ; i++){
			printf ("     ┃");
		}
		printf("     ┃\n");
	
	printf("\t┗");
	for(int i = 0 ; i < M-1 ; i++){
		printf ("━━━━━┻");
	}
	printf("━━━━━┛\n");
}

void Map::survivalShip(){
	color(12);		//light red
	for(int i = 0 ; i< N ; i++)
		for(int j = 0 ; j < M ; j++)
		{
			if(	map[i][j]>0 ){
				gotoxy(j*6+12,i*2+2);
				map[i][j] = -4;
				cout<<"■";
			}
		}
	color(15);		//light white
	for(int i = 0 ; i< N ; i++)
		for(int j = 0 ; j < M ; j++)
		{
			if(	map[i][j]==-1){
				gotoxy(j*6+12,i*2+2);
				map[i][j] = -4;
				cout<<"■";
			}
		}
}

void Map::calcscore(){
	int numOfBombed=0,				//	score  =  numOfBombed * 2  +  numOfDestroyed *5 
		numOfDestroyed=0;			
	for(int i = 0 ; i< N ; i++)
	{
		for(int j = 0 ; j < M ; j++)
		{
			if(	map[i][j]==-1||map[i][j]==-2 ){
				numOfBombed++;
			}
		}
	}
	numOfDestroyed += SS.state/2 + DD.state/2 + CL.state/2 + BB.state/2;	//state among 0,1,2
	
	score = numOfBombed*2 + numOfDestroyed*5;
}

void Map::showscore(){

	gotoxy(M*6+14,4);
	cout<<"highest score: ";
	color(11);
	cout<<highestscore;
	color(15);
	gotoxy(M*6+33,4);
	cout<<" by: ";
	color(11);
	cout<<topplayer;
	color(15);
	gotoxy(M*6+14,5);
	cout<<"current score: ";
	color(10);
	cout<<score;
	color(15);
	gotoxy(M*6+33,5);
	cout<<" by: ";
	color(10);
	cout<<curplayer;
	color(15);
}
Map mymap;

void boom(int x,int y){
	if(mymap.map[x][y]>0){
		
		if(mymap.map[x][y]==2){         //		SS
			if(SS.state!=2){
				SS.hp--;
				if(SS.hp==0){
					SS.state = 2;
						
					for(int i=SS.getStartX();i<=SS.getEndX();i++){
						for(int j=SS.getStartY();j<=SS.getEndY();j++){
							mymap.map[i][j]=-2;
						}
					}
					for(int i=SS.getStartX()*2+2;i<=SS.getEndX()*2+2;i+=2){
						for(int j=SS.getStartY()*6+12;j<=SS.getEndY()*6+12;j+=6){
							gotoxy(j,i); 
							cout<<"※";
						}
					}
									
				}
				else{
					SS.state = 1;
					mymap.map[x][y]=-1; 
				}			
			}	
   		}else if(mymap.map[x][y]==3){   //		DD
			if(DD.state!=2){
				DD.hp--;
				if(DD.hp==0){
					DD.state = 2;
							
					for(int i=DD.getStartX();i<=DD.getEndX();i++){
						for(int j=DD.getStartY();j<=DD.getEndY();j++){
							mymap.map[i][j]=-2;
						}
					}
					for(int i=DD.getStartX()*2+2;i<=DD.getEndX()*2+2;i+=2){
						for(int j=DD.getStartY()*6+12;j<=DD.getEndY()*6+12;j+=6){
							gotoxy(j,i); 
							cout<<"※";
						}
					}
				}
				else{
					DD.state = 1;
					mymap.map[x][y]=-1; 
				}			
			}	
		}else if(mymap.map[x][y]==4){   //		CL
			if(CL.state!=2){
				CL.hp--;
				if(CL.hp==0){
					CL.state = 2;
					
					for(int i=CL.getStartX();i<=CL.getEndX();i++){
						for(int j=CL.getStartY();j<=CL.getEndY();j++){
							mymap.map[i][j]=-2;
						}
					}
					for(int i=CL.getStartX()*2+2;i<=CL.getEndX()*2+2;i+=2){
						for(int j=CL.getStartY()*6+12;j<=CL.getEndY()*6+12;j+=6){
							gotoxy(j,i); 
							cout<<"※";
						}
					}
					
				}
				else{
					CL.state = 1;
					mymap.map[x][y]=-1; 
				}			
			}		
		}else{                          //		BB
			if(BB.state!=2){
				BB.hp--;
				if(BB.hp==0){
					BB.state = 2;
					
					for(int i=BB.getStartX();i<=BB.getEndX();i++){
						for(int j=BB.getStartY();j<=BB.getEndY();j++){
							mymap.map[i][j]=-2;
						}
					}
					for(int i=BB.getStartX()*2+2;i<=BB.getEndX()*2+2;i+=2){
						for(int j=BB.getStartY()*6+12;j<=BB.getEndY()*6+12;j+=6){
							gotoxy(j,i); 
							cout<<"※";
						}
					}
		
				}
				else{
					BB.state = 1;
					mymap.map[x][y]=-1;
				}			
			}	
		}
		if(mymap.map[x][y]!=-2){
			mymap.map[x][y]=-1;
		}
	}
	else if(mymap.map[x][y]==0){
		mymap.map[x][y]=-3; 
	}
/*
	gotoxy(1,N*3+3);
	cout<<"SS.state:"<<SS.state<<endl;
	cout<<"DD.state:"<<DD.state<<endl;
	cout<<"CL.state:"<<CL.state<<endl;
	cout<<"BB.state:"<<BB.state<<endl;
*/
}


class Key{
private:
	char key1,key,key2;
public:
	void run();	
};
Key mykey;
void Key::run(){
	int mapguncounter = 0;
	int mapflag = 0;
	gotoxy(M*6+14,7);
	cout<<"X: ";
	color(13);
	cout<<pos_c+1;
	color(15);
	cout<<"    Y: ";
	color(13);
	cout<<pos_r+1;
	color(15);
	while(1){
		key = getch();

		if(key != -32)
		{
			gotoxy(cur_x , cur_y);
			if(mymap.map[pos_r][pos_c]==-1){
				cout<<"■";
			}
			else if(mymap.map[pos_r][pos_c]==-2){
				cout<<"※";
			}
			else if(mymap.map[pos_r][pos_c]==-3){
				cout<<"×";
			}
			else if(mymap.map[pos_r][pos_c]==-4){
				color(12);
				cout<<"■";
				color(15); 
			}
			else{
				cout<<"  ";			
			}				
		}
		
		if(key1 == -32 && key == 72 && cur_y >= 4) // UP
		{
			cur_y -= 2;
			pos_r --;
		}	
		else if(key1 == -32 && key == 80 && cur_y <= N*2 -2) //DOWN 
		{
			cur_y += 2;
			pos_r ++;
		}
		else if(key1 == -32 && key == 75 && cur_x >= 18) //LEFT
		{
			cur_x -= 6;
			pos_c --;
		}
		else if(key1 == -32 && key == 77 && cur_x <= M*6 ) //RIGHT
		{
			cur_x += 6;
			pos_c ++;	
		} 
		else if(key == ' '){
			boom(pos_r,pos_c);
			cout<<'\a';
			judge();
		}
		else if(key == 'b' || key == 'B'){
			mapguncounter++;
			if(mapguncounter==5){
				MapGun(); 
			}	
		}
		else if(key == 27){
			gotoxy(1,N*3+3);
			exit(0);
		}
		else if(key=='m' || key=='M' && key1!=-32 && key0!=-32){	// show map
			if(mapflag==0){
				gotoxy(1,N*2+2);
				for(int i=0;i<N;i++){
					for(int j=0;j<M-1;j++)
					{
						printf("%-2d ",mymap.map[i][j]);
					}
					printf("%d\n",mymap.map[i][M-1]);
				}   
				mapflag = 1;	
			}
			else{
				gotoxy(1,N*2+2);
				for(int i=0;i<N;i++){
					for(int j=0;j<M-1;j++)
					{
						printf("   ",mymap.map[i][j]);
					}
					printf("  \n",mymap.map[i][M-1]);
				}   
				mapflag = 0;
			}
			
		}
		else if(key2=='a' && key1=='d' && key=='d'){
			numOfBombs += 10;
			showBombs();
		}
		if(key != -32)
		{
			gotoxy(cur_x , cur_y);
			cout<<"¤";
			gotoxy(M*6+14,7);
			cout<<"X: ";
			color(13);
			cout<<pos_c+1;
			color(15);
			cout<<"    Y: ";
			color(13);
			cout<<pos_r+1;
			color(15);
		}	
		
		key2 = key1;
		key1 = key;				
//----------------------------------------------test  
	}
}



void initOfSS(int t){ // 0 heng  ;  1 shu
	int px,py;        //N hangshu   ;   M lieshu
	int flag=1,count;
	
	if(t==0){// heng
	
		while(flag!=2){
			py = rand()%(M-SS.getlength()+1);  // 0~9  length=3  --> 0~7
			px = rand()%(N-SS.getwidth()+1);
			count = 0;
			for(int i=py;i<py+SS.getlength();i++){
				if(mymap.map[px][i]!=0){
					count++;
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				SS.setStartX(px);SS.setStartY(py);
				SS.setEndX(px);SS.setEndY(py+SS.getlength()-1);
				for(int i=py;i<py+SS.getlength();i++){
					mymap.map[px][i]=2;
				}
				flag = 2;
			}		
		}		
	}else{//shu
		while(flag!=2){
			py = rand()%(M-SS.getwidth()+1); //N hangshu   ;   M lieshu
			px = rand()%(N-SS.getlength()+1);  //  map[N][M]  0~9  length=3  --> 0~7
			                  
			count = 0;
			for(int i=px;i<px+SS.getlength();i++){
				if(mymap.map[i][py]!=0){
					count++;
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				SS.setStartX(px);SS.setStartY(py);
				SS.setEndX(px+SS.getlength()-1);SS.setEndY(py);
				for(int i=px;i<px+SS.getlength();i++){
					mymap.map[i][py]=2;
				}
				flag = 2;
			}		
		}		
	}	
}
void initOfDD(int t){ // 0 heng  ;  1 shu
	int px,py;        //N hangshu   ;   M lieshu
	int flag=1,count;
	
	if(t==0){// heng
	
		while(flag!=2){
			py = rand()%(M-DD.getlength()+1);  // 0~9  length=3  --> 0~7
			px = rand()%(N-DD.getwidth()+1);
			count = 0;
			for(int i=py;i<py+DD.getlength();i++){
				if(mymap.map[px][i]!=0){
					count++;
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				DD.setStartX(px);DD.setStartY(py);
				DD.setEndX(px);DD.setEndY(py+DD.getlength()-1);
				for(int i=py;i<py+DD.getlength();i++){
					mymap.map[px][i]=3;
				}
				flag = 2;
			}		
		}		
	}else{//shu
		while(flag!=2){
			py = rand()%(M-DD.getwidth()+1); //N hangshu   ;   M lieshu
			px = rand()%(N-DD.getlength()+1);  //  map[N][M]  0~9  length=3  --> 0~7
			                  
			count = 0;
			for(int i=px;i<px+DD.getlength();i++){
				if(mymap.map[i][py]!=0){
					count++;
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				DD.setStartX(px);DD.setStartY(py);
				DD.setEndX(px+DD.getlength()-1);DD.setEndY(py);
				for(int i=px;i<px+DD.getlength();i++){
					mymap.map[i][py]=3;
				}
				flag = 2;
			}		
		}		
	}	
}
void initOfCL(int t){ // 0 heng  ;  1 shu
	int px,py;        //N hangshu   ;   M lieshu
	int flag=1,count;
	
	if(t==0){// heng
	
		while(flag!=2){
			py = rand()%(M-CL.getlength()+1);  // 0~9  length=3  --> 0~7
			px = rand()%(N-CL.getwidth()+1);
			count = 0;
			for(int i=py;i<py+CL.getlength();i++){
				if(mymap.map[px][i]!=0){
					count++;
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				CL.setStartX(px);CL.setStartY(py);
				CL.setEndX(px);CL.setEndY(py+CL.getlength()-1);
				for(int i=py;i<py+CL.getlength();i++){
					mymap.map[px][i]=4;
				}
				flag = 2;
			}		
		}		
	}else{//shu
		while(flag!=2){
			py = rand()%(M-CL.getwidth()+1); //N hangshu   ;   M lieshu
			px = rand()%(N-CL.getlength()+1);  //  map[N][M]  0~9  length=3  --> 0~7
			                  
			count = 0;
			for(int i=px;i<px+CL.getlength();i++){
				if(mymap.map[i][py]!=0){
					count++;
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				CL.setStartX(px);CL.setStartY(py);
				CL.setEndX(px+CL.getlength()-1);CL.setEndY(py);
				for(int i=px;i<px+CL.getlength();i++){
					mymap.map[i][py]=4;
				}
				flag = 2;
			}		
		}		
	}	
}
void initOfBB(int t){ // 0 heng  ;  1 shu
	int px,py;        //N hangshu   ;   M lieshu
	int flag=1,count;
	
	if(t==0){// heng
	
		while(flag!=2){
			py = rand()%(M-BB.getlength()+1);  // 0~9  length=3  --> 0~7
			px = rand()%(N-BB.getwidth()+1);    //--------------------------------
			count = 0;
			for(int i=py;i<py+BB.getlength();i++){
				for(int j=px;j<px+BB.getwidth();j++){
					if(mymap.map[j][i]!=0){
						count++;
					}	
				}
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				BB.setStartX(px);BB.setStartY(py);
				BB.setEndX(px+BB.getwidth()-1);BB.setEndY(py+BB.getlength()-1);
				for(int i=py;i<py+BB.getlength();i++){
					for(int j=px;j<px+BB.getwidth();j++){
						mymap.map[j][i]=5;
					}
				}
				flag = 2;
			}		
		}		
	}else{//shu
		while(flag!=2){
			py = rand()%(M-BB.getwidth()+1); //N hangshu   ;   M lieshu
			px = rand()%(N-BB.getlength()+1);  //  map[N][M]  0~9  length=3  --> 0~7
			                  
			count = 0;
			for(int i=px;i<px+BB.getlength();i++){
				for(int j=py;j<py+BB.getwidth();j++){
					if(mymap.map[i][j]!=0){
						count++;
					}
				}	
			}
			if(count == 0){
				flag = 1;
			}else{
				flag = 0;
			}
			if(flag == 1){   //safe
				BB.setStartX(px);BB.setStartY(py);
				BB.setEndX(px+BB.getlength()-1);BB.setEndY(py+BB.getwidth()-1);
				for(int i=px;i<px+BB.getlength();i++){
					for(int j=py;j<py+BB.getwidth();j++){
						mymap.map[i][j]=5;
					}	
				}
				flag = 2;
			}		
		}		
	}	
}

void init(){
	cur_x =12;
	cur_y = 2;
	pos_r = 0 ;
	pos_c = 0; 
	SS.hp = SS.getlength()*SS.getwidth();
	DD.hp = DD.getlength()*DD.getwidth();
	CL.hp = CL.getlength()*CL.getwidth();
	BB.hp = BB.getlength()*BB.getwidth();
	
	SS.state = DD.state = CL.state = BB.state = 0;
	numOfBombs = ((float)SS.hp+(float)DD.hp+(float)CL.hp+(float)BB.hp)/(N*M)*105.45+sqrt(N*M)*2.15;  				 //设置bombs数量	 
	//                      17           0.265625  
	mymap.init();
	// 如果系统是win10 则打印win10适配的地图 
	if( getSystemName() == 10) {
		mymap.show10();	
	} else {
		mymap.show();
	}

    gotoxy(cur_x , cur_y);
	cout<<"¤"<<endl;
	
	showBombs();
	gotoxy(M*6+30,7);
	cout<<"PlayCount: ";
	color(13);
	cout<<playcount;
	color(15);
	
	srand(time(NULL));
	initOfBB( rand()%2 );
	initOfCL( rand()%2 );
	initOfDD( rand()%2 );
	initOfSS( rand()%2 );
	
	int score;
	char name[20];	
	FILE *fp=fopen("score.poi","r");
	fscanf(fp,"%s %d",name,&score);
	if( fp==NULL ){
		fp = fopen("score.poi","wt+");
		strcpy(name,"none");
		score = 0;
	}
//	fp = fopen("score.poi","rwt+");
	mymap.settopplayer(name);
	mymap.sethighestscore(score);
	fprintf(fp,"%s %d",(char*)mymap.gettopplayer().data(),mymap.getscore());
	fclose(fp);		
	mymap.showscore();	
}

int main(){

	system("mode con cols=100 lines=40");		//change the size of console 
	
	HideCursor();
	color(15);	
	UI();
	
	return 0;
}

void judge(){
	numOfBombs--;
	int totalscore;
	FILE *fp=NULL; 
	showBombs();
	mymap.calcscore();
	mymap.showscore();
	
	float total,k=0.95;											// weight score
	total = (float)mymap.getscore();
	float j=1;
	for(float i=numOfBombs;i>0;i--){
		total += i*j;
		j *= k; 
	}
	
	totalscore = (int)(total+0.5);
	
	if(SS.hp==0&&DD.hp==0&&CL.hp==0&&BB.hp==0&&numOfBombs>=0)
	{	
		if( mymap.gethighestscore()<totalscore ){
			fp=fopen("score.poi","rwt+");
			fseek(fp,0,SEEK_SET);
			for(int i=0;i<=30;i++){
				fprintf(fp," ");
			}
			fseek(fp,0,SEEK_SET);
			fprintf(fp,"%s %d",(char*)mymap.getcurplayer().data(),totalscore);
			fclose(fp);
		}
		gotoxy(M*6,N*2+2);
		cout<<"SUCCEED!";	
		cout<<"    left bombs: "<<setfill('0')<<setw(2)<<numOfBombs<<"(s)"; 
		gotoxy(M*6,N*2+4);
		cout<<"total score: ";
		color(10);
		cout<<mymap.getscore();
		color(13);
		cout<<"+";
		color(10);
		cout<<totalscore-mymap.getscore();
		color(3);
		cout<<"(bonus)";
		color(13);
		cout<<"=";
		color(10);
		cout<<totalscore;
		color(15);
		if( mymap.gethighestscore()<totalscore ){
			gotoxy(M*6,N*2+6);
			Sleep(500);
			color(14);
			cout<<"YOU ";
			Sleep(200);
			cout<<"SET ";
			Sleep(200);
			cout<<"A ";
			Sleep(200);
			cout<<"NEW ";
			Sleep(200);
			cout<<"RECORD! ";
			Sleep(400);
			cout<<"CONGRATULATION!";
			Sleep(500);
			color(15);
		}
		gotoxy(M*6,N*2+5);
		cout<<"\n\n...按 ";
		color(3);
		cout<<"ENTER";
		color(15);
		cout<<" 退出或 ";
		color(3);
		cout<<"R";
		color(15);
		cout<<" 键重玩";
		while( (key0=getch())!=13 )
		{
			if(key0=='r' || key0=='R')
			{
				playcount++;
				system("CLS");
				init(); 
				mykey.run();
			}
		}	
		fp = fopen("out\\ASCII-spinner-clear.poi", "r");
		color(14); 
		ASCII_pic(fp);		//---------------SUCCEED
		color(15);
		system("PAUSE");
		exit(0);
	}else if( (SS.hp!=0||DD.hp!=0||CL.hp!=0||BB.hp!=0)&&numOfBombs<=0){
		if( mymap.gethighestscore()<totalscore ){
			fp=fopen("score.poi","rwt+");
			fseek(fp,0,SEEK_SET);
			for(int i=0;i<=30;i++){
				fprintf(fp," ");
			}
			fseek(fp,0,SEEK_SET);
			fprintf(fp,"%s %d",(char*)mymap.getcurplayer().data(),totalscore);
			fclose(fp);
		}	
		mymap.survivalShip();	
		numOfBombs = 0;		
		gotoxy(M*6,N*2+2);
		cout<<"FAILED!";
		gotoxy(M*6,N*2+4);
		cout<<"total score: ";
		color(10);
		cout<<mymap.getscore();
		color(13);
		cout<<"+";
		color(10);
		cout<<totalscore-mymap.getscore();
		color(3);	
		cout<<"(bonus)";
		color(13);	
		cout<<"=";
		color(10);
		cout<<totalscore;
		color(15);
		if( mymap.gethighestscore()<totalscore ){
			gotoxy(M*6,N*2+6);
			Sleep(500);
			color(14);
			cout<<"YOU ";
			Sleep(200);
			cout<<"SET ";
			Sleep(200);
			cout<<"A ";
			Sleep(200);
			cout<<"NEW ";
			Sleep(200);
			cout<<"RECORD! ";
			Sleep(400);
			cout<<"CONGRATULATION!";
			Sleep(500);
			color(15);
		}
		gotoxy(M*6,N*2+5);
		cout<<"\n\n...按 ";
		color(3);
		cout<<"ENTER";
		color(15);
		cout<<" 退出或 ";
		color(3);
		cout<<"R";
		color(15);
		cout<<" 键重玩";
		while( (key0=getch())!=13 )
		{
			if(key0=='r' || key0=='R')
			{
				playcount++;
				system("CLS");
				init(); 
				mykey.run();
			};
		}
		fp = fopen("out\\ASCII-fail.poi", "r");
		color(8);
		ASCII_pic(fp);
		color(15);
		system("PAUSE");
		exit(0);
	}
}

void UI(){
	// loadinganime();
	playcount = 1;
	char ch;
	string name;	
	ifstream inf("out\\Title1.poi");
	stringbuf sb;
	inf>>&sb;
	cout<<sb.str();
	inf.close();
	sb.str("");
	while( (ch=getch())!=13 );	
	system("CLS");
	
	inf.open("out\\Title2.poi");
	inf>>&sb;
	cout<<sb.str();
	inf.close();
	sb.str("");
	while( (ch=getch())!=13 );	
	system("CLS");
	
	inf.open("out\\Account.poi");
	inf>>&sb;
	cout<<sb.str();	
	inf.close();
	sb.str("");
	ShowCursor();
	gotoxy(11,3);
	char str[20];
	int j = 0;
	while( (str[j]=getch())!=13 || str[0] == 13){
		if(str[j]==8&&j>0){
			cout<<"\b \b";
			j--;
		}
		else if(str[j] == 27){
			gotoxy(1,10);
			cout<<"              ";
			exit(0);
		}
		else if(str[j] == -32){
			getch();
		}
		else if(str[j]!=13 && str[j]!=8 && str[j]!=32 && str[j]!=-32){
			color(10);
			cout << str[j];
			color(15);
			j++;	
		}
		if(str[0] == 13){
			j = 0;
		}
	}
	name = str;
	mymap.setcurplayer(name);
	HideCursor();	
	system("CLS");
	
	inf.open("out\\Title3.poi");
	inf>>&sb;
	cout<<sb.str();	
	inf.close();
	sb.str("");
	
	key0 = getch();
	system("CLS");

//---------------------------------------
	int i = 6;	
	FILE *fp=NULL;
	char buf[581], seat[]="out\\ASCII-_0.poi", ai[2];	
	system("cls");
	gotoxy(1,1);
	while(i >= 1)
	{	
//		cout<<i-1;
		strcpy(seat, "out\\ASCII-_");
		sprintf(ai, "%d", i);
		strcat(seat, ai);
		strcat(seat, ".poi");
		i--;
		fp = fopen(seat, "r");
		fread(buf, sizeof(buf), 1, fp);
		buf[581] = '\0';
		fclose(fp);
		fprintf(stdout, "%s", buf);
		cout<<'\a';
		
		Sleep(750);
		system("cls");
		gotoxy(1,1);
	}
//-----------------------------------------------
	system("CLS");
	
	init();
	
	mykey.run();
}

void showBombs(){	
	gotoxy(M*6+14,2);
    cout<<"num of bombs:"<<setfill('0')<<setw(2)<<numOfBombs;
}
void MapGun(){
	SS.hp = 0;
	SS.state = 2;
	for(int i=SS.getStartX();i<=SS.getEndX();i++){
		for(int j=SS.getStartY();j<=SS.getEndY();j++){
			mymap.map[i][j]=-2;
		}
	}
	for(int i=SS.getStartX()*2+2;i<=SS.getEndX()*2+2;i+=2){
		for(int j=SS.getStartY()*6+12;j<=SS.getEndY()*6+12;j+=6){
			gotoxy(j,i); 
			cout<<"※";
		}
	}
	
	DD.hp = 0;
	DD.state = 2;
	for(int i=DD.getStartX();i<=DD.getEndX();i++){
		for(int j=DD.getStartY();j<=DD.getEndY();j++){
			mymap.map[i][j]=-2;
		}
	}
	for(int i=DD.getStartX()*2+2;i<=DD.getEndX()*2+2;i+=2){
		for(int j=DD.getStartY()*6+12;j<=DD.getEndY()*6+12;j+=6){
			gotoxy(j,i); 
			cout<<"※";
		}
	}
	
	CL.hp = 0;
	CL.state = 2;
	for(int i=CL.getStartX();i<=CL.getEndX();i++){
		for(int j=CL.getStartY();j<=CL.getEndY();j++){
			mymap.map[i][j]=-2;
		}
	}
	for(int i=CL.getStartX()*2+2;i<=CL.getEndX()*2+2;i+=2){
		for(int j=CL.getStartY()*6+12;j<=CL.getEndY()*6+12;j+=6){
			gotoxy(j,i); 
			cout<<"※";
		}
	}
	
	BB.hp = 0;
	BB.state = 2;
	for(int i=BB.getStartX();i<=BB.getEndX();i++){
		for(int j=BB.getStartY();j<=BB.getEndY();j++){
			mymap.map[i][j]=-2;
		}
	}
	for(int i=BB.getStartX()*2+2;i<=BB.getEndX()*2+2;i+=2){
		for(int j=BB.getStartY()*6+12;j<=BB.getEndY()*6+12;j+=6){
			gotoxy(j,i); 
			cout<<"※";
		}
	}
	numOfBombs++;
	judge();
}

void ASCII_pic(FILE *fp){		
	char buf[1920];	
	system("cls");
	gotoxy(1,1);
	fread(buf,sizeof(buf),1,fp);
	buf[1920] = '\0';
	fclose(fp);
	fprintf(stdout, "%s", buf);
	gotoxy(1,21);
}

void loadinganime(){
	int i = 19;	
	FILE *fp=NULL;
	char buf[2851], seat[]="loading\\ASCII-_00.txt", ai[3];	
	system("cls");
	gotoxy(1,1);
	char ch;
	while(i <= 89)
	{		
		strcpy(seat,"loading\\ASCII-_");
		sprintf(ai,"%d",i);
		strcat(seat,ai);
		strcat(seat,".txt");
		i++;
		fp = fopen(seat,"r");
		fread(buf,sizeof(buf),1,fp);
		buf[2851] = '\0';
		fclose(fp);
		fprintf(stdout,"%s",buf);
		if(i==47){
			Sleep(1880);	
		}
		Sleep(20);
		gotoxy(1,1);	
	}
	system("CLS");
	fp = fopen("loading\\ASCII-ship.txt","r");
	fread(buf,sizeof(buf),1,fp);
	buf[1366] = '\0';
	fclose(fp);
	fprintf(stdout,"%s",buf);
	Sleep(2000);
	system("CLS");
	gotoxy(1,1);
}

void color(const unsigned short _color)
{
	if( 0<=_color && _color<=15)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),_color);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	}

/*
0 黑色       8 灰色       others 白色 
1 蓝色       9 淡蓝色      (16)
2 绿色      10 淡绿色 
3 湖蓝色    11 浅绿色 
4 红色      12 淡红色 
5 紫色      13 淡紫色 
6 黄色      14 淡黄色 
7 白色      15 亮白色 
*/
}

void HideCursor()								//隐藏光标 
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void ShowCursor()								//显示光标 
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 1}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

