#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//Simple Pacman Clone in C++ using SFML
//Author: Sohan Chowdhury
//Email: Sifat3d@gmail.com
//Website:  sohan.cf
//Twitter: iamsohan
//Anyone is free to learn using this code however they want.
//Bugfixes or improvements are welcome
//SFML Library is required
//Made & tested in Windows 32 & 64 bit environment

using namespace std;
using namespace sf;


const int WINDOW_HW= 680;
const int GRIDS_NUM = 17;
const int GRID_AREA = WINDOW_HW/GRIDS_NUM;
const float GRID_SPACING = 0;
const int OBJECT_AREA = GRID_AREA - (GRID_SPACING*2);
const int Game_FPS = 60;
const int USER_MOVE_EVERY = 60;
const int ENEMY_MOVE_EVERY = 5; //times user

const Color BG = Color(110, 189, 195);
const Color WALL = Color(87, 98, 116);
const Color GOAL = Color(236, 133 ,88);
enum Movement{goleft,goright,goup,godown,gonone};

int toActual(int x)
{
    return x*GRID_AREA;
}

int toGrid(int x)
{
    return x/GRID_AREA;
}


class object{
public:
    CircleShape me;
    int x,y;
    Movement goThisWay;  //ONLY for ENEMY OBJECTS!!!

    object(int xval, int yval,int rval){
        x=xval;
        y=yval;
        me.setRadius(rval);
        me.setPosition(getGrid());
        goThisWay = goleft;
    }

    Vector2f getGrid()
    {
        return Vector2f(toActual(x),toActual(y));
    }

    void move(int movx, int movy)
    {
        //cout<<"movx,movy: "<<movx<<","<<movy<<endl;
        x = x+movx;
        y = y+movy;
        //cout<<"Rx,Ry: "<<toActual(movx)<<","<<toActual(movy)<<endl;
        me.move(toActual(movx),toActual(movy));

    }

};

bool tryMoveObjects(Movement Tusermove, object &Tstudent,bool (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{

    switch(Tusermove)
        {
            case 0: //left
                if(Tmaptaken[Tstudent.y][Tstudent.x-1] == 0  && Tstudent.x-1>=0)
                {
                    Tstudent.move(-1,0);
                    return 0;
                }
                return 1;
            case 1: //right
                if(Tmaptaken[Tstudent.y][Tstudent.x+1] == 0  && Tstudent.x+1<GRIDS_NUM)
                {
                    Tstudent.move(+1,0);
                    return 0;
                }
                return 1;
            case 2: //up
                if(Tmaptaken[Tstudent.y-1][Tstudent.x] == 0  && Tstudent.y-1>=0)
                {
                    Tstudent.move(0,-1);
                    return 0;
                }
                return 1;
            case 3: //down
                if(Tmaptaken[Tstudent.y+1][Tstudent.x] == 0  && Tstudent.y+1<GRIDS_NUM)
                {
                    Tstudent.move(0,+1);
                    return 0;
                }
                return 1;
            case 4:
                return 0;
        }
    return 1;
}

bool moveMonsters(Movement &TgoThisWay,object &TgradeMonster,bool (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{
                    bool flag = tryMoveObjects(TgoThisWay,TgradeMonster,Tmaptaken);
                    //cout<<"flag"<<flag<<" go"<<TgoThisWay<<endl;

                    if(flag==1 && TgoThisWay==goleft)
                        {
                         TgoThisWay = goright;
                        }
                    else if(TgoThisWay==goright && flag==1)
                        {
                         TgoThisWay = goleft;
                        }
            return 0;
}

bool moveMonstersUpDown(Movement &TgoThisWay,object &TgradeMonster,bool (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{
                    bool flag = tryMoveObjects(TgoThisWay,TgradeMonster,Tmaptaken);
                   // cout<<"flagupdown"<<flag<<" go"<<TgoThisWay<<endl;

                    if(flag==1 && TgoThisWay==goup)
                        {
                         TgoThisWay = godown;
                        }
                    else if(TgoThisWay==godown && flag==1)
                        {
                         TgoThisWay = goup;
                        }
            return 0;
}


bool isColliding(object &player,object &monster)
{
    return (player.x==monster.x && player.y==monster.y);
}

bool eatGrade(object &Tstudent, object &TaPlus,unsigned int &TscoreUser, Sound &sound)
{
            if(isColliding(Tstudent,TaPlus))
            {
                TscoreUser++;
                //hide object and move away
                TaPlus.x=-1;
                TaPlus.y=-1;
                TaPlus.me.setPosition(-WINDOW_HW,-WINDOW_HW);
                sound.play();


                return true;
            }

            return false;
}

int main(){

    //MAP
    //below map MUST be GRIDS_NUM*GRIDS_NUM.
    //THIS IS ONE OF THE NON VARIABLE THINGS THAT ARE PRESET
    string gridmap[GRIDS_NUM];
    gridmap[0]=   "--------*--------";
    gridmap[1]=   "                 ";
    gridmap[2]=   "- -------------- ";
    gridmap[3]=   "-                ";
    gridmap[4]=   "--------- -------";
    gridmap[5]=   "                 ";
    gridmap[6]=   "- ------------ --";
    gridmap[7]=   "            -    ";
    gridmap[8]=   "--------- ----- -";
    gridmap[9]=   "        -        ";
    gridmap[10]=  "             - - ";
    gridmap[11]=  "- -------- ------";
    gridmap[12]=  "-                ";
    gridmap[13]=  "--------- -------";
    gridmap[14]=  "                 ";
    gridmap[15]=  "- ---------------";
    gridmap[16]=  "*****************";
    bool maptaken[GRIDS_NUM][GRIDS_NUM];

    int i,j;
    for(i=0;i<GRIDS_NUM;i++)
    {
        for(j=0;j<GRIDS_NUM;j++)
        {
            maptaken[i][j]=1;
        }
    }

    RectangleShape backgrid;
    backgrid.setSize(Vector2f(OBJECT_AREA,OBJECT_AREA));
    backgrid.setOutlineThickness(GRID_SPACING);
    backgrid.setOutlineColor(Color::White);

    //player setup
    object student(8,16,OBJECT_AREA/2);
    //student.me.setPosition(toActual(5),toActual(10));

    Texture userTexture;
    userTexture.setSmooth(true);
    userTexture.loadFromFile("student.png");

    student.me.setTexture(&userTexture);


    //monster concept setup

    //load monster image
    Texture gradeMonsterTexture;
    gradeMonsterTexture.setSmooth(true);
    gradeMonsterTexture.loadFromFile("f.png");

    //set monsters
    object gradeMonster_1(9,7,OBJECT_AREA/2);
    gradeMonster_1.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_2(15,9,OBJECT_AREA/2);
    gradeMonster_2.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_3(11,3,OBJECT_AREA/2);
    gradeMonster_3.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_4(7,14,OBJECT_AREA/2);
    gradeMonster_4.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_UpDown(1,12,OBJECT_AREA/2);
    gradeMonster_UpDown.me.setTexture(&gradeMonsterTexture);
    gradeMonster_UpDown.goThisWay = goup;


    //aPlus concept setup

    //load aPlus image
    Texture aPlusTexture;
    aPlusTexture.setSmooth(true);
    aPlusTexture.loadFromFile("a.png");

    //set aPlus
    object aPlus_1(11,7,OBJECT_AREA/2);
    aPlus_1.me.setTexture(&aPlusTexture);

    object aPlus_2(14,10,OBJECT_AREA/2);
    aPlus_2.me.setTexture(&aPlusTexture);

    object aPlus_3(13,3,OBJECT_AREA/2);
    aPlus_3.me.setTexture(&aPlusTexture);

    object aPlus_4(0,10,OBJECT_AREA/2);
    aPlus_4.me.setTexture(&aPlusTexture);


    //set endGame window :p
    RectangleShape endGame;

        //Text stuff

        Font roboto;
        roboto.loadFromFile("Roboto-Regular.ttf");


    //Game Over text
        Text gameOverText;
        gameOverText.setFont(roboto);
        gameOverText.setCharacterSize((OBJECT_AREA-10)*2);
        gameOverText.setColor(Color::White);
        gameOverText.setPosition(toActual(5),toActual(6));
        gameOverText.setString("Game Over!");

    //setup Score text
        //Score

        Text scoreText;
        scoreText.setFont(roboto);
        scoreText.setCharacterSize(OBJECT_AREA-10);
        scoreText.setColor(Color::White);
        scoreText.setPosition(toActual(14),toActual(16));
        unsigned int scoreUser = 0;


    //window from library
    RenderWindow window(VideoMode(WINDOW_HW,WINDOW_HW), "CGPA_MAN b-v1.3");
    window.setFramerateLimit(Game_FPS);

//Library time functions
    Clock clock;
    Time time;
    float elapsed;
    bool runUser = false;
    int userMoved = 0;  //tracks user turns
    bool runGame = true;
    Movement usermove = gonone;


    //Play sound before Game Loop
    SoundBuffer pacmanIntro;
    pacmanIntro.loadFromFile("pacman_beginning.wav");
    Sound introMusic;
    introMusic.setBuffer(pacmanIntro);
    introMusic.play();

    SoundBuffer pacmanEat;
    pacmanEat.loadFromFile("pacman_eatfruit.wav");
    Sound pacmanEatMusic;
    pacmanEatMusic.setBuffer(pacmanEat);
//Library Game Loop
     while (window.isOpen())
    {
        runUser = false;
        time = clock.getElapsedTime();
        elapsed = time.asMilliseconds();
        if(elapsed>USER_MOVE_EVERY){
        clock.restart();
        runUser = true;
        }
        //cout<<elapsed<<endl;

        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed || (Keyboard::isKeyPressed(Keyboard::Escape)))
                window.close();
        }

        // clear the window with black color
        window.clear(Color::Black);

        // draw everything here...

    if(runGame==true)
        {

        //draws map
        int i,j;
        for(i=0;i<GRIDS_NUM;i++)
        {
            for(j=0;j<GRIDS_NUM;j++)
            {
                //cout<<"bool: "<<maptaken[i][j]<<endl;
                if(gridmap[i][j]=='-')
                {
                backgrid.setFillColor(WALL);
                }
                else  if(gridmap[i][j]=='*')
                {
                backgrid.setFillColor(GOAL);
                maptaken[i][j] = 0;
                }
                else
                {
                    backgrid.setFillColor(Color(110, 189, 195));
                    maptaken[i][j] = 0;
                }
                //Because j is X, and i is Y
                backgrid.setPosition(toActual(j),toActual(i));

                window.draw(backgrid);
            }
        }



        //starting player logic here.

        //keyboard movement of player
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Left)
            {
                usermove = goleft;
            }
            else if (event.key.code == Keyboard::Right)
            {
                usermove = goright;
            }
            else if (event.key.code == Keyboard::Up)
            {
                usermove = goup;
            }
            else if (event.key.code == Keyboard::Down)
            {
                usermove = godown;
            }
            else {
                usermove = gonone;
            }

            if(runUser==1)
            {
                tryMoveObjects(usermove,student,maptaken);
            }

        }


        //Grade Monster

        if(runUser==1)
            {
                userMoved++;
                if(userMoved>ENEMY_MOVE_EVERY){
                    userMoved=0;
                    moveMonsters(gradeMonster_1.goThisWay,gradeMonster_1,maptaken);
                    moveMonsters(gradeMonster_2.goThisWay,gradeMonster_2,maptaken);
                    moveMonsters(gradeMonster_3.goThisWay,gradeMonster_3,maptaken);
                    moveMonsters(gradeMonster_4.goThisWay,gradeMonster_4,maptaken);
                    moveMonstersUpDown(gradeMonster_UpDown.goThisWay,gradeMonster_UpDown,maptaken);
                    }
            }


        //Score display Update
            string scoreTextString = "CGPA:  0";
            scoreTextString[7] = scoreUser+'0';
            //cout<<scoreTextString<<endl;
            scoreText.setString(scoreTextString);


//Collision detection here
            if(isColliding(student,gradeMonster_1) || isColliding(student,gradeMonster_2) ||
               isColliding(student,gradeMonster_3) || isColliding(student,gradeMonster_4)
               || isColliding(student,gradeMonster_UpDown))
            {
                student.me.setFillColor(Color::Red);
                endGame.setSize(Vector2f(toActual(11),toActual(11)));
                endGame.setFillColor(Color(255,87,34));
                endGame.setOutlineColor(WALL);
                endGame.setOutlineThickness(0.5);
                endGame.setPosition(toActual(3),toActual(3));
                scoreText.setPosition(toActual(9),toActual(12));
                runGame = false;
            }

            //win
            if(student.x == 8 && student.y==0)
            {
                student.me.setFillColor(Color::Green);
                endGame.setSize(Vector2f(toActual(11),toActual(11)));
                endGame.setFillColor(Color(255,87,34));
                endGame.setOutlineColor(WALL);
                endGame.setOutlineThickness(0.5);
                endGame.setPosition(toActual(3),toActual(3));
                scoreText.setPosition(toActual(9),toActual(12));
                runGame = false;
                gameOverText.setString("You WIN!!!");

            }

        //Draw Stuff using Library
            //cout<<usermove;
            //cout<<usermove<<" x,y: "<<student.x<<","<<student.y<<endl;
            window.draw(aPlus_1.me);
            window.draw(aPlus_2.me);
            window.draw(aPlus_3.me);
            window.draw(aPlus_4.me);
            window.draw(student.me);
            window.draw(gradeMonster_1.me);
            window.draw(gradeMonster_2.me);
            window.draw(gradeMonster_3.me);
            window.draw(gradeMonster_4.me);
            window.draw(gradeMonster_UpDown.me);

            if(runGame==false)
            {
                window.draw(endGame);
                window.draw(gameOverText);
            }
            window.draw(scoreText);




            //Check user and grade eating
            eatGrade(student,aPlus_1,scoreUser,pacmanEatMusic);
            eatGrade(student,aPlus_2,scoreUser,pacmanEatMusic);
            eatGrade(student,aPlus_3,scoreUser,pacmanEatMusic);
            eatGrade(student,aPlus_4,scoreUser,pacmanEatMusic);


        //draw & end the current frame
        window.display();
        }
    }

    return 0;
}
