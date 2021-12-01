#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
using namespace sf;

RenderWindow window(VideoMode (96,96), "Tic-tac-toe");

//cистемное значение
const int GAME = -2;
const int DRAW = -1;
//действующие и системные значения
const int NO_VAL = 0;
const int X_VAL = 1;
const int O_VAL = 2;

enum class Game_State{
MENU = 0,AUTHOR,CHOOSE_MODE,CHOOSE_SIZE,CHOOSE_AI,CHOOSE,INGAME,WINNER_FRAME,END};

enum class Win{WTIC = 0, WTAC};


class Board{
public:
    bool is_big;
    std::vector<std::vector<int>> map;
    
    void setBoard(bool big)
    {
        if(big)
        {
            map.resize(5);
            for(int y = 0;y < 5;y++)
            {
                map[y].resize(5);
            }
            is_big = true;
        }else{
            map.resize(3);
            for(int y = 0;y < 3;y++)
            {
                map[y].resize(3);
            }
            is_big = false;
        }
    }
    
    int screan(int size)
    {
        int winner;
        int counter;
        int raw,column;
        
        for( int win = 1; win <= 2; win++)
        {
            for ( int diag = 0; diag < size; diag++)
            {
                raw = diag;
                column = diag;
                
                counter = 0;
                for(int x = 0;x < size;x++)
                {
                    if(map[raw][x] == win) counter++;
                }
                if(size == 3 && counter == 3) 
                {
                    return win;
                }else if(size == 5 && counter == 5){
                    return win;
                }else counter = 0;
                
                for(int y = 0;y < size;y++)
                {
                    if(map[y][column] == win) counter++;
                }
                if(size == 3 && counter == 3) 
                {
                    return win;
                }else if(size == 5 && counter == 5){
                    return win;
                }else counter = 0;
                
            }
            for(int y = 0;y < size;y++)
            {
                if(map[y][y] == win) counter++;
            }
            if(size == 3 && counter == 3) 
            {
                return win;
            }else if(size == 5 && counter == 5){
                return win;
            }else counter = 0;
            
            for(int y = 0;y < size;y++)
            {
                if(map[size-1-y][y] == win) counter++;
            }
            if(size == 3 && counter == 3) 
            {
                return win;
            }else if(size == 5 && counter == 5){
                return win;
            }else counter = 0;
        }
        
        
        for(int y = 0;y < size;y++){
            for(int x = 0;x < size;x++){
                if(map[y][x] == NO_VAL) return GAME;
            }
        }
        
        return DRAW;
    }
    
    int checkVictory()
    {
        if(is_big){
            screan (5);
        }else{
            screan (3);
        }
    }
    
    void setValue(int x,int y,const int XorO)
    {
        if(map[x][y] == NO_VAL)
        {
            map[x][y] = XorO;
        }
    }
    
    int getValue(int x,int y)
    {
        return map[x][y];
    }
    
    void removeValue(int x,int y)
    {
        map[x][y] = NO_VAL;
    }
    
    size_t getSize(){
        if(is_big) return 5;
        else return 3;
    }
    
    int checkFill(){
        int counter = 0;
        for(int y = 0;y < map.size();y++){
            for(int x = 0;x < map.size();x++){
                if(map[y][x] != NO_VAL) counter++;
            }
        }
        return counter;
    }
    
};

struct AiMove{
    AiMove(){};
    AiMove(int Score):score(Score){}; 
    int x;
    int y;
    int score;
};

class AI
{
    bool is_map_big;
public:
    void init(const int aiPlayer,bool big)
    {
        _aiPlayer = aiPlayer;
        if(_aiPlayer == X_VAL){
            _humanPlayer = O_VAL;
        }else{
            _humanPlayer = X_VAL;
        }
        is_map_big = big;
        
    }
    void performMove(Board& board)
    {
        int how_m_fill = board.checkFill();
        AiMove bestMove;
        if(how_m_fill >= 5){
            bestMove = getBestMove(board,_aiPlayer,1,20);
        } else {
            bestMove = getBestMove(board,_aiPlayer,1,10);
        }
        if(board.checkVictory()!= DRAW){
            board.setValue(bestMove.x,bestMove.y,_aiPlayer);
        }else return;
    }
private:
    AiMove getBestMove(Board& board,int player,int from_level,int to_level)
    {
        int rv = board.checkVictory();
        if (rv == _aiPlayer){
            return AiMove(10);
        }else if (rv == _humanPlayer){
            return AiMove(-10);
        }else if (rv == DRAW){
            return AiMove(0);
        }
        
        if(from_level > to_level && is_map_big) return AiMove(0);
        
        std::vector<AiMove> moves;
        
        for (int y = 0;y < board.getSize(); y++){
            for(int x = 0;x < board.getSize();x++){
                if(board.getValue(y,x) == NO_VAL){
                    AiMove move;
                    move.x = y;
                    move.y = x;
                    board.setValue(y, x ,player);
                    if(player == _aiPlayer){
                        from_level++;
                        move.score = getBestMove(board,_humanPlayer,from_level,to_level).score;
                    }else{
                        from_level++;
                        move.score = getBestMove(board,_aiPlayer,from_level,to_level).score;
                    }
                    moves.push_back(move);
                    board.removeValue(y,x);
                }
            }
        }
        
        int bestMove = 0;
        if(player == _aiPlayer) {
            int bestScore = -100000000;
            for (int i = 0; i < moves.size();i++){
                if(moves[i].score > bestScore){
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        } else {
            int bestScore = 100000000;
            for(int i = 0; i < moves.size();i++) {
                if(moves[i].score < bestScore){
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }
        return moves[bestMove];
        
    }
    int _aiPlayer;
    int _humanPlayer;
};

class Engine
{
public:
    Sprite s_map;
    
    bool is_tic;
    int winner = GAME;
    
    AI supercomp;
    bool is_big = false;
    bool is_ai_on = false;
    bool is_ai_turn = false;
    bool is_ai_X = false;
    
    AI supercomp_sec;
    bool ai_vs_ai = false;
    bool ai1_turn = false;
    
    Board map;

    int humanPlayer;
    
    void inti_eng_ai(){
       
    }
    
    void init_eng()
    {
        map.setBoard(is_big);
        
        if(is_ai_on && is_ai_turn)
        {
            if(is_ai_X){
                supercomp.init(X_VAL,is_big);
                humanPlayer = O_VAL;
            }else{
                supercomp.init(O_VAL,is_big);
                humanPlayer = X_VAL;
            }
        }else if (is_ai_on && !is_ai_turn){
            if(is_ai_X){
                supercomp.init(X_VAL,is_big);
                humanPlayer = O_VAL;
            }else{
                supercomp.init(O_VAL,is_big);
                humanPlayer = X_VAL;
            }  
        }else if (ai_vs_ai){
            supercomp.init(X_VAL,is_big);
            supercomp_sec.init(O_VAL,is_big);
        }
    }
    
    
    void update_game_proc(Vector2f vec)
    {
        int raw = vec.y/32;
        int column = vec.x/32;
        
        if(!is_ai_on)
        {
            if(is_tic){
                map.setValue(raw,column,X_VAL);
                winner = map.checkVictory();
                is_tic = false;
            }else{
                map.setValue(raw,column,O_VAL);
                winner = map.checkVictory();
                is_tic = true;
            }
        }else{
            if(is_ai_turn){
                
                supercomp.performMove(map);
                winner = map.checkVictory();
                is_ai_turn = false;
            }else{
                if(map.getValue(raw,column) == NO_VAL){
                    map.setValue(raw,column,humanPlayer);
                    winner = map.checkVictory();
                    is_ai_turn = true;
                }
            }
        }
    }
    
    void update_game_proc_ai(){
            if(ai1_turn)
            {winner = map.checkVictory(); 
                supercomp.performMove(map);
                 
                ai1_turn = false;
            }else{winner = map.checkVictory();
                supercomp_sec.performMove(map);
                
                ai1_turn = true;
            }
        }
    
    void draw_map(Sprite s_map)
    {
        window.clear();
        for(int y = 0;y < map.getSize();y++)
        {
            for(int x = 0;x < map.getSize();x++)
            {
                if(map.getValue(y,x) == 0) s_map.setTextureRect(IntRect(0,0,32,32));
                if(map.getValue(y,x) == 1) s_map.setTextureRect(IntRect(64,0,32,32));
                if(map.getValue(y,x) == 2) s_map.setTextureRect(IntRect(32,0,32,32));
                
                s_map.setPosition(x*32,y*32);
                window.draw(s_map);
            }
        }
        window.display();
    }
    
            
};


class Game 
{
public:
    //состояние игры
    Game_State state;
    
    //Тайлы карты

    Texture map_texture;
    Sprite s_map;
    Font font;
    
    
    //Стиль шрифта в игре

    
    //подключаем наш игровой движок
    Engine engine;
    
    Game (Sprite map, Font f,Texture m):s_map(map),font(f),map_texture(m)
    {
        state = Game_State::MENU;
    }
    
    
    //подключаем обработчик состояния игры
    void runGame()
    {
            while (state != Game_State::END)
            {
                    switch (state)
                    {
                        case Game_State::MENU:
                            menu();   
                            break;
                            
                        case Game_State::AUTHOR:
                            author();
                            break;
                            
                        case Game_State::CHOOSE_SIZE:
                            choose_size();
                            break;
                            
                        case Game_State::CHOOSE_MODE:
                            choose_mode();
                            break;
                            
                        case Game_State::CHOOSE:
                            choose_first();
                            break;
                            
                        case Game_State::CHOOSE_AI:
                            choose_ai();
                            break;
                            
                        case Game_State::INGAME:
                            game();
                            break;
                            
                        case Game_State::WINNER_FRAME:
                            win_frame();
                            break;
                    }
            }
    }
    
    void menu()
    {
            window.clear();
            
            Text title("Tic-tac-toE",font,10);
            title.setStyle(Text::Bold);
            title.setPosition(96/2 - title.getGlobalBounds().width/2,10);
            
            const int count_bar = 3;
            Text bars[count_bar];
            
            std::string name_bar[] = {"Play","Author","Exit"};
            
            for(int i = 0;i < 3;i++)
            {
                bars[i].setFont(font);
                bars[i].setCharacterSize(10);
                bars[i].setString(name_bar[i]);
                bars[i].setPosition(96/2 - bars[i].getGlobalBounds().width/2,30 + i*20);
                
                window.draw(bars[i]);
            }
            
            window.draw(title);
            window.display();
            
            while(state == Game_State::MENU)
            {
                
                Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));
                
                Event M_event;
                while(window.pollEvent(M_event))
                {
                    if(M_event.type == Event::Closed)
                    { 
                        state = Game_State::END;
                        window.close();
                        continue;
                    }
                    
                    if(M_event.type == Event::KeyPressed 
                        && M_event.key.code == Keyboard::Escape) window.close();
                    
                    if(bars[0].getGlobalBounds().contains(globMouse)
                        && M_event.type == Event::MouseButtonPressed
                        && M_event.key.code == Mouse::Left)
                    {
                        state = Game_State::CHOOSE_SIZE;
                    }
                    if(bars[1].getGlobalBounds().contains(globMouse)
                        && M_event.type == Event::MouseButtonPressed
                        && M_event.key.code == Mouse::Left)
                    {
                        state = Game_State::AUTHOR;
                    }
                    if(bars[2].getGlobalBounds().contains(globMouse)
                        && M_event.type == Event::MouseButtonPressed
                        && M_event.key.code == Mouse::Left)
                    {
                        state = Game_State::END;
                        window.close();
                    }
                }
            }
    }
    
    void author()
    {
            window.clear();
            Text log(" Egorov \nVsevolod",font,10);
            Text tap_any(" Tap any key \nand go to menu",font,7);
            log.setPosition(96/2 - log.getGlobalBounds().width/2,96/2 - log.getGlobalBounds().height/2);
            tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
            window.draw(log);
            window.draw(tap_any);
            window.display();
            
            while(state == Game_State::AUTHOR)
            {
                Event A_event;
                while(window.pollEvent(A_event))
                {
                    if(A_event.type == Event::Closed) 
                    {
                        state = Game_State::END;
                        window.close();
                        break;
                    }
                    if(A_event.type == Event::KeyPressed || A_event.type ==
                        Event::MouseButtonPressed)
                    {
                        state = Game_State::MENU;
                    }
                }
            }
    }
    
    void choose_size()
    {
        window.clear();
        Text log("Choose size",font,10);
        Text five_bar(" 5 ",font, 14);
        Text three_bar(" 3 ",font,14);
        log.setPosition(96/2 - log.getGlobalBounds().width/2,96/2 - 6*log.getGlobalBounds().height/2);
        five_bar.setPosition(96/2 - five_bar.getGlobalBounds().width/2,96/2-5);
        three_bar.setPosition(96/2 - three_bar.getGlobalBounds().width/2,96/2 +  3*three_bar.getGlobalBounds().height/2);
        window.draw(log);
        window.draw(five_bar);
        window.draw(three_bar);
        window.display();
        
        while(state == Game_State::CHOOSE_SIZE){
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));    
            Event event;
            while (window.pollEvent(event)){
                
                if(event.type == Event::Closed) 
                {
                    state = Game_State::END;
                    window.close();
                    break;
                }
                
                if(three_bar.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.is_big = false;
                    state = Game_State::CHOOSE_MODE;
                }
                
                if(five_bar.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.is_big = true;
                    window.create(VideoMode(160,160),"Tic-tac-toe");
                    state = Game_State::CHOOSE_MODE;
                }
            }
        }
        
    }
        
    
    void choose_mode()
    {
        window.clear();
        Sprite pvp,pvc,cvc;
        
        Image mode_image;
        mode_image.loadFromFile("Choosing_mode.png");
        Texture mode_texture;
        mode_texture.loadFromImage(mode_image);
        
        
        pvp.setTexture(mode_texture);
        pvc.setTexture(mode_texture);
        cvc.setTexture(mode_texture);
        
        if(!engine.is_big)
        {
            pvp.setTextureRect(IntRect(0,32,64,32));
            pvp.setPosition(96/2 - pvp.getGlobalBounds().width/2,0);
            window.draw(pvp);
            
            pvc.setTextureRect(IntRect(0,0,64,32));
            pvc.setPosition(96/2 - pvc.getGlobalBounds().width/2,32);
            window.draw(pvc);
            
            cvc.setTextureRect(IntRect(0,64,64,32));
            cvc.setPosition(96/2 - cvc.getGlobalBounds().width/2,64);
            window.draw(cvc);
            
            window.display();
        }else{
            pvp.setTextureRect(IntRect(0,32,64,32));
            pvp.setPosition(160/2 - pvp.getGlobalBounds().width/2,160/2-3.5*pvp.getGlobalBounds().height/2);
            window.draw(pvp);
            
            pvc.setTextureRect(IntRect(0,0,64,32));
            pvc.setPosition(160/2 - pvc.getGlobalBounds().width/2,160/2 -pvc.getGlobalBounds().height/2);
            window.draw(pvc);
            
            cvc.setTextureRect(IntRect(0,64,64,32));
            cvc.setPosition(160/2 - cvc.getGlobalBounds().width/2,160/2 + 2*cvc.getGlobalBounds().height/2-10);
            window.draw(cvc);
            
            window.display();
        }
         while(state == Game_State::CHOOSE_MODE){
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));    
            Event event;
            while (window.pollEvent(event)){
                
                if(event.type == Event::Closed) 
                {
                    state = Game_State::END;
                    window.close();
                    break;
                }
                
                if(pvp.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.is_ai_on = false;
                    state = Game_State::CHOOSE;
                }
                
                if(pvc.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.is_ai_on = true;
                    state = Game_State::CHOOSE;
                }
                
                  if(cvc.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.ai_vs_ai = true;
                    engine.ai1_turn = true;
                    state = Game_State::INGAME;
                }
            }
        }
        
    }
    
    void choose_first()
    {
        
        window.clear();
        Sprite tic,tac;
        
        if(!engine.is_big)
        {
            tic.setTexture(map_texture);
            tac.setTexture(map_texture);
            
            tic.setTextureRect(IntRect(160,0,32,32));
            tic.setPosition(96/2 - tic.getGlobalBounds().width/2,96/2-2*tic.getGlobalBounds().height/2-5);
            window.draw(tic);
            
            tac.setTextureRect(IntRect(192,0,32,32));
            tac.setPosition(96/2 - tac.getGlobalBounds().width/2,96/2+tac.getGlobalBounds().height/2-10);
            window.draw(tac);
            window.display();
        }else{
            tic.setTexture(map_texture);
            tac.setTexture(map_texture);
            
            tic.setTextureRect(IntRect(160,0,32,32));
            tic.setPosition(160/2 - tic.getGlobalBounds().width/2,160/2-2*tic.getGlobalBounds().height/2-5);
            window.draw(tic);
            
            tac.setTextureRect(IntRect(192,0,32,32));
            tac.setPosition(160/2 - tac.getGlobalBounds().width/2,160/2+tac.getGlobalBounds().height/2-10);
            window.draw(tac);
            window.display();
        }
            
        while(state == Game_State::CHOOSE)
        {
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));
            
            Event C_event;
            while(window.pollEvent(C_event))
            {
                
                if(C_event.type == Event::Closed || 
                    (C_event.type == Event::KeyPressed
                    && C_event.key.code == Keyboard::Escape))
                {
                    state = Game_State::END;
                    window.close();
                    continue;
                }
                if(tic.getGlobalBounds().contains(globMouse) 
                    && C_event.type == Event::MouseButtonPressed 
                    && C_event.key.code == Mouse::Left)
                {
                    if(!engine.is_ai_on){
                        engine.is_tic = true;
                        state = Game_State::INGAME;
                    }else {
                        engine.is_ai_X = false;
                        state = Game_State::CHOOSE_AI;
                    }
                        
                }
                if(tac.getGlobalBounds().contains(globMouse) 
                    && C_event.type == Event::MouseButtonPressed 
                    && C_event.key.code == Mouse::Left)
                {
                    if(!engine.is_ai_on){
                        engine.is_tic = false;
                        state = Game_State::INGAME;
                    }else {
                        engine.is_ai_X = true;
                        state = Game_State::CHOOSE_AI;
                    }
                }
                
            }
        }
    }
    
    void choose_ai()
    {
        
        window.clear();
        
        Text log("Computer first?",font,10);
        Text y(" yes ",font, 14);
        Text n(" no ",font,14);
        if(!engine.is_big)
        {
            log.setPosition(96/2 - log.getGlobalBounds().width/2,96/2 - 3*log.getGlobalBounds().height/2);
            y.setPosition(96/2 - y.getGlobalBounds().width/2,96/2);
            n.setPosition(96/2 - n.getGlobalBounds().width/2,96/2 +  3*n.getGlobalBounds().height/2);
            window.draw(log);
            window.draw(y);
            window.draw(n);
            window.display();
        }else{;
            log.setPosition(160/2 - log.getGlobalBounds().width/2,160/2 - 4*log.getGlobalBounds().height/2);
            y.setPosition(160/2 - y.getGlobalBounds().width/2,160/2);
            n.setPosition(160/2 - n.getGlobalBounds().width/2,160/2 +  3*n.getGlobalBounds().height/2);
            window.draw(log);
            window.draw(y);
            window.draw(n);
            window.display();
        }
            
        
        while(state == Game_State::CHOOSE_AI)
        {
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));    
            Event event;
            while (window.pollEvent(event)){
                
                if(event.type == Event::Closed) 
                {
                    state = Game_State::END;
                    window.close();
                    break;
                }
                
                if(y.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.is_ai_turn = true;
                    state = Game_State::INGAME;
                }
                
                if(n.getGlobalBounds().contains(globMouse) 
                    && event.type == Event::MouseButtonPressed
                    && event.key.code == Mouse::Left)
                {
                    engine.is_ai_turn = false;
                    state = Game_State::INGAME;
                }
            }
        }
    }
    
    void game()
    {
        engine.init_eng();
        Vector2f globMouse;
        globMouse.x = 0;
        globMouse.y = 0;
        
        if(engine.ai_vs_ai == true){
            while(engine.winner == GAME)
            {
                engine.update_game_proc_ai();
                engine.draw_map(s_map);
                sleep(1);
            }
            state = Game_State::WINNER_FRAME ;
        }
        
        if(engine.is_ai_on && engine.is_ai_turn){
            engine.update_game_proc(globMouse);
            engine.draw_map(s_map);
        }
        
        
        while(state == Game_State::INGAME){
            
        Event G_event;
        while(window.pollEvent(G_event))
        {
            if(G_event.type == Event::Closed 
                || (G_event.type == Event::KeyPressed 
                && G_event.key.code == Keyboard::Escape))
            { 
                state = Game_State::END;
                window.close();
                continue;
            }
            if(!engine.is_ai_turn && G_event.type == Event::MouseButtonPressed
                && G_event.key.code == Mouse::Left)
            {
                globMouse = window.mapPixelToCoords(Mouse::getPosition(window));
                engine.update_game_proc(globMouse);

                
            }

            
        }
        
       
        engine.draw_map(s_map);
        
        if(engine.winner == X_VAL
                    || engine.winner == O_VAL 
                    || engine.winner == DRAW){
            sleep(1);
            state = Game_State::WINNER_FRAME ;
        }
        
        if(engine.is_ai_turn) engine.update_game_proc(globMouse);
        
         
        
        
        
        }
    }
    
    void win_frame()
    {
        window.clear();
        if(!engine.is_big){
            if(engine.winner == X_VAL)
            {
                Text win_log("Krestici win!!",font,10);
                Text tap_any("Tap any key,to escape",font,5);
                win_log.setPosition(96/2 - win_log.getGlobalBounds().width/2,96/2 - win_log.getGlobalBounds().height/2);
                tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
                window.draw(win_log);
                window.draw(tap_any);
            }else if (engine.winner == O_VAL){
                Text win_log("Noliki win!!",font,10);
                Text tap_any("Tap any key,to escape",font,5);
                win_log.setPosition(96/2 - win_log.getGlobalBounds().width/2,96/2 - win_log.getGlobalBounds().height/2);
                tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
                window.draw(win_log);
                window.draw(tap_any);
            }else if (engine.winner == DRAW){
                Text win_log("No winner!!",font,10);
                Text tap_any("Tap any key,to escape",font,5);
                win_log.setPosition(96/2 - win_log.getGlobalBounds().width/2,96/2 - win_log.getGlobalBounds().height/2);
                tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
                window.draw(win_log);
                window.draw(tap_any);
            }
        }else{
            if(engine.winner == X_VAL)
            {
                Text win_log("Krestici win!!",font,15);
                Text tap_any("Tap any key,to escape",font,10);
                win_log.setPosition(160/2 - win_log.getGlobalBounds().width/2,160/2 - win_log.getGlobalBounds().height/2);
                tap_any.setPosition(160/2 - tap_any.getGlobalBounds().width/2,160/2 + 15);
                window.draw(win_log);
                window.draw(tap_any);
            }else if (engine.winner == O_VAL){  
                Text win_log("Noliki win!!",font,15);
                Text tap_any("Tap any key,to escape",font,10);
                win_log.setPosition(160/2 - win_log.getGlobalBounds().width/2,160/2 - win_log.getGlobalBounds().height/2);
                tap_any.setPosition(160/2 - tap_any.getGlobalBounds().width/2,160/2 + 15);
                window.draw(win_log);
                window.draw(tap_any);
            }else if (engine.winner == DRAW){
                Text win_log("No winner!!",font,15);
                Text tap_any("Tap any key,to escape",font,10);
                win_log.setPosition(160/2 - win_log.getGlobalBounds().width/2,160/2 - win_log.getGlobalBounds().height/2);
                tap_any.setPosition(160/2 - tap_any.getGlobalBounds().width/2,160/2 + 15);
                window.draw(win_log);
                window.draw(tap_any);
            }
            
                
        }
        
        window.display();
        while(state == Game_State::WINNER_FRAME)
        {
            Event E_event;
            while(window.pollEvent(E_event))
            {
                if(E_event.type == Event::KeyPressed 
                    || E_event.type == Event::MouseButtonPressed)
                {
                    state = Game_State::END;
                    window.close();
                }
            }
        }
    }
    
    
};

int main()
{    
    Image map_images;
    map_images.loadFromFile("Untitled.png");
    Texture map_texture;
    map_texture.loadFromImage(map_images);
    Sprite s_map;
    s_map.setTexture(map_texture);
    
        Font font;
    font.loadFromFile("NeutralFace.otf");
    
    Game new_game(s_map,font,map_texture);
    new_game.runGame();
    
    return 0;
}
