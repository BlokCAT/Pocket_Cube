#include<GL/glut.h>      
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<iostream>
#include <cstring>
#include <queue>

using namespace std;

int f1, f2, f3, f4, f5, f6, f_allx, f_ally, cnt = 0;//cnt记录bfs深度,其余控制动画效果

const float PI = 3.141592653589793f;

const float ag = (PI / 18);//一个面每帧转角度

const float clr = 0;//魔方底板色调

float R = 0.496f, G = 0.769f, B = 1.0f;//视图背景色

int CHANGE = 1, idx, stop, Flag, OK, Angle;//改变方向

char Path_Record[100010];

float lookx = 70, lookz = -70;//初始相机位置

bool LEFT_isDragging = 0;//检测是否左键拖动

bool RIGHT_isDragging = 0;//检测是否右键拖动

float rot = 0.07f;//相机每次旋转的角度








//定义“魔方”
struct Cube
{
    int a[25];
    queue <char> step;//step记录当前状态的魔方的来源
}cube, Aim_Cube;

queue<Cube> Queue_of_Bfs;


//函数申明
void AUTOMATIC_RECOVERY(int idx);
bool CHECK_ALL(Cube t);
void inite_cube_color();
void BFS();
void onKeyboard(unsigned char key, int x, int y);
void mouseCallback(int button, int state, int x, int y);





/*********************初始化***********************/



void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, 1, 1, 600);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(lookx, 70, lookz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

}

//初始魔方颜色数据
//6黄色  1绿色  2红色  3蓝色    4橙色   5白色
void inite_cube_color()
{
    for (int i = 1; i <= 4; i++)  cube.a[i] = 6;
    for (int i = 5; i <= 8; i++)  cube.a[i] = 4;
    for (int i = 9; i <= 12; i++)  cube.a[i] = 1;
    for (int i = 13; i <= 16; i++)  cube.a[i] = 2;
    for (int i = 17; i <= 20; i++)  cube.a[i] = 3;
    for (int i = 21; i <= 24; i++)  cube.a[i] = 5;
}



GLfloat red[8][4][3] = {
    //4个底板
    -20 , 20 , -20,
    0 , 20 , -20,
    0 , 0 , -20,
    -20 , 0 , -20,

    0 , 20 , -20,
    20 , 20 , -20,
    20 , 0 , -20,
    0 , 0 , -20,

    0 , 0 , -20,
    20 , 0 , -20,
    20 , -20 , -20,
    0 , -20 , -20,

    -20 , 0 , -20,
    0 , 0 , -20,
    0 , -20 , -20,
    -20 , -20 , -20,




    //红色face


     -19 , 19 , -21,
    -1 , 19 , -21,
    -1 , 1 ,-21,
    -19 , 1 , -21,

    1 , 19 , -21,
    19 , 19 , -21,
    19 , 1 , -21,
    1 , 1 , -21,

    1 , -1 , -21,
    19 , -1 , -21,
    19 , -19 , -21,
    1 , -19 ,-21,

    -19 , -1 , -21,
    -1 , -1 , -21,
    -1 , -19 , -21,
    -19 , -19 , -21,
};
GLfloat green[8][4][3] = {

    //四个底板
    20, 20 , -20,
    20 , 20 , 0,
    20 , 0 , 0,
    20 , 0 , -20,

    20 , 20 , 0,
    20 , 20 , 20 ,
    20 , 0 , 20 ,
    20 , 0 , 0 ,

    20 , 0 , 0,
    20 , 0 , 20 ,
    20 , -20 , 20,
    20 , -20 , 0 ,

    20 , 0 , -20,
    20 , 0 , 0,
    20 , -20 , 0,
    20 , -20 , -20,



    //色块

    21, 19 , -19,
    21 , 19 , -1,
    21 , 1 , -1,
    21 , 1 , -19,

    21 , 19 , 1,
    21 , 19 , 19 ,
    21 , 1 , 19 ,
    21 , 1 , 1 ,

    21 , -1 , 1,
    21 , -1 , 19 ,
    21 , -19 , 19,
    21 , -19 , 1 ,

    21 , -1 , -19,
    21 , -1 , -1,
    21 , -19 , -1,
    21 , -19 , -19


};
GLfloat orange[8][4][3] = {


    //底板

    20 , 20 , 20 ,
    0 , 20 , 20,
    0 , 0 , 20,
    20 , 0 , 20,

    0 , 20 , 20,
    -20 , 20 , 20,
    -20 , 0 , 20,
    0 , 0 , 20,

    0 , 0 , 20 ,
    -20 ,0 , 20 ,
    -20 , -20 , 20,
    0 , -20  , 20,

    20 , 0 , 20,
    0 , 0 , 20,
    0 , -20 , 20,
    20 , -20 , 20,



    //色块


    19 , 19 , 21 ,
    1 , 19 , 21,
    1 , 1 , 21,
    19 , 1 , 21,

    -1 , 19 , 21,
    -19 , 19 , 21,
    -19 , 1 , 21,
    -1 , 1 , 21,

    -1 , -1 , 21 ,
    -19 ,-1 , 21 ,
    -19 , -19 , 21,
    -1 , -19  , 21,

    19 , -1 , 21,
    1 , -1 , 21,
    1 , -19 , 21,
    19 , -19 , 21
};
GLfloat blue[8][4][3] = {


    //底板

    -20 , 20 , 20,
    -20 , 20 , 0 ,
    -20 , 0 , 0,
    -20 , 0 , 20,

    -20 , 20 , 0 ,
    -20 , 20 , -20,
    -20 , 0 , -20,
    -20, 0 , 0 ,

    -20 , 0 , 0,
    -20 , 0 , -20,
    -20 , -20 , -20 ,
    -20 , -20 , 0,

    -20 , 0 , 20 ,
    -20 , 0 , 0,
    -20 , -20 , 0 ,
    -20 , -20 , 20,



    //色块

    -21 , 19 , 19,
    -21 , 19 , 1 ,
    -21 , 1 , 1,
    -21 , 1 , 19,

    -21 , 19 , -1 ,
    -21 , 19 , -19,
    -21 , 1 , -19,
    -21, 1 , -1 ,

    -21 , -1 , -1,
    -21 , -1 , -19,
    -21 , -19 , -19 ,
    -21 , -19 , -1,

    -21 , -1 , 19 ,
    -21 , -2 , 1,
    -21 , -19 , 1 ,
    -21 , -19 , 19

};
GLfloat white[8][4][3] = {



    //底板

    -20 , -20 , 20 ,
    0 , -20 , 20,
    0 , -20 , 0 ,
    -20 ,-20 , 0,

    0 , -20 , 20,
    20 , -20 , 20,
    20 , -20 , 0,
    0 , -20 , 0,

    0 , -20 , 0,
    20 , -20 ,0,
    20 , -20 , -20,
    0 , -20 , -20,

    -20 , -20 , 0,
    0 , -20 , 0,
    0 , -20 , -20,
    -20 , -20 ,-20,



    //色块



   -19 , -21 , 19 ,
   -1 , -21 , 19,
   -1 , -21 , 1 ,
   -19 ,-21 , 1,

   1 , -21 , 19,
   19 , -21 , 19,
   19 , -21 , 1,
   1 , -21 , 1,

   1 , -21 , -1,
   19 , -21 ,-1,
   19 , -21 , -19,
   1 , -21 , -19,

   -19 , -21 , -1,
   -1 , -21 , -1,
   -1 , -21 , -19,
   -19 , -21 ,-19


};
GLfloat yellow[8][4][3] = {

    //底板
    -20 , 20 , 20,
    0 , 20 , 20,
    0 , 20 , 0,
    -20 , 20 ,0,

    0 , 20 , 20 ,
    20 , 20 , 20 ,
    20 ,20 ,0,
    0 , 20 , 0 ,

    0 , 20 , 0 ,
    20 , 20 , 0,
    20 , 20 , -20,
    0 , 20 , -20,

    -20 , 20 ,0 ,
    0 , 20 , 0,
    0 , 20 , -20 ,
    -20 , 20 , -20,


    //色块
   -19 , 21 , 19,
   -1 , 21 , 19,
   -1 , 21 , 1,
   -19 , 21 ,1,

   1 , 21 , 19 ,
   19 , 21 , 19 ,
   19 ,21 ,1,
   1 , 21 , 1 ,

   1 , 21 , -1 ,
   19 , 21 , -1,
   19 , 21 , -19,
   1 , 21 , -19,

   -19 , 21 ,-1 ,
   -1 , 21 , -1,
   -1 , 21 , -19 ,
   -19 , 21 , -19
};
GLfloat color[6][3] = {
    1 , 0 , 0,
    0 , 1 , 0,
    1 , 0.7 ,0,
    0 , 0 , 1,
    1 , 1, 1,
    1 , 1, 0
};




/*********************颜色数组在各阶段的变化***********************/


//自动复原前 ，颜色矩阵的6个操作
void move_F()
{
    Cube t = cube;
    cube.a[9] = t.a[10];
    cube.a[12] = t.a[9];
    cube.a[11] = t.a[12];
    cube.a[4] = t.a[13];
    cube.a[3] = t.a[16];
    cube.a[6] = t.a[3];
    cube.a[7] = t.a[4];
    cube.a[24] = t.a[6];
    cube.a[23] = t.a[7];
    cube.a[16] = t.a[24];
    cube.a[13] = t.a[23];
    cube.a[10] = t.a[11];

}
void move_R()
{
    Cube t = cube;
    cube.a[13] = t.a[16];
    cube.a[16] = t.a[15];
    cube.a[15] = t.a[14];
    cube.a[14] = t.a[13];
    cube.a[3] = t.a[11];
    cube.a[2] = t.a[10];
    cube.a[10] = t.a[23];
    cube.a[11] = t.a[22];
    cube.a[23] = t.a[20];
    cube.a[22] = t.a[17];
    cube.a[20] = t.a[2];
    cube.a[17] = t.a[3];
}
void move_U()
{
    Cube t = cube;
    cube.a[1] = t.a[4];
    cube.a[4] = t.a[3];
    cube.a[3] = t.a[2];
    cube.a[2] = t.a[1];
    cube.a[10] = t.a[14];
    cube.a[9] = t.a[13];
    cube.a[13] = t.a[17];
    cube.a[14] = t.a[18];
    cube.a[17] = t.a[5];
    cube.a[18] = t.a[6];
    cube.a[5] = t.a[9];
    cube.a[6] = t.a[10];
}
void move_D()
{
    Cube t = cube;
    cube.a[24] = t.a[23];
    cube.a[23] = t.a[22];
    cube.a[22] = t.a[21];
    cube.a[21] = t.a[24];
    cube.a[12] = t.a[16];
    cube.a[11] = t.a[15];
    cube.a[16] = t.a[20];
    cube.a[15] = t.a[19];
    cube.a[20] = t.a[8];
    cube.a[19] = t.a[7];
    cube.a[8] = t.a[12];
    cube.a[7] = t.a[11];
}
void move_L()
{
    Cube t = cube;
    cube.a[5] = t.a[6];
    cube.a[6] = t.a[7];
    cube.a[7] = t.a[8];
    cube.a[8] = t.a[5];
    cube.a[1] = t.a[9];
    cube.a[4] = t.a[12];
    cube.a[9] = t.a[24];
    cube.a[12] = t.a[21];
    cube.a[24] = t.a[19];
    cube.a[21] = t.a[18];
    cube.a[19] = t.a[1];
    cube.a[18] = t.a[4];
}
void move_B()
{
    Cube t = cube;
    cube.a[18] = t.a[17];
    cube.a[19] = t.a[18];
    cube.a[20] = t.a[19];
    cube.a[17] = t.a[20];
    cube.a[1] = t.a[14];
    cube.a[2] = t.a[15];
    cube.a[14] = t.a[22];
    cube.a[15] = t.a[21];
    cube.a[22] = t.a[8];
    cube.a[21] = t.a[5];
    cube.a[8] = t.a[1];
    cube.a[5] = t.a[2];
}




//自动复原后 ，颜色矩阵的6个操作
void AT_move_F(Cube t)
{
    Cube tmp = t;
    tmp.a[9] = t.a[10];
    tmp.a[12] = t.a[9];
    tmp.a[11] = t.a[12];
    tmp.a[4] = t.a[13];
    tmp.a[3] = t.a[16];
    tmp.a[6] = t.a[3];
    tmp.a[7] = t.a[4];
    tmp.a[24] = t.a[6];
    tmp.a[23] = t.a[7];
    tmp.a[16] = t.a[24];
    tmp.a[13] = t.a[23];
    tmp.a[10] = t.a[11];

    tmp.step.push('F');
    if (CHECK_ALL(tmp))
    {
        OK = 1;
        Aim_Cube = tmp;
    }
    Queue_of_Bfs.push(tmp);

}
void AT_move_R(Cube t)
{
    Cube tmp = t;
    tmp.a[13] = t.a[16];
    tmp.a[16] = t.a[15];
    tmp.a[15] = t.a[14];
    tmp.a[14] = t.a[13];
    tmp.a[3] = t.a[11];
    tmp.a[2] = t.a[10];
    tmp.a[10] = t.a[23];
    tmp.a[11] = t.a[22];
    tmp.a[23] = t.a[20];
    tmp.a[22] = t.a[17];
    tmp.a[20] = t.a[2];
    tmp.a[17] = t.a[3];

    tmp.step.push('R');
    if (CHECK_ALL(tmp))
    {
        OK = 1;
        Aim_Cube = tmp;
    }
    Queue_of_Bfs.push(tmp);

}
void AT_move_U(Cube t)
{
    Cube tmp = t;
    tmp.a[1] = t.a[4];
    tmp.a[4] = t.a[3];
    tmp.a[3] = t.a[2];
    tmp.a[2] = t.a[1];
    tmp.a[10] = t.a[14];
    tmp.a[9] = t.a[13];
    tmp.a[13] = t.a[17];
    tmp.a[14] = t.a[18];
    tmp.a[17] = t.a[5];
    tmp.a[18] = t.a[6];
    tmp.a[5] = t.a[9];
    tmp.a[6] = t.a[10];

    tmp.step.push('U');
    if (CHECK_ALL(tmp))
    {
        OK = 1;
        Aim_Cube = tmp;
    }
    Queue_of_Bfs.push(tmp);
}
void AT_move_D(Cube t)
{
    Cube tmp = t;
    tmp.a[24] = t.a[23];
    tmp.a[23] = t.a[22];
    tmp.a[22] = t.a[21];
    tmp.a[21] = t.a[24];
    tmp.a[12] = t.a[16];
    tmp.a[11] = t.a[15];
    tmp.a[16] = t.a[20];
    tmp.a[15] = t.a[19];
    tmp.a[20] = t.a[8];
    tmp.a[19] = t.a[7];
    tmp.a[8] = t.a[12];
    tmp.a[7] = t.a[11];

    tmp.step.push('D');
    if (CHECK_ALL(tmp))
    {
        OK = 1;
        Aim_Cube = tmp;
    }
    Queue_of_Bfs.push(tmp);
}
void AT_move_L(Cube t)
{
    Cube tmp = t;
    tmp.a[5] = t.a[6];
    tmp.a[6] = t.a[7];
    tmp.a[7] = t.a[8];
    tmp.a[8] = t.a[5];
    tmp.a[1] = t.a[9];
    tmp.a[4] = t.a[12];
    tmp.a[9] = t.a[24];
    tmp.a[12] = t.a[21];
    tmp.a[24] = t.a[19];
    tmp.a[21] = t.a[18];
    tmp.a[19] = t.a[1];
    tmp.a[18] = t.a[4];

    tmp.step.push('L');
    if (CHECK_ALL(tmp))
    {
        OK = 1;
        Aim_Cube = tmp;
    }
    Queue_of_Bfs.push(tmp);
}
void AT_move_B(Cube t)
{
    Cube tmp = t;
    tmp.a[18] = t.a[17];
    tmp.a[19] = t.a[18];
    tmp.a[20] = t.a[19];
    tmp.a[17] = t.a[20];
    tmp.a[1] = t.a[14];
    tmp.a[2] = t.a[15];
    tmp.a[14] = t.a[22];
    tmp.a[15] = t.a[21];
    tmp.a[22] = t.a[8];
    tmp.a[21] = t.a[5];
    tmp.a[8] = t.a[1];
    tmp.a[5] = t.a[2];

    tmp.step.push('B');
    if (CHECK_ALL(tmp))
    {
        OK = 1;
        Aim_Cube = tmp;
    }
    Queue_of_Bfs.push(tmp);
}
//void AT_move_f(Cube t)
//{
//    Cube tmp = t;
//    tmp.a[10] = t.a[9];
//    tmp.a[9] = t.a[12];
//    tmp.a[12] = t.a[11];
//    tmp.a[13] = t.a[4];
//    tmp.a[16] = t.a[3];
//    tmp.a[3] = t.a[6];
//    tmp.a[4] = t.a[7];
//    tmp.a[6] = t.a[24];
//    tmp.a[7] = t.a[23];
//    tmp.a[24] = t.a[16];
//    tmp.a[23] = t.a[13];
//    tmp.a[11] = t.a[10];
//
//    tmp.step.push('f');
//    if (CHECK_ALL(tmp))
//    {
//        OK = 1;
//        Aim_Cube = tmp;
//    }
//    q.push(tmp);
//
//}
//void AT_move_r(Cube t)
//{
//    Cube tmp = t;
//    tmp.a[16] = t.a[13];
//    tmp.a[15] = t.a[16];
//    tmp.a[14] = t.a[15];
//    tmp.a[13] = t.a[14];
//    tmp.a[11] = t.a[3];
//    tmp.a[10] = t.a[2];
//    tmp.a[23] = t.a[10];
//    tmp.a[22] = t.a[11];
//    tmp.a[20] = t.a[23];
//    tmp.a[17] = t.a[22];
//    tmp.a[2] = t.a[20];
//    tmp.a[3] = t.a[17];
//
//    tmp.step.push('r');
//    if (CHECK_ALL(tmp))
//    {
//        OK = 1;
//        Aim_Cube = tmp;
//    }
//    q.push(tmp);
//
//}
//void AT_move_u(Cube t)
//{
//    Cube tmp = t;
//    tmp.a[4] = t.a[1];
//    tmp.a[3] = t.a[4];
//    tmp.a[2] = t.a[3];
//    tmp.a[1] = t.a[2];
//    tmp.a[14] = t.a[10];
//    tmp.a[13] = t.a[9];
//    tmp.a[17] = t.a[13];
//    tmp.a[18] = t.a[14];
//    tmp.a[5] = t.a[17];
//    tmp.a[6] = t.a[18];
//    tmp.a[9] = t.a[5];
//    tmp.a[10] = t.a[6];
//
//    tmp.step.push('u');
//    if (CHECK_ALL(tmp))
//    {
//        OK = 1;
//        Aim_Cube = tmp;
//    }
//    q.push(tmp);
//}
//void AT_move_d(Cube t)
//{
//    Cube tmp = t;
//    tmp.a[23] = t.a[24];
//    tmp.a[22] = t.a[23];
//    tmp.a[21] = t.a[22];
//    tmp.a[24] = t.a[21];
//    tmp.a[16] = t.a[12];
//    tmp.a[15] = t.a[11];
//    tmp.a[10] = t.a[16];
//    tmp.a[19] = t.a[15];
//    tmp.a[8] = t.a[10];
//    tmp.a[7] = t.a[19];
//    tmp.a[12] = t.a[8];
//    tmp.a[11] = t.a[7];
//
//    tmp.step.push('d');
//    if (CHECK_ALL(tmp))
//    {
//        OK = 1;
//        Aim_Cube = tmp;
//    }
//    q.push(tmp);
//}
//void AT_move_l(Cube t)
//{
//    Cube tmp = t;
//    tmp.a[5] = t.a[6];
//    tmp.a[6] = t.a[7];
//    tmp.a[7] = t.a[8];
//    tmp.a[8] = t.a[5];
//    tmp.a[1] = t.a[9];
//    tmp.a[4] = t.a[12];
//    tmp.a[9] = t.a[24];
//    tmp.a[12] = t.a[21];
//    tmp.a[24] = t.a[19];
//    tmp.a[21] = t.a[18];
//    tmp.a[19] = t.a[1];
//    tmp.a[18] = t.a[4];
//
//    tmp.step.push('l');
//    if (CHECK_ALL(tmp))
//    {
//        OK = 1;
//        Aim_Cube = tmp;
//    }
//    q.push(tmp);
//}
//void AT_move_b(Cube t)
//{
//    Cube tmp = t;
//    tmp.a[17] = t.a[18];
//    tmp.a[18] = t.a[19];
//    tmp.a[19] = t.a[10];
//    tmp.a[10] = t.a[17];
//    tmp.a[14] = t.a[1];
//    tmp.a[15] = t.a[2];
//    tmp.a[22] = t.a[14];
//    tmp.a[21] = t.a[15];
//    tmp.a[8] = t.a[22];
//    tmp.a[5] = t.a[21];
//    tmp.a[1] = t.a[8];
//    tmp.a[2] = t.a[5];
//
//    tmp.step.push('b');
//    if (CHECK_ALL(tmp))
//    {
//        OK = 1;
//        Aim_Cube = tmp;
//    }
//    q.push(tmp);
//}


//小范围搜索最优解
void BFS_FIND_RECOVER_PATH()
{
    if (OK == 1 || cnt > 600) return;
    cnt++;

    Cube tt = Queue_of_Bfs.front();
    Queue_of_Bfs.pop();

    AT_move_F(tt);
    AT_move_R(tt);
    AT_move_U(tt);
    AT_move_D(tt);
    AT_move_B(tt);
    AT_move_L(tt);

    BFS_FIND_RECOVER_PATH();

}



/*********************判断是否是目标角块***********************/

bool CHECK_1(GLfloat a[][4][3], int i)
{
    for (int j = 0; j < 4; j++)
        if (a[i][j][2] > 0.1) return false;
    return true;
}
bool CHECK_2(GLfloat a[][4][3], int i)
{
    for (int j = 0; j < 4; j++)
        if (a[i][j][0] < -0.1)  return false;
    return true;
}
bool CHECK_3(GLfloat a[][4][3], int i)
{
    for (int j = 0; j < 4; j++)
        if (a[i][j][1] < -0.1)  return false;
    return true;
}
bool CHECK_4(GLfloat a[][4][3], int i)
{
    for (int j = 0; j < 4; j++)
        if (a[i][j][1] > 0.1)  return false;
    return true;
}
bool CHECK_5(GLfloat a[][4][3], int i)
{
    for (int j = 0; j < 4; j++)
        if (a[i][j][2] < -0.1)  return false;
    return true;
}
bool CHECK_6(GLfloat a[][4][3], int i)
{
    for (int j = 0; j < 4; j++)
        if (a[i][j][0] > 0.1)  return false;
    return true;
}
bool CHECK_ALL(Cube t)
{
    if (t.a[1] == t.a[2] && t.a[1] == t.a[3] && t.a[1] == t.a[4] &&
        t.a[9] == t.a[10] && t.a[9] == t.a[11] && t.a[9] == t.a[12] &&
        t.a[5] == t.a[6] && t.a[5] == t.a[7] && t.a[5] == t.a[8] &&
        t.a[13] == t.a[14] && t.a[13] == t.a[15] && t.a[13] == t.a[16] &&
        t.a[17] == t.a[18] && t.a[17] == t.a[19] && t.a[17] == t.a[20] &&
        t.a[21] == t.a[22] && t.a[21] == t.a[23] && t.a[21] == t.a[24])
        return true;
    else return false;
}


//处理键盘鼠标事件，实现旋转效果
void turn_fR(int trash)
{
    if (f1 < 9)
    {
        //红色变
        for (int i = 0; i < 8; i++)
            if (CHECK_1(red, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = red[i][j][0];
                    GLfloat ty = red[i][j][1];
                    red[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    red[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //绿色变
        for (int i = 0; i < 8; i++)
            if (CHECK_1(green, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = green[i][j][0];
                    GLfloat ty = green[i][j][1];
                    green[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    green[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //橙色变
        for (int i = 0; i < 8; i++)
            if (CHECK_1(orange, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = orange[i][j][0];
                    GLfloat ty = orange[i][j][1];
                    orange[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    orange[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //蓝色变
        for (int i = 0; i < 8; i++)
            if (CHECK_1(blue, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = blue[i][j][0];
                    GLfloat ty = blue[i][j][1];
                    blue[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    blue[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //白色变
        for (int i = 0; i < 8; i++)
            if (CHECK_1(white, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = white[i][j][0];
                    GLfloat ty = white[i][j][1];
                    white[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    white[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //黄色变
        for (int i = 0; i < 8; i++)
            if (CHECK_1(yellow, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = yellow[i][j][0];
                    GLfloat ty = yellow[i][j][1];
                    yellow[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    yellow[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }
        glutPostRedisplay();
        glutTimerFunc(10, turn_fR, 1);
        f1++;
    }
    else {
        f1 = 0;
        if (Flag)  AUTOMATIC_RECOVERY(++idx);
    }
}
void turn_fF(int trash)
{
    if (f2 < 9)
    {
        //红色变
        for (int i = 0; i < 8; i++)
            if (CHECK_2(red, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = red[i][j][1];
                    GLfloat tz = red[i][j][2];
                    red[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    red[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //绿色变
        for (int i = 0; i < 8; i++)
            if (CHECK_2(green, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = green[i][j][1];
                    GLfloat tz = green[i][j][2];
                    green[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    green[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //橙色变
        for (int i = 0; i < 8; i++)
            if (CHECK_2(orange, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = orange[i][j][1];
                    GLfloat tz = orange[i][j][2];
                    orange[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    orange[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //蓝色变
        for (int i = 0; i < 8; i++)
            if (CHECK_2(blue, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = blue[i][j][1];
                    GLfloat tz = blue[i][j][2];
                    blue[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    blue[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //白色变
        for (int i = 0; i < 8; i++)
            if (CHECK_2(white, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = white[i][j][1];
                    GLfloat tz = white[i][j][2];
                    white[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    white[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //黄色变
        for (int i = 0; i < 8; i++)
            if (CHECK_2(yellow, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = yellow[i][j][1];
                    GLfloat tz = yellow[i][j][2];
                    yellow[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    yellow[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        glutPostRedisplay();
        glutTimerFunc(10, turn_fF, 1);
        f2++;


    }
    else {
        f2 = 0;
        if (Flag) AUTOMATIC_RECOVERY(++idx);
    }
}
void turn_fU(int trash)
{
    if (f3 < 9)
    {
        //红色变
        for (int i = 0; i < 8; i++)
            if (CHECK_3(red, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = red[i][j][0];
                    GLfloat tz = red[i][j][2];
                    red[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    red[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_3(green, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = green[i][j][0];
                    GLfloat tz = green[i][j][2];
                    green[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    green[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_3(orange, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = orange[i][j][0];
                    GLfloat tz = orange[i][j][2];
                    orange[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    orange[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_3(blue, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = blue[i][j][0];
                    GLfloat tz = blue[i][j][2];
                    blue[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    blue[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_3(white, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = white[i][j][0];
                    GLfloat tz = white[i][j][2];
                    white[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    white[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_3(yellow, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = yellow[i][j][0];
                    GLfloat tz = yellow[i][j][2];
                    yellow[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    yellow[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }
        glutPostRedisplay();
        glutTimerFunc(10, turn_fU, 1);
        f3++;

    }
    else {
        f3 = 0;
        if (Flag) AUTOMATIC_RECOVERY(++idx);
    }
}
void turn_fD(int trash)
{
    if (f4 < 9)
    {
        //红色变
        for (int i = 0; i < 8; i++)
            if (CHECK_4(red, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = red[i][j][0];
                    GLfloat tz = red[i][j][2];
                    red[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    red[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_4(green, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = green[i][j][0];
                    GLfloat tz = green[i][j][2];
                    green[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    green[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_4(orange, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = orange[i][j][0];
                    GLfloat tz = orange[i][j][2];
                    orange[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    orange[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_4(blue, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = blue[i][j][0];
                    GLfloat tz = blue[i][j][2];
                    blue[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    blue[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_4(white, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = white[i][j][0];
                    GLfloat tz = white[i][j][2];
                    white[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    white[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        for (int i = 0; i < 8; i++)
            if (CHECK_4(yellow, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = yellow[i][j][0];
                    GLfloat tz = yellow[i][j][2];
                    yellow[i][j][0] = (tx * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    yellow[i][j][2] = ((tx * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }
        glutPostRedisplay();
        glutTimerFunc(10, turn_fD, 1);
        f4++;

    }
    else
    {
        f4 = 0;
        if (Flag) AUTOMATIC_RECOVERY(++idx);
    }
}
void turn_fL(int trash)
{
    if (f5 < 9)
    {

        //红色变
        for (int i = 0; i < 8; i++)
            if (CHECK_5(red, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = red[i][j][0];
                    GLfloat ty = red[i][j][1];
                    red[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    red[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //绿色变
        for (int i = 0; i < 8; i++)
            if (CHECK_5(green, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = green[i][j][0];
                    GLfloat ty = green[i][j][1];
                    green[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    green[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //橙色变
        for (int i = 0; i < 8; i++)
            if (CHECK_5(orange, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = orange[i][j][0];
                    GLfloat ty = orange[i][j][1];
                    orange[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    orange[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //蓝色变
        for (int i = 0; i < 8; i++)
            if (CHECK_5(blue, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = blue[i][j][0];
                    GLfloat ty = blue[i][j][1];
                    blue[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    blue[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //白色变
        for (int i = 0; i < 8; i++)
            if (CHECK_5(white, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = white[i][j][0];
                    GLfloat ty = white[i][j][1];
                    white[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    white[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }

        //黄色变
        for (int i = 0; i < 8; i++)
            if (CHECK_5(yellow, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat tx = yellow[i][j][0];
                    GLfloat ty = yellow[i][j][1];
                    yellow[i][j][0] = (tx * cos(ag)) - ((ty * sin(ag)) * CHANGE);
                    yellow[i][j][1] = ((tx * sin(ag)) * CHANGE) + (ty * cos(ag));
                }
            }
        glutPostRedisplay();
        glutTimerFunc(10, turn_fL, 1);
        f5++;
    }
    else
    {
        f5 = 0;
        if (Flag) AUTOMATIC_RECOVERY(++idx);
    }
}
void turn_fB(int trash)
{
    if (f6 < 9)
    {
        //红色变
        for (int i = 0; i < 8; i++)
            if (CHECK_6(red, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = red[i][j][1];
                    GLfloat tz = red[i][j][2];
                    red[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    red[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //绿色变
        for (int i = 0; i < 8; i++)
            if (CHECK_6(green, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = green[i][j][1];
                    GLfloat tz = green[i][j][2];
                    green[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    green[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //橙色变
        for (int i = 0; i < 8; i++)
            if (CHECK_6(orange, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = orange[i][j][1];
                    GLfloat tz = orange[i][j][2];
                    orange[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    orange[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //蓝色变
        for (int i = 0; i < 8; i++)
            if (CHECK_6(blue, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = blue[i][j][1];
                    GLfloat tz = blue[i][j][2];
                    blue[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    blue[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //白色变
        for (int i = 0; i < 8; i++)
            if (CHECK_6(white, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = white[i][j][1];
                    GLfloat tz = white[i][j][2];
                    white[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    white[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        //黄色变
        for (int i = 0; i < 8; i++)
            if (CHECK_6(yellow, i))
            {
                for (int j = 0; j < 4; j++)
                {
                    GLfloat ty = yellow[i][j][1];
                    GLfloat tz = yellow[i][j][2];
                    yellow[i][j][1] = (ty * cos(ag)) - ((tz * sin(ag)) * CHANGE);
                    yellow[i][j][2] = ((ty * sin(ag)) * CHANGE) + (tz * cos(ag));
                }
            }

        glutPostRedisplay();
        glutTimerFunc(10, turn_fB, 1);
        f6++;


    }
    else {
        f6 = 0;
        if (Flag) AUTOMATIC_RECOVERY(++idx);
    }
}
void turn_allx(int trash)
{
    if (f_allx < 9)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = red[i][j][1];
                GLfloat tz = red[i][j][2];
                red[i][j][1] = (ty * cos(ag)) - (tz * sin(ag));
                red[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = green[i][j][1];
                GLfloat tz = green[i][j][2];
                green[i][j][1] = (ty * cos(ag)) - (tz * sin(ag));
                green[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = orange[i][j][1];
                GLfloat tz = orange[i][j][2];
                orange[i][j][1] = (ty * cos(ag)) - (tz * sin(ag));
                orange[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = blue[i][j][1];
                GLfloat tz = blue[i][j][2];
                blue[i][j][1] = (ty * cos(ag)) - (tz * sin(ag));
                blue[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = white[i][j][1];
                GLfloat tz = white[i][j][2];
                white[i][j][1] = (ty * cos(ag)) - (tz * sin(ag));
                white[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = yellow[i][j][1];
                GLfloat tz = yellow[i][j][2];
                yellow[i][j][1] = (ty * cos(ag)) - (tz * sin(ag));
                yellow[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }


        glutPostRedisplay();
        glutTimerFunc(10, turn_allx, 1);

        f_allx++;
    }
    else  f_allx = 0;

}
void turn_ally(int trash)
{
    if (f_ally < 9)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = red[i][j][0];
                GLfloat tz = red[i][j][2];
                red[i][j][0] = (ty * cos(ag)) - (tz * sin(ag));
                red[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = green[i][j][0];
                GLfloat tz = green[i][j][2];
                green[i][j][0] = (ty * cos(ag)) - (tz * sin(ag));
                green[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = orange[i][j][0];
                GLfloat tz = orange[i][j][2];
                orange[i][j][0] = (ty * cos(ag)) - (tz * sin(ag));
                orange[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = blue[i][j][0];
                GLfloat tz = blue[i][j][2];
                blue[i][j][0] = (ty * cos(ag)) - (tz * sin(ag));
                blue[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = white[i][j][0];
                GLfloat tz = white[i][j][2];
                white[i][j][0] = (ty * cos(ag)) - (tz * sin(ag));
                white[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 4; j++) {
                GLfloat ty = yellow[i][j][0];
                GLfloat tz = yellow[i][j][2];
                yellow[i][j][0] = (ty * cos(ag)) - (tz * sin(ag));
                yellow[i][j][2] = (ty * sin(ag)) + (tz * cos(ag));
            }


        glutPostRedisplay();
        glutTimerFunc(10, turn_ally, 1);

        f_ally++;
    }
    else f_ally = 0;
}



//实现自动复原效果
void AUTOMATIC_RECOVERY(int idx)
{
    if (idx <= stop)
    {
        switch (Path_Record[idx])
        {
        case 'R':
            glutTimerFunc(200, turn_fR, 1);
            break;
        case 'F':
            glutTimerFunc(200, turn_fF, 1);
            break;
        case 'U':
            glutTimerFunc(200, turn_fU, 1);
            break;
        case 'D':
            glutTimerFunc(200, turn_fD, 1);
            break;
        case 'L':
            glutTimerFunc(200, turn_fL, 1);
            break;
        case 'B':
            glutTimerFunc(200, turn_fB, 1);
            break;
        }
    }
    else//复原完成时
    {
        Flag = 0;
        memset(Path_Record, 0, sizeof Path_Record);
        CHANGE = 1;
        R = 0.5, G = 1, B = 0.5;
        init();
        glutPostRedisplay();
    }

}







/*********************鼠标键盘回调控制***********************/

void Mos_Left_turn_look(int trash)
{
    if (LEFT_isDragging)
    {
        float temp_lookx = lookx;
        float temp_lookz = lookz;
        lookx = (temp_lookx * cos(rot)) - (temp_lookz * sin(rot));
        lookz = (temp_lookx * sin(rot)) + (temp_lookz * cos(rot));

        init();

        glutPostRedisplay();

        glutTimerFunc(10, Mos_Left_turn_look, 10);
    }
}

void Mos_Right_turn_look(int trash)
{
    if (RIGHT_isDragging)
    {
        float temp_lookx = lookx;
        float temp_lookz = lookz;
        lookx = (temp_lookx * cos(rot)) + (temp_lookz * sin(rot));
        lookz = (-1 * temp_lookx * sin(rot)) + (temp_lookz * cos(rot));

        init();

        glutPostRedisplay();

        glutTimerFunc(10, Mos_Right_turn_look, 10);
    }
}

void mouseCallback(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        LEFT_isDragging = 1;
        glutTimerFunc(1, Mos_Left_turn_look, 1);

    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        LEFT_isDragging = 0;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        RIGHT_isDragging = 1;
        glutTimerFunc(1, Mos_Right_turn_look, 1);

    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        RIGHT_isDragging = 0;
    }

}

void onKeyboard(unsigned char key, int x, int y)
{

    if (key == 'R') {
        glutTimerFunc(1, turn_fR, 1);
        move_R();
    }


    else if (key == 'F')
    {
        glutTimerFunc(1, turn_fF, 1);
        move_F();
    }


    else if (key == 'U') {
        glutTimerFunc(1, turn_fU, 1);
        move_U();
    }


    else if (key == 'D') {
        glutTimerFunc(1, turn_fD, 1);
        move_D();
    }


    else if (key == 'L') {
        glutTimerFunc(1, turn_fL, 1);
        move_L();
    }

    else if (key == 'B') {
        glutTimerFunc(1, turn_fB, 1);
        move_B();
    }


    else if (key == 'X')
        glutTimerFunc(10, turn_allx, 1);

    else if (key == 'Y')
        glutTimerFunc(10, turn_ally, 1);




    else if (key == VK_RETURN)
    {
        Queue_of_Bfs.push(cube);

        if (!CHECK_ALL(cube)) BFS_FIND_RECOVER_PATH();

        cout << "最优还原公式： ";

        for (; !Aim_Cube.step.empty(); ) {
            cout << Aim_Cube.step.front() << "  ";
            Path_Record[++idx] = Aim_Cube.step.front();
            Aim_Cube.step.pop();
        }
        cout << endl;
        cout << "The depth of BFS: " << cnt << endl;
        cout << "The step of recovery: " << idx;

        stop = idx;
        idx = 0;


        Flag = 1;
        AUTOMATIC_RECOVERY(++idx);
    }
}




/**********************绘制整个模仿与大循环**********************/

void myDisplay()
{

    glClearColor(R, G, B, 0.0f);
    glEnable(GL_DEPTH_TEST);


    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    //红
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i >= 4) {
                glColor3f(color[0][0], color[0][1], color[0][2]);
                glVertex3f(red[i][j][0], red[i][j][1], red[i][j][2]);
            }
            else {
                glColor3f(clr, clr, clr);
                glVertex3f(red[i][j][0], red[i][j][1], red[i][j][2]);
            }
        }
    glEnd();


    //绿色
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i >= 4) {
                glColor3f(color[1][0], color[1][1], color[1][2]);
                glVertex3f(green[i][j][0], green[i][j][1], green[i][j][2]);
            }
            else {
                glColor3f(clr, clr, clr);
                glVertex3f(green[i][j][0], green[i][j][1], green[i][j][2]);
            }
        }
    glEnd();


    //橙色
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i >= 4) {
                glColor3f(color[2][0], color[2][1], color[2][2]);
                glVertex3f(orange[i][j][0], orange[i][j][1], orange[i][j][2]);
            }
            else {
                glColor3f(clr, clr, clr);
                glVertex3f(orange[i][j][0], orange[i][j][1], orange[i][j][2]);
            }
        }
    glEnd();


    //蓝色
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i >= 4) {
                glColor3f(color[3][0], color[3][1], color[3][2]);
                glVertex3f(blue[i][j][0], blue[i][j][1], blue[i][j][2]);
            }
            else {
                glColor3f(clr, clr, clr);
                glVertex3f(blue[i][j][0], blue[i][j][1], blue[i][j][2]);
            }
        }
    glEnd();


    //白色
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i >= 4) {
                glColor3f(color[4][0], color[4][1], color[4][2]);
                glVertex3f(white[i][j][0], white[i][j][1], white[i][j][2]);
            }
            else {
                glColor3f(clr, clr, clr);
                glVertex3f(white[i][j][0], white[i][j][1], white[i][j][2]);
            }
        }
    glEnd();


    //黄色
    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i >= 4) {
                glColor3f(color[5][0], color[5][1], color[5][2]);
                glVertex3f(yellow[i][j][0], yellow[i][j][1], yellow[i][j][2]);
            }
            else {
                glColor3f(clr, clr, clr);
                glVertex3f(yellow[i][j][0], yellow[i][j][1], yellow[i][j][2]);
            }
        }
    glEnd();


    glFlush();
    glutSwapBuffers();
}



int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(700, 700);

    glutCreateWindow("二阶魔方-Pocket Cube");


    inite_cube_color();//初始魔方颜色数组数据

    init();


    glutDisplayFunc(&myDisplay);//启动绘制
    glutKeyboardFunc(onKeyboard);//实时监测键盘的变化
    glutMouseFunc(mouseCallback);



    glutMainLoop();
    return 0;
}