
#include "CommonFunc.h"
#include "BaseObject.h"
#include "TextObject.h"
int SDLCommonFunc::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
    const std::string& menu1,
    const std::string& menu2,
    const std::string& menu3,
    const std::string& img_name,
    std:: string coinest)
{
        if (coinest != " ") coinest = "HIGHTEST COIN : " + coinest;
        char* ch1 = (char*)menu1.c_str(); // tra ve con tro hang sau do ep kieu thanh con tro
        char* ch2 = (char*)menu2.c_str();
        char* ch3 = (char*)menu3.c_str();
        char* ch4 = (char*)coinest.c_str();
        char* img_file = (char*)img_name.c_str();

        int size1 = menu1.length();
        int size2 = menu2.length();
        int size3 = menu3.length();
        int size4 = coinest.length();

        int time = 0;
        int x = 0;
        int y = 0;
        const int kMenuNum = 4;
        char* labels[kMenuNum];

        labels[0] = new char[size1 + 1];
        labels[1] = new char[size2 + 1];
        labels[2] = new char[size3 + 1];
        labels[3] = new char[size4 + 1];

        strcpy_s(labels[0], size1 + 1, ch1);
        strcpy_s(labels[1], size2 + 1, ch2);
        strcpy_s(labels[2], size3 + 1, ch3);
        strcpy_s(labels[3], size4 + 1, ch4);



        bool selected[kMenuNum - 1 ] = { 0, 0 , 0 };
        SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };

        TextObject text_object[kMenuNum];
        text_object[0].SetText(labels[0]);
        text_object[0].setColor(color[0].r, color[0].g, color[0].b);



        text_object[1].SetText(labels[1]);
        text_object[1].setColor(color[0].r, color[0].g, color[0].b);

        text_object[2].SetText(labels[2]);
        text_object[2].setColor(color[0].r, color[0].g, color[0].b);

        text_object[3].SetText(labels[3]);
        text_object[3].setColor(color[0].r, color[0].g, color[0].b);



        SDL_Rect pos[kMenuNum];
        pos[0].x = SCREEN_WIDTH * 0.5 - 50;
        pos[0].y = SCREEN_HEIGHT * 0.4;

        pos[1].x = SCREEN_WIDTH * 0.5 - 50;
        pos[1].y = SCREEN_HEIGHT * 0.4  + 70;

        pos[2].x = SCREEN_WIDTH * 0.5 - 50;
        pos[2].y = SCREEN_HEIGHT * 0.4 + 140;

        pos[3].x = SCREEN_WIDTH * 0.4 - 50;
        pos[3].y = SCREEN_HEIGHT * 0.4 - 100;

        text_object[3].loadFromRenderedText(font, g_screen);

        BaseObject gBackground;
        bool ret = gBackground.LoadImg(img_file, g_screen);

        SDL_Event event;
        while (1)
        {
            for (int i = 0; i < kMenuNum - 1; i++) {
                text_object[i].loadFromRenderedText(font, g_screen);
            }
            time = SDL_GetTicks();
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    text_object[0].Free();
                    text_object[0].Free();
                    gBackground.Free();
                    return 2;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for (int i = 0; i < kMenuNum - 1; ++i)
                    {
                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h)
                        {
                            if (!selected[i])
                            {
                                selected[i] = 1;

                                text_object[i].setColor(color[1].r, color[1].g, color[1].b);

                            }
                        }
                        else
                        {
                            if (selected[i])
                            {
                                selected[i] = 0;

                                text_object[i].setColor(color[0].r, color[0].g, color[0].b);

                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    for (int i = 0; i < kMenuNum - 1; ++i) {
                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h)
                        {
                            text_object[0].Free();
                            text_object[1].Free();
                            text_object[2].Free();
                            text_object[3].Free();
                            gBackground.Free();
                            return i;
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        text_object[2].Free();
                        text_object[3].Free();
                        gBackground.Free();
                        return 2;
                    }
                }
            }

            gBackground.Render(g_screen, NULL);

            for (int i = 0; i < kMenuNum; ++i)
            {
                text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
                pos[i].w = text_object[i].getWidth();
                pos[i].h = text_object[i].getHeight();
            }


            SDL_RenderPresent(g_screen);
            if (1000 / 30 > (SDL_GetTicks() - time))
                SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
        }

        return 0;
    
   
}


// Hàm hiển thị cửa hàng
int SDLCommonFunc::ShowStore(SDL_Renderer* g_screen, 
                            TTF_Font* font, 
                            std::vector<Item>& items, 
                            int& coin, 
                            int& skill) {
    std::string coin_ ="coin: " + std::to_string(coin);
    int size = 4;
    int time = 0;
    int x = 0;
    int y = 0;
    const int kMenuNum = 4;
    char* labels[kMenuNum];

    TextObject text_object[kMenuNum];
    SDL_Rect pos[kMenuNum];



    for (int i = 0; i < size ; ++i) {
        if (i != 3) {
            labels[i] = new char[items[i].name.length() + 1];
            strcpy_s(labels[i], items[i].name.length() + 1, (char*)items[i].name.c_str());

            text_object[i].SetText(labels[i]);
            text_object[i].setColor(255, 0, 0);

            pos[i].x = SCREEN_WIDTH * 0.1 + 400 * (i);
            pos[i].y = SCREEN_HEIGHT * 0.65;
        }
        else {
            labels[3] = new char[coin_.length() + 1];
            strcpy_s(labels[3], coin_.length() + 1, (char*)coin_.c_str());

            text_object[3].SetText(labels[i]);
            text_object[3].setColor(0, 0, 0);

            pos[3].x = 20;
            pos[3].y = 10;
        }

       
       
    }

    BaseObject gBackground;
    bool ret = gBackground.LoadImg("img//STORE.png", g_screen);

    SDL_Event event;
    while (1) {
        for (int i = 0; i < kMenuNum; i++) {
            text_object[i].loadFromRenderedText(font, g_screen);
        }
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                for (int i = 0; i < kMenuNum; ++i) {
                    text_object[i].Free();
                }
                gBackground.Free();
                return -1;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    x = event.button.x;
                    y = event.button.y;
                    for (int i = 0; i < kMenuNum; ++i) {
                        if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                            y >= pos[i].y && y <= pos[i].y + pos[i].h) {


                            if (items[i].price == 0) {
                                if (items[i].name == "Equip") {
                                    items[skill].name = "Equip";
                                    items[i].name = "Equipped";
                                    skill = i;
                                }
                            }
                            else {
                                if (coin - items[i].price >= 0) {
                                    coin -= items[i].price;
                                    items[i].price = 0;
                                    items[i].name = "Equip";
                                    
                                }

                            }

                            for (int j = 0; j < kMenuNum; ++j) {
                                text_object[j].Free();
                            }
                            gBackground.Free();
                            return i; // Trả về chỉ số vật phẩm được chọn
                        }

                        
                    }
                    
                }
                else
                {
                    for (int i = 0; i < kMenuNum; ++i) {
                        text_object[i].Free();
                    }
                    gBackground.Free();
                    return -1;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    for (int i = 0; i < kMenuNum; ++i) {
                        text_object[i].Free();
                    }
                    gBackground.Free();
                    return -1;
                }
            }
        }

        gBackground.Render(g_screen, NULL);

        for (int i = 0; i < kMenuNum; ++i) {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].getWidth();
            pos[i].h = text_object[i].getHeight();
        }

        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}
