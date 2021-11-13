#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace sf;

//договоримся, что tic - крестик, а tac - нолик

enum class Game_State{MENU = 0,AUTHOR,CHOOSE,INGAME,WINNER_FRAME,END};
enum class Win{WTIC = 0, WTAC};

class Game{
public:
    Game_State state;
};


class Player{
public:
    bool is_tic;
    Win winner;
    void update_game_proc(const Vector2f& vec,std::vector<std::vector<int>>& map)
    {
        int raw = static_cast<int>(vec.y/32);
        int column = static_cast<int>(vec.x/32);
        const int screen_size = 3;
        int counter;
        if(winner != Win::WTIC || winner != Win::WTAC)
        {
            if(is_tic == true) 
            {
                map[raw][column] = 2;
                is_tic = false;
                //Анализ победы
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[raw][j] == 2) counter++;
                }
                if(counter == 3) 
                {
                    winner = Win::WTIC;
                }
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[j][column] == 2) counter++;
                }
                if(counter == 3) 
                {
                    winner = Win::WTIC;
                }
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[j][j] == 2) counter++;
                }
                if(counter == 3)
                {
                    winner = Win::WTIC;
                }
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[screen_size-1-j][j] == 2) counter++;
                }
                if(counter == 3)
                {
                    winner = Win::WTIC;
                }
                
            }else
            {
                map[raw][column] = 1;
                is_tic = true;
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[raw][j] == 1) counter++;
                }
                if(counter == 3) 
                {
                    winner = Win::WTAC;
                }
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[j][column] == 1) counter++;
                }
                if(counter == 3) 
                {
                    winner = Win::WTAC;
                }
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[j][j] == 1) counter++;
                }
                if(counter == 3)
                {
                    winner = Win::WTAC;
                }
                counter = 0;
                for(int j = 0;j < screen_size;j++)
                {
                    if(map[screen_size-1-j][j] == 1) counter++;
                }
                if(counter == 3)
                {
                    winner = Win::WTAC;
                }
            }
        }
    }
        
        
};




int main()
{
    
    RenderWindow window(VideoMode (96,96), "Tic-tac-toe");
    
    std::vector <std::vector<int>> height_map (3);
    for (int i = 0;i<height_map.size();i++){
        height_map[i].resize(3);
    }
    
    //Font's
    Font font;
    font.loadFromFile("NeutralFace.otf");
    
    
    
    //Map's
    Image map_image;
    map_image.loadFromFile("Untitled.png");
    Texture map_texture;
    map_texture.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map_texture);
    
    Player people;
    
    //Game
    Game g;
    g.state = Game_State::MENU;
    
    
    while (window.isOpen())
    {
        while(g.state!=Game_State::END)
        {    
            switch(g.state)
            {
                
                case Game_State::MENU:
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
                    
                    while(g.state == Game_State::MENU)
                    {
                        
                        Vector2f globMouse = window.mapPixelToCoords(Mouse::getPosition(window));
                        
                        Event M_event;
                        while(window.pollEvent(M_event))
                        {
                            if(M_event.type == Event::Closed)
                            { 
                                g.state = Game_State::END;
                                window.close();
                                continue;
                            }
                            
                            if(M_event.type == Event::KeyPressed 
                                && M_event.key.code == Keyboard::Escape) window.close();
                            
                            if(bars[0].getGlobalBounds().contains(globMouse)
                                && M_event.type == Event::MouseButtonPressed
                                && M_event.key.code == Mouse::Left)
                            {
                                g.state = Game_State::CHOOSE;
                            }
                            if(bars[1].getGlobalBounds().contains(globMouse)
                                && M_event.type == Event::MouseButtonPressed
                                && M_event.key.code == Mouse::Left)
                            {
                                g.state = Game_State::AUTHOR;
                            }
                            if(bars[2].getGlobalBounds().contains(globMouse)
                                && M_event.type == Event::MouseButtonPressed
                                && M_event.key.code == Mouse::Left)
                            {
                                g.state = Game_State::END;
                                window.close();
                            }
                        }
                    }
                }
                break;
                
                
                case Game_State::AUTHOR:
                {
                    window.clear();
                    Text log(" Egorov \nVsevolod",font,10);
                    Text tap_any(" Tap any key \nand go to menu",font,7);
                        log.setPosition(96/2 - log.getGlobalBounds().width/2,96/2 - log.getGlobalBounds().height/2);
                          tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
                        window.draw(log);
                        window.draw(tap_any);
                        window.display();
                        
                        while(g.state == Game_State::AUTHOR)
                        {
                            Event A_event;
                            while(window.pollEvent(A_event))
                            {
                                if(A_event.type == Event::KeyPressed || A_event.type ==
                                    Event::MouseButtonPressed)
                                {
                                    g.state = Game_State::MENU;
                                }
                            }
                        }
                }
                    break;
                    
                case Game_State::CHOOSE:
                {
                    
                    window.clear();
                    Sprite tic,tac;
                    
                    tic.setTexture(map_texture);
                    tac.setTexture(map_texture);
                    
                    tic.setTextureRect(IntRect(160,0,32,32));
                    tic.setPosition(96/2 - tic.getGlobalBounds().width/2,96/2-2*tic.getGlobalBounds().height/2-5);
                    window.draw(tic);
                    
                    tac.setTextureRect(IntRect(192,0,32,32));
                    tac.setPosition(96/2 - tac.getGlobalBounds().width/2,96/2+tac.getGlobalBounds().height/2-10);
                    window.draw(tac);
                    window.display();
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
                                people.is_tic = true;
                                g.state = Game_State::INGAME;
                                continue;
                            }
                            if(tac.getGlobalBounds().contains(globMouse) 
                                && C_event.type == Event::MouseButtonPressed 
                                && C_event.key.code == Mouse::Left)
                            {
                                people.is_tic = false;
                                g.state = Game_State::INGAME;
                                ;
                            }
                            
                        }
                    }
                }
                break;
                
                
                case Game_State::INGAME:
                {   
                    
                    
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
                            people.update_game_proc(globMouse,height_map);
                        }
                        if(G_event.type == Event::MouseButtonPressed
                            || G_event.type == Event::KeyPressed)
                        {
                            if(people.winner == Win::WTIC
                                || people.winner == Win::WTAC) g.state = Game_State::WINNER_FRAME ;
                        }
                        
                    }
                    
                    window.clear();
                    for(int i = 0;i < 3;i++)
                    {
                        for(int j = 0;j < 3;j++)
                        {
                            if(height_map[i][j] == 0) s_map.setTextureRect(IntRect(0,0,32,32));
                            if(height_map[i][j] == 1) s_map.setTextureRect(IntRect(32,0,32,32));
                            if(height_map[i][j] == 2) s_map.setTextureRect(IntRect(64,0,32,32));
                            
                            s_map.setPosition(j*32,i*32);
                            window.draw(s_map);
                        }
                    }
                    window.display();
                  
                }
                break;
                
                case Game_State::WINNER_FRAME:
                {
                    window.clear();
                    Text tap_any("Tap any key,to escape",font,5);
                    if(people.winner == Win::WTIC)
                    {
                        Text win_log("Krestici win!!",font,10);
                        win_log.setPosition(96/2 - win_log.getGlobalBounds().width/2,96/2 - win_log.getGlobalBounds().height/2);
                        tap_any.setPosition(96/2 - tap_any.getGlobalBounds().width/2,96/2 + 15);
                        window.draw(win_log);
                        window.draw(tap_any);
                    }else if (people.winner == Win::WTAC){
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
        if (g.state == Game_State::END) window.close();
        
    }
    }
    
}
