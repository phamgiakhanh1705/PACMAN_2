#include "game_engine.h"
#include <limits>
#include <random>
#include <time.h>

using namespace std;

typedef std :: pair <int , int> II;
typedef std :: pair <int , std :: pair <int , int> > IP;

void Engine :: init(SDL_Renderer* &renderer)
{
    map = new Map();
    objectTexture = new TextureSrc();
    objectTexture->loadTileTexture(renderer);
    objectTexture->loadPacmanAndGhostTexture(renderer);
    tickManager = new TickManager();
    gameManager = new GameManager(renderer);
    soundManager = new SoundManager();
    soundManager->loadSound();
    srand(time(nullptr));
    apple = new Item(renderer , "Assets/Entity Image/greenapple.png");
    newGame();
    SDL_Surface* Image = IMG_Load("Assets/Menu Image/nextlevel.png");
    nextLevel = SDL_CreateTextureFromSurface(renderer , Image);
    SDL_FreeSurface(Image);
    Image = IMG_Load("Assets/Menu Image/ready.png");
    ready = SDL_CreateTextureFromSurface(renderer , Image);
    SDL_FreeSurface(Image);
}

void Engine :: newGame()
{
    map->map_reset();
    gameManager->reset();
    delete pacman;
    pacman = new Pacman();
    delete blinky;
    if(gameManager->getLevel() < 5) blinky = new Ghost(13 , 11 , false);
    else blinky = new Ghost(12 , 11 , false);
    delete pinky;
    pinky = new Ghost(13 , 14 , true);
    delete inky;
    inky = new Ghost(11 , 14 , true);
    delete clyde;
    clyde = new Ghost(15 , 14 , true);
    if(gameManager->getLevel() >= 1)
    {
        apple->spawnAt(1 , 1);
        delete greendy;
        greendy = new Ghost(12 , 15 , true);
    }
    if(gameManager->getLevel() >= 2)
    {
        delete friendy;
        friendy = new Ghost(14 , 11 , false);
    }
    soundManager->insertPlayList(SoundManager :: START);
    tickManager->resetTick(gameManager->getLevel());
    tickManager->pauseTick(true);
    runningEGBoard = false;
}

void Engine :: respawnObject()
{
    delete pacman;
    pacman = new Pacman();
    delete blinky;
    blinky = new Ghost(13 , 11 , false);
    delete pinky;
    pinky = new Ghost(13 , 14 , true);
    delete inky;
    inky = new Ghost(11 , 14 , true);
    delete clyde;
    clyde = new Ghost(15 , 14 , true);
    if(greendy != nullptr)
    {
        delete greendy;
        greendy = new Ghost(12 , 15 , true);
    }
    if(friendy != nullptr)
    {
        delete friendy;
        friendy = new Ghost(14 , 11 , false);
    }
    soundManager->reset();
    tickManager->pauseTick(false);
}

void Engine :: handleEvent(SDL_Event &e , std :: vector<std :: string> &scoreData)
{
    if(Mix_Playing(2) || Mix_Playing(4)) return;
    if(pacman->isDead())
    {
        if(runningEGBoard) gameManager->handleEGBoard(e , scoreData);
        return;
    }
    if(e.type == SDL_KEYDOWN)
    {
        if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT ||
           e.key.keysym.sym == SDLK_s    || e.key.keysym.sym == SDLK_w  || e.key.keysym.sym == SDLK_a    || e.key.keysym.sym == SDLK_d    )
        {
            int newDir = -1 , lastDir = -1;
            int pacmanTileX = pacman->getTileX();
            int pacmanTileY = pacman->getTileY();
            int pacmanPosX = pacman->getPosX();
            int pacmanPosY = pacman->getPosY();
            if(!pacman->is_direction_empty()) lastDir = pacman->get_pacman_direction();
            if(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) newDir = 0;
            else if(e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) newDir = 1;
            else if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) newDir = 2;
            else if(e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) newDir = 3;
            if(lastDir == -1)
            {
                if(map->can_change_direction(pacmanTileX , pacmanTileY , newDir))
                {
                    pacman->clear_special();
                    pacman->push_to_direction(newDir);
                }
            }
            else
            {
                if(newDir % 2 == lastDir % 2)
                {
                    if(map->can_change_direction(pacmanTileX , pacmanTileY , newDir))
                    {
                        pacman->push_to_direction(newDir);
                        pacman->clear_special();
                    }
                }
                else
                {
                    std :: pair<int , int> nextCross = map->get_nearest_cross_tile_id(pacmanTileX , pacmanTileY , lastDir);
                    if(lastDir % 2 == 1 && newDir % 2 == 0)
                    {
                        if(pacmanPosY == pacmanTileY * 16)
                        {
                            if(map->can_change_direction(pacmanTileX , pacmanTileY , newDir))
                                pacman->push_to_special(newDir , II(pacmanTileX , pacmanTileY));
                            else if(nextCross != II(-1 , -1) && !map->is_beside_cross_is_wall(nextCross , newDir) && abs(pacmanPosX - nextCross.first * 16) <= 32)
                                pacman->push_to_special(newDir , nextCross);
                        }
                    }
                    else if(lastDir % 2 == 0 && newDir % 2 == 1)
                    {
                        if(pacmanPosX == pacmanTileX * 16)
                        {
                            if(map->can_change_direction(pacmanTileX , pacmanTileY , newDir))
                                pacman->push_to_special(newDir , II(pacmanTileX , pacmanTileY));
                            else if(nextCross != II(-1 , -1) && !map->is_beside_cross_is_wall(nextCross , newDir) && abs(pacmanPosY - nextCross.second * 16) <= 32)
                                pacman->push_to_special(newDir , nextCross);
                        }
                    }
                }
            }
        }
    }
}

void Engine :: render(SDL_Renderer* &renderer , const std :: vector<std :: string> &scoreData)
{
    tickManager->stablizeFPS();
    SDL_Rect dsRect;
    for(int i = 0; i < 28; ++i)
    {
        for(int j = 0; j < 31; ++j)
        {
            dsRect = {i * 16 + 217 , j * 16 , 16 , 16};
            objectTexture->renderTileTexture(renderer , map->get_tile_id(i , j) , &dsRect);
        }
    }
    if(gameManager->getLevel() >= 3 && !apple->isDestroyed()) apple->renderItem(renderer);

    if(!runningEGBoard)
    {
        int dir = -1;
        if(!pacman->is_direction_empty()) dir = pacman->get_pacman_direction();
        if(!pacman->isDead())
        {
            renderGhost(renderer , blinky , TextureSrc :: BLINKY);
            renderGhost(renderer , pinky , TextureSrc :: PINKY);
            renderGhost(renderer , inky , TextureSrc :: INKY);
            renderGhost(renderer , clyde , TextureSrc :: CLYDE);
            if(greendy != nullptr) renderGhost(renderer , greendy , TextureSrc :: GREENDY);
            if(friendy != nullptr) renderGhost(renderer , friendy , TextureSrc :: FRIENDY);
            if(Mix_Playing(2))
            {
                dsRect = {441 - 82 , 285 - 15 - 7 , 164 , 30};
                SDL_RenderCopy(renderer , ready , nullptr , &dsRect);
            }
        }
        if(pacman->isDead())
        {
            if(objectTexture->pacmanIsDead())
            {
                if(gameManager->getRemainLife() > 0) respawnObject();
                else
                {
                    runningEGBoard = true;
                    gameManager->checkScoreData(scoreData);
                }
            }
            else objectTexture->renderPacmanTexture(renderer , pacman->getPosX() , pacman->getPosY() , TextureSrc :: DEAD_PACMAN);
        }
        else objectTexture->renderPacmanTexture(renderer , pacman->getPosX() , pacman->getPosY() , dir);
        if(waitTime > 0)
        {
            dsRect = {441 - 97 , 248 - 52 , 194 , 104};
            SDL_RenderCopy(renderer , nextLevel , nullptr , &dsRect);
        }
        if(Mix_Playing(4))
        {
            objectTexture->renderGhostScore(renderer , gameManager->getEatenGhostPosX() , gameManager->getEatenGhostPosY() , gameManager->getEatenGhostStreak());
        }
        soundManager->playSound();
    }

    if(runningEGBoard) gameManager->runEGBoard(renderer);
    else gameManager->renderHUD(renderer);
}

void Engine :: pacmanMeatGhost(Ghost* &ghost)
{
    if(ghost->isDead()) return;
    int distance = (pacman->getPosX() - ghost->getPosX()) * (pacman->getPosX() - ghost->getPosX()) +
                   (pacman->getPosY() - ghost->getPosY()) * (pacman->getPosY() - ghost->getPosY());
    if(distance <= 9)
    {
        if(ghost->is_ghost_frighten())
        {
            gameManager->eatGhost(ghost->getPosX() , ghost->getPosY());
            ghost->setDead(true);
            ghost->set_ghost_frighten(false);
            soundManager->insertPlayList(SoundManager :: EAT_GHOST);
            soundManager->insertPlayList(SoundManager :: GHOST_GO_HOME);
        }
        else
        {
            pacman->setDead(true , 1);
            gameManager->lostALife();
            soundManager->insertPlayList(SoundManager :: DEAD);
            tickManager->pauseTick(true);
        }
    }
}

void Engine :: renderGhost(SDL_Renderer* &renderer , Ghost* &ghost , int ghostID)
{
    if(ghost == nullptr) return;
    if(ghost->isDead())
    {
        objectTexture->renderGhostTexture(renderer , ghost->getPosX() , ghost->getPosY() , TextureSrc :: GHOST_SPIRIT , ghost->get_ghost_direction());
    }
    else if(ghost->is_ghost_frighten())
    {
        if(tickManager->remainFrightenTime() < 2.0)
        {
            objectTexture->renderGhostTexture(renderer , ghost->getPosX() , ghost->getPosY() , ghostID , TextureSrc :: FRIGHTEN_GHOST_2);
        }
        else
        {
            objectTexture->renderGhostTexture(renderer , ghost->getPosX() , ghost->getPosY() , ghostID , TextureSrc :: FRIGHTEN_GHOST_1);
        }
    }
    else
    {
        objectTexture->renderGhostTexture(renderer , ghost->getPosX() , ghost->getPosY() , ghostID , ghost->get_ghost_direction());
    }
}

void Engine :: ghostMove(Ghost* &ghost)
{
    if(ghost == nullptr) return;
    int ghostTileX = ghost->getTileX();
    int ghostTileY = ghost->getTileY();
    int ghostPosX = ghost->getPosX();
    int ghostPosY = ghost->getPosY();
    int ghostOldDir = ghost->get_ghost_direction();
    int ghostNextTileX = ghost->get_target_tile_col();
    int ghostNextTileY = ghost->get_target_tile_row();

    if(ghostTileX * 16 == ghostPosX && ghostTileY * 16 == ghostPosY)
    {
        if(map->is_tile_cross(ghostTileX , ghostTileY))
        {
            if(ghost->is_ghost_frighten() || (ghost == friendy && !tickManager->isFriendyChaseTime()))
            {
                std :: stack<int> whichDir;
                if(ghostOldDir % 2 == 1)
                {
                    if(map->can_change_direction(ghostTileX , ghostTileY , Map :: UP)) whichDir.push(0);
                    if(map->can_change_direction(ghostTileX , ghostTileY , Map :: DOWN)) whichDir.push(2);
                    if(map->can_change_direction(ghostTileX , ghostTileY , ghostOldDir)) whichDir.push(ghostOldDir);
                }
                else
                {
                    if(map->can_change_direction(ghostTileX , ghostTileY , Map :: LEFT)) whichDir.push(3);
                    if(map->can_change_direction(ghostTileX , ghostTileY , Map :: RIGHT)) whichDir.push(1);
                    if(map->can_change_direction(ghostTileX , ghostTileY , ghostOldDir)) whichDir.push(ghostOldDir);
                }
                int dir = rand() % (int) whichDir.size() + 1;
                while(dir > 1) whichDir.pop() , --dir;
                ghost->set_ghost_direction(whichDir.top());
                while(!whichDir.empty()) whichDir.pop();
            }
            else
            {
                int dU = __INT32_MAX__ , dD = __INT32_MAX__ , dL = __INT32_MAX__ , dR = __INT32_MAX__;
                if(map->can_change_direction(ghostTileX , ghostTileY , Map :: UP))
                    dU = map->get_distance(II(ghostTileX , ghostTileY - 1) , II(ghostNextTileX , ghostNextTileY) , Map :: UP);
                if(map->can_change_direction(ghostTileX , ghostTileY , Map :: DOWN))
                    dD = map->get_distance(II(ghostTileX , ghostTileY + 1) , II(ghostNextTileX , ghostNextTileY) , Map :: DOWN);
                if(map->can_change_direction(ghostTileX , ghostTileY , Map :: LEFT))
                    dL = map->get_distance(II(ghostTileX - 1 , ghostTileY) , II(ghostNextTileX , ghostNextTileY) , Map :: LEFT);
                if(map->can_change_direction(ghostTileX , ghostTileY , Map :: RIGHT))
                    dR = map->get_distance(II(ghostTileX + 1 , ghostTileY) , II(ghostNextTileX , ghostNextTileY) , Map :: RIGHT);

                if(ghostOldDir == Map :: UP)
                {
                    int dMin = min({dU , dL , dR});
                    if(dMin == dU) ghost->set_ghost_direction(Map :: UP);
                    else if(dMin == dL) ghost->set_ghost_direction(Map :: LEFT);
                    else ghost->set_ghost_direction(Map :: RIGHT);
                }
                else if(ghostOldDir == Map :: DOWN)
                {
                    int dMin = min({dD , dL , dR});
                    if(dMin == dD) ghost->set_ghost_direction(Map :: DOWN);
                    else if(dMin == dL) ghost->set_ghost_direction(Map :: LEFT);
                    else ghost->set_ghost_direction(Map :: RIGHT);
                }
                else if(ghostOldDir == Map :: LEFT)
                {
                    int dMin = min({dU , dD , dL});
                    if(dMin == dU) ghost->set_ghost_direction(Map :: UP);
                    else if(dMin == dD) ghost->set_ghost_direction(Map :: DOWN);
                    else ghost->set_ghost_direction(Map :: LEFT);
                }
                else if(ghostOldDir == Map :: RIGHT)
                {
                    int dMin = min({dU , dD , dR});
                    if(dMin == dU) ghost->set_ghost_direction(Map :: UP);
                    else if(dMin == dR) ghost->set_ghost_direction(Map :: RIGHT);
                    else ghost->set_ghost_direction(Map :: DOWN);
                }
            }
        }
        if(map->can_change_direction(ghostTileX , ghostTileY , ghost->get_ghost_direction())) ghost->ghost_moving();
    }
    else
    {
        if(map->can_change_direction(ghostTileX , ghostTileY , ghost->get_ghost_direction())) ghost->ghost_moving();
        else
        {
            if(ghostTileX * 16 == ghostPosX && ghostTileY * 16 != ghostPosY && ghost->get_ghost_direction() % 2 == 0) ghost->ghost_moving();
            else if(ghostTileY * 16 == ghostPosY && ghostTileX * 16 != ghostPosX && ghost->get_ghost_direction() % 2 == 1) ghost->ghost_moving();
        }
    }
    ghost->goThroughTunnel();
    if(ghostPosX == ghostNextTileX * 16 && ghostPosY == ghostNextTileY * 16)
    {
        if(ghost->isDead())
        {
            ghost->setDead(false);
            soundManager->insertPlayList(SoundManager :: REVIVAL_GHOST);
        }
        else if(ghost == greendy)
        {
            tickManager->greendyStartChasePacman();
            ghost->resetObjectTile(ghostTileX , ghostTileY);
            apple->destroyItem();
            eatGreenApple = true;
        }
    }
    pacmanMeatGhost(ghost);
}

void Engine :: loop(bool &exitToMenu)
{
    if(gameManager->clearAllCoins())
    {
        if(waitTime > 0) --waitTime;
        else
        {
            gameManager->nextLevel();
            tickManager->resetTick(gameManager->getLevel());
            respawnObject();
            map->map_reset();
        }
        return;
    }
    if(Mix_Playing(2) || Mix_Playing(4))
    {
        if(Mix_Playing(2)) tickManager->pauseTick(true);
        return;
    }
    if(pacman->isDead())
    {
        int decision = gameManager->getPlayerDecision();
        if(runningEGBoard)
        {
            if(decision == GameManager :: AGAIN) newGame();
            else if(decision == GameManager :: QUIT) exitToMenu = true;
        }
        return;
    }

    tickManager->updateStatus();

    int pacmanTileX = pacman->getTileX();
    int pacmanTileY = pacman->getTileY();
    int pacmanPosX = pacman->getPosX();
    int pacmanPosY = pacman->getPosY();
    int lastDir = -1;
    if(!pacman->is_direction_empty()) lastDir = pacman->get_pacman_direction();

    if(!pacman->isDead() && lastDir != -1)
    {
        if(pacmanTileX * 16 == pacmanPosX && pacmanTileY * 16 == pacmanPosY)
        {
            if(map->is_tile_cross(pacmanTileX , pacmanTileY))
            {
                if(!pacman->is_special_empty() && pacman->get_pacman_special_cross_id() == II(pacmanTileX , pacmanTileY)) pacman->pacman_turn();
            }
            if(map->can_change_direction(pacmanTileX , pacmanTileY , pacman->get_pacman_direction())) pacman->pacman_moving();
            else pacman->pacman_stopmoving();
        }
        else
        {
            if(map->can_change_direction(pacmanTileX , pacmanTileY , lastDir)) pacman->pacman_moving();
            else
            {
                if(pacmanTileX * 16 == pacmanPosX && pacmanTileY * 16 != pacmanPosY) pacman->pacman_moving();
                else if(pacmanTileX * 16 != pacmanPosX && pacmanTileY * 16 == pacmanPosY) pacman->pacman_moving();
                else pacman->pacman_stopmoving();
            }
        }
    }

    int remainCoin = gameManager->getRemainCoin();
    if(remainCoin < 50) soundManager->insertPlayList(SoundManager :: MOVE_3);
    else if(remainCoin < 100) soundManager->insertPlayList(SoundManager :: MOVE_2);
    else if(remainCoin < 150) soundManager->insertPlayList(SoundManager :: MOVE_1);
    else soundManager->insertPlayList(SoundManager :: MOVE_0);

    pacmanTileX = pacman->getTileX();
    pacmanTileY = pacman->getTileY();
    int typeOfCoin = map->pacman_eat_coins(pacmanTileX , pacmanTileY);
    if(typeOfCoin != GameManager :: notCoin)
    {
        gameManager->eatCoins(typeOfCoin);
        soundManager->insertPlayList(SoundManager :: EAT_DOT);
        if(typeOfCoin == GameManager :: superCoin)
        {
            tickManager->setFrightenTime();
            soundManager->insertPlayList(SoundManager :: GHOST_TURN_BLUE);
            if(!blinky->isDead()) blinky->set_ghost_frighten(true);
            if(!pinky ->isDead()) pinky ->set_ghost_frighten(true);
            if(!inky  ->isDead()) inky  ->set_ghost_frighten(true);
            if(!clyde ->isDead()) clyde ->set_ghost_frighten(true);
            if(greendy != nullptr && !greendy->isDead()) greendy->set_ghost_frighten(true);
            if(friendy != nullptr) tickManager->friendyStartChasePacman();
        }
    }

    if(!tickManager->isFrightenTime())
    {
        soundManager->insertPlayList(SoundManager :: NORMAL_GHOST);
        blinky->set_ghost_frighten(false);
        pinky ->set_ghost_frighten(false);
        inky  ->set_ghost_frighten(false);
        clyde ->set_ghost_frighten(false);
        if(greendy != nullptr) greendy->set_ghost_frighten(false);
    }

    bool scatter = tickManager->isScatteringTime();
    blinky->set_ghost_scatter(scatter);
    pinky ->set_ghost_scatter(scatter);
    inky  ->set_ghost_scatter(scatter);
    clyde ->set_ghost_scatter(scatter);

    if(!tickManager->isGreendyChaseTime())
    {
        eatGreenApple = false;
    }
    else if(apple->isDestroyed())
    {
        int r = rand() % 4;
        if(r == 0) apple->spawnAt(1 , 1);
        else if(r == 1) apple->spawnAt(26 , 1);
        else if(r == 2) apple->spawnAt(26 , 29);
        else apple->spawnAt(1 , 29);
    }

    pacmanPosX = pacman->getPosX();
    pacmanPosY = pacman->getPosY();
    lastDir = -1;
    if(!pacman->is_direction_empty()) lastDir = pacman->get_pacman_direction();

    if(!pacman->isDead())
    {
        tickManager->pauseTick(false);
        if(blinky->isDead()) blinky->set_ghost_destination(13 , 11);
        else if(!blinky->is_ghost_scatter()) blinky->set_ghost_destination(pacmanTileX , pacmanTileY);
        else blinky->set_ghost_destination(Ghost :: DEFAULT_BLINKY_TILE_COL , Ghost :: DEFAULT_BLINKY_TILE_ROW);

        if(pinky->isDead()) pinky->set_ghost_destination(13 , 11);
        else if(!pinky->is_ghost_scatter())
        {
            if(lastDir == Map :: UP) pinky->set_ghost_destination(pacmanTileX , pacmanTileY - 4);
            else if(lastDir == Map :: DOWN) pinky->set_ghost_destination(pacmanTileX , pacmanTileY + 4);
            else if(lastDir == Map :: LEFT) pinky->set_ghost_destination(pacmanTileX - 4 , pacmanTileY);
            else if(lastDir == Map :: RIGHT) pinky->set_ghost_destination(pacmanTileX + 4 , pacmanTileY);
        }
        else pinky->set_ghost_destination(Ghost :: DEFAULT_PINKY_TILE_COL , Ghost :: DEFAULT_PINKY_TILE_ROW);

        if(inky->isDead()) inky->set_ghost_destination(13 , 11);
        else if(!inky->is_ghost_scatter()) inky->set_ghost_destination(2 * pacmanTileX - blinky->getTileX() , 2 * pacmanTileY - blinky->getTileY());
        else inky->set_ghost_destination(Ghost :: DEFAULT_INKY_TILE_COL , Ghost :: DEFAULT_INKY_TILE_ROW);

        if(clyde->isDead()) clyde->set_ghost_destination(13 , 11);
        else if(!clyde->is_ghost_scatter())
        {
            int dx = pacmanTileX - clyde->getTileX();
            int dy = pacmanTileY - clyde->getTileY();
            if(dx * dx + dy * dy <= 64) clyde->set_ghost_destination(Ghost :: DEFAULT_CLYDE_TILE_COL , Ghost :: DEFAULT_CLYDE_TILE_ROW);
            else clyde->set_ghost_destination(pacmanTileX , pacmanTileY);
        }
        else clyde->set_ghost_destination(Ghost :: DEFAULT_CLYDE_TILE_COL , Ghost :: DEFAULT_CLYDE_TILE_ROW);

        if(greendy != nullptr)
        {
            if(greendy->isDead()) greendy->set_ghost_destination(13 , 11);
            else if(!eatGreenApple) greendy->set_ghost_destination(apple->getPosX() , apple->getPosY());
            else if(!greendy->is_ghost_frighten()) greendy->set_ghost_destination(pacmanTileX , pacmanTileY , 2);
        }

        if(friendy != nullptr && tickManager->isFriendyChaseTime())
        {
            friendy->set_ghost_destination(pacmanTileX , pacmanTileY , 1);
        }
    }

    pacman->goThroughTunnel();
    ghostMove(blinky);
    ghostMove(pinky);
    ghostMove(inky);
    ghostMove(clyde);
    ghostMove(greendy);
    ghostMove(friendy);

    gameManager->handleGhostPos(pinky , inky , clyde , greendy);

    if(gameManager->clearAllCoins())
    {
        soundManager->insertPlayList(SoundManager :: WINNING);
        waitTime = 100;
    }
}
