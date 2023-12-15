#ifndef APP_HPP
#define APP_HPP

#include <string_view>
#include <array>
#include <unordered_map>

#include <SDL2pp/SDL.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Point.hh>
#include <SDL2pp/SDLImage.hh>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/SDLMixer.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Chunk.hh>

#include <SDL2/SDL_render.h>

constexpr int FRAME_RATE = 30;



class App final
{
public:
    App();
    App(const App&) = default;
    App& operator=(const App&) = default;

    App(App&&) = default;
    App& operator=(App&&) = default;

    void run();

    ~App() = default;

private:
    class FrameRate final
    {
    public:
        FrameRate() = default;

        FrameRate(const FrameRate&) = delete;
        FrameRate& operator=(const FrameRate&) = delete;

        FrameRate(FrameRate&&) = default;
        FrameRate& operator=(FrameRate&&) = default;

        ~FrameRate() = default;

        bool checkFrame();
    private:
        unsigned int m_lastFrame {};
    };

    struct Goal
    {
        int prix;
        std::string str_goal;
    };


    void update();
    void printTitle();
    void drawBackground();
    void printSubGoal();

    const std::array<Goal, 15> m_goal = {
    {
        { .prix = 2, .str_goal = "Je montre mon setup"},
        { .prix = 6, .str_goal = "Live karaoké" },
        { .prix = 10, .str_goal = "Try hard d'AoE IV" },
        { .prix = 20, .str_goal = "Je mange des insectes" },
        { .prix = 30, .str_goal = "Je rase ma barbe" },
        { .prix = 40, .str_goal = "Fresque avec vos pseudo"},
        { .prix = 60, .str_goal = "Je mange un carolina reaper !!!"},
        { .prix = 90, .str_goal = "Je decouvre Shotgun King !" },
        { .prix = 100, .str_goal = "Je fais un tuto culture de tomate"},
        { .prix = 125, .str_goal = "Apero IRL avec streamer et modo"},
        { .prix = 150, .str_goal = "Chaine TikTok \"Les conprimés\""},
        { .prix = 175, .str_goal = "Je m'inscrit à un tournoi FIDE blitz"},
        { .prix = 200, .str_goal = "creation de casquette Shintaro"},
        { .prix = 250, .str_goal = "Je teins ma barbe pour noel"},
        { .prix = 500, .str_goal = "Je fait un giveway du bonnet lama"}
    }};

    FrameRate m_frameRate;
    SDL2pp::SDL m_sdl;
    SDL2pp::SDLImage m_sdlImage;
    SDL2pp::SDLTTF m_sdlTtf;
    SDL2pp::Window m_window;
    SDL2pp::Renderer m_renderer;
    std::unordered_map <SDL_KeyCode, bool> m_touch;
    SDL2pp::Font m_waltographFont;
    SDL2pp::Font m_waltographFontSmall;
    SDL2pp::Texture m_background;
    SDL2pp::Texture m_logoTelethon;
    SDL2pp::Texture m_logoFinish;
    int m_currentFocus;
    int m_currentGoal;

};



#endif