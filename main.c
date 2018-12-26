#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>
#include "resource.h"
#include "acllib.h"
#define COL 15
#define ROW 15
ACL_Image indexImg ;
ACL_Image npc;
ACL_Image photo;
ACL_Image backgrnd ;
ACL_Sound sound;
ACL_Image draw;//ƽ��
ACL_Image win;
ACL_Image lose;
ACL_Image monster[8];
ACL_Image player0;
ACL_Image player1;
ACL_Image player01;
ACL_Image player11;
ACL_Image player02;
ACL_Image player12;
int gameStat = 0;//��Ϸ״̬ 0���� 1����
int chess[15][15] = {-1};//����
int resi;//����������
int resj;//���̺�����
int i;
int j;
int tempResj;//����֮ǰ�ĺ�����
int tempResi;//����֮ǰ��������
int tempStat;
int round = 0;//�غ�
int cnt = 0;//������
int stat0 = 0;
int stat1 = 0;
int stat2 = 0;
int kind0 = -1;
int kind1 = -1;
int mode = -1;
int whoFirst = -1;
char *say0 = "�������ӵĲ�ͬ����������Է�Ϊ��ͬ�����ࡣ\n������ʲô���������壿";
char *say1 = "�뵥��ѡ�����1�����ӡ�";
char *say2  = "�뵥��ѡ�����2�����ӡ�";
char *say3  = "��ѡ��ģʽ:";
char *say4 = "��ѡ�����ֵ����:";
char *first0 = "���1";
char *first1 = "���2/����";
char *mode0 = "�˻�ģʽ";
char *mode1 = "˫��ģʽ";
char *kinds[] =
{
    "��Ģ��",
    "��ˮ��",
    "����",
    "��������",
    "��߷���",
    "�����è",
    "������˫��",
    "��֩��"
};
int Score[15][15];//��������
int winerValue;
int tp;//���Ե�����
int isTouch;
int ExWhile1 ;

int isWin(int row, int col,int whoFirst);
char* myStrCat(char *dst,const char *src);
void mouseListener0(int x , int y ,int button ,int event);
void mouseListener1(int x , int y ,int button ,int event);
void mouseListener2(int x , int y ,int button ,int event);
void mouseListener3(int x , int y ,int button ,int event);
void showIndex(void);
void initImage1(void);
void initImage2(void);
void playMusic(void);
void chooseMode(void);
void gameMode0(void);
void gameMode1(void);
void timerListener(int id);
void Robot(int *x, int *y, int *Sum) ;
void Findscore(int *x, int *y);
void ChessOne(int *x, int *y);
void ChessScore();

char * myStr(char *str,int n,int m);

int isWin(int row, int col,int whoFirst)
{
    // ����б���ִ������ÿ����������ݵ�ǰ�����������5�����ӣ���һ�ַ��Ͼ���Ӯ
    // ˮƽ����
    int i;
    for (i = 0; i < 5; i++)
    {
        // ����5��������ƥ��4���ӣ�20�����
        if (col - i >= 0 &&
                col - i + 4 < 15 &&
                chess[row][col - i] == whoFirst &&
                chess[row][col - i] == chess[row][col - i + 1] &&
                chess[row][col - i] == chess[row][col - i + 2] &&
                chess[row][col - i] == chess[row][col - i + 3] &&
                chess[row][col - i] == chess[row][col - i + 4])
            return 1;
    }

    // ��ֱ����(��������4��)
    for ( i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
                row - i + 4 < 15 &&
                chess[row - i][col] == whoFirst &&
                chess[row - i][col] == chess[row - i + 1][col] &&
                chess[row - i][col] == chess[row - i + 2][col] &&
                chess[row - i][col] == chess[row - i + 3][col] &&
                chess[row - i][col] == chess[row - i + 4][col])
            return 1;
    }

    // ��б����
    for (i = 0; i < 5; i++)
    {
        if (row + i < 15 &&
                row + i - 4 >=0 &&
                col - i >=0 &&
                col - i + 4 < 15 &&
                chess[row+i][col - i] == whoFirst &&
                chess[row + i][col - i] == chess[row + i - 1][col - i + 1] &&
                chess[row + i][col - i] == chess[row + i - 2][col - i + 2] &&
                chess[row + i][col - i] == chess[row + i - 3][col - i + 3] &&
                chess[row + i][col - i] == chess[row + i - 4][col - i + 4])
            return 1;
    }

    // ��б����
    for (i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
                row - i + 4 < 15 &&
                col - i >= 0 &&
                col - i + 4 < 15 &&
                chess[row - i][col - i] == whoFirst &&
                chess[row - i][col - i] == chess[row - i + 1][col - i + 1] &&
                chess[row - i][col - i] == chess[row - i + 2][col - i + 2] &&
                chess[row - i][col - i] == chess[row - i + 3][col - i + 3] &&
                chess[row - i][col - i] == chess[row - i + 4][col - i + 4])
            return 1;
    }
    return 0;
}

int Setup()
{
    loadImage("image/photo0.jpg",&photo);
    loadImage("image/photo1.jpg",&photo+1);
    loadImage("image/photo2.jpg",&photo+2);
    loadImage("image/photo3.jpg",&photo+3);
    loadImage("image/photo4.jpg",&photo+4);
    loadImage("image/photo5.jpg",&photo+5);
    loadImage("image/photo6.jpg",&photo+6);
    loadImage("image/photo7.jpg",&photo+7);
    initWindow("MapleStory GoBang",DEFAULT,DEFAULT,800,600);
    initImage1();
    memset(chess,-1,sizeof(chess));
    showIndex();
    playMusic();
    return 0;
}

void chooseMode(void)//ѡ���˻�/��սģʽ
{
    registerMouseEvent(mouseListener1);
    beginPaint();
    putImageScale(&indexImg,DEFAULT,DEFAULT,800,600);
    setTextColor(WHITE);
    setTextBkColor(EMPTY);
    setTextSize(50);
    setTextFont("΢���ź�");
    paintText( 70,40,say3);
    setTextColor(BLACK);
    setTextSize(100);
    paintText( 250,150,mode0);
    paintText( 250,300,mode1);
    setTextSize(50);
    setTextColor(WHITE);
    paintText(70,450,say4);
    setTextColor(BLACK);
    paintText(200,520,first0);
    paintText(450,520,first1);
    endPaint();
}

void showIndex(void) //��ʾ��ҳ��ѡ����������
{
    registerMouseEvent(mouseListener0);
    beginPaint();
    int i =0;
    putImageScale(&indexImg,DEFAULT,DEFAULT,800,600);
    setTextColor(WHITE);
    setTextBkColor(EMPTY);
    setTextSize(27);

    setTextFont("΢���ź�");
    paintText( 70,40,say0);
    kind0 = -1;
    kind1 = -1;
    if(kind0 == -1)
    {
        setTextColor(BLACK);
        setTextSize(30);
        paintText( 70,80,say1);
        for( i = 0; i<8; i++)
        {
            putImageScale(&monster+i,70,85+55*(i+1),23,23);
            setTextSize(30);
            paintText( 100,80+55*(i+1),kinds[i]);
        }
    }

    if(kind1 == -1)
    {
        setTextColor(BLACK);
        setTextSize(30);
        paintText( 470,80,say2);
        for( i = 0; i<8; i++)
        {
            putImageScale(&monster+i,470,85+55*(i+1),23,23);
            setTextSize(30);
            paintText( 500,80+55*(i+1),kinds[i]);
        }
    }
    endPaint();
}

void initImage1(void)
{
    loadImage("image/index.jpg",&indexImg);
    loadImage("image/m01.jpg",&monster);
    loadImage("image/m11.jpg",&monster+1);
    loadImage("image/m21.jpg",&monster+2);
    loadImage("image/m31.jpg",&monster+3);
    loadImage("image/m41.jpg",&monster+4);
    loadImage("image/m51.jpg",&monster+5);
    loadImage("image/m61.jpg",&monster+6);
    loadImage("image/m71.jpg",&monster+7);
}




void playMusic(void)
{
    loadSound("sound/sound.mp3",&sound);//���ر�������
    playSound(sound,SND_LOOP);
}

//mouseListener0 �������ָ��λ����ʾ����
void mouseListener0(int x , int y ,int button ,int event)
{
    if(stat0 == 1)
    {
        return;
    }
    printf("x=%d y=%d button=%d event=%d\n",x,y,button,event);
    //0 ��Ģ��
    if(x>=70 && y>=140 && x <=164 && y<=161)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(0+1),kinds[0]);
        endPaint();
        if(button = 1 && event == 0 && kind1!=0)//������������ʱ
        {
            kind0 = 0;
        }
    }
    else if(kind0 != 0)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(0+1),kinds[0]);
        endPaint();
    }

    //1 ��ˮ��
    if(x>=70 && y>=140+1*55 && x <=164 && y<=161+1*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+1),kinds[1]);
        endPaint();
        if(button = 1 && event == 0 &&kind1!=1)//������������ʱ
        {
            kind0 = 1;
        }
    }
    else if(kind0 !=1)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+1),kinds[1]);
        endPaint();
    }

    //2 ����
    if(x>=70 && y>=140+2*55 && x <=145 && y<=161+2*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+2),kinds[2]);
        endPaint();
        if(button = 1 && event == 0 &&kind1!=2)//������������ʱ
        {
            kind0 = 2;
        }
    }
    else if(kind0 != 2)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+2),kinds[2]);
        endPaint();
    }

    //3 ��������
    if(x>=70 && y>=140+3*55 && x <=185 && y<=161+3*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+3),kinds[3]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=3)//������������ʱ
        {
            kind0 = 3;
        }
    }
    else if(kind0 != 3)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+3),kinds[3]);
        endPaint();
    }

    //4 ��߷���
    if(x>=70 && y>=140+4*55 && x <=185 && y<=161+4*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+4),kinds[4]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=4)//������������ʱ
        {
            kind0 = 4;
        }
    }
    else if(kind0 != 4)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+4),kinds[4]);
        endPaint();
    }

    //5 �����è
    if(x>=70 && y>=140+5*55 && x <=185 && y<=161+5*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+5),kinds[5]);
        endPaint();
        if(button = 1 && event == 0 &&kind1!=5)//������������ʱ
        {
            kind0 = 5;
        }
    }
    else if(kind0 != 5)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+5),kinds[5]);
        endPaint();
    }

    //6 ������˫��
    if(x>=70 && y>=140+6*55 && x <=208 && y<=161+6*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+6),kinds[6]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=6)//������������ʱ
        {
            kind0 = 6;
        }
    }
    else if(kind0 != 6)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+6),kinds[6]);
        endPaint();
    }

    //7 ��֩��
    if(x>=70 && y>=140+7*55 && x <=164 && y<=161+7*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+7),kinds[7]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=7)//������������ʱ
        {
            kind0 = 7;
        }
    }
    else if(kind0 != 7)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100,80+55*(1+7),kinds[7]);
        endPaint();
    }

    //0 ��Ģ��
    if(x>=70+400 && y>=140 && x <=164+400 && y<=161)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(0+1),kinds[0]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=0)//������������ʱ
        {
            kind1 = 0;
        }
    }
    else if(kind1 != 0)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(0+1),kinds[0]);
        endPaint();
    }

    //1 ��ˮ��
    if(x>=70+400 && y>=140+1*55 && x <=164+400 && y<=161+1*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+1),kinds[1]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=1)//������������ʱ
        {
            kind1 = 1;
        }
    }
    else if(kind1 != 1)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+1),kinds[1]);
        endPaint();
    }

    //2 ����
    if(x>=70+400 && y>=140+2*55 && x <=145+400 && y<=161+2*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+2),kinds[2]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=2)//������������ʱ
        {
            kind1 = 2;
        }
    }
    else if(kind1 != 2)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+2),kinds[2]);
        endPaint();
    }

    //3 ��������
    if(x>=70+400 && y>=140+3*55 && x <=185+400 && y<=161+3*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+3),kinds[3]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=3)//������������ʱ
        {
            kind1 = 3;
        }
    }
    else if(kind1 != 3)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+3),kinds[3]);
        endPaint();
    }

    //4 ��߷���
    if(x>=70+400 && y>=140+4*55 && x <=185+400 && y<=161+4*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+4),kinds[4]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=4)//������������ʱ
        {
            kind1 = 4;
        }
    }
    else if(kind1 != 4)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+4),kinds[4]);
        endPaint();
    }

    //5 �����è
    if(x>=70+400 && y>=140+5*55 && x <=185+400 && y<=161+5*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+5),kinds[5]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=5)//������������ʱ
        {
            kind1 = 5;
        }
    }
    else if(kind1 != 5)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+5),kinds[5]);
        endPaint();
    }

    //6 ������˫��
    if(x>=70+400 && y>=140+6*55 && x <=208+400 && y<=161+6*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+6),kinds[6]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=6)//������������ʱ
        {
            kind1 = 6;
        }
    }
    else if(kind1 != 6)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+6),kinds[6]);
        endPaint();
    }

    //7 ��֩��
    if(x>=70+400 && y>=140+7*55 && x <=164+400 && y<=161+7*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+7),kinds[7]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=7)//������������ʱ
        {
            kind1 = 7;
        }
    }
    else if(kind1 != 7)
    {
        beginPaint();
        setTextColor(BLACK);
        paintText( 100+400,80+55*(1+7),kinds[7]);
        endPaint();
    }
    if(kind0!= -1 && kind1 != -1)
    {
        stat0 = 1;
        chooseMode();
    }

}

void mouseListener1(int x , int y ,int button ,int event)
{
    if(stat1)
    {
        return;
    }
    printf("%d %d\n",x,y);
    beginPaint();
    if(x >= 250 && y >= 168 && x<= 250+300 && y <=168+75)
    {
        setTextColor(YELLOW);
        setTextSize(100);
        paintText( 250,150,mode0);
        if(button = 1 && event == 0)
        {
            mode = 0;
        }
    }
    else if(mode != 0)
    {
        setTextColor(BLACK);
        setTextSize(100);
        paintText( 250,150,mode0);
    }
    if(x >= 250 && y >= 322 && x<= 250+300 && y <=322+75)
    {
        setTextColor(YELLOW);
        setTextSize(100);
        paintText( 250,300,mode1);
        if(button = 1 && event == 0)
        {
            mode = 1;
        }
    }
    else if(mode != 1)
    {
        setTextColor(BLACK);
        setTextSize(100);
        paintText( 250,300,mode1);
    }

    if(x >= 200 && y >= 530 && x<= 290 && y <=560)
    {
        setTextColor(YELLOW);
        setTextSize(50);
        paintText( 200,520,first0);
        if(button = 1 && event == 0)
        {
            whoFirst = 0;//1���������
        }
    }
    else if(whoFirst != 0)
    {
        setTextColor(BLACK);
        setTextSize(50);
        paintText(200 ,520,first0);
    }
    if(x >= 450 && y >= 530 && x<= 636 && y <=560)
    {
        setTextColor(YELLOW);
        setTextSize(50);
        paintText( 450,520,first1);
        if(button = 1 && event == 0)
        {
            whoFirst = 1;//2�����/��������
        }
    }
    else if(whoFirst != 1)
    {
        setTextColor(BLACK);
        setTextSize(50);
        paintText(450 ,520,first1);
    }
    endPaint();
    if(mode==0 && whoFirst!= -1)
    {
        stat1 = 1;
        gameMode0();
    }
    else if(mode == 1 && whoFirst != -1)
    {
        stat1 = 1;
        gameMode1();
    }
}

void mouseListener2(int x , int y ,int button ,int event)
{
    printf("%d %d %d %d\n",x,y,button,event);
    registerTimerEvent(timerListener);
    resi = 0;//���ĺ�����
    resj = 0;//����������
    i = 0;
    j = 0;
    int xo[15] = {42,76,109,143,176,210,243,277,310,344,377,411,444,478,511};
    int yo[15] = {51,84,118,151,185,218,252,285,319,352,386,419,453,486,520};
    tempStat = 0;
    if(button == 1 && event == 0)
    {
        for( i = 0; i<15; i++)
        {
            if(x >= xo[i]-7 && x <= xo[i]+7)
            {
                resi = xo[i];
                tempStat++;
                break;
            }
        }
        for( j = 0; j<15; j++)
        {
            if(y >= yo[j]-7 && y <= yo[j]+7)
            {
                resj = yo[j];
                tempStat++;
                break;
            }
        }
        if(chess[j][i] == -1 && tempStat == 2)
        {
            tempResi = resi;
            tempResj = resj;
        }
        if(resi != 0 && resj != 0 && chess[j][i] == -1)
        {
            printf("chess %d %d\n",i,j);
            beginPaint();
            //˫�˶�սģʽ
            if(mode == 1)
            {
                char temp[99];
                if((whoFirst+round)%2 == 0)
                {
                    setBrushColor(WHITE);
                    rectangle(410,560,540,590);
                    paintText(423,560,"Step:");
                    paintText(490,560,itoa(round+1,temp,10));

                    putImageScale(&player0,resi-15,resj-15,30,30);
                    startTimer(1,500);
                    chess[j][i] = 0;
                    //printf("%d :%d %d\n",whoFirst+round,i,j);
                    round++;
                    if(round == 225) //ƽ��
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,0)!=0)
                    {
                        printf("���1��ʤ!\n");
                        loadImage("image/win.jpg",&win);
                        putImageScale(&win,412,28,170,97);
                        loadImage("image/lose.jpg",&lose);
                        putImageScale(&lose,412,152,170,97);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                }
                else if((whoFirst+round)%2 == 1)
                {
                    setBrushColor(WHITE);
                    rectangle(410,560,540,590);
                    paintText(423,560,"Step:");
                    paintText(490,560,itoa(round+1,temp,10));
                    chess[j][i] = 1;
                    putImageScale(&player1,resi-15,resj-15,30,30);
                    printf("%d\n",whoFirst+round);
                    round++;
                    if(round == 225) //ƽ��
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,1)!=0)
                    {
                        printf("���2��ʤ!\n");
                        loadImage("image/win.jpg",&win);
                        putImage(&win,412,152);
                        loadImage("image/lose.jpg",&lose);
                        putImage(&lose,412,28);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                }
            }
            endPaint();
        }
    }
}

void mouseListener3(int x , int y ,int button ,int event)
{
   // printf("%d %d %d %d\n",x,y,button,event);
    registerTimerEvent(timerListener);
    resi = 0;//���ĺ�����
    resj = 0;//����������
    i = 0;
    j = 0;
    tp = (whoFirst+round)%2;
    int xo[15] = {42,76,109,143,176,210,243,277,310,344,377,411,444,478,511};
    int yo[15] = {51,84,118,151,185,218,252,285,319,352,386,419,453,486,520};
    tempStat = 0;
    if(button == 1 && event == 0)
    {
        for( i = 0; i<15; i++)
        {
            if(x >= xo[i]-7 && x <= xo[i]+7)
            {
                resi = xo[i];
                tempStat++;
                break;
            }
        }
        for( j = 0; j<15; j++)
        {
            if(y >= yo[j]-7 && y <= yo[j]+7)
            {
                resj = yo[j];
                tempStat++;
                break;
            }
        }
        if(chess[j][i] == -1 && tempStat == 2)
        {
            tempResi = resi;
            tempResj = resj;
        }
        if(resi != 0 && resj != 0 && chess[j][i] == -1)
        {
          //  printf("chess %d %d\n",i,j);
            beginPaint();
            //�˻�ģʽ
            if(mode == 0)
            {
                char temp[99];
                if((whoFirst+round)%2 == 0)
                {
                    setBrushColor(WHITE);
                    rectangle(410,560,540,590);
                    paintText(423,560,"Step:");
                    paintText(490,560,itoa(round+1,temp,10));

                    putImageScale(&player0,resi-15,resj-15,30,30);
                    //startTimer(1,500); �˻�ģʽ��ո��µĲ���Ҫ��˸
                    chess[j][i] = 0;
                    //printf("%d :%d %d\n",whoFirst+round,i,j);
                    round++;
                    if(round == 225) //ƽ��
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,0)!=0)
                    {
                        printf("���1��ʤ!\n");
                        loadImage("image/win.jpg",&win);
                        putImageScale(&win,412,28,170,97);
                        loadImage("image/lose.jpg",&lose);
                        putImageScale(&lose,412,152,170,97);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                }
                else if((whoFirst+round)%2 == 1)
                {
                    setBrushColor(WHITE);
                    rectangle(410,560,540,590);
                    paintText(423,560,"Step:");
                    paintText(490,560,itoa(round+1,temp,10));
                    chess[j][i] = 1;
                    putImageScale(&player1,resi-15,resj-15,30,30);
                    printf("%d\n",whoFirst+round);
                    round++;
                    if(round == 225) //ƽ��
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,1)!=0)
                    {
                        printf("���2��ʤ!\n");
                        loadImage("image/win.jpg",&win);
                        putImage(&win,412,152);
                        loadImage("image/lose.jpg",&lose);
                        putImage(&lose,412,28);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                }
            }
            endPaint();
        }
    }
    //��������
      if(button == 1 && event == 2 && (round+whoFirst)%2 == 1){//��Ҫ�ж��Ƿ�����ɹ�

        Robot(&j, &i, &round+1) ;
        printf("%d %d\n",j,i);
        beginPaint();
        putImageScale(&player1,xo[i]-15,yo[j]-15,30,30);
        tempResi = xo[i];
        tempResj = yo[j];
        startTimer(1,500);
        round++;
         if(round == 225) //ƽ��
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,1)!=0)
                    {
                        printf("���Ի�ʤ!\n");
                        loadImage("image/win.jpg",&win);
                        putImage(&win,412,152);
                        loadImage("image/lose.jpg",&lose);
                        putImage(&lose,412,28);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
        endPaint();

      }
}

char* myStrCat(char *dst,const char *src)
{
    char *temp = dst;
    while(*dst!='\0')
    {
        dst++;
    }
    while(*src!='\0')
    {
        *dst = *src;
        dst++;
        src++;

    }
    *dst = '\0';
    return temp;
}

//�˻�ģʽ
void gameMode0(void)
{
    loadImage("image/backgrnd.jpg",&backgrnd);

    char file1[99];
    itoa(kind0+kind1,file1,10);
    char temp1[99];
    myStrCat(temp1,"image/photo");
    myStrCat(temp1,file1);
    myStrCat(temp1,".jpg");

    loadImage(temp1,&photo);
    beginPaint();

    putImageScale(&backgrnd,0,0,800,600);
    putImageScale(&photo,570,370,220,220);
    setTextSize(30);
    setTextColor(BLACK);
    paintText(570,0,"���:");
    paintText(570,120,"����:");
    setBrushColor(WHITE);
    rectangle(410,560,540,590);
    char file2[99];
    itoa(kind0,file2,10);
    char temp2[99];
    myStrCat(temp2,"image/m");
    myStrCat(temp2,file2);
    myStrCat(temp2,"1.jpg");
    loadImage(temp2,&player0);

    char file3[99];
    itoa(kind1,file3,10);
    char temp3[99];
    myStrCat(temp3,"image/m");
    myStrCat(temp3,file3);
    myStrCat(temp3,"1.jpg");
    loadImage(temp3,&player1);

    char temp4[] = "image/m00.jpg";
    loadImage(myStr(temp4,kind0,0),&player01);
    loadImage(myStr(temp4,kind0,2),&player02);
    loadImage(myStr(temp4,kind1,0),&player11);
    loadImage(myStr(temp4,kind1,2),&player12);

//���0 gif˳��ӦΪ player01 player0 player02
//���1 gif˳��ӦΪ player11 player1 player12

    putImageScale(&player0,650,50,50,50);
    putImageScale(&player1,650,170,50,50);
    round = 0;

    registerMouseEvent(mouseListener3);
    endPaint();
}

//˫��ģʽ
void gameMode1(void)
{
    loadImage("image/backgrnd.jpg",&backgrnd);

    char file[99];
    itoa(kind0+kind1,file,10);
    char temp[99];
    myStrCat(temp,"image/photo");
    myStrCat(temp,file);
    myStrCat(temp,".jpg");

    loadImage(temp,&photo);
    beginPaint();
    putImageScale(&backgrnd,0,0,800,600);
    putImageScale(&photo,570,370,220,220);
    setTextSize(30);
    setTextColor(BLACK);
    paintText(570,0,"���1:");
    paintText(570,120,"���2:");
    setBrushColor(WHITE);
    rectangle(410,560,540,590);

    char file2[99];
    itoa(kind0,file2,10);
    char temp2[99];
    myStrCat(temp2,"image/m");
    myStrCat(temp2,file2);
    myStrCat(temp2,"1.jpg");
    loadImage(temp2,&player0);

    char file3[99];
    itoa(kind1,file3,10);
    char temp3[99];
    myStrCat(temp3,"image/m");
    myStrCat(temp3,file3);
    myStrCat(temp3,"1.jpg");
    loadImage(temp3,&player1);

    char temp4[] = "image/m00.jpg";
    loadImage(myStr(temp4,kind0,0),&player01);
    loadImage(myStr(temp4,kind0,2),&player02);
    loadImage(myStr(temp4,kind1,0),&player11);
    loadImage(myStr(temp4,kind1,2),&player12);

//���0 gif˳��ӦΪ player01 player0 player02
//���1 gif˳��ӦΪ player11 player1 player12

    putImageScale(&player0,650,50,50,50);
    putImageScale(&player1,650,170,50,50);
    round = 0;
    registerMouseEvent(mouseListener2);
    endPaint();
}

void timerListener(int id)
{
    if(gameStat == 1 && id == 0)
    {
        exit(0);
    }
    else if(id == 1)
    {
        //round�Ѿ��ۼӹ�
        if((whoFirst+round)%2 == 1) //0�����
        {
            if(cnt %3 == 0 )
            {
                beginPaint();
                putImageScale(&player01,tempResi-15,tempResj-15,30,30);
                endPaint();
            }
            else if(cnt %3 == 1 )
            {
                beginPaint();
                putImageScale(&player0,tempResi-15,tempResj-15,30,30);
                endPaint();
            }
            else if(cnt %3 == 2)
            {
                beginPaint();
                putImageScale(&player02,tempResi-15,tempResj-15,30,30);
                endPaint();
            }
            cnt++;

        }
        else if((whoFirst+round)%2 == 0) //1�����
        {
            if(cnt %3 == 0)
            {
                beginPaint();
                putImageScale(&player11,tempResi-15,tempResj-15,30,30);
                endPaint();
            }
            else if(cnt %3 == 1)
            {
                beginPaint();
                putImageScale(&player1,tempResi-15,tempResj-15,30,30);
                endPaint();
            }
            else if(cnt %3 == 2)
            {
                beginPaint();
                putImageScale(&player12,tempResi-15,tempResj-15,30,30);
                endPaint();
            }
            cnt++;
            printf("hello1!");
        }
    }
}

char * myStr(char *str,int n,int m)
{
    int i = 0;
    for( i = 0; str[i]!='\0'; i++)
    {
        if(str[i] == '.')
        {
            str[i-2] = n+48;
            str[i-1] = m+48;
            break;
        }
    }
    return str;
}

void Robot(int *x, int *y, int *Sum)
{
    ExWhile1 = 1;
    if (*Sum == 1)
    {
        while (ExWhile1)
        {
            ChessOne(x, y);
            if (chess[*x][*y] == -1)
            {
                ExWhile1 = 0;
            }
        }
        chess[*x][*y] = tp;   //��¼�����
        // printpart(*x, *y, tp);     //��ӡ������AI��һ������
        isTouch = 1;
        tp++;
        tp = tp % 2;
    }
    else      //�ӵ�2����ʼ��ʹ������ϵͳ
    {
        Findscore(x, y);
    }
}


void Findscore(int *x, int *y)   //����������ߵ�����
{
    srand((unsigned)time(NULL));
    int i, j, x1, x2, y1, y2, lx;
    int Max = 0;
    ChessScore();      //�������ֺ���
    for (i = 0; i<15; i++)
    {
        for (j = 0; j<15; j++)
        {
            if (Score[i][j]>Max)
            {
                Max = Score[i][j];  //��ȡ���е��У�������ߵ�
                x1 = i;
                y1 = j;
            }
        }
    }
    x2 = x1;
    y2 = y1;
    for (i = 0; i<15; i++)    //���ܵĻ�����������ͬ�Ķ����
    {
        for (j = 0; j<15; j++)
        {
            if (Score[i][j] == Max&&i != x2&&j != y2)   //����ô�����ͬ�����ĵ��У������һ��
            {
                lx = rand() % 10;
                if (lx<5)
                {
                    x2 = i, y2 = j;
                    break;
                }
            }
        }
    }
    if (x2 != x1 || y2 != y1)   //��������2����߷�
    {
        lx = rand() % 10;    //���һ��
        if (lx>6)
        {
            *x = x1, *y = y1;
        }
        else
        {
            *x = x2, *y = y2;
        }
    }
    else    //������ֻ��һ����߷�
    {
        *x = x1, *y = y1;
    }
    Max = 0;    //������ֵ
    chess[*x][*y] = tp;       //��¼�����
    //printpart(x, y, tp);    //��ӡ������AI����
    if (winerValue==2)
    {
        isTouch = 1;
    }

    tp++;
    tp = tp % 2;
}


void ChessOne(int *x, int *y)   //����ߵ�1��ʱ������
{
    int i, j;
    srand((unsigned)time(NULL));    //���������ʱ��ĸı���ı�
    for (i = 0; i<15; i++)
    {
        for (j = 0; j<15; j++)
        {
            if (chess[i][j] == 0)  //����ҵ�����ҵ����ӣ�������8����������һ������
            {
                int lx = rand() % 7;
                if (lx == 0)
                {
                    *x = i + 1;
                    *y = j + 1;
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 1)
                {
                    *x = i + 1;
                    *y = j - 1;
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 2)
                {
                    *x = i - 1;
                    *y = j - 1;
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 3)
                {
                    *x = i - 1;
                    *y = j + 1;
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 4)
                {
                    *x = i - 1;
                    *y = j;    //��
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 5)
                {
                    *x = i;
                    *y = j - 1;   //��
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 6)
                {
                    *x = i;
                    *y = j + 1;   //��
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else
                {
                    *x = i + 1;
                    *y = j;    //��
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
            }
        }
    }
}

void ChessScore()
{
    int x, y, i, j, k;      //ѭ������
    int number1 = 0, number2 = 0;   //number����ͳ����һ�����������ɸ���
    int empty = 0;    //empty����ͳ�ƿյ����
    memset(Score, 0, sizeof(Score));                    //����������������
    for (x = 0; x<15; x++)
    {
        for (y = 0; y<15; y++)
        {
            if (chess[x][y] == -1)    //��������Ϊ��
            {
                for (i = -1; i <= 1; i++)
                {
                    for (j = -1; j <= 1; j++)   //�ж�8������
                    {
                        if (i != 0 || j != 0)   //���Ƕ�Ϊ0�Ļ����ǲ�����ԭ������
                        {
                            //������������
                            for (k = 1; i <= 4; k++)   //ѭ��4��
                            {
                                //���ûԽ��  �������ں��ӣ���ң�
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 0)
                                {
                                    number1++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)     //����Ǹ��յ㣬+1���˳�
                                {
                                    empty++;
                                    break;
                                }
                                else                    //������ǽ���߶Է���������
                                {
                                    break;
                                }
                            }
                            for (k = -1; k >= -4; k--)            //�������෴�����ж�
                            {
                                //���ûԽ��  �������ں��ӣ���ң�
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 0)
                                {
                                    number1++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)     //����Ǹ��յ㣬+1���˳�
                                {
                                    empty++;
                                    break;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if (number2 == 1)   //2������
                            {
                                Score[x][y] += 1;
                            }
                            else if (number1 == 2)   //3������
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 5;   //��һ���յ�+5�� ��3
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 10;  //�������յ�+10�� ��3
                                }
                            }
                            else if (number1 == 3)   //4������
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 20;  //��һ���յ�+20�� ��4
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 100;  //��2���յ�+100�� ��4
                                }
                            }
                            else if (number1 >= 4)
                            {
                                Score[x][y] += 1000;  //�Է���5�����ӣ�����Ҫ�ߵ㣬�ȶ�
                            }

                            empty = 0;   //ͳ�ƿյ�����ı�������

                            //�Ե����������
                            for (k = 1; i <= 4; k++)   //ѭ��4��
                            {
                                //���ûԽ��  �������ڰ��ӣ����ԣ�
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 1)
                                {
                                    number2++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)
                                {
                                    empty++;
                                    break;   //�յ�
                                }
                                else
                                {
                                    break;
                                }
                            }
                            for (k = -1; k >= -4; k--)   //�������෴�����ж�
                            {
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 1)
                                {
                                    number2++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)
                                {
                                    empty++;
                                    break;
                                }
                                else
                                {
                                    break;   //ע����������Ұ���ͬ
                                }
                            }
                            if (number2 == 0)
                            {
                                Score[x][y] += 1;    //1������
                            }
                            else if (number2 == 1)
                            {
                                Score[x][y] += 2;    //2������
                            }
                            else if (number2 == 2)   //3������
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 8;  //��3
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 30;  //��3
                                }
                            }
                            else if (number2 == 3)   //4������
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 50;   //��4
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 200;   //��4
                                }
                            }
                            else if (number2 >= 4)
                            {
                                Score[x][y] += 10000;   //�Լ�����������γ�5����Ҳ����ʤ���ˣ��������
                            }

                            number1 = 0;     //���㣬�Ա��´�����ͳ��
                            number2 = 0;
                            empty = 0;
                        }
                    }
                }
            }
        }
    }
}
