struct Sprite_Animation
{
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
};

extern struct Sprite_Animation standing;
extern struct Sprite_Animation running;
extern struct Sprite_Animation idle;
extern struct Sprite_Animation mining;
extern struct Sprite_Animation dying;



void Animation(int Player_State)
{
    if(Player_State == 0)
    {
        if(++standing.frameCount >= standing.frameDelay)
        {
            if(++standing.curFrame >= standing.maxFrame)
                standing.curFrame = 0;
            standing.frameCount = 0;
        }
    }
    else
    {
        standing.curFrame = 0;
    }

    if(Player_State == 1 || Player_State == 2)
    {
        if(++running.frameCount >= running.frameDelay)
        {
            if(++running.curFrame >= running.maxFrame)
                running.curFrame = 0;
            running.frameCount = 0;
        }
    }
    else
    {
        running.curFrame = 0;
    }

    if(Player_State == 3)
    {
        if(++idle.frameCount >= idle.frameDelay)
        {
            if(++idle.curFrame >= idle.maxFrame)
                idle.curFrame = 1;
            idle.frameCount = 0;
        }
    }
    else
    {
        idle.curFrame = 0;
    }

    if(Player_State == 4)
    {
        if(++mining.frameCount >= mining.frameDelay)
        {
            if(++mining.curFrame >= mining.maxFrame)
                mining.curFrame = 0;
            mining.frameCount = 0;
        }
    }
    else
    {
        mining.curFrame = 0;
    }

    if(Player_State == 5)
    {
        if(++dying.frameCount >= dying.frameDelay)
        {
            if(++dying.curFrame >= dying.maxFrame)
                dying.curFrame = 0;
            dying.frameCount = 0;
        }
    }
    else
    {
        dying.curFrame = 0;
    }
}
