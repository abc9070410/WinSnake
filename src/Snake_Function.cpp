#include <windows.h>
#include <Winuser.h> //GDI�������禡�P��Ƶ��c�ŧi��Winuser.h

#include <stdlib.h>
#include <time.h>

#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

#include "resource.h" //�ޤJ�귽�w�q��


// NEW

#define RADIUS 7  //�D�Y�M�P�P���I���e�\�Z��
#define MOVE_DISTANCE 6 //�@�����ʪ��Z��
#define SNAKE_SIZE 15//�D���j�p
#define STAR_SIZE 15 //�P�P�j�p
//#define BACKGROUND_PIC "B1.bmp"  //�I���Ϥ�
#define SNAKE_DISTANCE 10 //�D���C�@�q�۹j���Z��

char backgroundPic[10] = "B1.bmp";

//  OLD

#define EQU ==
#define RIGHT 365
#define LEFT 10
#define UP 10
#define DOWN 205
#define WALKED 15000  //���W�L15000��N�|���Y��Jarray
#define TAIL_NUM 50 //���ڶW�L50�ӴN�|��


//�q�D�{���h�L�Ӫ�

const  char* szAppName = "MyWndClass";
int	nTick = 0;

int nTickX = 1;
int nTickY = 1;

BOOL	bFlag =0;
char	szText[5] = "";
char	szTextKey[10] ="";	//�Ψ���ܪ��A���r���}�C
char	szTick[100] ;   //��ܮɶ�
char	szCharKey[10]; 	//�Ψ���ܫ��䪺�r��
char    szOut[10] = "�X��!!";
char    szPoint[100];
char   szMenu[30] = "";	//�r���}�C��--����ܿ�榸����

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int nTopXY(UINT, UINT);


//  NEW

int cpuTime = 60;  //����D���t��

//�ثe��(X,Y)��m
int nowX = 160;
int nowY = 100;
char nowXYStr[20];

//�W�����ʪ��첾
int moveX = 0;
int moveY = 0;

//�P�P����m
int starX = 0;
int starY = 0;

int nowKeyNumber = 0; //�ثe���U����L�����ƭ�
int beforeKeyNumber = 0; //�e�@�Ӥ�V

int nowXYPlace = 0;   //�ثe��m
int tailNumber = 0;   //���ڼƥ�

int walkedNumber = 0; //�`�@���F�h�֨B

//                 �W �U  ��             �k
int aspectX[4] = { 0 ,0  ,SNAKE_DISTANCE,-SNAKE_DISTANCE };
int aspectY[4] = { SNAKE_DISTANCE,-SNAKE_DISTANCE,0 ,0   };


//  OLD

int point = 0; //�o��
int currentAspect = 0; //�ثe��V
int walkX[WALKED] = {0};  //���L��
int walkY[WALKED] = {0};


//WinAPI�g���D�����[�J��
void randStar(); //���P�P�Q������H���]
void keyClicked(); //�W�U���k����
void walked(); //���L����
void outLine(HWND ,HDC ,char ); //�ˬd�O�_�X�ɡ@�p�G�X�ɴN����
void PrintTail(HWND ,HDC ,char); //�e�X�D������
void starAndSnakeHand(HWND ,HDC ,HBRUSH); //�e�X�P�P�M�D�Y
void speed();  //�H���ƽվ�t��
void reStart(); //���s�}�l���k�s�ʧ@


//DOS�g���D��������
int Move(int,int,int);  //���ʦ�m
void Star(int*,int*,int,int,int*);  //�e�X�P�P������y���H��
void PrintBorder(int );  //�e�X�~��
void Play();  //�C���D�n�M��
void PrintSnake();  //�e�X�D���y��
void Start(); //�}�l�B�z
int End();  //�����B�z
void PrintTail();  //�e�X�D������
void PrintAspect();  //�ХX�ثe����V
void Restart(); //���s�����k�s�ʧ@
int IsAgain();  //�O�_���s��
int Walked(); //���L����
int IsBumpTail(); //�Y�������
void Speed(); //�̤��ƽո`�t��
void EndView(); //�̫᪺���ݵe��


//   �H�U��WinAPI�g���D�����[�J��    	

void randStar()
//���P�P�Q������H������
{

    if(starX == 0 && starY == 0         ){//�Ĥ@��
        srand(time(NULL));
        starX = rand()%330 + 20;
        starY = rand()%180 + 20;
    }
    else if( ((nowX-starX) <= RADIUS && (nowX-starX) >= -RADIUS) &&
             ((nowY-starY) <= RADIUS && (nowY-starY) >= -RADIUS)    ){ //�D�I��P�P
       srand(time(NULL));
       starX = rand()%200+rand()%160 + 15 ;
       starY = rand()%100+rand()%55 + 20;
       point += 10; //�I��P�P�ҥH�[��
    }
}

void starAndSnakeHand(HWND hwnd,HDC hMyDC,HBRUSH hBrush)
//�e�X�P�P�M�D�Y
{
    hBrush = CreateSolidBrush(RGB(255,255,48));
        	SelectObject(hMyDC,hBrush);
        	Ellipse(hMyDC,starX,starY,starX+STAR_SIZE,starY+STAR_SIZE); //�P�P
        	
        	DeleteObject(hBrush);
        	
        	hBrush = CreateSolidBrush(RGB(0,255,0));
        	SelectObject(hMyDC,hBrush);
        	Ellipse(hMyDC,nowX,nowY,nowX+SNAKE_SIZE+3,nowY+SNAKE_SIZE+3); //�D�Y
        	
        	DeleteObject(hBrush);
}

void keyClicked()
//�W�U���k���� �ì�����V
{
    if(nowKeyNumber == 38 ){ //�W
        nowY = nowY - MOVE_DISTANCE ;
        moveX = 0;
        moveY = -MOVE_DISTANCE;
        currentAspect = 0;
    }
    else if(nowKeyNumber == 40){ //�U
        nowY = nowY + MOVE_DISTANCE ;
        moveX = 0;
        moveY = MOVE_DISTANCE;
        currentAspect = 1;
    }
    else if(nowKeyNumber == 37){ //��
        nowX = nowX - MOVE_DISTANCE ;
        moveX = -MOVE_DISTANCE;
        moveY = 0;
        currentAspect = 2;
    }
    else if(nowKeyNumber == 39){ //�k
        nowX = nowX + MOVE_DISTANCE ;
        moveX = MOVE_DISTANCE;
        moveY = 0;
        currentAspect = 3;
    }
    else{
        nowX = nowX + moveX;
        nowY = nowY + moveY;
    }
}

void walked()
//�������L����
{
    nowXYPlace = (walkedNumber+WALKED-1) % WALKED;  //�������L���� (0-49�@�`��)
   	tailNumber = point / 10 ; //�p��ثe�D�����ڦ��h��
   	
    walkedNumber ++;
    int i = nowXYPlace;

    walkX[i] = nowX;
    walkY[i] = nowY;
}

void PrintTail(HWND hwnd,HDC hMyDC,char gameOverStr[10],UINT msg, WPARAM wParam, LPARAM lParam)
//�e�X�D������
{
    int distance = 1; //�D���C�@�q���۹j�Z��
    int handDistance = 3;
    int first = 0; //�ѨM�Y�M�Ĥ@�Ө���a�Ӫ񪺰��D
    int id = 0; //�P�_�O�P�_
    
    while( tailNumber > 0){
        if(nowXYPlace > 10  ){   //���Q�B�H�W�~�}�l�p��
            if(distance == 1)
                first = -2;
            else
                first = 0;

            Ellipse(hMyDC,
                walkX[nowXYPlace-2*distance+first],
                walkY[nowXYPlace-2*distance+first],
                walkX[nowXYPlace-2*distance+first]+SNAKE_SIZE,
                walkY[nowXYPlace-2*distance+first]+SNAKE_SIZE ); //�D��

                //�ˬd�O�_�������
            if(nowX == walkX[nowXYPlace-2*distance+first] &&
               nowY == walkY[nowXYPlace-2*distance+first]    ||
               nowX == walkX[nowXYPlace-2*distance-1] &&
               nowY == walkY[nowXYPlace-2*distance-1]       ){
                KillTimer(hwnd,1);  //���ɶ��p�ⰱ��

   	            id = MessageBox(hwnd,gameOverStr,"�C������", MB_YESNO | MB_ICONINFORMATION);

                if(id == IDNO)
                    PostQuitMessage(0);   //�����{��
                else{
                    reStart();  //������ܼ��k�s
                    DefWindowProc(hwnd, msg, wParam, lParam); //���s�}�l
                }
            }
   	
            tailNumber--;
       	    nowXYPlace--;
       	    distance++;     //�W�[�o�ӬO���F��C�@�`�������Z���Ԫ�
        }
    }
}

void reStart()
//�k�s�ʧ@
{
    nTick = 0;

    nTickX = 1;
    nTickY = 1;

    bFlag =0;
    
    strcpy(szText,"");
    strcpy(szTextKey,"");
    strcpy(szTick,"");
    strcpy(szCharKey,"");
    strcpy(szOut,"");
    strcpy(szPoint,"");


//  NEW

    cpuTime = 60;  //����D���t��

//�ثe��(X,Y)��m
    nowX = 160;
    nowY = 100;
    strcpy(nowXYStr,"");

//�W�����ʪ��첾
    moveX = 0;
    moveY = 0;

//�P�P����m
    starX = 0;
    starY = 0;

    nowKeyNumber = 0; //�ثe���U����L�����ƭ�
    beforeKeyNumber = 0; //�e�@�Ӥ�V

    nowXYPlace = 0;   //�ثe��m
    tailNumber = 0;   //���ڼƥ�

    walkedNumber = 0; //�`�@���F�h�֨B


//  OLD

    point = 0; //�o��
    currentAspect = 0; //�ثe��V
    
    for(int i = 0 ; i < WALKED ; i++){
        walkX[i] = 0;  //���L��
        walkY[i] = 0;
    }
}


void outLine(HWND hwnd,HDC hMyDC,char gameOverStr[10],UINT msg, WPARAM wParam, LPARAM lParam)
//�ˬd�O�_�X�ɡ@�p�G�X�ɴN����
{
    LPOPENFILENAME lpofn ;
    
    int id = 0; //�P�_���U�O�٬O�_
    
    HBRUSH hBrush; //�ܴ�����C���
    
    hBrush = CreateSolidBrush(RGB(132,255,77));
    SelectObject(hMyDC,hBrush);
    
    if( nowX > RIGHT || nowX < LEFT || nowY > DOWN || nowY < UP){
        KillTimer(hwnd,1);  //���ɶ��p�ⰱ��
        id = MessageBox(hwnd,gameOverStr,"�C������", MB_YESNO | MB_ICONINFORMATION);

        if(id == IDNO)
            PostQuitMessage(0);   //�����{��
        else{
            GetOpenFileName(lpofn);
            DeleteObject(hBrush);

            reStart();  //������ܼ��k�s
            DefWindowProc(hwnd, msg, wParam, lParam); //���s�}�l
        }
    }
}

void printOther(HWND hwnd,HDC hMyDC)
{
    int	nLen = sizeof(szText)-1;
    
    SetTextColor(hMyDC,RGB(230,95,230));
    TextOut(hMyDC,10,245,szTick,lstrlen(szTick)); //��ܥثe���

    SetTextColor(hMyDC,RGB(110,95,230));
    wsprintf(szPoint,"���� : %d",point);  //�ثe����
    TextOut(hMyDC,170,245,szPoint,strlen(szPoint));//��ܥثe����

    SetTextColor(hMyDC,RGB(0,0,0));
    wsprintf(nowXYStr," ( %d , %d )",nowX,nowY);
    TextOut(hMyDC,80,245,nowXYStr,lstrlen(nowXYStr));  //��ܥثe��m
    
    SetTextColor(hMyDC,RGB(45,95,115));
    TextOut(hMyDC,320,245,szCharKey,lstrlen(szCharKey));//��ܫ��U���r��
    
    SetTextColor(hMyDC,RGB(111,0,115));
    TextOut(hMyDC,260,245,szTextKey,lstrlen(szTextKey));//��ܦ��S�����U   
}

void speed(HWND hwnd,HDC hMyDC,UINT msg, WPARAM wParam, LPARAM lParam)
//�H���ƽվ�t��
{
    if(point > 300)
        cpuTime = 20;
    else if(point > 200){
        cpuTime = 25 ;
        strcpy(backgroundPic,"B14.bmp");
    }
    else if(point > 140){
        cpuTime = 30 ;
        strcpy(backgroundPic,"B13.bmp");
    }
    else if(point > 90){
        cpuTime = 40;
        strcpy(backgroundPic,"B12.bmp");
    }
    else if(point > 40){
        cpuTime = 50;
        strcpy(backgroundPic,"B11.bmp");
        DefWindowProc(hwnd, msg, wParam, lParam);
    }

    SetTimer(hwnd,1,cpuTime,NULL);   //���s�]��t��
}

