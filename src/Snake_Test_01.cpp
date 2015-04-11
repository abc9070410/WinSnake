/*WinAPI版本的貪食蛇  :

　版本：v1.4

  遊戲流程 : 按方向鍵開始遊戲，按滑鼠左鍵可以暫停遊戲
  　　   　　碰到邊界或撞到尾巴都會導致遊戲結束，結束時顯示分數
            並可選擇是否結束遊戲
            如果未讀取到B1.bmp，則以灰色畫面進入遊戲 

  實作日期 : 2007.3.18
*/

#include "Snake_Function.cpp"

//程式進入點
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    char pic[10];
    
    
    //SetBkMode(hMyDC, );

    int wnd_W;
    int wnd_H;
    wnd_W=403;
    wnd_H=325;
        
    //亂入結束

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance ;
    //wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIcon = LoadIcon(hInstance,
               MAKEINTRESOURCE(ID_ICON_1));  //加入資源檔的系統大圖示
                
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    
    //以下建立背景圖片
    LOGBRUSH background;
    background.lbStyle     = BS_PATTERN;
    background.lbHatch     = (long) LoadImage(hInstance,//實體代碼
                    backgroundPic,//圖像名稱
                    IMAGE_BITMAP,//圖像種類
                        0, //期望的寬度
                    0,//期望的高度
                    LR_LOADFROMFILE);//讀取選項);
    //建立結束 
                    
    if(background.lbHatch != 0)
        wc.hbrBackground = CreateBrushIndirect(&background);
    else   //如果同資料夾中沒有名為B1.bmp的背景圖,則以白色背景取代 
        wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
                                        //如果改成NULL_BRUSH,背景就會變透明       

    wc.lpszMenuName  = MAKEINTRESOURCE(ID_MENU);  //顯示menu選單
    wc.lpszClassName = szAppName;
    wc.hIconSm = LoadIcon(hInstance,
                 MAKEINTRESOURCE(ID_ICON_1)); //加入資源檔的系統小圖示
    

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "視窗類別登記失敗!", "發生錯誤!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szAppName,
        "歡 迎 來 到 貪 食 蛇 世 界   ! !",
        WS_OVERLAPPEDWINDOW,
        nTopXY(wnd_W, GetSystemMetrics(SM_CXSCREEN)),
        nTopXY(wnd_H, GetSystemMetrics(SM_CYSCREEN)),
        wnd_W,wnd_H,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "視窗建立失敗!", "發生錯誤!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
 HDC	hMyDC;
 PAINTSTRUCT	ps;
 WNDCLASSEX wc;
 
 char	Text[] = "文字顯示測試!!";

 int	nLen = sizeof(szText)-1;
 
 int i,x,y;
 int SnakeHand = 5*nTick;  //蛇頭移動的設定變數
 
 char gameOverStr[100]; //遊戲結束的文字敘述
 
 HBRUSH hBrush; //變換方格顏色用
 
 nTickX = nTick;

    switch(msg)
    {
      case WM_CREATE:
            SetTimer(hwnd,1,cpuTime,NULL);

      break;
      case WM_TIMER:
            if (bFlag == 0 ){
            nTick ++;
            if(point == 0)
                nTick = 0;
            wsprintf(szTick,"時間:%d",nTick);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            
            }

       break;

       case WM_KEYDOWN: //keydown可以讀取到字鍵和系統鍵
             wsprintf(szTextKey,"按鍵:%s",TEXT("按"));
             wsprintf(szCharKey,"字鍵為:%s",&wParam);
             nowKeyNumber = wParam ; //把捕捉到的值記錄起來
             InvalidateRect(hwnd, NULL, TRUE);
             UpdateWindow(hwnd);
       break;
       /*
       case WM_CHAR:    //char可以讀取大小寫字鍵
             wsprintf(szCharKey,"字鍵為:%d",wParam);
             nowKeyNumber = wParam ; //把捕捉到的值給全域變數
             InvalidateRect(hwnd, NULL, TRUE);
             UpdateWindow(hwnd);
       break;
       */
       case WM_KEYUP:
             wsprintf(szTextKey,"按鍵:%s",TEXT("開"));
             InvalidateRect(hwnd, NULL, TRUE);
             UpdateWindow(hwnd);

       break;
       //end
       case WM_LBUTTONDOWN:
            if (bFlag == 0){
               bFlag = 1;
               wsprintf(szText,"%s",TEXT("按滑鼠左鍵繼續"));
               InvalidateRect(hwnd, NULL, TRUE);
               
               //亂入測試音樂開始

               /*

               MCI_OPEN_PARMS OpenParms;
OpenParms.lpstrDeviceType =
(LPCSTR) MCI_DEVTYPE_SEQUENCER; //是MIDI?型文件
OpenParms.lpstrElementName = (LPCSTR) "wholenew.mid"; //文件名
OpenParms.wDeviceID = 0; //打?的??的??，后面需要使用
mciSendCommand (NULL, MCI_OPEN,
MCI_WAIT | MCI_OPEN_TYPE |
MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT,
(DWORD)(LPVOID) &OpenParms); //打???
        //接?就可以播放MIDI了：
MCI_PLAY_PARMS PlayParms;
PlayParms.dwFrom = 0; //?什么??位置播放，?位?毫秒
mciSendCommand (OpenParms, MCI_PLAY, //DeviceID需等于上面的????
MCI_FROM, (DWORD)(LPVOID)&PlayParms); //播放MIDI

                 */
                 
                 //第二種測試

                 //PlaySound("glass1.wav",
                 //NULL, SND_FILENAME | SND_ASYNC);
                 
                 //sndPlaySound("glass1.wav",SND_ASYNC);
               
               
               //亂入測試音樂結束
               
               
            } else {
               bFlag = 0;
               wsprintf(szText,"%s",TEXT("按滑鼠左鍵暫停"));
               InvalidateRect(hwnd, NULL, TRUE);
            }
       break;
       case WM_PAINT:
            wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
       
            //撥放音樂....還未學會
            //PlaySound("1.wav",NULL,SND_FILENAME | SND_ASYNC);

        	hMyDC=BeginPaint(hwnd, &ps);
        	
        	
        	SetBkMode(hMyDC, TRANSPARENT); //設定為背景通透
        	
        	/*
        	//因為用雙圖層會有問題,所以改用畫線的方式
        	//放入圖片開始
        	
        	LOGBRUSH rectangleBackground;

            rectangleBackground.lbStyle     = BS_PATTERN;
            rectangleBackground.lbHatch     = (long) LoadImage(NULL,//實體代碼
                    BACKGROUND_PIC,//圖像名稱
                    IMAGE_BITMAP,//圖像種類
                        0, //期望的寬度
                    0,//期望的高度
                    LR_LOADFROMFILE);//讀取選項

            //把rectangleBackground放入hBrush
            hBrush = CreateBrushIndirect(&rectangleBackground);
            
            SelectObject(hMyDC,hBrush);  //選擇這個塗色方式
        	
        	Rectangle(hMyDC,10,10,380,220);  //畫出邊界四方形
        	
        	//放入圖片結束
        	*/
        	
        	//畫出邊界
        	MoveToEx(hMyDC,10,10,NULL);
        	LineTo(hMyDC,380,10);
        	MoveToEx(hMyDC,380,10,NULL);
        	LineTo(hMyDC,380,220);
        	MoveToEx(hMyDC,380,220,NULL);
        	LineTo(hMyDC,10,220);
        	MoveToEx(hMyDC,10,220,NULL);
        	LineTo(hMyDC,10,10);
        	
        	speed(hwnd,hMyDC, msg, wParam, lParam); //控制速度
        	
        	randStar(); //看看有沒有碰到星星        	

            starAndSnakeHand(hwnd,hMyDC,hBrush);  //畫出星星和蛇頭

            walked(); //計算走幾步,過去走過的路

  	        hBrush = CreateSolidBrush(RGB(132,255,77));
        	SelectObject(hMyDC,hBrush);
        	
        	wsprintf(gameOverStr," 頭尾相撞 !! \n\n 總分：%d  \n\n 繼續遊戲 ? ",point);

            PrintTail(hwnd,hMyDC,gameOverStr, msg, wParam, lParam);  //畫出尾巴　並判斷有沒有頭尾相撞
        	
        	keyClicked();   //上下左右移動
            
            wsprintf(gameOverStr," 出界 !! \n\n 總分：%d  \n\n 繼續遊戲 ? ",point);
            outLine(hwnd,hMyDC,gameOverStr, msg, wParam, lParam);  //檢查是否出界　如果出界就結束
            
            printOther(hwnd,hMyDC);  //畫出其他有的沒的資料

            SetTextColor(hMyDC,RGB(255,111,33));
            TextOut(hMyDC,140,100,szText,lstrlen(szText));//顯示暫停與否(按滑鼠左鍵)
            
            strcpy(szText,"");  //這樣才不會殘留在畫面上
            
            DeleteObject(hBrush);
            
            EndPaint(hwnd, &ps);
            
            
        	 //亂入結束
        	 
        break;
        case WM_COMMAND:
             //解析使用者選取哪個功能表項目
             switch(LOWORD(wParam)){
               case IDM_GAME_ABOUT:
                 MessageBox(NULL,
                            "這是一個\n    小巧簡單的\n        貪食蛇遊戲",
                            "說明視窗" ,
                             MB_OK);
                 InvalidateRect(hwnd, NULL, TRUE);
               break;
               case IDM_DESIGNER_ABOUT:
                 MessageBox(NULL, "查無此人 !!","說明視窗" ,
                            MB_ICONINFORMATION | MB_OK);
                 InvalidateRect(hwnd, NULL, TRUE);
               break;
               case IDM_CLOSE:
                 SendMessage(hwnd,WM_CLOSE,0,0);
               break;

                case WM_CLOSE:
                  KillTimer(hwnd,1);  //讓時間計算停止
                int id = 0;
                 id = MessageBox(hwnd,gameOverStr,"遊戲結束", MB_YESNO | MB_ICONINFORMATION);

                 if(id == IDNO)
                    PostQuitMessage(0);   //關閉程式
                else
                    SetTimer(hwnd,1,cpuTime,NULL);
                DestroyWindow(hwnd);
                break;
        }
        break;
        case WM_DESTROY:

            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
        return 0;
}

int nTopXY(UINT wnd_XY, UINT wnd_Dim)
{
int resl;
resl=((wnd_Dim/2)-(wnd_XY/2));
return resl;
}

//-----------------end---------------------



