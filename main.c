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
ACL_Image draw;//平局
ACL_Image win;
ACL_Image lose;
ACL_Image monster[8];
ACL_Image player0;
ACL_Image player1;
ACL_Image player01;
ACL_Image player11;
ACL_Image player02;
ACL_Image player12;
int gameStat = 0;//游戏状态 0运行 1结束
int chess[15][15] = {-1};//棋盘
int resi;//棋盘纵坐标
int resj;//棋盘横坐标
int i;
int j;
int tempResj;//保存之前的横坐标
int tempResi;//保存之前的纵坐标
int tempStat;
int round = 0;//回合
int cnt = 0;//计数器
int stat0 = 0;
int stat1 = 0;
int stat2 = 0;
int kind0 = -1;
int kind1 = -1;
int mode = -1;
int whoFirst = -1;
char *say0 = "按照棋子的不同，五子棋可以分为不同的种类。\n你想玩什么样的五子棋？";
char *say1 = "请单击选择玩家1的棋子。";
char *say2  = "请单击选择玩家2的棋子。";
char *say3  = "请选择模式:";
char *say4 = "请选择先手的玩家:";
char *first0 = "玩家1";
char *first1 = "玩家2/电脑";
char *mode0 = "人机模式";
char *mode1 = "双人模式";
char *kinds[] =
{
    "花蘑菇",
    "绿水灵",
    "猪猪",
    "三眼章鱼",
    "玩具粉熊",
    "玩具熊猫",
    "吹泡泡双鱼",
    "绿蜘蛛"
};
int Score[15][15];//棋盘评分
int winerValue;
int tp;//电脑的棋子
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
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    int i;
    for (i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i >= 0 &&
                col - i + 4 < 15 &&
                chess[row][col - i] == whoFirst &&
                chess[row][col - i] == chess[row][col - i + 1] &&
                chess[row][col - i] == chess[row][col - i + 2] &&
                chess[row][col - i] == chess[row][col - i + 3] &&
                chess[row][col - i] == chess[row][col - i + 4])
            return 1;
    }

    // 竖直方向(上下延伸4个)
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

    // 左斜方向
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

    // 右斜方向
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

void chooseMode(void)//选择人机/对战模式
{
    registerMouseEvent(mouseListener1);
    beginPaint();
    putImageScale(&indexImg,DEFAULT,DEFAULT,800,600);
    setTextColor(WHITE);
    setTextBkColor(EMPTY);
    setTextSize(50);
    setTextFont("微软雅黑");
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

void showIndex(void) //显示首页，选择棋子种类
{
    registerMouseEvent(mouseListener0);
    beginPaint();
    int i =0;
    putImageScale(&indexImg,DEFAULT,DEFAULT,800,600);
    setTextColor(WHITE);
    setTextBkColor(EMPTY);
    setTextSize(27);

    setTextFont("微软雅黑");
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
    loadSound("sound/sound.mp3",&sound);//加载背景音乐
    playSound(sound,SND_LOOP);
}

//mouseListener0 鼠标移至指定位置显示高亮
void mouseListener0(int x , int y ,int button ,int event)
{
    if(stat0 == 1)
    {
        return;
    }
    printf("x=%d y=%d button=%d event=%d\n",x,y,button,event);
    //0 花蘑菇
    if(x>=70 && y>=140 && x <=164 && y<=161)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(0+1),kinds[0]);
        endPaint();
        if(button = 1 && event == 0 && kind1!=0)//当鼠标左键单击时
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

    //1 绿水灵
    if(x>=70 && y>=140+1*55 && x <=164 && y<=161+1*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+1),kinds[1]);
        endPaint();
        if(button = 1 && event == 0 &&kind1!=1)//当鼠标左键单击时
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

    //2 猪猪
    if(x>=70 && y>=140+2*55 && x <=145 && y<=161+2*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+2),kinds[2]);
        endPaint();
        if(button = 1 && event == 0 &&kind1!=2)//当鼠标左键单击时
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

    //3 三眼章鱼
    if(x>=70 && y>=140+3*55 && x <=185 && y<=161+3*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+3),kinds[3]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=3)//当鼠标左键单击时
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

    //4 玩具粉熊
    if(x>=70 && y>=140+4*55 && x <=185 && y<=161+4*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+4),kinds[4]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=4)//当鼠标左键单击时
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

    //5 玩具熊猫
    if(x>=70 && y>=140+5*55 && x <=185 && y<=161+5*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+5),kinds[5]);
        endPaint();
        if(button = 1 && event == 0 &&kind1!=5)//当鼠标左键单击时
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

    //6 吹泡泡双鱼
    if(x>=70 && y>=140+6*55 && x <=208 && y<=161+6*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+6),kinds[6]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=6)//当鼠标左键单击时
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

    //7 绿蜘蛛
    if(x>=70 && y>=140+7*55 && x <=164 && y<=161+7*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100,80+55*(1+7),kinds[7]);
        endPaint();
        if(button = 1 && event == 0&& kind1!=7)//当鼠标左键单击时
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

    //0 花蘑菇
    if(x>=70+400 && y>=140 && x <=164+400 && y<=161)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(0+1),kinds[0]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=0)//当鼠标左键单击时
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

    //1 绿水灵
    if(x>=70+400 && y>=140+1*55 && x <=164+400 && y<=161+1*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+1),kinds[1]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=1)//当鼠标左键单击时
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

    //2 猪猪
    if(x>=70+400 && y>=140+2*55 && x <=145+400 && y<=161+2*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+2),kinds[2]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=2)//当鼠标左键单击时
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

    //3 三眼章鱼
    if(x>=70+400 && y>=140+3*55 && x <=185+400 && y<=161+3*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+3),kinds[3]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=3)//当鼠标左键单击时
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

    //4 玩具粉熊
    if(x>=70+400 && y>=140+4*55 && x <=185+400 && y<=161+4*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+4),kinds[4]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=4)//当鼠标左键单击时
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

    //5 玩具熊猫
    if(x>=70+400 && y>=140+5*55 && x <=185+400 && y<=161+5*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+5),kinds[5]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=5)//当鼠标左键单击时
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

    //6 吹泡泡双鱼
    if(x>=70+400 && y>=140+6*55 && x <=208+400 && y<=161+6*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+6),kinds[6]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=6)//当鼠标左键单击时
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

    //7 绿蜘蛛
    if(x>=70+400 && y>=140+7*55 && x <=164+400 && y<=161+7*55)
    {
        beginPaint();
        setTextColor(YELLOW);
        paintText( 100+400,80+55*(1+7),kinds[7]);
        endPaint();
        if(button = 1 && event == 0 && kind0!=7)//当鼠标左键单击时
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
            whoFirst = 0;//1号玩家先手
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
            whoFirst = 1;//2号玩家/电脑先手
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
    resi = 0;//落点的横坐标
    resj = 0;//落点的纵坐标
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
            //双人对战模式
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
                    if(round == 225) //平局
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,0)!=0)
                    {
                        printf("玩家1获胜!\n");
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
                    if(round == 225) //平局
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,1)!=0)
                    {
                        printf("玩家2获胜!\n");
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
    resi = 0;//落点的横坐标
    resj = 0;//落点的纵坐标
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
            //人机模式
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
                    //startTimer(1,500); 人机模式你刚刚下的不需要闪烁
                    chess[j][i] = 0;
                    //printf("%d :%d %d\n",whoFirst+round,i,j);
                    round++;
                    if(round == 225) //平局
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,0)!=0)
                    {
                        printf("玩家1获胜!\n");
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
                    if(round == 225) //平局
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,1)!=0)
                    {
                        printf("玩家2获胜!\n");
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
    //电脑下棋
      if(button == 1 && event == 2 && (round+whoFirst)%2 == 1){//需要判断是否下棋成功

        Robot(&j, &i, &round+1) ;
        printf("%d %d\n",j,i);
        beginPaint();
        putImageScale(&player1,xo[i]-15,yo[j]-15,30,30);
        tempResi = xo[i];
        tempResj = yo[j];
        startTimer(1,500);
        round++;
         if(round == 225) //平局
                    {
                        loadImage("image/draw.jpg",&draw);
                        putImageTransparent(&draw,226,268,170,97,WHITE);
                        gameStat = 1;
                        startTimer(0,5000);
                    }
                    else if(isWin(j,i,1)!=0)
                    {
                        printf("电脑获胜!\n");
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

//人机模式
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
    paintText(570,0,"玩家:");
    paintText(570,120,"电脑:");
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

//玩家0 gif顺序应为 player01 player0 player02
//玩家1 gif顺序应为 player11 player1 player12

    putImageScale(&player0,650,50,50,50);
    putImageScale(&player1,650,170,50,50);
    round = 0;

    registerMouseEvent(mouseListener3);
    endPaint();
}

//双人模式
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
    paintText(570,0,"玩家1:");
    paintText(570,120,"玩家2:");
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

//玩家0 gif顺序应为 player01 player0 player02
//玩家1 gif顺序应为 player11 player1 player12

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
        //round已经累加过
        if((whoFirst+round)%2 == 1) //0号玩家
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
        else if((whoFirst+round)%2 == 0) //1号玩家
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
        chess[*x][*y] = tp;   //记录这个点
        // printpart(*x, *y, tp);     //打印出电脑AI第一次落子
        isTouch = 1;
        tp++;
        tp = tp % 2;
    }
    else      //从第2步开始，使用评分系统
    {
        Findscore(x, y);
    }
}


void Findscore(int *x, int *y)   //查找评分最高的坐标
{
    srand((unsigned)time(NULL));
    int i, j, x1, x2, y1, y2, lx;
    int Max = 0;
    ChessScore();      //调用评分函数
    for (i = 0; i<15; i++)
    {
        for (j = 0; j<15; j++)
        {
            if (Score[i][j]>Max)
            {
                Max = Score[i][j];  //获取所有点中，评分最高的
                x1 = i;
                y1 = j;
            }
        }
    }
    x2 = x1;
    y2 = y1;
    for (i = 0; i<15; i++)    //可能的话，有评分相同的多个点
    {
        for (j = 0; j<15; j++)
        {
            if (Score[i][j] == Max&&i != x2&&j != y2)   //在这么多个相同分数的点中，随机找一个
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
    if (x2 != x1 || y2 != y1)   //棋盘上有2个最高分
    {
        lx = rand() % 10;    //随机一个
        if (lx>6)
        {
            *x = x1, *y = y1;
        }
        else
        {
            *x = x2, *y = y2;
        }
    }
    else    //棋盘上只有一个最高分
    {
        *x = x1, *y = y1;
    }
    Max = 0;    //清空最大值
    chess[*x][*y] = tp;       //记录这个点
    //printpart(x, y, tp);    //打印出电脑AI落子
    if (winerValue==2)
    {
        isTouch = 1;
    }

    tp++;
    tp = tp % 2;
}


void ChessOne(int *x, int *y)   //玩家走第1步时的落子
{
    int i, j;
    srand((unsigned)time(NULL));    //随机数随着时间的改变而改变
    for (i = 0; i<15; i++)
    {
        for (j = 0; j<15; j++)
        {
            if (chess[i][j] == 0)  //如果找到了玩家的棋子，在它的8个方的任意一点落子
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
                    *y = j;    //上
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 5)
                {
                    *x = i;
                    *y = j - 1;   //左
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else if (lx == 6)
                {
                    *x = i;
                    *y = j + 1;   //右
                    if (chess[*x][*y] == -1)
                    {
                        break;
                    }
                }
                else
                {
                    *x = i + 1;
                    *y = j;    //下
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
    int x, y, i, j, k;      //循环变量
    int number1 = 0, number2 = 0;   //number用来统计玩家或电脑棋子连成个数
    int empty = 0;    //empty用来统计空点个数
    memset(Score, 0, sizeof(Score));                    //把评分数组先清零
    for (x = 0; x<15; x++)
    {
        for (y = 0; y<15; y++)
        {
            if (chess[x][y] == -1)    //如果这个点为空
            {
                for (i = -1; i <= 1; i++)
                {
                    for (j = -1; j <= 1; j++)   //判断8个方向
                    {
                        if (i != 0 || j != 0)   //若是都为0的话，那不就是原坐标嘛
                        {
                            //对玩家落点评分
                            for (k = 1; i <= 4; k++)   //循环4次
                            {
                                //这点没越界  且这点存在黑子（玩家）
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 0)
                                {
                                    number1++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)     //这点是个空点，+1后退出
                                {
                                    empty++;
                                    break;
                                }
                                else                    //否则是墙或者对方的棋子了
                                {
                                    break;
                                }
                            }
                            for (k = -1; k >= -4; k--)            //向它的相反方向判断
                            {
                                //这点没越界  且这点存在黑子（玩家）
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 0)
                                {
                                    number1++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)     //这点是个空点，+1后退出
                                {
                                    empty++;
                                    break;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if (number2 == 1)   //2个棋子
                            {
                                Score[x][y] += 1;
                            }
                            else if (number1 == 2)   //3个棋子
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 5;   //有一个空点+5分 死3
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 10;  //有两个空点+10分 活3
                                }
                            }
                            else if (number1 == 3)   //4个棋子
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 20;  //有一个空点+20分 死4
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 100;  //有2个空点+100分 活4
                                }
                            }
                            else if (number1 >= 4)
                            {
                                Score[x][y] += 1000;  //对方有5个棋子，分数要高点，先堵
                            }

                            empty = 0;   //统计空点个数的变量清零

                            //对电脑落点评分
                            for (k = 1; i <= 4; k++)   //循环4次
                            {
                                //这点没越界  且这点存在白子（电脑）
                                if (x + k*i >= 0 && x + k*i <= 14 &&
                                        y + k*j >= 0 && y + k*j <= 14 &&
                                        chess[x + k*i][y + k*j] == 1)
                                {
                                    number2++;
                                }
                                else if (chess[x + k*i][y + k*j] == -1)
                                {
                                    empty++;
                                    break;   //空点
                                }
                                else
                                {
                                    break;
                                }
                            }
                            for (k = -1; k >= -4; k--)   //向它的相反方向判断
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
                                    break;   //注释与上面玩家版相同
                                }
                            }
                            if (number2 == 0)
                            {
                                Score[x][y] += 1;    //1个棋子
                            }
                            else if (number2 == 1)
                            {
                                Score[x][y] += 2;    //2个棋子
                            }
                            else if (number2 == 2)   //3个棋子
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 8;  //死3
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 30;  //活3
                                }
                            }
                            else if (number2 == 3)   //4个棋子
                            {
                                if (empty == 1)
                                {
                                    Score[x][y] += 50;   //死4
                                }
                                else if (empty == 2)
                                {
                                    Score[x][y] += 200;   //活4
                                }
                            }
                            else if (number2 >= 4)
                            {
                                Score[x][y] += 10000;   //自己落在这点能形成5个，也就能胜利了，分数最高
                            }

                            number1 = 0;     //清零，以便下次重新统计
                            number2 = 0;
                            empty = 0;
                        }
                    }
                }
            }
        }
    }
}
