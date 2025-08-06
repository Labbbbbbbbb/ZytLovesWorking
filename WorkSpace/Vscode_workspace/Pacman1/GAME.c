#include<stdio.h>	
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

#define H 27
#define L 28
char s;//难度标记
int so,lso;//积分 
int mx,my;//吃豆人坐标 
int lmx,lmy;//吃豆人上次坐标 
int ex1,ey1,ex2,ey2,ex3,ey3;//敌人坐标 
int flag;//撞击敌人标志
char r;//玩家输入选择
int mapr[H][L];//储存原始地图 
int replay=0;//重开的标志
//地图中0是墙，1是可走路线，2是玩家，3是敌人，4是豆子
int map[H][L]={	
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
	{0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
	{0,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,0},
	{0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
	{0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
	{0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
	{0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0},
	{0,4,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,4,0},
	{0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0},
	{0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0},
	{0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0},
	{0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

void HideCursor()     //ȥ���   
{
	CONSOLE_CURSOR_INFO cursor_info = {1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void gotoxy(int x, int y)   //�ƶ���꣬��Ҫ�������� 
{
    COORD pos = {x,y};
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void starup()//��Ϸ���ݳ�ʼ��
{
    HideCursor();
    int i,j;
	if(replay=1){
	for(i=0;i<H;i++){
    	for(j=0;j<L;j++){
    		map[i][j]=mapr[i][j];
	}
}
replay=0;
}
	so=0;
	lso=0;
	mx=1,my=1;
	map[mx][my]=2;
	flag=0;
	r=0; 
    ex1=12,ey1=12;
    map[ey1][ex1]=3;
    if(s=='2'){
    ex2=14,ey2=12;
    map[ey2][ex2]=3;
    ex3=13,ey3=12;
    map[ey3][ex3]=3;
	} 
} 

void show()//������� 
{
	int i,j;
    gotoxy(0,0) ; 
    
	 for(i=0;i<H;i++){
	 	for(j=0;j<H;j++){
	 			printf(" ");
			 }
	 	for(j=0;j<L;j++){
	 		if (map[i][j]==0)printf("#");
	 		else if(map[i][j]==1)printf(".");
	 		else if(map[i][j]==2)printf("@");
	 		else if(map[i][j]==3)printf("&");
	 		else if(map[i][j]==4)printf("$");
		 }
		 printf("\n");
		 
	 }
	 gotoxy(60,15);
	printf("�Ѿ��Ե�  %d ������\n",so);
			
}


void outup()//���������ݸ��� 
{
	//�����ƶ�����仯  X=1����2����3����4���� 
	int q;
	static int i=0;
	int lex1,ley1; 
	int lex2,ley2;
	int lex3,ley3;
	
if (i==4){
	q=rand()%4+1;
	if(q==1){
		if(map[ey1][ex1-1]==1){
		lex1=ex1;ley1=ey1;ex1=ex1-1;
		map[ley1][lex1]=1;
		map[ey1][ex1]=3;
		}
		
		else if(map[ey1][ex1-1]==2)
		{
			flag=1;
		}
	}
	else if(q==2){
		if(map[ey1][ex1+1]==1){
		lex1=ex1;ley1=ey1;ex1=ex1+1;
		map[ley1][lex1]=1;
		map[ey1][ex1]=3;
		}
		
		else if(map[ey1][ex1+1]==2)
		{
			flag=1;
		}
	}
	else if(q==3)
	{
		if(map[ey1+1][ex1]==1){
		lex1=ex1;ley1=ey1;ey1=ey1+1;
		map[ley1][lex1]=1;
		map[ey1][ex1]=3;	
		}
		else if(map[ey1+1][ex1]==2)
		{
			flag=1;
		}
	}
	else if(q==4)
	{
		if (map[ey1-1][ex1]==1){
		lex1=ex1;ley1=ey1;ey1=ey1-1;
		map[ley1][lex1]=1;
		map[ey1][ex1]=3;	
		}
		else if(map[ey1-1][ex1]==2)
		{
			flag=1;
		}
		
	}
	i=0;
	if (s=='2'){ //��Ϸ�Ѷ�Ϊ����ʱ ����ĵ���  
	q=rand()%4+1;
	if(q==1){
		if(map[ey2][ex2-1]==1){
		lex2=ex2;ley2=ey2;ex2=ex2-1;
		map[ley2][lex2]=1;
		map[ey2][ex2]=3;
		}
		
		else if(map[ey2][ex2-1]==2)
		{
			flag=1;
		}
	}
	else if(q==2){
		if(map[ey2][ex2+1]==1){
		lex2=ex2;ley2=ey2;ex2=ex2+1;
		map[ley2][lex2]=1;
		map[ey2][ex2]=3;
		}
		
		else if(map[ey2][ex2+1]==2)
		{
			flag=1;
		}
	}
	else if(q==3)
	{
		if(map[ey2+1][ex2]==1){
		lex2=ex2;ley2=ey2;ey2=ey2+1;
		map[ley2][lex2]=1;
		map[ey2][ex2]=3;	
		}
		else if(map[ey2+1][ex2]==2)
		{
			flag=1;
		}
	}
	else if(q==4)
	{
		if (map[ey2-1][ex2]==1){
		lex2=ex2;ley2=ey2;ey2=ey2-1;
		map[ley2][lex2]=1;
		map[ey2][ex2]=3;	
		}
		else if(map[ey2-1][ex2]==2)
		{
			flag=1;
		}
		
	}
	q=rand()%4+1;
	if(q==1){
		if(map[ey3][ex3-1]==1){
		lex3=ex3;ley3=ey3;ex3=ex3-1;
		map[ley3][lex3]=1;
		map[ey3][ex3]=3;
		}
		
		else if(map[ey3][ex3-1]==2)
		{
			flag=1;
		}
	}
	else if(q==2){
		if(map[ey3][ex3+1]==1){
		lex3=ex3;ley3=ey3;ex3=ex3+1;
		map[ley3][lex3]=1;
		map[ey3][ex3]=3;
		}
		
		else if(map[ey3][ex3+1]==2)
		{
			flag=1;
		}
	}
	else if(q==3)
	{
		if(map[ey3+1][ex3]==1){
		lex3=ex3;ley3=ey3;ey3=ey3+1;
		map[ley3][lex3]=1;
		map[ey3][ex3]=3;	
		}
		else if(map[ey3+1][ex3]==2)
		{
			flag=1;
		}
	}
	else if(q==4)
	{
		if (map[ey3-1][ex3]==1){
		lex3=ex3;ley3=ey3;ey3=ey3-1;
		map[ley3][lex3]=1;
		map[ey3][ex3]=3;	
		}
		else if(map[ey3-1][ex3]==2)
		{
			flag=1;
		}
	}
	}
}
else i++;
	//�����������
	int x,y;
	if (lso<so){
	 	do {
	 		x=rand()%H;
	 	    y=rand()%L;
		 }while (map[x][y]!=1);
		 map[x][y]=4;
		 lso++;
	 }
}
void inup()//�������ݸ��� 
{
	lmx=mx;lmy=my;
	char p;
	if(kbhit()) {
		p=getch();
		if(p=='w'||p=='W'){
			my-=1;
		}
		if(p=='s'||p=='S'){
			my+=1;
		}
		if(p=='a'||p=='A'){
			mx-=1;
		}
		if(p=='d'||p=='D'){
			mx+=1;
		}
	
}
//�Զ�������仯 
	if((map[my][mx]==1)||(map[my][mx]==4)){
		if(map[my][mx]==4)so++;
		map[my][mx]=2;map[lmy][lmx]=1;
	}
	else if (map[my][mx]==3) {
		flag=1; 
	}
	else if (map[my][mx]==0) {
		mx=lmx;my=lmy;
	}
	
}

void jm() {  //��ʼ���� 
	gotoxy(40,8);
    printf("��ӭ�����Զ�����Ϸ��");
    gotoxy(43,12);
    printf("��Ϸ��������");
    gotoxy(30,17);
	printf("�á�w����s����a����d�����ֱ���ƳԶ��˵��ƶ�");
	gotoxy(29,19);
	printf("�Զ��˳Ե�10��������Ϸͨ��  ��������ʱ����Ϸʧ��");
	gotoxy(40,25);
    system("pause");
    system("cls");
    gotoxy(40,10);
    printf("��ѡ����Ϸ�Ѷ�"); //�Ѷ�ѡ����� 
	gotoxy(38,13);
    printf("1��һ��      2������");

    do 	{
    	s=getch();
	}
    	while(s!='1'&&s!='2');

    gotoxy(40,25);
    system("pause");
    system("cls");
 
}

int main(){
	int i,j;
    for(i=0;i<H;i++){
    	for(j=0;j<L;j++){
    		mapr[i][j]=map[i][j];//����ԭʼ��ͼ�������ؿ�ʱ��ͼ��ԭ 
		}
	}
	h:
	jm();
	hh:
	starup();
	while(1){
		show();
		outup();
		inup();
		if(flag==1||so==10) break;
	}
		system("cls");
		gotoxy(25,15);
	if(flag==1)
	{
	printf("��Ϸʧ��!  �Ƿ�������Ϸ:     ȷ���밴1�����밴2\n");
	}
	else
	{
	printf("��Ϸͨ�أ� �Ƿ�������Ϸ:     ȷ���밴1����2��\n");
	}
	    do {
	    	r=getch(); 
		}while (r!='1'&&r!='2') ; 
	    if(r=='1') {
	    	system("cls");
		    replay=1;
		    goto hh ;
		}
		else {
			system("cls");
			goto h ;
		} 
	} 
