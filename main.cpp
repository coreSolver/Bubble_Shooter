#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
const int INIT_SCORE = 0;
const int INIT_HEALTH = 3;
const int INIT_TIME = 90;

struct Point{double x, y;};

double distance(Point A, Point B)
{   //return the distance between 2 points.
    return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y));
}

void move_bullets(vector<Bullet> &bullets)
{
    // move all bullets //if bullet goes out of screen display, delete it.
    for(unsigned int i=0; i<bullets.size(); i++)
    {
        if(!bullets[i].nextStep(STEP_TIME))
        {
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles)
{
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    return bubbles;
}

bool shooter_bubble_collision(Bubble &bubble, Shooter &shooter)
{
    Point shooter_head_center = {shooter.get_head_center_x(),shooter.get_head_center_y()};
    Point bubble_center = {bubble.get_center_x(),bubble.get_center_y()};
    return ((distance(shooter_head_center, bubble_center) <= shooter.get_head_radius() + bubble.get_radius())//Shooter head - bubble collision
            ||
            (((abs(shooter.get_body_center_x()-bubble.get_center_x()) <= shooter.get_body_width()/2 + bubble.get_radius())) && (bubble.get_center_y()+bubble.get_radius() >= shooter.get_head_center_y() - shooter.get_head_radius())));
}           //Shooter body - bubble collision


/*void decide_direction(Bubble &bubble, Shooter &shooter)
{
    Point shooter_head_center = {shooter.get_head_center_x(),shooter.get_head_center_y()};
    Point bubble_center = {bubble.get_center_x(),bubble.get_center_y()};
    if (distance(shooter_head_center, bubble_center) <= shooter.get_head_radius() + bubble.get_radius())
    {
        bubble.reverse_direction();
    }
    else
    {
        bubble.reverse_vx();
    }
}*/

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    int score = INIT_SCORE;
    int health = INIT_HEALTH;
    int time = INIT_TIME;
    int micro_time_counter=0; //this variable is intended to increase by 1 after every STEP_TIME.

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));
    
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    string Health_String("Health: "+to_string(INIT_HEALTH));
    Text Health_Text(LEFT_MARGIN+150, BOTTOM_MARGIN, Health_String);

    string Score_String("Score: "+to_string(INIT_SCORE));
    Text Score_Text(LEFT_MARGIN+300, BOTTOM_MARGIN, Score_String);

    string Time_String("Time: "+to_string(INIT_TIME));
    Text Time_Text(LEFT_MARGIN, TOP_MARGIN, Time_String);

    string Status_String("Status: In Play");
    Text Status_Text(LEFT_MARGIN+300, TOP_MARGIN, Status_String);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {   
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);
            
            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        // Update the bubbles
        move_bubbles(bubbles);
        // Update the bullets
        move_bullets(bullets);

        for (unsigned int i = 0; i < bubbles.size(); i++)// Bubble collision with bullet and shooter
        {
            Point bubble_center = {bubbles[i].get_center_x(),bubbles[i].get_center_y()};
            for (unsigned int j = 0; j < bullets.size(); j++)
            {
                Point bullet_center = {bullets[j].get_center_x(),bullets[j].get_center_y()};
                if (distance(bubble_center, bullet_center) <= bubbles[i].get_radius()+bullets[j].get_height()/2)
                {   // Bubble bullet collision
                    bullets.erase(bullets.begin()+j);
                    score++;
                    Score_String[Score_String.length() - 1] = 48+score;
                    Score_Text.setMessage(Score_String);
                    if (bubbles[i].get_radius()==BUBBLE_DEFAULT_RADIUS)
                    {
                        bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS/2, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
                        bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS/2, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
                        bubbles.erase(bubbles.begin()+i);
                    }
                    else
                    {
                        bubbles.erase(bubbles.begin()+i);
                    }
                }
            }
            if (shooter_bubble_collision(bubbles[i], shooter))// Shooter bullet collision
            {
                health--;
                Health_String = Health_String.substr(0, 8) + to_string(health);
                Health_Text.setMessage(Health_String);
                bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, bubbles[i].get_radius(), BUBBLE_DEFAULT_VX, -BUBBLE_DEFAULT_VX, COLOR(255,105,180)));
                bubbles.erase(bubbles.begin()+i);
            }
        }
        if (health <= 0||time <=0||score>=6)
        {
            for(unsigned int i=0; i<bullets.size(); i++)
            {
               bullets.erase(bullets.begin()+i);
            }
            for(unsigned int i=0; i<bubbles.size(); i++)
            {
               bubbles.erase(bubbles.begin()+i); 
            }
            wait(1);
            cout << "END OF GAME" << endl;
            break;
        }
        if (micro_time_counter%25==0)
        {
            time--;
            Time_String = Time_String.substr(0, 6) + to_string(time);
            Time_Text.setMessage(Time_String);
        }
        wait(STEP_TIME);
        micro_time_counter++;
    }
    if (health <=0)
    {
        Status_String = Status_String.substr(0,7) + "Lost by Health";
        Status_Text.setMessage(Status_String);
    }
    else if (time <=0)
    {
        Status_String = Status_String.substr(0,7) + "Lost by Time";
        Status_Text.setMessage(Status_String);
    }
    else
    {
        Status_String = Status_String.substr(0,7) + "You won";
        Status_Text.setMessage(Status_String);
    }    
    wait(3);
}
