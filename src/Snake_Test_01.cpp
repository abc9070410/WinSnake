/*WinAPI�������g���D  :

�@�����Gv1.4

  �C���y�{ : ����V��}�l�C���A���ƹ�����i�H�Ȱ��C��
  �@�@   �@�@�I����ɩμ�����ڳ��|�ɭP�C�������A��������ܤ���
            �åi��ܬO�_�����C��
            �p�G��Ū����B1.bmp�A�h�H�Ǧ�e���i�J�C�� 

  ��@��� : 2007.3.18
*/

#include "Snake_Function.cpp"

//�{���i�J�I
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
        
    //�äJ����

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance ;
    //wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIcon = LoadIcon(hInstance,
               MAKEINTRESOURCE(ID_ICON_1));  //�[�J�귽�ɪ��t�Τj�ϥ�
                
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    
    //�H�U�إ߭I���Ϥ�
    LOGBRUSH background;
    background.lbStyle     = BS_PATTERN;
    background.lbHatch     = (long) LoadImage(hInstance,//����N�X
                    backgroundPic,//�Ϲ��W��
                    IMAGE_BITMAP,//�Ϲ�����
                        0, //���檺�e��
                    0,//���檺����
                    LR_LOADFROMFILE);//Ū���ﶵ);
    //�إߵ��� 
                    
    if(background.lbHatch != 0)
        wc.hbrBackground = CreateBrushIndirect(&background);
    else   //�p�G�P��Ƨ����S���W��B1.bmp���I����,�h�H�զ�I�����N 
        wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
                                        //�p�G�令NULL_BRUSH,�I���N�|�ܳz��       

    wc.lpszMenuName  = MAKEINTRESOURCE(ID_MENU);  //���menu���
    wc.lpszClassName = szAppName;
    wc.hIconSm = LoadIcon(hInstance,
                 MAKEINTRESOURCE(ID_ICON_1)); //�[�J�귽�ɪ��t�Τp�ϥ�
    

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "�������O�n�O����!", "�o�Ϳ��~!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szAppName,
        "�w �� �� �� �g �� �D �@ ��   ! !",
        WS_OVERLAPPEDWINDOW,
        nTopXY(wnd_W, GetSystemMetrics(SM_CXSCREEN)),
        nTopXY(wnd_H, GetSystemMetrics(SM_CYSCREEN)),
        wnd_W,wnd_H,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "�����إߥ���!", "�o�Ϳ��~!",
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
 
 char	Text[] = "��r��ܴ���!!";

 int	nLen = sizeof(szText)-1;
 
 int i,x,y;
 int SnakeHand = 5*nTick;  //�D�Y���ʪ��]�w�ܼ�
 
 char gameOverStr[100]; //�C����������r�ԭz
 
 HBRUSH hBrush; //�ܴ�����C���
 
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
            wsprintf(szTick,"�ɶ�:%d",nTick);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            
            }

       break;

       case WM_KEYDOWN: //keydown�i�HŪ����r��M�t����
             wsprintf(szTextKey,"����:%s",TEXT("��"));
             wsprintf(szCharKey,"�r�䬰:%s",&wParam);
             nowKeyNumber = wParam ; //�⮷���쪺�ȰO���_��
             InvalidateRect(hwnd, NULL, TRUE);
             UpdateWindow(hwnd);
       break;
       /*
       case WM_CHAR:    //char�i�HŪ���j�p�g�r��
             wsprintf(szCharKey,"�r�䬰:%d",wParam);
             nowKeyNumber = wParam ; //�⮷���쪺�ȵ������ܼ�
             InvalidateRect(hwnd, NULL, TRUE);
             UpdateWindow(hwnd);
       break;
       */
       case WM_KEYUP:
             wsprintf(szTextKey,"����:%s",TEXT("�}"));
             InvalidateRect(hwnd, NULL, TRUE);
             UpdateWindow(hwnd);

       break;
       //end
       case WM_LBUTTONDOWN:
            if (bFlag == 0){
               bFlag = 1;
               wsprintf(szText,"%s",TEXT("���ƹ������~��"));
               InvalidateRect(hwnd, NULL, TRUE);
               
               //�äJ���խ��ֶ}�l

               /*

               MCI_OPEN_PARMS OpenParms;
OpenParms.lpstrDeviceType =
(LPCSTR) MCI_DEVTYPE_SEQUENCER; //�OMIDI?�����
OpenParms.lpstrElementName = (LPCSTR) "wholenew.mid"; //���W
OpenParms.wDeviceID = 0; //��?��??��??�A�Z���ݭn�ϥ�
mciSendCommand (NULL, MCI_OPEN,
MCI_WAIT | MCI_OPEN_TYPE |
MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT,
(DWORD)(LPVOID) &OpenParms); //��???
        //��?�N�i�H����MIDI�F�G
MCI_PLAY_PARMS PlayParms;
PlayParms.dwFrom = 0; //?���\??��m����A?��?�@��
mciSendCommand (OpenParms, MCI_PLAY, //DeviceID�ݵ��_�W����????
MCI_FROM, (DWORD)(LPVOID)&PlayParms); //����MIDI

                 */
                 
                 //�ĤG�ش���

                 //PlaySound("glass1.wav",
                 //NULL, SND_FILENAME | SND_ASYNC);
                 
                 //sndPlaySound("glass1.wav",SND_ASYNC);
               
               
               //�äJ���խ��ֵ���
               
               
            } else {
               bFlag = 0;
               wsprintf(szText,"%s",TEXT("���ƹ�����Ȱ�"));
               InvalidateRect(hwnd, NULL, TRUE);
            }
       break;
       case WM_PAINT:
            wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
       
            //���񭵼�....�٥��Ƿ|
            //PlaySound("1.wav",NULL,SND_FILENAME | SND_ASYNC);

        	hMyDC=BeginPaint(hwnd, &ps);
        	
        	
        	SetBkMode(hMyDC, TRANSPARENT); //�]�w���I���q�z
        	
        	/*
        	//�]�������ϼh�|�����D,�ҥH��εe�u���覡
        	//��J�Ϥ��}�l
        	
        	LOGBRUSH rectangleBackground;

            rectangleBackground.lbStyle     = BS_PATTERN;
            rectangleBackground.lbHatch     = (long) LoadImage(NULL,//����N�X
                    BACKGROUND_PIC,//�Ϲ��W��
                    IMAGE_BITMAP,//�Ϲ�����
                        0, //���檺�e��
                    0,//���檺����
                    LR_LOADFROMFILE);//Ū���ﶵ

            //��rectangleBackground��JhBrush
            hBrush = CreateBrushIndirect(&rectangleBackground);
            
            SelectObject(hMyDC,hBrush);  //��ܳo�Ӷ��覡
        	
        	Rectangle(hMyDC,10,10,380,220);  //�e�X��ɥ|���
        	
        	//��J�Ϥ�����
        	*/
        	
        	//�e�X���
        	MoveToEx(hMyDC,10,10,NULL);
        	LineTo(hMyDC,380,10);
        	MoveToEx(hMyDC,380,10,NULL);
        	LineTo(hMyDC,380,220);
        	MoveToEx(hMyDC,380,220,NULL);
        	LineTo(hMyDC,10,220);
        	MoveToEx(hMyDC,10,220,NULL);
        	LineTo(hMyDC,10,10);
        	
        	speed(hwnd,hMyDC, msg, wParam, lParam); //����t��
        	
        	randStar(); //�ݬݦ��S���I��P�P        	

            starAndSnakeHand(hwnd,hMyDC,hBrush);  //�e�X�P�P�M�D�Y

            walked(); //�p�⨫�X�B,�L�h���L����

  	        hBrush = CreateSolidBrush(RGB(132,255,77));
        	SelectObject(hMyDC,hBrush);
        	
        	wsprintf(gameOverStr," �Y���ۼ� !! \n\n �`���G%d  \n\n �~��C�� ? ",point);

            PrintTail(hwnd,hMyDC,gameOverStr, msg, wParam, lParam);  //�e�X���ڡ@�çP�_���S���Y���ۼ�
        	
        	keyClicked();   //�W�U���k����
            
            wsprintf(gameOverStr," �X�� !! \n\n �`���G%d  \n\n �~��C�� ? ",point);
            outLine(hwnd,hMyDC,gameOverStr, msg, wParam, lParam);  //�ˬd�O�_�X�ɡ@�p�G�X�ɴN����
            
            printOther(hwnd,hMyDC);  //�e�X��L�����S�����

            SetTextColor(hMyDC,RGB(255,111,33));
            TextOut(hMyDC,140,100,szText,lstrlen(szText));//��ܼȰ��P�_(���ƹ�����)
            
            strcpy(szText,"");  //�o�ˤ~���|�ݯd�b�e���W
            
            DeleteObject(hBrush);
            
            EndPaint(hwnd, &ps);
            
            
        	 //�äJ����
        	 
        break;
        case WM_COMMAND:
             //�ѪR�ϥΪ̿�����ӥ\�����
             switch(LOWORD(wParam)){
               case IDM_GAME_ABOUT:
                 MessageBox(NULL,
                            "�o�O�@��\n    �p��²�檺\n        �g���D�C��",
                            "��������" ,
                             MB_OK);
                 InvalidateRect(hwnd, NULL, TRUE);
               break;
               case IDM_DESIGNER_ABOUT:
                 MessageBox(NULL, "�d�L���H !!","��������" ,
                            MB_ICONINFORMATION | MB_OK);
                 InvalidateRect(hwnd, NULL, TRUE);
               break;
               case IDM_CLOSE:
                 SendMessage(hwnd,WM_CLOSE,0,0);
               break;

                case WM_CLOSE:
                  KillTimer(hwnd,1);  //���ɶ��p�ⰱ��
                int id = 0;
                 id = MessageBox(hwnd,gameOverStr,"�C������", MB_YESNO | MB_ICONINFORMATION);

                 if(id == IDNO)
                    PostQuitMessage(0);   //�����{��
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



