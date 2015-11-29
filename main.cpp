#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
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
//SFML 32bit Library is required
//Made & tested in Windows 32 & 64 bit environment

using namespace std;
using namespace sf;

const string CGPA_MAN_VERSION = "CGPA MAN 1.7 beta";
const int PLAYER_LIVES = 3;
const int WINDOW_HW= 694;
const int GRIDS_NUM = 21;
const int GRID_AREA = WINDOW_HW/GRIDS_NUM;
const float GRID_SPACING = 0;
const int OBJECT_AREA = GRID_AREA - (GRID_SPACING*2);
const int Game_FPS = 60;
const int USER_MOVE_EVERY = 50;
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

double roundToTwo( double f )
{
    return (floor((f * 100) + 0.5)) / 100;
}

class object{
public:
    CircleShape me;
    int x,y;
    Movement goThisWay;  //ONLY for ENEMY OBJECTS!!!
    unsigned int retakes;   //ONLY for PLAYER OBJECT!!!
    bool tempImmune;    //ONLY for PLAYER OBJECT!!!
    int somethingHappenedFrame;

    object(int xval, int yval,int rval){
        x=xval;
        y=yval;
        me.setRadius(rval);
        me.setPosition(getGrid());
        goThisWay = goleft;
        retakes = PLAYER_LIVES;
        tempImmune = false;
        somethingHappenedFrame = 0;
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

bool tryMoveObjects(Movement Tusermove, object &Tstudent,int (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{

    switch(Tusermove)
        {
            case 0: //left
                if(Tmaptaken[Tstudent.y][Tstudent.x-1] != 1  && Tstudent.x-1>=0)
                {
                    Tstudent.move(-1,0);
                    return 0;
                }
                return 1;
            case 1: //right
                if(Tmaptaken[Tstudent.y][Tstudent.x+1] != 1  && Tstudent.x+1<GRIDS_NUM)
                {
                    Tstudent.move(+1,0);
                    return 0;
                }
                return 1;
            case 2: //up
                if(Tmaptaken[Tstudent.y-1][Tstudent.x] != 1  && Tstudent.y-1>=0)
                {
                    Tstudent.move(0,-1);
                    return 0;
                }
                return 1;
            case 3: //down
                if(Tmaptaken[Tstudent.y+1][Tstudent.x] != 1  && Tstudent.y+1<GRIDS_NUM)
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

bool CanMoveObjects(Movement Tusermove, object &Tstudent,int (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{

    switch(Tusermove)
        {
            case 0: //left
                if(Tmaptaken[Tstudent.y][Tstudent.x-1] != 1  && Tstudent.x-1>=0)
                {
                    return true;
                }
                break;

            case 1: //right
                if(Tmaptaken[Tstudent.y][Tstudent.x+1] != 1  && Tstudent.x+1<GRIDS_NUM)
                {
                    return true;
                }
                break;

            case 2: //up
                if(Tmaptaken[Tstudent.y-1][Tstudent.x] != 1  && Tstudent.y-1>=0)
                {
                    return true;
                }
                break;

            case 3: //down
                if(Tmaptaken[Tstudent.y+1][Tstudent.x] != 1  && Tstudent.y+1<GRIDS_NUM)
                {
                    return true;
                }
                break;

            case 4:
                return true;
                break;
        }
    return false;
}

bool moveMonsters(object &TgradeMonster,int (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{
                    bool flag = tryMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken);
                    //cout<<"flag"<<flag<<" go"<<TgoThisWay<<endl;

                    if(flag==1 && TgradeMonster.goThisWay==goleft)
                        {
                         TgradeMonster.goThisWay = goright;
                        }
                    else if(TgradeMonster.goThisWay==goright && flag==1)
                        {
                         TgradeMonster.goThisWay = goleft;
                        }
            return 0;
}

bool moveMonstersRandom(object &TgradeMonster,int (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{

                    //THIS CODE IS FOR EMERGENCY :p
                    //These make the movements dynamic but closed in a loop.
                    //the new implementation is fully random
                    /*
                    bool flag = 0;
                    if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==false && TgradeMonster.goThisWay!=goright)
                        {
                         TgradeMonster.goThisWay = goleft;
                         if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==true)
                            {flag = 1;}
                        }
                    if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==false  && flag == 0 && TgradeMonster.goThisWay!=godown)
                        {
                         TgradeMonster.goThisWay = goup;
                         if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==true)
                            {flag = 1;}
                        }
                    if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==false  && flag == 0 && TgradeMonster.goThisWay!=goup)
                        {
                         TgradeMonster.goThisWay = godown;
                         if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==true)
                            {flag = 1;}
                        }

                    if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==false && flag == 0 && TgradeMonster.goThisWay!=goleft)
                        {
                         TgradeMonster.goThisWay = goright;
                         if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==true)
                            {flag = 1;}
                        }
                        */



                    if(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)==false)
                        {
                            while(CanMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken)!=true)
                            {
                            int randomMove = rand() % 4;
                            TgradeMonster.goThisWay = (Movement)randomMove;
                            }
                        }

                    tryMoveObjects(TgradeMonster.goThisWay,TgradeMonster,Tmaptaken);
            return 0;
}


bool isColliding(object &player,object &monster)
{
    return (player.x==monster.x && player.y==monster.y);
}

bool isEating(object &player,int (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM])
{
    return (Tmaptaken[player.y][player.x]==9);
}

bool EatTheGrade(object &player,int (&Tmaptaken)[GRIDS_NUM][GRIDS_NUM],string (&TRTgridmap)[GRIDS_NUM], double &TstudentCgpa,int TaPlusCount, Sound &sound)
{
    //cout<<TaPlusCount<<endl;
    TstudentCgpa = TstudentCgpa+(4.0/TaPlusCount);
    //cout<<TstudentCgpa<<endl;
    sound.play();
    TRTgridmap[player.y][player.x]=' ';
    player.me.setFillColor(Color::White);
    return true;
}



int main(){


    //MAP
    //below map MUST be GRIDS_NUM*GRIDS_NUM.
    //THIS IS ONE OF THE NON VARIABLE THINGS THAT ARE PRESET
    string gridmap[GRIDS_NUM];
    gridmap[0]=   "XXXXXXXXXX*XXXXXXXXXX";
    gridmap[1]=   "X +   +   +   +   + X";
    gridmap[2]=   "X X+ XXX  X  XXX+ X X";
    gridmap[3]=   "X  +X+ +  X  + +X+  X";
    gridmap[4]=   "X +X+X+X+XXX+X+X+X+ X";
    gridmap[5]=   "X+ X    + + +    X +X";
    gridmap[6]=   "X +X++XXXXXXXXX++X+ X";
    gridmap[7]=   "XXX  X+++++++++X  XXX";
    gridmap[8]=   "X  +   X+++++X   +  X";
    gridmap[9]=   "X   X   X+++X + X + X";
    gridmap[10]=  "X+X + + +XXX+ + + X+X";
    gridmap[11]=  "X  +   +     +   +  X";
    gridmap[12]=  "X+XXXX   XXX   XXXX+X";
    gridmap[13]=  "X + + + X + X + + + X";
    gridmap[14]=  "X+XX X XX   XX X XX+X";
    gridmap[15]=  "X   +  +  X  +  +   X";
    gridmap[16]=  "X   XXX +   + XXX   X";
    gridmap[17]=  "X+X     X + X     X+X";
    gridmap[18]=  "X XXX+XXX X XXX+XXX X";
    gridmap[19]=  "X + + + +   + + + + X";
    gridmap[20]=  "XXXXXXXXXXXXXXXXXXXXX";

 //this is the ingame map, it changes according to player
    string RTgridmap[GRIDS_NUM] = gridmap;

 //SEED for RANDOM NUMBER GENERATOR in MOVEMENT
    srand (time(NULL));

    //aPlus Count from map
        unsigned int aPlusCounter=0;
    //where walls take spaces
    int maptaken[GRIDS_NUM][GRIDS_NUM];

    int i,j;
    for(i=0;i<GRIDS_NUM;i++)
    {
        for(j=0;j<GRIDS_NUM;j++)
        {
           if(RTgridmap[i][j]=='+'){aPlusCounter++;}
            maptaken[i][j]=1;
        }
    }

    //map tile
    RectangleShape backgrid;
    backgrid.setSize(Vector2f(OBJECT_AREA,OBJECT_AREA));
    backgrid.setOutlineThickness(GRID_SPACING);
    backgrid.setOutlineColor(Color::White);

    //aplus tile
        //load aPlus image
        Texture aPlusTexture;
        aPlusTexture.setSmooth(true);
        aPlusTexture.loadFromFile("media/a.png");

        CircleShape aPlusTile;
        aPlusTile.setRadius(OBJECT_AREA/2);
        aPlusTile.setTexture(&aPlusTexture);


    //player setup
    object student(GRIDS_NUM/2,GRIDS_NUM-2,OBJECT_AREA/2);

    //student.me.setPosition(toActual(5),toActual(10));

    Texture userTexture;
    userTexture.setSmooth(true);
    userTexture.loadFromFile("media/student.png");

    student.me.setTexture(&userTexture);


    //monster concept setup

    //load monster image
    Texture gradeMonsterTexture;
    gradeMonsterTexture.setSmooth(true);
    gradeMonsterTexture.loadFromFile("media/f.png");

    //set monsters
    object gradeMonster_1(1,5,OBJECT_AREA/2);
    gradeMonster_1.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_2(4,9,OBJECT_AREA/2);
    gradeMonster_2.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_3(9,9,OBJECT_AREA/2);
    gradeMonster_3.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_4(12,2,OBJECT_AREA/2);
    gradeMonster_4.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_5(16,16,OBJECT_AREA/2);
    gradeMonster_5.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_6(19,12,OBJECT_AREA/2);
    gradeMonster_6.me.setTexture(&gradeMonsterTexture);

    object gradeMonster_Test(GRIDS_NUM/2,GRIDS_NUM-6,OBJECT_AREA/2);
    gradeMonster_Test.me.setTexture(&gradeMonsterTexture);


    //set endGame window :p
    RectangleShape endGame;
    endGame.setSize(Vector2f(toActual(GRIDS_NUM/1.4),toActual(GRIDS_NUM/1.4)));
    endGame.setOutlineColor(WALL);
    endGame.setOutlineThickness(0.3);
    endGame.setPosition(toActual((GRIDS_NUM/3)/2),toActual((GRIDS_NUM/3)/2));

        //Text stuff

        Font roboto;
        roboto.loadFromFile("media/Roboto-Regular.ttf");


    //Game Over text
        Text gameOverText;
        gameOverText.setFont(roboto);
        gameOverText.setCharacterSize((OBJECT_AREA-10)*2);
        gameOverText.setColor(Color::White);
        gameOverText.setPosition(toActual(GRIDS_NUM/3),toActual(GRIDS_NUM/3));
        gameOverText.setString("You have Failed!");

    //setup STATUS text
        //Score/CGPA

        Text scoreText;
        scoreText.setFont(roboto);
        scoreText.setCharacterSize(OBJECT_AREA-10);
        scoreText.setColor(Color::White);
        scoreText.setPosition(toActual(GRIDS_NUM-4),toActual(GRIDS_NUM-1));
        double studentCgpa = 0;

        //Retakes/Lives :v

        Text retakesText;
        retakesText.setFont(roboto);
        retakesText.setCharacterSize(OBJECT_AREA-10);
        retakesText.setColor(Color::White);
        retakesText.setPosition(toActual(GRIDS_NUM-8),toActual(GRIDS_NUM-1));

        //Notification text
        Text NotifText;
        NotifText.setFont(roboto);
        NotifText.setCharacterSize((OBJECT_AREA/2)-1);
        NotifText.setColor(Color::White);
        NotifText.setPosition(toActual(6),toActual(6));


    //window from library
    RenderWindow window(VideoMode(WINDOW_HW,WINDOW_HW), CGPA_MAN_VERSION);
    window.setFramerateLimit(Game_FPS);

//Library time functions
    Clock clock;
    Time time;
    float elapsed;
    bool runUser = false;
    int userMoved = 0;  //tracks user turns
    bool runGame = true;
    Movement usermove = gonone;

    unsigned int countFrames = 0;


    //Play sound before Game Loop
    SoundBuffer pacmanIntro;
    pacmanIntro.loadFromFile("media/pacman_beginning.wav");
    Sound introMusic;
    introMusic.setBuffer(pacmanIntro);
    introMusic.play();

    SoundBuffer pacmanEat;
    pacmanEat.loadFromFile("media/pacman_eatfruit.wav");
    Sound pacmanEatMusic;
    pacmanEatMusic.setBuffer(pacmanEat);
//Library Game Loop
     while (window.isOpen())
    {
        countFrames++;
        if(countFrames>60*60*60*5)
        {
            countFrames = 0;
        }

        runUser = false;
        time = clock.getElapsedTime();
        elapsed = time.asMilliseconds();
        if(elapsed>USER_MOVE_EVERY)
        {
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
                if(RTgridmap[i][j]=='X')
                {
                backgrid.setFillColor(WALL);
                }
                else  if(RTgridmap[i][j]=='*')
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
                if(RTgridmap[i][j]=='+')
                {
                    maptaken[i][j] = 9;
                    aPlusTile.setPosition(toActual(j),toActual(i));
                    window.draw(aPlusTile);
                }
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
                    moveMonstersRandom(gradeMonster_1,maptaken);
                    moveMonstersRandom(gradeMonster_2,maptaken);
                    moveMonstersRandom(gradeMonster_3,maptaken);
                    moveMonstersRandom(gradeMonster_4,maptaken);
                    moveMonstersRandom(gradeMonster_5,maptaken);
                    moveMonstersRandom(gradeMonster_6,maptaken);
                    moveMonstersRandom(gradeMonster_Test,maptaken);
                    }
            }


//Collision detection here

            if((isColliding(student,gradeMonster_1) || isColliding(student,gradeMonster_2) ||
               isColliding(student,gradeMonster_3) || isColliding(student,gradeMonster_4) ||
               isColliding(student,gradeMonster_5) || isColliding(student,gradeMonster_6) ||
               isColliding(student,gradeMonster_Test)) && student.tempImmune==false)
            {
                student.me.setFillColor(Color::Red);
                student.retakes--;
                student.tempImmune = true;
                student.somethingHappenedFrame = countFrames;
                //cout<<"retakes: "<<student.retakes<<endl;
            }

            if(student.tempImmune == true)
            {
                student.me.setFillColor(Color::Red);
            }

            if(student.tempImmune == true && countFrames - student.somethingHappenedFrame > 1*60)
            {
            student.tempImmune = false;
            student.somethingHappenedFrame = 0;
            }


            if(student.retakes==0)
            {
                runGame = false;
                endGame.setFillColor(Color(255,87,34));
                scoreText.setPosition(toActual(9),toActual(12));
            }

            //eating grades
            if(isEating(student,maptaken))
            {
                EatTheGrade(student,maptaken,RTgridmap,studentCgpa,aPlusCounter,pacmanEatMusic);
            }

            //win
            NotifText.setString("");  //set empty so that text goes away after user moves away
            if(RTgridmap[student.y][student.x]=='*') //since 2d array
            {
                if(studentCgpa>=2.50){
                student.me.setFillColor(Color::Green);
                endGame.setFillColor(Color(255,87,34));
                scoreText.setPosition(toActual(9),toActual(12));
                runGame = false;
                gameOverText.setString("You Graduated!!!");
                }
                else {
                    NotifText.setString("You need minimum 2.50 Cgpa to graduate!");
                }

            }

//STATUS display Update

            //Score
            string scoreTextString = "CGPA: ";
            if(runGame==false)
            {
                scoreTextString = "Your CGPA is:  ";
            }

            ostringstream strs;
            strs <<roundToTwo(studentCgpa);
            string strTemp = strs.str();

            scoreTextString = scoreTextString+strTemp;
            //cout<<scoreTextString<<endl;
            scoreText.setString(scoreTextString);

            //Retakes
            string retakesTextString = "Retakes: N";
            retakesTextString[9] = (student.retakes + '0');
            retakesText.setString(retakesTextString);

        //Draw Stuff using Library
            //cout<<usermove;
            //cout<<usermove<<" x,y: "<<student.x<<","<<student.y<<endl;

            window.draw(student.me);
            window.draw(gradeMonster_1.me);
            window.draw(gradeMonster_2.me);
            window.draw(gradeMonster_3.me);
            window.draw(gradeMonster_4.me);
            window.draw(gradeMonster_5.me);
            window.draw(gradeMonster_6.me);
            window.draw(gradeMonster_Test.me);

            window.draw(NotifText);

            if(runGame==false)
            {
                window.draw(endGame);
                window.draw(gameOverText);
            }
            else {window.draw(retakesText);}

            window.draw(scoreText);



        //draw & end the current frame
        window.display();
        }
    }

    return 0;
}
