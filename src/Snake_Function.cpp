#include <windows.h>
#include <Winuser.h> //GDI相關的函式與資料結構宣告於Winuser.h

#include <stdlib.h>
#include <time.h>

#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

#include "resource.h" //引入資源定義檔


// NEW

#define RADIUS 7  //蛇頭和星星的碰撞容許距離
#define MOVE_DISTANCE 6 //一次移動的距離
#define SNAKE_SIZE 15//蛇的大小
#define STAR_SIZE 15 //星星大小
//#define BACKGROUND_PIC "B1.bmp"  //背景圖片
#define SNAKE_DISTANCE 10 //蛇身每一段相隔的距離

char backgroundPic[10] = "B1.bmp";

//  OLD

#define EQU ==
#define RIGHT 365
#define LEFT 10
#define UP 10
#define DOWN 205
#define WALKED 15000  //走超過15000格就會重頭放入array
#define TAIL_NUM 50 //尾巴超過50個就會當掉


//從主程式搬過來的

const  char* szAppName = "MyWndClass";
int	nTick = 0;

int nTickX = 1;
int nTickY = 1;

BOOL	bFlag =0;
char	szText[5] = "";
char	szTextKey[10] ="";	//用來顯示狀態的字元陣列
char	szTick[100] ;   //顯示時間
char	szCharKey[10]; 	//用來顯示按鍵的字元
char    szOut[10] = "出界!!";
char    szPoint[100];
char   szMenu[30] = "";	//字元陣列用--來顯示選單次項目

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int nTopXY(UINT, UINT);


//  NEW

int cpuTime = 60;  //控制蛇的速度

//目前的(X,Y)位置
int nowX = 160;
int nowY = 100;
char nowXYStr[20];

//上次移動的位移
int moveX = 0;
int moveY = 0;

//星星的位置
int starX = 0;
int starY = 0;

int nowKeyNumber = 0; //目前按下的鍵盤對應數值
int beforeKeyNumber = 0; //前一個方向

int nowXYPlace = 0;   //目前位置
int tailNumber = 0;   //尾巴數目

int walkedNumber = 0; //總共走了多少步

//                 上 下  左             右
int aspectX[4] = { 0 ,0  ,SNAKE_DISTANCE,-SNAKE_DISTANCE };
int aspectY[4] = { SNAKE_DISTANCE,-SNAKE_DISTANCE,0 ,0   };


//  OLD

int point = 0; //得分
int currentAspect = 0; //目前方向
int walkX[WALKED] = {0};  //走過的
int walkY[WALKED] = {0};


//WinAPI貪食蛇版本加入的
void randStar(); //讓星星被撞到後隨機跑
void keyClicked(); //上下左右移動
void walked(); //走過的路
void outLine(HWND ,HDC ,char ); //檢查是否出界　如果出界就結束
void PrintTail(HWND ,HDC ,char); //畫出蛇的尾巴
void starAndSnakeHand(HWND ,HDC ,HBRUSH); //畫出星星和蛇頭
void speed();  //隨分數調整速度
void reStart(); //重新開始的歸零動作


//DOS貪食蛇版本有的
int Move(int,int,int);  //移動位置
void Star(int*,int*,int,int,int*);  //畫出星星並讓其軌跡隨機
void PrintBorder(int );  //畫出外框
void Play();  //遊戲主要套件
void PrintSnake();  //畫出蛇的軌跡
void Start(); //開始處理
int End();  //結束處理
void PrintTail();  //畫出蛇的尾巴
void PrintAspect();  //標出目前的方向
void Restart(); //重新玩的歸零動作
int IsAgain();  //是否重新玩
int Walked(); //走過的路
int IsBumpTail(); //頭撞到尾巴
void Speed(); //依分數調節速度
void EndView(); //最後的等待畫面


//   以下為WinAPI貪食蛇版本加入的    	

void randStar()
//讓星星被撞到後隨機移動
{

    if(starX == 0 && starY == 0         ){//第一次
        srand(time(NULL));
        starX = rand()%330 + 20;
        starY = rand()%180 + 20;
    }
    else if( ((nowX-starX) <= RADIUS && (nowX-starX) >= -RADIUS) &&
             ((nowY-starY) <= RADIUS && (nowY-starY) >= -RADIUS)    ){ //蛇碰到星星
       srand(time(NULL));
       starX = rand()%200+rand()%160 + 15 ;
       starY = rand()%100+rand()%55 + 20;
       point += 10; //碰到星星所以加分
    }
}

void starAndSnakeHand(HWND hwnd,HDC hMyDC,HBRUSH hBrush)
//畫出星星和蛇頭
{
    hBrush = CreateSolidBrush(RGB(255,255,48));
        	SelectObject(hMyDC,hBrush);
        	Ellipse(hMyDC,starX,starY,starX+STAR_SIZE,starY+STAR_SIZE); //星星
        	
        	DeleteObject(hBrush);
        	
        	hBrush = CreateSolidBrush(RGB(0,255,0));
        	SelectObject(hMyDC,hBrush);
        	Ellipse(hMyDC,nowX,nowY,nowX+SNAKE_SIZE+3,nowY+SNAKE_SIZE+3); //蛇頭
        	
        	DeleteObject(hBrush);
}

void keyClicked()
//上下左右移動 並紀錄方向
{
    if(nowKeyNumber == 38 ){ //上
        nowY = nowY - MOVE_DISTANCE ;
        moveX = 0;
        moveY = -MOVE_DISTANCE;
        currentAspect = 0;
    }
    else if(nowKeyNumber == 40){ //下
        nowY = nowY + MOVE_DISTANCE ;
        moveX = 0;
        moveY = MOVE_DISTANCE;
        currentAspect = 1;
    }
    else if(nowKeyNumber == 37){ //左
        nowX = nowX - MOVE_DISTANCE ;
        moveX = -MOVE_DISTANCE;
        moveY = 0;
        currentAspect = 2;
    }
    else if(nowKeyNumber == 39){ //右
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
//紀錄走過的路
{
    nowXYPlace = (walkedNumber+WALKED-1) % WALKED;  //紀錄走過的路 (0-49一循環)
   	tailNumber = point / 10 ; //計算目前蛇的尾巴有多長
   	
    walkedNumber ++;
    int i = nowXYPlace;

    walkX[i] = nowX;
    walkY[i] = nowY;
}

void PrintTail(HWND hwnd,HDC hMyDC,char gameOverStr[10],UINT msg, WPARAM wParam, LPARAM lParam)
//畫出蛇的尾巴
{
    int distance = 1; //蛇身每一段的相隔距離
    int handDistance = 3;
    int first = 0; //解決頭和第一個身體靠太近的問題
    int id = 0; //判斷是與否
    
    while( tailNumber > 0){
        if(nowXYPlace > 10  ){   //走十步以上才開始計算
            if(distance == 1)
                first = -2;
            else
                first = 0;

            Ellipse(hMyDC,
                walkX[nowXYPlace-2*distance+first],
                walkY[nowXYPlace-2*distance+first],
                walkX[nowXYPlace-2*distance+first]+SNAKE_SIZE,
                walkY[nowXYPlace-2*distance+first]+SNAKE_SIZE ); //蛇尾

                //檢查是否撞到尾巴
            if(nowX == walkX[nowXYPlace-2*distance+first] &&
               nowY == walkY[nowXYPlace-2*distance+first]    ||
               nowX == walkX[nowXYPlace-2*distance-1] &&
               nowY == walkY[nowXYPlace-2*distance-1]       ){
                KillTimer(hwnd,1);  //讓時間計算停止

   	            id = MessageBox(hwnd,gameOverStr,"遊戲結束", MB_YESNO | MB_ICONINFORMATION);

                if(id == IDNO)
                    PostQuitMessage(0);   //關閉程式
                else{
                    reStart();  //把全域變數歸零
                    DefWindowProc(hwnd, msg, wParam, lParam); //重新開始
                }
            }
   	
            tailNumber--;
       	    nowXYPlace--;
       	    distance++;     //增加這個是為了把每一節之間的距離拉長
        }
    }
}

void reStart()
//歸零動作
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

    cpuTime = 60;  //控制蛇的速度

//目前的(X,Y)位置
    nowX = 160;
    nowY = 100;
    strcpy(nowXYStr,"");

//上次移動的位移
    moveX = 0;
    moveY = 0;

//星星的位置
    starX = 0;
    starY = 0;

    nowKeyNumber = 0; //目前按下的鍵盤對應數值
    beforeKeyNumber = 0; //前一個方向

    nowXYPlace = 0;   //目前位置
    tailNumber = 0;   //尾巴數目

    walkedNumber = 0; //總共走了多少步


//  OLD

    point = 0; //得分
    currentAspect = 0; //目前方向
    
    for(int i = 0 ; i < WALKED ; i++){
        walkX[i] = 0;  //走過的
        walkY[i] = 0;
    }
}


void outLine(HWND hwnd,HDC hMyDC,char gameOverStr[10],UINT msg, WPARAM wParam, LPARAM lParam)
//檢查是否出界　如果出界就結束
{
    LPOPENFILENAME lpofn ;
    
    int id = 0; //判斷按下是還是否
    
    HBRUSH hBrush; //變換方格顏色用
    
    hBrush = CreateSolidBrush(RGB(132,255,77));
    SelectObject(hMyDC,hBrush);
    
    if( nowX > RIGHT || nowX < LEFT || nowY > DOWN || nowY < UP){
        KillTimer(hwnd,1);  //讓時間計算停止
        id = MessageBox(hwnd,gameOverStr,"遊戲結束", MB_YESNO | MB_ICONINFORMATION);

        if(id == IDNO)
            PostQuitMessage(0);   //關閉程式
        else{
            GetOpenFileName(lpofn);
            DeleteObject(hBrush);

            reStart();  //把全域變數歸零
            DefWindowProc(hwnd, msg, wParam, lParam); //重新開始
        }
    }
}

void printOther(HWND hwnd,HDC hMyDC)
{
    int	nLen = sizeof(szText)-1;
    
    SetTextColor(hMyDC,RGB(230,95,230));
    TextOut(hMyDC,10,245,szTick,lstrlen(szTick)); //顯示目前秒數

    SetTextColor(hMyDC,RGB(110,95,230));
    wsprintf(szPoint,"分數 : %d",point);  //目前分數
    TextOut(hMyDC,170,245,szPoint,strlen(szPoint));//顯示目前分數

    SetTextColor(hMyDC,RGB(0,0,0));
    wsprintf(nowXYStr," ( %d , %d )",nowX,nowY);
    TextOut(hMyDC,80,245,nowXYStr,lstrlen(nowXYStr));  //顯示目前位置
    
    SetTextColor(hMyDC,RGB(45,95,115));
    TextOut(hMyDC,320,245,szCharKey,lstrlen(szCharKey));//顯示按下的字元
    
    SetTextColor(hMyDC,RGB(111,0,115));
    TextOut(hMyDC,260,245,szTextKey,lstrlen(szTextKey));//顯示有沒有按下   
}

void speed(HWND hwnd,HDC hMyDC,UINT msg, WPARAM wParam, LPARAM lParam)
//隨分數調整速度
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

    SetTimer(hwnd,1,cpuTime,NULL);   //重新設制速度
}

