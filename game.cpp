#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

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
MENU = 0,AUTHOR,CHOOSE_MODE,CHOOSE_SIZE,CHOOSE_AI,GHOOSE,INGAME,WINNER_FRAME,END};

enum class Win{WTIC = 0, WTAC};


class Board{
public:
    bool is_big;
    std::vector<std::vector<int>> map;
    
    void setBoard(bool big)
    {
        if(big)
        {
            for(int y = 0;y < 5;y++)
            {
                map[y].resize(5);
            }
            is_big = true;
        }else{
            for(int y = 0;y < 3;y++)
            {
                map[y].resize(3);
            }
            is_big = false;
        }
    }
    
    int screen(int size)
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
                if(counter == 3) 
                {
                    return win;
                }else counter = 0;
                
                for(int y = 0;y < size;y++)
                {
                    if(map[y][column] == win) counter++;
                }
                if(counter == 3) 
                {
                    return win;
                }else counter = 0;
                
            }
            for(int y = 0;y < size;y++)
            {
                if(map[y][y] == win) counter++;
            }
            if(counter == 3)
            {
                return win;
            }else counter = 0;
            
            for(int y = 0;y < size;y++)
            {
                if(map[size-1-y][y] == win) counter++;
            }
            if(counter == 3)
            {
                return win;
            }
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
        if(map[x][x] == NO_VAL)
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
public:
    void init(const int aiPlayer)
    {
        _aiPlayer = aiPlayer;
        if(_aiPlayer == X_VAL){
            _humanPlayer = O_VAL;
        }else{
            _humanPlayer = X_VAL;
        }
    }
    void performMove(Board& board)
    {
        AiMove bestMove = getBestMove(board,_aiPlayer);
        board.setValue(bestMove.x,bestMove.y,_aiPlayer);
    }
    private;
    AiMove getBestMove(Board& board,int player)
    {
        int rv = board.checkVictory();
        if (rv == _aiPlayer){
            return AiMove(10);
        }else if (rv == _humanPlayer){
            return AiMove(-10);
        }else if (rv == DRAW){
            return AiMove(0);
        }
        
        std::vector<AiMove> moves;
        
        for (int y = 0;y < board.getSize(); y++){
            for(int x = 0;x < board.getSize(),x++){
                if(board.getVal(x,y) == NO_VAL){
                    AiMove move;
                    move.x = x;
                    move.y = y;
                    board.setValue(x, y ,player);
                    if(player == _aiPlayer){
                        move.score = getBestMove(board,_humanPlayer).score;
                    }else{
                        move.score = getBestMove(board,_aiPlayer).score;
                    }
                    moves.push_back(move);
                    board.removeValue(x,y);
                }
            }
        }
        
        int bestMove = 0;
        if(player == _aiPlayer) {
            int bestScore = -1000000;
            for (int i = 0; i < moves.size();i++){
                if(moves[i].score > bestScore){
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        } else {
            int bestScore = 1000000;
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
    bool is_tic;
    int winner;
    
    AI supercomp;
    bool is_big;
    bool is_ai_on;
    bool is_ai_turn;
    bool is_ai_X;
    
    Board map;

    int humanPlayer;
    
    void init_eng()
    {
        map.setBoard(is_big)

        if(is_ai_on && is_ai_turn)
        {
            supercomp.init(X_VAL);
            humanPlayer = O_VAL;
        }else if (is_ai_on && !is_ai_turn){
            if(!is_ai_X){
                supercomp.init(O_VAl);
                humanPlayer = X_VAL;
            }else{
                supercomp.init(X_VAL);
                humanPlayer = O_VAL;
            }
        }
    }
    
    
    void update_game_proc(Vector2f vec)
    {
        int raw = vec.y/32;
        int column = vec.x/32;
        
        if(!is_ai_on)
        {
            if(is_tic){
                board.setValue(raw,column,X_VAL);
                winner = board.checkVictory();
                is_tic = false;
            }else{
                board.setValue(raw,column,O_VAL);
                winner = board.checkVictory();
                it_tic = true;
            }
        }else{
            if(ai_turn){
                AI.performMove(map);
                ai_turn = false;
            }else{
                board.setValue(raw,column,humanPlayer);
                winner = board.checkVictory();
                ai_turn = true;
            }
        }
    }
    
    void draw_map(Sprite map)
    {
        window.clear();
        for(int x = 0;x < board.getSize();x++)
        {
            for(int y = 0;y < board.getSize();y++)
            {
                if(board.getValue(x,y) == 0) map.setTextureRect(IntRect(0,0,32,32));
                if(board.getValue(x,y) == 1) map.setTextureRect(IntRect(32,0,32,32));
                if(board.getValue(x,y) == 2) map.setTextureRect(IntRect(64,0,32,32));
                
                map.setPosition(j*32,i*32);
                window.draw(map);
            }
        }
        window.display();
    }
    
            
}


class Game 
{
public:
    //состояние игры
    Game_State state;
    
    //Тайлы карты
    Image map_image;
    map_image.loadFromFile("Untitled.png");
    Texture map_texture;
    map_texture.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map_texture);
    
    
    
    //Стиль шрифта в игре
    Font font;
    font.loadFromFile("NeutralFace.otf");
    
    //подключаем наш игровой движок
    Engine engine;
    
    Game ()
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
                        case GameState::MENU:
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
                            choose();
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
                        state = Game_State::CHOOSE_MODE;
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
        window.clear()
        Text log("Choose size",font,10);
        Text five_bar(" 5 ",font, 14);
        Text three_bar(" 3 ",font,14);
        log.setPosition(96/2 - log.getGlobalBounds().width/2,96/2 - 2*log.getGlobalBounds().height/2);
        five_bar.setPosition(96/2 - five_bar.getGlobalBounds().width/2,96/2);
        three_bar.setPosition(96/2 - three_bar.getGlobalBounds().width/2,96/2 +  2*three_bar.getGlobalBounds().height/2);
        window.draw(log);
        window.draw(five_bar);
        window.draw(three_bar);
        window.display();
        
        while(state == Game_State::CHOOSE_SIZE){
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window);    
            Event event;
            while (window.waitEvent(event)){
                
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
                    && event.type == Mouse::Left)
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
        Sprite pvp,pvc;
        
        Image mode_image;
        mode_image.loadFromFile("Choosing_mode.png");
        Texture mode_texture;
        mode_texture.loadFromImage(mode_image);
        
        
        pvp.setTexture(mode_texture);
        pvc.setTexture(mode_texture);
        
        if(!engine.is_big)
        {
            pvp.setTextureRect(IntRect(0,0,64,32));
            pvp.setPosition(96/2 - pvp.getGlobalBounds().width/2,96/2-1.5*pvp.getGlobalBounds().height/2);
            window.draw(pvp);
            
            pvc.setTextureRect(IntRect(0,32,64,32));
            pvc.setPosition(96/2 - pvc.getGlobalBounds().width/2,96/2+pvc.getGlobalBounds().height/2);
            window.draw(pvc);
            window.display();
        }else{
            pvp.setTextureRect(IntRect(0,0,64,32));
            pvp.setPosition(160/2 - pvp.getGlobalBounds().width/2,160/2-2*pvp.getGlobalBounds().height/2);
            window.draw(pvp);
            
            pvc.setTextureRect(IntRect(0,32,64,32));
            pvc.setPosition(160/2 - pvc.getGlobalBounds().width/2,160/2+1*5pvc.getGlobalBounds().height/2);
            window.draw(pvc);
            window.display();
        }
         while(state == Game_State::CHOOSE_MODE){
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window);    
            Event event;
            while (window.waitEvent(event)){
                
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
                    && event.type == Mouse::Left)
                {
                    engine.is_ai_on = true;
                    state = Game_State::CHOOSE;
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
            
        while(g.state == Game_State::CHOOSE)
        {
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));
            
            Event C_event;
            while(window.pollEvent(C_event))
            {
                
                if(C_event.type == Event::Closed || 
                    (C_event.type == Event::KeyPressed
                    && C_event.key.code == Keyboard::Escape))
                {
                    g.state = Game_State::END;
                    window.close();
                    continue;
                }
                if(tic.getGlobalBounds().contains(globMouse) 
                    && C_event.type == Event::MouseButtonPressed 
                    && C_event.key.code == Mouse::Left)
                {
                    if(!engine.is_ai_on){
                        engine.is_tic = true;
                    }else {
                        engine.is_ai_X = true;
                        state = Game_State::CHOOSE_AI;
                    }
                        
                }
                if(tac.getGlobalBounds().contains(globMouse) 
                    && C_event.type == Event::MouseButtonPressed 
                    && C_event.key.code == Mouse::Left)
                {
                    if(!engine.is_ai_on){
                        engine.is_tic = false;
                    }else {
                        engine.is_ai_X = false;
                        state = Game_State::CHOOSE_AI;
                    }
                }
                
            }
        }
    }
    
    void choose_ai()
    {
        
        window.clear()
        
        if(!engine.is_big)
        {
            Text log("Computer first?",font,10);
            Text y(" yes ",font, 14);
            Text n(" no ",font,14);
            log.setPosition(96/2 - log.getGlobalBounds().width/2,96/2 - 2*log.getGlobalBounds().height/2);
            y.setPosition(96/2 - y.getGlobalBounds().width/2,96/2);
            n.setPosition(96/2 - n.getGlobalBounds().width/2,96/2 +  2*n.getGlobalBounds().height/2);
            window.draw(log);
            window.draw(y);
            window.draw(n);
            window.display();
        }else{
            Text log("Computer first?",font,10);
            Text y(" yes ",font, 14);
            Text n(" no ",font,14);
            log.setPosition(160/2 - log.getGlobalBounds().width/2,160/2 - 2*log.getGlobalBounds().height/2);
            y.setPosition(160/2 - y.getGlobalBounds().width/2,160/2);
            n.setPosition(160/2 - n.getGlobalBounds().width/2,160/2 +  2*n.getGlobalBounds().height/2);
            window.draw(log);
            window.draw(y);
            window.draw(n);
            window.display();
        }
            
        
        while(state == Game_State::CHOOSE_SIZE)
        {
            Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window);    
            Event event;
            while (window.waitEvent(event)){
                
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
                    && event.type == Mouse::Left)
                {
                    engine.is_ai_turn = false;
                    state = Game_State::INGAME;
                }
            }
        }
    }
    
    void game()
    {
        engine.init();
        
        
        while(state = Game_State::INGAME){
            
        Event G_event;
        while(window.pollEvent(G_event))
        {
            if(G_event.type == Event::Closed 
                || (G_event.type == Event::KeyPressed 
                && G_event.key.code == Keyboard::Escape))
            { 
                g.state = Game_State::END;
                window.close();
                continue;
            }
            if(G_event.type == Event::MouseButtonPressed
                && G_event.key.code == Mouse::Left)
            {
                Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));
                engine.update_game_proc(globMouse);
            }
            if(G_event.type == Event::MouseButtonPressed
                || G_event.type == Event::KeyPressed)
            {
                if(engine.winner == X_VAL
                    || engine.winner == O_VAL) g.state = Game_State::WINNER_FRAME ;
            }
            
        }
        
        engine.draw_map(s_map);
        
        }
    }
    
    void win_frame()
    {
        window.clear();
        Text tap_any("Tap any key,to escape",font,5);
        if(engine.winner == X_VAL)
        {
            Text win_log("Krestici win!!",font,10);
            win_log.setPosition(96/2 - win_log.getGlobalBounds().width/2,96/2 - win_log.getGlobalBounds().height/2);
            tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
            window.draw(win_log);
            window.draw(tap_any);
        }else if (engine.winner == O_VAL){
            Text win_log("Noliki win!!",font,10);
            win_log.setPosition(96/2 - win_log.getGlobalBounds().width/2,96/2 - win_log.getGlobalBounds().height/2);
            tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
            window.draw(win_log);
            window.draw(tap_any);
        }
        window.display();
        while(g.state == Game_State::WINNER_FRAME)
        {
            Event E_event;
            while(window.pollEvent(E_event))
            {
                if(E_event.type == Event::KeyPressed 
                    || E_event.type == Event::MouseButtonPressed)
                {
                    g.state = Game_State::END;
                    window.close();
                }
            }
        }
    }
    
    
}

int main()
{
    Game new_game;
    new_game.runGame();
    
    return 0;
}
